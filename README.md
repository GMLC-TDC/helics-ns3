# helics-ns3

**This version of the helics-ns3 module WILL NOT WORK with ns-3.36 or newer. Use ns-3.35 (or slightly older).**
It uses the WAF contrib build system, and ns-3 switched build systems from WAF to CMake for version 3.36, along with making other breaking changes to their code.

It is recommended to update to the latest versions of helics-ns3 and ns-3; older versions using WAF will not be supported/maintained.

[![Build Status](https://github.com/GMLC-TDC/helics-ns3/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/GMLC-TDC/helics-ns3/actions/workflows/ci.yml)

[helics-ns3](https://github.com/GMLC-TDC/helics-ns3) is an [ns-3](https://www.nsnam.org/) module for coupling network simulations with other simulators using [HELICS](https://www.helics.org/).

### Older versions of HELICS and/or ns-3

For the last supported releases with older versions of HELICS and ns-3, look for the tags starting with `HELICS-v`and match the HELICS version with the build system (waf or cmake). Or look at the [Releases page](https://github.com/GMLC-TDC/helics-ns3/releases) and find the most recent release for your version of HELICS and ns-3 build system.

For HELICS 3 and ns-3 using waf, `HELICS-v3.x-waf` is the last tagged release.

For HELICS 2 and ns-3 using CMake, `HELICS-v2.x-cmake` is the last tagged release.

For HELICS 2 and ns-3 using waf, `HELICS-v2.x-waf` is the last tagged release.

For HELICS 1 (upgrade **highly** recommended), ns-3 using CMake is not supported; the final tag is `HELICS-v1.3.x`.

## Prerequisites

Install version 3+ of [HELICS](https://github.com/GMLC-TDC/HELICS); for building from source, be sure to set the CMake variable `-DHELICS_BUILD_CXX_SHARED_LIB=ON` and `-DCMAKE_INSTALL_PREFIX=<path to install folder you have access to>`.

Get a copy of ns-3.35 (slightly older may also work). 

Git:
```bash
git clone -b ns-3.35 https://gitlab.com/nsnam/ns-3-dev.git
```

## Installation

Clone a copy of this repository into a folder named `helics` in the ns-3 `contrib` directory. The module directory name *must* be `helics`, otherwise the ns-3 build system will be confused.

```bash
cd ns-3-dev/
git clone https://github.com/GMLC-TDC/helics-ns3 contrib/helics
```

Run `./waf configure` with the `--disable-werror` option, and set the `--with-helics` option to the path of your HELICS installation. To enable examples or tests use `--enable-examples` or `--enable-tests`, respectively. If ZMQ is not found, `--with-zmq` can be used to specify where it is installed (for macOS Homebrew users, `brew --prefix` will give the correct location). Paths should be absolute.

After configuration is done, run `./waf build` to compile ns-3 with the HELICS module.

```bash
./waf configure --with-helics=/usr/local --disable-werror --enable-examples --enable-tests
./waf build
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.  Please see [CONTRIBUTING](./CONTRIBUTING.md) for some additional notes on licensing of new components and modifications.  

## Release
The helics-ns3 repository is distributed under the terms of the [GPL-v2](LICENSE) license as required by NS3 since this library derives some code from that code base.

Individual source files that do not require derivation from NS3 are licensed as BSD style.  Any contributions to non-NS3 derived files ideally should be made with BSD markings in the source file.
At some point in the future if legally allowed or the NS-3 derived components can be removed the repository will be relicensed.  

SPDX-License-Identifier: GPL-v2
