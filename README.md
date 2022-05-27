# helics ns3 Module

**This version of the helics ns3 module is no longer maintained or supported. Please upgrade to HELICS 3.**

[![Build Status](https://github.com/GMLC-TDC/helics-ns3/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/GMLC-TDC/helics-ns3/actions/workflows/ci.yml)

[helics ns3](https://github.com/GMLC-TDC/helics-ns3) is an [ns-3](https://www.nsnam.org/) module for coupling network simulations with other simulators using [HELICS](https://www.helics.org/).

## Prerequisites

Install CMake, preferrably as new as possible (or at least 3.12+), though the absolute minimum is 3.10.

Install version 2.1.1+ of [HELICS](https://github.com/GMLC-TDC/HELICS); if building from source, be sure to set the CMake variable `JSONCPP_OBJLIB=ON` and `-DCMAKE_INSTALL_PREFIX=<path to install folder you have access to>`.
For versions 2.3+,  be sure to set the cmake variables `-DHELICS_BUILD_CXX_SHARED_LIB=ON`.

Get a copy of ns-3 that is version 3.36 or later, ideally from their GitLab repository.

Git:
```bash
git clone https://gitlab.com/nsnam/ns-3-dev.git
```

Build and install ns-3. See [https://www.nsnam.org/docs/tutorial/html/getting-started.html](https://www.nsnam.org/docs/tutorial/html/getting-started.html) for details on using their wrapper script. Options you should probably set during the configure step include `--prefix=<install_path>` to install ns-3 to a non-system folder. 

An example set of commands might look like:
```bash
./ns3 configure --enable-examples --enable-tests --prefix=$HOME/ns3-install
./ns3 build
./ns3 install
```
While the helics ns3 module could be built by checking out this repository into the ns-3 `contrib` folder and then following the above steps, we **highly discourage this** due to issues with how the ns-3 contrib module dependencies are propagated (**you will encounter errors trying to build ns-3 contrib modules that use external libraries**).
The best option is to write your code as a standard CMake (or Meson) project/library, then treat ns-3 as a library/package that you find and import.

## Installation

Clone this repository.

```bash
git clone https://github.com/GMLC-TDC/helics-ns3
```

Run CMake to configure and build the helics ns3 module. For CMake 3.12+,
`-Dns3_ROOT=<path_to_ns3_installation>` and `-DHELICS_ROOT=<path_to_HELICS_installation>` are options
that can be helpful for telling CMake where to find ns3 and HELICS installs. Otherwise, options like
`-Dns3_DIR=<path_to_ns3_installation>/<lib_folder>/cmake/ns3` and
`-DHELICS_DIR=<path_to_HELICS_installation>/<lib_folder>/cmake/HELICS` can be used.

An example set of commands to build the helics ns3 module is shown below. Modify the arguments as needed,
in particular, CMake error messages during the 2nd step **will almost always tell you how to fix the error**.

```bash
cd helics-ns3 && mkdir build && cd build
cmake .. -Dns3_ROOT=$HOME/ns3-install -DCMAKE_INSTALL_PREFIX=$HOME/ns3helics-install
cmake --build .
cmake --install .
```

## Using the helics ns3 module in your project

In the CMakeLists.txt file **for your project**, call `find_package(ns3helics REQUIRED)` to import
the library you compiled in the previous step. This will also try to locate the ns3 and HELICS
dependencies.

Next, for any libraries or executables in your project that make use of the helics ns3 module,
use `target_link_libraries(<your_exe_or_library> ns3::helics::ns3helics)`.

When running CMake to configure your project, `-Dns3helics_ROOT` and/or `-Dns3helics_DIR`
variables can be used to specify where the helics ns3 module was installed to.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.  Please see [CONTRIBUTING](./CONTRIBUTING.md) for some additional notes on licensing of new components and modifications.  

## Release
The helics-ns3 repository is distributed under the terms of the [GPL-v2](LICENSE) license as required by NS3 since this library derives some code from that code base.

Individual source files that do not require derivation from NS3 are licensed as BSD style.  Any contributions to non-NS3 derived files ideally should be made with BSD markings in the source file.
At some point in the future if legally allowed or the NS-3 derived components can be removed the repository will be relicensed.  

SPDX-License-Identifier: GPL-v2
