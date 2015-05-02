# Concepts Setup

## Introduction

This library and the included documentation assumes you have a compiler with experimental support for C++ concepts --- basically the `concept` and `requires` keywords. We used the [`c++-concepts` branch](https://gcc.gnu.org/svn.html) of gcc.

If you've never compiled gcc before, here's how to do it. Alternatively, you may download our OVF virtual machine image, which has the environment set up already.

[ConceptsGCC.tar, 1.4 GB](http://files.kevinchen.co/ConceptsGCC.tar) (MD5 checksum `7b9c022a1f3fe4534b3002c86704a20d`)

Note: Although [Concepts-Lite (clite)](http://concepts.axiomatics.org/%7Eans/) has a nice-looking website, it is not the most recent implementation and you should not use it. Also, clite is pretty hard to compile on modern versions of Linux.

## Compiling the compiler

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

## Get the concepts-enabled standard library

If you want to develop your own applications or libraries using concepts, it is easiest if you are able to use the prewritten constraints. That way, you don't have to write things like `Function` or `Number` by yourself.

[Andrew Sutton's origin library](https://github.com/asutton/origin) has the most complete concepts implementation. (There are some concepts in clite, but not very many.)

    git clone git@github.com:asutton/origin.git

If you only need the constraints, you can just use our header file in `src/all_concepts.hpp`, which brings in all of the constraints in origin.

In case you have to compile the library, here's how:

You need CMake to compile origin. The version of CMake in many package managers is too old, so download it from the [cmake website](http://www.cmake.org/download/). Once you have CMake, you should be able to compile origin with:

    cd origin
    cmake CMakeLists.txt
    make

...although we were not able to get it compiling.
