## Overview

The _eratosthene-suite_ offers a front-end to the eratosthene indexation server implemented in the _liberatosthene_ library. It consists in a software suite offering front-end solutions to the functionalities of the eratosthene library.

The principal software implements the server itself, allowing to simply create server instances and to maintain network services. The suite also offers a front-end software for the data injection in server instances. It also offers a graphical 4D solution allowing to browse the 4D model of Earth offered by servers.

See the [usage page](USAGE.md) for more information on the usage of the _eratosthene-suite_ and to find complementary information for the compilation of the code on specific platforms.

## Eratosthene Project

The Eratosthene Project was initially designed to address the challenge of very large point-based models of cities and environments. Indeed, mapping a whole city at the level of the centimeter induces very large and heavy models that are difficult to manipulate, explore and exploit. Moreover, considering the time dimension implies a quick increase of the size of the models, making them impossible to handle as a single and monolithic object.

The Eratosthene Project is designed to provide a solution by considering a space-time indexation formalism. This indexation formalism is used to implement a server able to manage, store and broadcast over the network very large 4D models distributed all over the Earth surface and covering large periods of time. In addition to the server, the Eratosthene Project also implements a graphical client able to communicate with the remote servers to offer a convenient way to browse the models in both space and time.

The space-time indexation formalism is designed to efficiently manage the storage of the incoming 4D models and to offer a simple and efficient way of addressing queries to the server to allow models broadcasting. The space-time indexation also allows to perform queries to the server in an intuitive way giving the possibility to both address the challenge of the Earth structures level of details and time management. This allows to explore the entire Earth surface in terms of 4D structures from large mountain chains in 2000 to very small details of cities in 1850.

The subsequent indexation formalism used to drive the storage and access of the data also naturally brings a standardised discretisation of space and time. Point-based models injected in the Eratosthene information system are then standardised from both spatial and temporal point of views. This allows the implementation of binary and logical operators when considering a model at two different times. The operators allow to deduce a third model on the fly providing a differential point of view of the two models separated in time. The evolution of cities and environments can be enhanced and analysed in an efficient way, opening the way to more advanced geographic services.

The ability of indexation formalism to consider polygonal models in addition to large scale point-based models allows to enhance the possibilities in terms of 4D geographic services. More than the possibility to automatically manage the polygonal models level of details and large scale display, the indexation formalism also allows compositions on the fly of mixed models allowing to compare different type of models, from lidar point-based models to land registers.

## Server and Client

The following illustrations give an overview of the interface of the 4D graphical client as it communicates with a demonstration remote server. Different models are made available by this server through previous injection including large scale topographic radar and LIDAR point-based models, large scale dense photogrammetric models along with polygonal and land register models.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-1a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-1b.jpg?raw=true" width="384">
<br />
<i>View of Europa and the Swiss Alps - Data : NASA SRTM topographic model coloured using NASA Visible Earth images</i>
</p>
<br />

The previous images give an example of a large-scale model showing details of the Earth topography. Such models are not able to provide small-scale details, but they cover the entire Earth surface providing a worldwide topographic view of the Earth.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-2a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-2b.jpg?raw=true" width="384">
<br />
<i>View of the city of Geneva - Data : aerial LIDAR campaign conducted by the Geneva SITG in 2009</i>
</p>
<br />

The previous images show an illustration of LIDAR data of Geneva covering the entire Geneva State with a high spatial resolution. It shows how the server is able to ensure the continuity between topographic models and such regional high-resolution models.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-3a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-3b.jpg?raw=true" width="384">
<br />
<i>View of the city of Venice in 2010 - Data : photogrammetric model computed at the DHLAB using IUAV University aerial images, openMVG and openMVS open source solutions</i>
</p>
<br />

In addition to be able to maintain the continuity between models of different scale, the server is also able to sustain the continuity of models regardless of their type. Models coming from topographic radar, LIDAR campaign or from photogrammetric computation can easily coexist and communicate within the same solution. This allows to compare the models, each providing specific information and details.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-4a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-4b.jpg?raw=true" width="384">
<br />
<i>View of the Fontainebleau Castle in 2017 - Data : photogrammetric model computed at the DHLAB using openMVG and openMVS open source solutions</i>
</p>
<br />

The previous images show how the continuity between scales and types can be ensured from global topographic views to very local details. The server is able to provide a view of the Earth from mountain chains to fine details of building structures. When communicating with the server, the 4D client is able to provide a powerful way of browsing through space on large amounts of scales.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-5a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-5b.jpg?raw=true" width="384">
<br />
<i>View of the city of Geneva in 2009 and 2013 - Data : photogrammetric model computed at the DHLAB using SITG aerial images, openMVG and openMVS open source solutions</i>
</p>
<br />

In addition to scales continuity, the server is also able to sustain the continuity of the models through time. The previous images show an example of the same spatial point of view, but at two distinct temporal positions. The Eratosthene Project solution provides a reliable way of browsing through both space and time on large amount of scales and large periods of time.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-6a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-6b.jpg?raw=true" width="384">
<br />
<i>View of the AND logical operator applied on the LIDAR models of Geneva in 2005 and 2009 - Data : aerial LIDAR campaigns conducted by the Geneva SITG in 2005 and 2009</i>
</p>
<br />

These last images show the ability of the server to take advantage of the standardization of space-time to perform on the fly computation of differential models comparing two temporal point of views. On the previous example, the AND operator exhibits black spots that indicates where structures have changed during the specified time interval.

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-7a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-7b.jpg?raw=true" width="384">
<br />
<i>View of a volumetric polygonal of the city of Geneva in 2018 and the land register of the city of Chaud-de-Fonds in 2004 - Data : SITG building model and SITN land register</i>
</p>
<br />

The previous illustrations show examples of land register and polygonal models injected in the demonstration server. The land register model is crossed with a digital elevation model to assign height to the representation. This allows such data to be compared with each other withing a single interface, regardless of the model type and location in time. The next two images shows example of mixed model composition for comparison purpose :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-8a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-8b.jpg?raw=true" width="384">
<br />
<i>View of mixed models of the city of Chaud-de-Fonds : 2004 land register and 2018 volumetric model (left) 2004 land register and 2016 colored lidar model (right)</i>
</p>
<br />

The Eratosthene Project solution provides a network based solution allowing to manage large models of different scale and type representing the reality at different epochs within a unified interface. It ensures a fluid communication and continuity between the models offering a way to efficiently browse the Earth through space and time.

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

* liberatosthene 1.1

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
