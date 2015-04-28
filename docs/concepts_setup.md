# Concepts Setup

## Introduction

This library and the included documentation assumes you have a compiler with experimental support for C++ concepts --- basically the `concept` and `requires` keywords. We used the [`c++-concepts` branch](https://gcc.gnu.org/svn.html) of gcc.

If you've never compiled gcc before, here's how to do it. Alternatively, you may download our virtual machine image, which has the environment set up already.

## Instructions

While writing these instructions, we were helped by ["Building gcc 4.8 from source on Ubuntu 12.04"](http://eli.thegreenplace.net/2014/01/16/building-gcc-4-8-from-source-on-ubunu-12-04) by Eli Bendersky.

Get dependencies:

    sudo apt-get install build-essential svn libmpfr-dev libgmp3-dev libmpc-dev flex bison

Download the gcc source into a folder named `gcc-concepts-latest`. Note that if you run this command too many times, the GNU SVN server will block your IP address and you won't be able to download their stuff for awhile.

    svn checkout svn://gcc.gnu.org/svn/gcc/branches/c++-concepts gcc-concepts-latest

Use configure to generate the makefile.

    mkdir ~/install
    mkdir build
    cd gcc-concepts-latest
    ./configure --disable-checking --enable-languages=c,c++ --enable-multiarch --enable-shared --enable-threads=posix --without-included-gettext --with-tune=generic --prefix=$HOME/install/gcc-concepts-latest/ --disable-multilib

Explanation of the important changes:

* `--enable-languages=c,c++` --- gcc also supports languages like Ada and FORTRAN. Since we only care about C and C++, do not waste time compiling the frontends for any other languages. This also improves iteration time in case you get compile errors.
* `--disable-multilib` --- Only build support for this computer's architecture and calling convention.
* Having the build directory inside the source directory is technically not supported by gcc, so we moved the build directory outside. (However, nothing bad happens if you keep it inside...)

Compile:

    make -j8 # replace '8' with the number of CPUs on your computer
    make install

Now, there should be a bunch of gcc executables and libraries in `~/install/gcc-concepts-latest/`. To verify this:

    ls ~/install/gcc-concepts-latest/

To make our freshly compiled gcc and libstdc++ the default, add this to your `.profile` or similar:

    export PATH="$HOME/install/gcc-concepts-latest/bin:$PATH"
    export LD_LIBRARY_PATH="$HOME/install/gcc-concepts-latest/lib64/"

- The first line makes your shell search for gcc in our install directory before looking elsewhere.
- The second causes the dynamic linker to link your applications with our libstdc++. It is important to use the same standard library when compiling and running the program.
