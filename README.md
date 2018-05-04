## Overview

The _eratosthene-suite_ offers a front-end to the eratosthene indexation server implemented in the _liberatosthene_ library. It consists in a softwares suite offering front-end solutions to the functionalities of the eratosthene library.

The principal software implements the server itself allowing to simply create instances and maintain services. The suite also offers a front-end software for the data injection in the available servers. It also offers a graphical software allowing to browse the 4D worlds offered by the available servers.

Other tools are also available as the raster from address software that allows to build 3D raster from the server answer to a query address. Time and address conversion tools are also included in the software suite.

## Eratosthene Project

The Eratosthene Project was initially imagined to answer the question of very large point-based models of cities and environments. Indeed, mapping a whole city at the level of the centimetre induces very large and heavy models that are difficult to manipulate, explore and exploit. Moreover, considering the time dimension implies a quick increase of the size of the models, making them impossible to handle as a single and monolithic object.

The Eratosthene Project is designed to answer this question by implementing an space-time indexation formalism able to handle, store and access very large 4D models of cities and environments. The indexation formalism is used to implement a server able to manage, store and broadcast very large 4D models distributed all over the earth surface and covering large period of time. In addition to the server, the Eratosthene Project also implements a graphical client able to communicate with the server and to offer a convenient way of browsing the models in both space and time.

The space-time indexation formalism is designed to efficiently manage the storage of the incoming 4D models and to offer and simple and efficient ways of addressing queries to the server allowing models broadcasting. The space-time indexation also allows to perform queries to the server in a intuitive way giving the possibility to both address the questions of the earth structures level of detail and time management. This allows to explore the entire earth surface in terms of 4D structures from large mountain chains in 2000 to very small details of cities in 1850.

The subsequent indexation formalism used to drive the storage and access of the data also naturally brings a standardised discretisation of space and time. Models injected in the information system are then standardised from both spatial and temporal point of view. This allows the implementation of binary and logical operators when considering a model at two different times. The operators allows to deduce a third model providing a differential point of view of the two models separated in time. The evolution of cities and environments can be enhanced and analysed in an efficient way, opening the way to more advanced geographic services.

## Server and Client

The following illustrations give an overview of the interface of the graphical client as it communicates with an experimental server hosted by the DHLAB. The following image gives an illustration of the view offered by the graphical client looking at the whole Europa from a very high altitude :

<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-0.jpg?raw=true" width="512">
<br />
<i>View of Europa using eratosthene-suite graphical client</i>
</p>

At such position and time, the server provides a topographic model built using the SRTM data it holds. The following image shows a view of the Swiss Alps looking in the direction of Geneva city :

<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-1.jpg?raw=true" width="512">
<br />
<i>View of Swiss Alps using eratosthene-suite graphical client</i>
</p>

Again, the information provided by the server to the graphical client are mainly based on the SRTM according to the position in space and time of the point of view. The following image shows a closer view of the Geneva city with a position in time in the year 2009 :

<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-2.jpg?raw=true" width="512">
<br />
<i>View the city of Geneva using eratosthene-suite graphical client</i>
</p>

In this case, the model is dominantly made of points coming from the SITG lidar model of the city of Geneva and that has been injected in the remote server. Finally, this last illustration shows a view of the city Venice using a point of view in time in the year 2010 :

<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-3.jpg?raw=true" width="512">
<br />
<i>View of the city of Venice using eratosthene-suite graphical client</i>
</p>

On this last image, the model provided by the remote server is mainly composed of points coming from a photogrammetric model of the city of Venice computed at the DHLAB using aerial images.

## Copyright and License

**eratosthene-suite** - Nils Hamel <br >
Copyright (c) 2016-2018 DHLAB, EPFL

This program is licensed under the terms of the GNU GPLv3.

## Dependencies

The _eratosthene-suite_ comes with the following package dependencies (Ubuntu 16.04 LTS) :

* build-essential
* mesa-common-dev
* libsdl2-dev

and the following external dependencies provided as sub-modules :

* liberatosthene 0.3.2

The code documentation is built using Doxygen.

## Compilation

To build the project, including the sub-modules, use make with the following targets :

    $ make clean-all && make all

To rebuild the binaries without rebuilding sub-modules, use the make targets :

    $ make clean && make build

To only rebuild sub-modules, use the make command with the targets :

    $ make clean-module && make module

To generate the documentation of the project, use the targets :

    $ make clean-doc && make doc

and the targets :

    $ make clean-all-doc && make all-doc

to generate the documentation of the project and its sub-modules.
