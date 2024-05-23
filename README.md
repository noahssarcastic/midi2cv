# midi2cv

## Requirements

Follow instructions at https://github.com/electro-smith/DaisyWiki/wiki/1.-Setting-Up-Your-Development-Environment.

## Install

Download and compile dependencies (only needs to be done once or if updating libs):

```sh
git submodule update --init --recursive
(cd libDaisy && make clean && make)
(cd DaisySP && make clean && make)
```

Build and install project:

```sh
make clean && make
# hold BOOT down, and press RESET
make program-dfu
```
