# Halide

Halide is a programming language designed to make it easier to write
high-performance image and array processing code on modern machines. Halide
currently targets:

- CPU architectures: X86, ARM, Hexagon, PowerPC, RISC-V
- Operating systems: Linux, Windows, macOS, Android, iOS, Qualcomm QuRT
- GPU Compute APIs: CUDA, OpenCL, Apple Metal, Microsoft
  Direct X 12, Vulkan

Rather than being a standalone programming language, Halide is embedded in C++.
This means you write C++ code that builds an in-memory representation of a
Halide pipeline using Halide's C++ API. You can then compile this representation
to an object file, or JIT-compile it and run it in the same process. Halide also
provides a Python binding that provides full support for writing Halide embedded
in Python without C++.

Halide requires C++17 (or later) to use.

For more detail about what Halide is, see http://halide-lang.org.

For API documentation see http://halide-lang.org/docs

To see some example code, look in the tutorials directory.

If you've acquired a full source distribution and want to build Halide, see the
[notes below](#building-halide-with-cmake).

# Getting Halide

## Binary tarballs

The latest version of Halide can always be found on GitHub
at https://github.com/halide/Halide/releases

We provide binary releases for many popular platforms and architectures, 
including 32/64-bit x86 Windows, 64-bit macOS, and 32/64-bit x86/ARM
Ubuntu Linux.

## Vcpkg

If you use [vcpkg](https://github.com/microsoft/vcpkg) to manage dependencies,
you can install Halide via:

```
$ vcpkg install halide:x64-windows # or x64-linux/x64-osx
```

One caveat: vcpkg installs only the minimum Halide backends required to compile
code for the active platform. If you want to include all the backends, you
should install `halide[target-all]:x64-windows` instead. Note that since this
will build LLVM, it will take a _lot_ of disk space (up to 100GB).

## Homebrew

Alternatively, if you use macOS, you can install Halide via
[Homebrew](https://brew.sh/) like so:

```
$ brew install halide
```

## Other package managers

We are interested in bringing Halide to other popular package managers and
Linux distribution repositories including, but not limited to, Conan,
Debian, [Ubuntu (or PPA)](https://github.com/halide/Halide/issues/5285),
CentOS/Fedora, and Arch. If you have experience publishing packages we would be
happy to work with you!

If you are a maintainer of any other package distribution platform, we would be
excited to work with you, too.

# Platform Support

There are two sets of platform requirements relevant to Halide: those required
to run the compiler library in either JIT or AOT mode, and those required to run
the _binary outputs_ of the AOT compiler.

These are the **tested** host toolchain and platform combinations for building
and running the Halide compiler library.

| Compiler   | Version      | OS                     | Architectures   |
|------------|--------------|------------------------|-----------------|
| GCC        | 9.4          | Ubuntu Linux 20.04 LTS | x86, x64, ARM32 |
| GCC        | 9.4          | Ubuntu Linux 18.04 LTS | ARM32, ARM64    |
| MSVC       | 2019 (19.28) | Windows 10 (20H2)      | x86, x64        |
| AppleClang | 14.0.3       | macOS 13.4             | x86_64          |
| AppleClang | 14.0.3       | macOS 13.4             | ARM64           |

Some users have successfully built Halide for Linux using Clang 9.0.0+, for
Windows using ClangCL 11.0.0+, and for Windows ARM64 by cross-compiling with
MSVC. We do not actively test these scenarios, however, so your mileage may
vary.

Beyond these, we are willing to support (by accepting PRs for) platform and
toolchain combinations that still receive _active, first-party, public support_
from their original vendors. For instance, at time of writing, this excludes
Windows 7 and includes Ubuntu 18.04 LTS.

Compiled AOT pipelines are expected to have much broader platform support. The
binaries use the C ABI, and we expect any compliant C compiler to be able to use
the generated headers correctly. The C++ bindings currently require C++17. If
you discover a compatibility problem with a generated pipeline, please open an
issue.

# Building Halide with Make

### TL;DR

Have llvm-16.0 (or greater) installed and run `make` in the root directory of
the repository (where this README is).

### Acquiring LLVM

At any point in time, building Halide requires either the latest stable version
of LLVM, the previous stable version of LLVM, and trunk. At the time of writing,
this means versions 18, 17, and 16 are supported, but 15 is not. The commands
`llvm-config` and `clang` must be somewhere in the path.

If your OS does not have packages for LLVM, you can find binaries for it at
http://llvm.org/releases/download.html. Download an appropriate package and then
either install it, or at least put the `bin` subdirectory in your path. (This
works well on macOS and Ubuntu.)

If you want to build it yourself, first check it out from GitHub:

```
% git clone --depth 1 --branch llvmorg-18.1.8 https://github.com/llvm/llvm-project.git
```

If you want a different version than 18.1.8, pick a different branch.

Then build it like so:

```
% cmake -G Ninja -S llvm-project/llvm -B llvm-build \
        -DCMAKE_BUILD_TYPE=Release \
        -DLLVM_ENABLE_PROJECTS="clang;lld;clang-tools-extra" \
        -DLLVM_ENABLE_RUNTIMES="compiler-rt" \
        -DLLVM_TARGETS_TO_BUILD="AArch64;AMDGPU;ARM;Hexagon;NVPTX;PowerPC;RISCV;WebAssembly;X86" \
        -DLLVM_BUILD_32_BITS=OFF \
        -DLLVM_ENABLE_ASSERTIONS=ON \
        -DLLVM_ENABLE_EH=ON \
        -DLLVM_ENABLE_RTTI=ON \
        -DLLVM_ENABLE_TERMINFO=OFF \
        -DLLVM_ENABLE_ZLIB=OFF \
        -DLLVM_ENABLE_ZSTD=OFF
% cmake --build llvm-build
% cmake --install llvm-build --prefix llvm-install
```

We strongly recommend using CMake's Ninja generator (as shown above) for
speed.

```
% export LLVM_ROOT=$PWD/llvm-install
% export LLVM_CONFIG=$LLVM_ROOT/bin/llvm-config
```

Note that you _must_ add `clang` to `LLVM_ENABLE_PROJECTS`; adding `lld` to
`LLVM_ENABLE_PROJECTS` is only required when using WebAssembly,
`LLVM_ENABLE_RUNTIMES="compiler-rt"` is only required if building the fuzz
tests, and adding `clang-tools-extra` is only necessary if you plan to
contribute code to Halide (so that you can run `clang-tidy` on your pull
requests). We recommend enabling both in all cases to simplify builds. You can
disable exception handling (EH) and RTTI if you don't want the Python bindings.

### Building Halide

With `LLVM_CONFIG` set (or `llvm-config` in your path), you should be able to
just run `make` in the root directory of the Halide source tree.
`make run_tests` will run the JIT test suite, and `make test_apps` will make
sure all the apps compile and run (but won't check their output).

There is no `make install`. If you want to build the Halide package, use CMake.

### Building Halide out-of-tree

If you wish to build Halide in a separate directory, you can do that like so:

    % cd ..
    % mkdir halide_build
    % cd halide_build
    % make -f ../Halide/Makefile

# Building Halide with CMake

## MacOS and Linux

Follow the above instructions to build LLVM or acquire a suitable binary
release. Then change directory to the Halide repository and run:

```
% cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_DIR=$LLVM_ROOT/lib/cmake/llvm -S . -B build
% cmake --build build
```

`LLVM_DIR` is the folder in the LLVM installation tree **(do not use the build
tree by mistake)** that contains `LLVMConfig.cmake`. It is not required to set
this variable if you have a suitable system-wide version installed. If you have
multiple system-wide versions installed, you can specify the version with
`Halide_REQUIRE_LLVM_VERSION`. Remove `-G Ninja` if you prefer to build with a
different generator.

## Windows

We suggest building with Visual Studio 2022. Your mileage may vary with earlier
versions. Be sure to install the "C++ CMake tools for Windows" in the Visual
Studio installer. For older versions of Visual Studio, do not install the CMake
tools, but instead acquire CMake and Ninja from their respective project
websites.

These instructions start from the `D:` drive. We assume this git repo is cloned
to `D:\Halide`. We also assume that your shell environment is set up correctly.
For a 64-bit build, run:

```
D:\> "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
```

For a 32-bit build, run:

```
D:\> "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64_x86
```

### Managing dependencies with vcpkg

The best way to get compatible dependencies on Windows is to use
[vcpkg](https://github.com/Microsoft/vcpkg). Install it like so:

```
D:\> git clone https://github.com/Microsoft/vcpkg.git
D:\> cd vcpkg
D:\vcpkg> .\bootstrap-vcpkg.bat -disableMetrics
D:\vcpkg> .\vcpkg integrate install
...
CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

Vcpkg will read [vcpkg.json](./vcpkg.json) to determine the set of dependencies
when configuring Halide. To produce a build with all supported targets, set

```
-DVCPKG_MANIFEST_FEATURES="target-all"
```

when configuring Halide. Downstream projects that want to use `Halide::ImageIO`
should add `libpng` and `libjpeg-turbo` to _their_ list of dependencies.

To use a system-provided LLVM, set
`-DVCPKG_OVERLAY_PORTS=/path/to/Halide/cmake/vcpkg`.

### Building LLVM manually (optional)

Follow these steps if you want to build LLVM yourself. First, download LLVM's
sources (these instructions use the latest 17.0 release)

```
D:\> git clone --depth 1 --branch llvmorg-18.1.8 https://github.com/llvm/llvm-project.git
```

For a 64-bit build, run:

```
D:\> cmake -G Ninja -S llvm-project\llvm -B llvm-build ^
           -DCMAKE_BUILD_TYPE=Release ^
           -DLLVM_ENABLE_PROJECTS=clang;lld;clang-tools-extra ^
           -DLLVM_ENABLE_RUNTIMES=compiler-rt ^
           -DLLVM_TARGETS_TO_BUILD="AArch64;AMDGPU;ARM;Hexagon;NVPTX;PowerPC;RISCV;WebAssembly;X86" ^
           -DLLVM_BUILD_32_BITS=OFF ^
           -DLLVM_ENABLE_ASSERTIONS=ON ^
           -DLLVM_ENABLE_EH=ON ^
           -DLLVM_ENABLE_RTTI=ON ^
           -DLLVM_ENABLE_TERMINFO=OFF ^
           -DLLVM_ENABLE_ZLIB=OFF ^
           -DLLVM_ENABLE_ZSTD=OFF
```

For a 32-bit build, work inside the `amd64_x86` environment from `vcvars` and run:

```
D:\> cmake -G Ninja -S llvm-project\llvm -B llvm32-build ^
           -DCMAKE_BUILD_TYPE=Release ^
           -DLLVM_ENABLE_PROJECTS=clang;lld;clang-tools-extra ^
           -DLLVM_ENABLE_RUNTIMES=compiler-rt ^
           -DLLVM_TARGETS_TO_BUILD="AArch64;AMDGPU;ARM;Hexagon;NVPTX;PowerPC;RISCV;WebAssembly;X86" ^
           -DLLVM_BUILD_32_BITS=ON ^
           -DLLVM_ENABLE_ASSERTIONS=ON ^
           -DLLVM_ENABLE_EH=ON ^
           -DLLVM_ENABLE_RTTI=ON ^
           -DLLVM_ENABLE_TERMINFO=OFF ^
           -DLLVM_ENABLE_ZLIB=OFF ^
           -DLLVM_ENABLE_ZSTD=OFF
```

Finally, run:

```
D:\> cmake --build llvm-build --config Release
D:\> cmake --install llvm-build --prefix llvm-install
```

You can substitute `Debug` for `Release` in the above `cmake` commands if you
want a debug build. Make sure to add `-DLLVM_DIR=D:/llvm-install/lib/cmake/llvm`
to the Halide CMake command to override `vcpkg`'s LLVM.

**MSBuild:** If you want to build LLVM with MSBuild instead of Ninja, use
`-G "Visual Studio 16 2019" -Thost=x64 -A x64` or
`-G "Visual Studio 16 2019" -Thost=x64 -A Win32` in place of `-G Ninja`.

### Building Halide

Create a separate build tree and call CMake with vcpkg's toolchain. This will
build in either 32-bit or 64-bit depending on the environment script (`vcvars`)
that was run earlier.

```
D:\Halide> cmake -G Ninja ^
                 -DCMAKE_BUILD_TYPE=Release ^
                 -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
                 -S . -B build
```

**Note:** If building with Python bindings on 32-bit (enabled by default), be
sure to point CMake to the installation path of a 32-bit Python 3. You can do
this by specifying, for example:
`"-DPython3_ROOT_DIR=C:\Program Files (x86)\Python38-32"`.

Then run the build with:

```
D:\Halide> cmake --build build --config Release
```

To run all the tests:

```
D:\Halide> cd build
D:\Halide\build> ctest -C Release
```

Subsets of the tests can be selected with `-L` and include `correctness`,
`python`, `error`, and the other directory names under `/tests`.

## If all else fails...

Do what the build-bots do: https://buildbot.halide-lang.org/master/#/builders

If the column that best matches your system is red, then maybe things aren't
just broken for you. If it's green, then you can click the "stdio" links in the
latest build to see what commands the build bots run, and what the output was.

# Some useful environment variables

`HL_TARGET=...` will set Halide's AOT compilation target.

`HL_JIT_TARGET=...` will set Halide's JIT compilation target.

`HL_DEBUG_CODEGEN=1` will print out pseudocode for what Halide is compiling.
Higher numbers will print more detail.

`HL_NUM_THREADS=...` specifies the number of threads to create for the thread
pool. When the async scheduling directive is used, more threads than this number
may be required and thus allocated. A maximum of 256 threads is allowed. (By
default, the number of cores on the host is used.)

`HL_TRACE_FILE=...` specifies a binary target file to dump tracing data into
(ignored unless at least one `trace_` feature is enabled in `HL_TARGET` or
`HL_JIT_TARGET`). The output can be parsed programmatically by starting from the
code in `utils/HalideTraceViz.cpp`.

# Using Halide on OSX

Precompiled Halide distributions are built using XCode's command-line tools with
Apple clang 500.2.76. This means that we link against libc++ instead of
libstdc++. You may need to adjust compiler options accordingly if you're using
an older XCode which does not default to libc++.

# Halide for Hexagon HVX

Halide supports offloading work to Qualcomm Hexagon DSP on Qualcomm Snapdragon
845/710 devices or newer. The Hexagon DSP provides a set of 128 byte vector
instruction extensions - the Hexagon Vector eXtensions (HVX). HVX is well suited
for image processing, and Halide for Hexagon HVX will generate the appropriate
HVX vector instructions from a program authored in Halide.

Halide can be used to compile Hexagon object files directly, by using a target
such as `hexagon-32-qurt-hvx`.

Halide can also be used to offload parts of a pipeline to Hexagon using the
`hexagon` scheduling directive. To enable the `hexagon` scheduling directive,
include the `hvx` target feature in your target. The currently supported
combination of targets is to use the HVX target features with an x86 linux
host (to use the simulator) or with an ARM android target (to use Hexagon DSP
hardware). For examples of using the `hexagon` scheduling directive on both the
simulator and a Hexagon DSP, see the blur example app.

To build and run an example app using the Hexagon target,

1. Obtain and build trunk LLVM and Clang. (Earlier versions of LLVM may work but
   are not actively tested and thus not recommended.)
2. Download and install the Hexagon SDK and Hexagon Tools. Hexagon SDK 4.3.0 or
   later is needed. Hexagon Tools 8.4 or later is needed.
3. Build and run an example for Hexagon HVX

### 1. Obtain and build trunk LLVM and Clang

(Follow the instructions given previously, just be sure to check out the `main`
branch.)

### 2. Download and install the Hexagon SDK and Hexagon Tools

Go to https://qpm.qualcomm.com/#/main/home

1. Go to Tools, and download Qualcomm Package Manager 3. Install the package manager on your machine.
2. Run the installed Qualcomm Package Manager and install the Qualcomm Hexagon SDK 5.x (or 4.x).
   The SDK can be selected from the Qualcomm Hexagon SDK Products.
3. Set an environment variable to point to the SDK installation location
   ```
   export SDK_LOC=/location/of/SDK
   ```

### 3. Build and run an example for Hexagon HVX

In addition to running Hexagon code on device, Halide also supports running
Hexagon code on the simulator from the Hexagon tools.

To build and run the blur example in Halide/apps/blur on the simulator:

```
cd apps/blur
export HL_HEXAGON_SIM_REMOTE=../../src/runtime/hexagon_remote/bin/v65/hexagon_sim_remote
export HL_HEXAGON_TOOLS=$SDK_LOC/Hexagon_Tools/8.x/Tools/
LD_LIBRARY_PATH=../../src/runtime/hexagon_remote/bin/host/:$HL_HEXAGON_TOOLS/lib/iss/:. HL_TARGET=host-hvx make test
```

### To build and run the blur example in Halide/apps/blur on Android:

To build the example for Android, first ensure that you have Android NDK r19b or
later installed, and the ANDROID_NDK_ROOT environment variable points to it.
(Note that Qualcomm Hexagon SDK v4.3.0 includes Android NDK r19c, which is
fine.)

Now build and run the blur example using the script to run it on device:

```
export HL_HEXAGON_TOOLS=$SDK_LOC/HEXAGON_Tools/8.4.11/Tools/
HL_TARGET=arm-64-android-hvx ./adb_run_on_device.sh
```
