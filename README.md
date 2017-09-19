## Overview

The _eratosthene-suite_ offers a front-end to the eratosthene indexation server implemented in the _liberatosthene_ library. It consists in a softwares suite offering front-end solutions to the functionalities of the eratosthene library.

The principal software implements the server itself allowing to simply create instances and maintain services. The suite also offers a front-end software for the data injection in the available servers. It also offers a graphical software allowing to browse the 4D worlds offered by the available servers.

Other tools are also available as the isometric projection creation and the address and time conversion softwares.

## Eratosthene Project

The Eratosthene Project was initially imagined to answer the question of very large point-based models of cities and environments. Indeed, mapping a whole city at the level of the centimetre induces very large and heavy models that are difficult to manipulate, explore and exploit. Moreover, considering the time dimension implies a quick increase of the size of the models, making them almost impossible to handle as a single object.

The Eratosthene Project is designed to answer this question by implementing an space-time indexation formalism able to handle, store and access very large 4D models of cities and environments. The indexation formalism is used to implement a server able to manage, store and broadcast very large 4D models distributed all over the earth surface and covering large period of time. In addition to the server, the Eratosthene Project also implements a graphical client able to communicate with the server and to offer a convenient way of browsing the models in both space and time.

The space-time indexation formalism is designed to efficiently manage the storage of the incoming 4D models and to offer and simple and efficient ways of addressing queries to the server allowing models broadcasting. The space-time indexation also allows to perform queries to the server in a intuitive way giving the possibility to both address the questions of the earth structures levels of detail and time management. This allows to explore the entire earth surface in terms of 4D structures from large mountain chains in 2000 to very small details of cities in 1850.

## Copyright and License

**eratosthene-suite** - Nils Hamel <br >
Copyright (c) 2016-2017 DHLAB, EPFL

This program is licensed under the terms of the GNU GPLv3.

## Dependencies

The _eratosthene-suite_ comes with the following package dependencies (Ubuntu 16.04 LTS) :

* build-essential
* mesa-common-dev
* libglew-dev
* freeglut3-dev
* libpng12-dev

and the following external dependencies provided as sub-modules :

* liberatosthene 0.3.1

The code documentation is built using Doxygen.

## Compilation

To build sub-modules and binaries with make, use the command :

    $ make clean-all && make

To rebuild the binaries without rebuilding sub-modules :

    $ make clean && make build

To rebuild sub-modules only, use the command :

    $ make clean-modules && make modules

To generate the documentation, use the command :

    $ make clean-documentation && make documentation
