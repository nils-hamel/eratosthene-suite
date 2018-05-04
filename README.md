## Overview

The _eratosthene-suite_ offers a front-end to the eratosthene indexation server implemented in the _liberatosthene_ library. It consists in a softwares suite offering front-end solutions to the functionalities of the eratosthene library.

The principal software implements the server itself allowing to simply create server instances and to maintain services. The suite also offers a front-end software for the data injection in the available servers. It also offers a graphical 4D solution allowing to browse the 4D earth model offered by the available servers.

In addition to the server, client data injection and the 4D graphical client softwares, the suite also provides a tools allowing to massively query 4D earth cells, following the _liberatosthene_ indexation formalism, to build large 3D-raster datasets. Such datasets can be used to train and validate machine learning algorithms working on 3D/4D geographic information.

## Eratosthene Project

The Eratosthene Project was initially imagined to answer the question of very large point-based models of cities and environments. Indeed, mapping a whole city at the level of the centimetre induces very large and heavy models that are difficult to manipulate, explore and exploit. Moreover, considering the time dimension implies a quick increase of the size of the models, making them impossible to handle as a single and monolithic object.

The Eratosthene Project is designed to answer this question by implementing an space-time indexation formalism able to handle, store and access very large 4D models of cities and environments. The indexation formalism is used to implement a server able to manage, store and broadcast very large 4D models distributed all over the earth surface and covering large period of time. In addition to the server, the Eratosthene Project also implements a graphical client able to communicate with the server and to offer a convenient way of browsing the models in both space and time.

The space-time indexation formalism is designed to efficiently manage the storage of the incoming 4D models and to offer and simple and efficient ways of addressing queries to the server allowing models broadcasting. The space-time indexation also allows to perform queries to the server in a intuitive way giving the possibility to both address the questions of the earth structures level of detail and time management. This allows to explore the entire earth surface in terms of 4D structures from large mountain chains in 2000 to very small details of cities in 1850.

The subsequent indexation formalism used to drive the storage and access of the data also naturally brings a standardised discretisation of space and time. Models injected in the information system are then standardised from both spatial and temporal point of view. This allows the implementation of binary and logical operators when considering a model at two different times. The operators allows to deduce a third model providing a differential point of view of the two models separated in time. The evolution of cities and environments can be enhanced and analysed in an efficient way, opening the way to more advanced geographic services.

## Server and Client

The following illustrations give an overview of the interface of the 4D graphical client as it communicates with a demonstration remote server hosted by the DHLAB. Different models have been injected in the demonstration server including large scale models such as SRTM and LIDAR along with photogrammetric models computed at the laboratory using both aerial and ground based photographies.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-1a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-1b.jpg?raw=true" width="384">
<br />
<i>View of Europa and the Swiss Alps - The data come from the NASA SRTM topographic model coloured using NASA Visible Earth images</i>
</p>
<br />

The previous images give an example of a large scale model showing details of the earth topography. Such models are not able to provided small scale details but they cover the entire earth surface providing a world wide topographic view of the earth.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-2a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-2b.jpg?raw=true" width="384">
<br />
<i>View of the city of Geneva - The data come from an aerial LIDAR campaign conducted by the Geneva SITG in 2009</i>
</p>
<br />

The previous images show an illustration of LIDAR data of Geneva covering the entire Geneva State with a high spatial resolution. It shows how the server is able to store and give access to medium scale models and how it is able to ensure the continuity between topographic models and such regional high resolution models.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-3a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-3b.jpg?raw=true" width="384">
<br />
<i>View of the city of Venice in 2010 - The data come from a photogrammetric model computed at the DHLAB using IUAV University aerial images, openMVG and openMVS open source solutions</i>
</p>
<br />

In addition to be able to maintain the continuity between models of different scale, the server is also able to sustain the continuity of models regardless of their type. Models coming from topographic radar, LIDAR campaign or from photogrammetric computation on images can easily coexist and communicate within the same solution. This allows to compare the models, each providing specific information and details.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-4a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-4b.jpg?raw=true" width="384">
<br />
<i>View of the Fontainebleau Castle in 2017 - The data come from a photogrammetric model computed at the DHLAB using openMVG and openMVS open source solutions</i>
</p>
<br />

The previous images show how the continuity between models scale and type can be ensured from global topographic models to very local models. The server is able to provided a view of the earth from mountain chains to fine details of building structures. When communicating with the server, the 4D client is able to provide a powerful way of browsing through space on a large amount of scales.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-5a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-5b.jpg?raw=true" width="384">
<br />
<i>View of the city of Geneva in 2009 and 2013 - The data come from a photogrammetric model computed at the DHLAB using SITG aerial images, openMVG and openMVS open source solutions</i>
</p>
<br />

In addition to scales continuity, the server is also able to sustain the continuity of the models through time. The previous images shows an example of the same spatial point of view but at two distinct temporal positions. The _eratosthene_ solution provides a reliable way of browsing through both space and time on large amount of scales and large periods of time.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-6a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-6b.jpg?raw=true" width="384">
<br />
<i>View of the AND logical operator applied on the LIDAR models of Geneva in 2005 and 2009 - The data come from aerial LIDAR campaigns conducted by the Geneva SITG</i>
</p>
<br />

These last images show the ability of the server to take advantage of the standardisation of space-time to perform on the fly computation of differential models comparing two times. On the previous example, the AND operator exhibit black spots that indicates where structures have changed during the specified period of time.

The _eratosthen_ solution provides a network based solution allowing to manage large models of different scale and type representing the reality at different epochs within a single solution. It ensures a fluid communication and continuity between the models offering a way to efficiently browse the earth through space and time.

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

* liberatosthene 1.0

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
