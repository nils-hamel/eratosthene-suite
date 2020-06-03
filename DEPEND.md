## Packages and external dependencies installation

The following sections give the installation instructions for the required distribution packages and external dependencies for the supported platforms.

## Required distribution packages

### Ubuntu 16.04 LTS

```
sudo apt-get install build-essential mesa-common-dev libsdl2-dev libpng12-dev doxygen
```

### MacOS

The dependencies are here installed through the _homebrew_ tool.

```
brew install gcc
brew install make
brew install libomp
brew install sdl2
brew install libpng
```

It is assumed here that _OpenMP_ and _SLD2_ are installed through the _homebrew_ tool :

    $ brew install libomp
    $ brew install sdl2
    $ brew install libpng

If these dependencies are installed as a framework, it is necessary to change the line _MAKE_FLNK_ (39) of the _eratosthene-client_ software _Makefile_. Considering _SDL2_ installed as a framework, the correction is :

    -lSDL2 to -framework SDL2

and the same for the other dependencies.

### liberatosthene

Fulfill the requirements following the [instructions](https://github.com/nils-hamel/liberatosthene).