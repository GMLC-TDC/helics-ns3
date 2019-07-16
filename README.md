# helics-ns3

[![Build Status](https://dev.azure.com/HELICS-test/helics-ns3/_apis/build/status/GMLC-TDC.helics-ns3?branchName=master)](https://dev.azure.com/HELICS-test/helics-ns3/_build/latest?definitionId=1?branchName=master)

[helics-ns3](https://github.com/GMLC-TDC/helics-ns3) is an [ns-3](https://www.nsnam.org/) module for coupling network simulations with other simulators using [HELICS](https://www.helics.org/).

## Prerequisites

Install version 2.1.1+ of [HELICS](https://github.com/GMLC-TDC/HELICS); if building from source, be sure to set the CMake variable `JSONCPP_OBJLIB=ON` .

Get a recent copy of ns-3, ideally from their GitLab repository.

Git:
```bash
git clone https://gitlab.com/nsnam/ns-3-dev.git
```

## Installation

Clone a copy of this repository into a folder named `helics` in the ns-3 `contrib` directory. The module directory name *must* be helics, otherwise the ns-3 build system will be confused.

```bash
cd ns-3-dev/
git clone https://github.com/GMLC-TDC/helics-ns3 contrib/helics
```

Run `./waf configure` with the `--disable-werror` option, and set the `--with-helics` option to the path of your HELICS installation. To enable examples or tests use `--enable-examples` or `--enable-tests`, respectively. If ZMQ is not found, `--with-zmq` can be used to specify where it is installed. Paths should be absolute.

After configuration is done, run `./waf build` to compile ns-3 with the HELICS module.

```bash
./waf configure --with-helics=/usr/local --disable-werror --enable-examples --enable-tests
./waf build
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.
