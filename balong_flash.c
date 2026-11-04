name: Build Balong Flash (Windows)

on:
  push:
    branches:
      - master
  workflow_dispatch:

jobs:
  build-windows:
    runs-on: ubuntu-latest

    strategy:
      matrix:
        include:
          - arch: x86_64
            cc: x86_64-w64-mingw32-gcc
            out: balong_flash-x64.exe
          - arch: i686
            cc: i686-w64-mingw32-gcc
            out: balong_flash-x86.exe

    steps:
      - name: Checkout repository
        uses: actions/checkout@v4

      - name: Install MinGW cross-compiler
        run: |
          sudo apt-get update
          sudo apt-get install -y mingw-w64 make

      - name: Verify winbuild headers exist
        run: |
          echo "=== Checking winbuild headers ==="
          ls -la winbuild/ 2>/dev/null || echo "winbuild folder NOT found!"
          test -f winbuild/printf.h && echo "printf.h OK" || echo "printf.h MISSING"
          test -f winbuild/getopt.h && echo "getopt.h OK" || echo "getopt.h MISSING"

      - name: Build (static, with winbuild include path)
        run: |
          set -e
          test -f build || echo "1" > build

          make CC=${{ matrix.cc }} \
               CFLAGS="-O2 -I. -Iwinbuild -static -D_WIN32_WINNT=0x0600" \
               LDFLAGS="-static" \
               V=1

      - name: Rename output
        run: |
          set -e
          if [ -f balong_flash.exe ]; then
            mv balong_flash.exe ${{ matrix.out }}
          elif [ -f balong_flash ]; then
            mv balong_flash ${{ matrix.out }}
          else
            echo "Build output not found!"
            ls -la
            exit 1
          fi
          file ${{ matrix.out }}

      - name: Upload artifact
        uses: actions/upload-artifact@v4
        with:
          name: balong-flash-${{ matrix.arch }}
          path: ${{ matrix.out }}
          if-no-files-found: error
