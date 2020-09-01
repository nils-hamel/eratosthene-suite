## Overview

The [_liberatosthene_](https://github.com/nils-hamel/liberatosthene) and _eratosthene-suite_ offer an four dimensional indexation framework for large scale three dimensional models. It allows to organize, store and broadcast network access to large scale models referenced in both space and time in an efficient manner.

Initially, the framework solved the problem of large scale point-based models management and visualization. As _LIDAR_ and _structure from motion_ being able to produce large point-based models, their storage and exploitation become a major challenge with the increase of their size. In addition, these technologies also allow to create models of the same location at different times increasing the data management problem.

To solve the initial challenge, the _eratosthene_ framework implemented an _Earth-attached_ indexation formalism for point based models. This allows to browse arbitrary large models through a simple series of multi-scale and time-attached queries. On the following image, a graphical view created with the framework shows the ability of managing large scale models such as the _SRTM_ topographic model weighting around _500_ Gio as a monolithic object :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-1a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-1b.jpg?raw=true" width="384">
<br />
<i>View of Europa and the Swiss Alps - Data : NASA SRTM topographic model colored using NASA Visible Earth images</i>
</p>
<br />

Another example of large scale model being able to be efficiently stored and accessed through the _eratosthene_ framework are the following _LIDAR_ model of the _Geneva_ state weighting up to _256_ Gio as a monolithic object :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-2a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-2b.jpg?raw=true" width="384">
<br />
<i>View of the city of Geneva - Data : aerial LIDAR campaign conducted by the Geneva SITG in 2009</i>
</p>
<br />

Of course, any type of point-based models can be considered through the _eratosthene_ framework as for example model computed from images and _strcutrue from motion_ :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-3a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-3b.jpg?raw=true" width="384">
<br />
<i>View of the city of Venice in 2010 - Data : photogrammetric model computed at the DHLAB using IUAV University aerial images, openMVG and openMVS open source solutions</i>
</p>
<br />

The indexation formalism implemented initially for the point-based models allows to deal with model coming with any type of scale, from large scale topographic models to very specifically located models in order to ensure a continuity in the space dimensions :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-4a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-4b.jpg?raw=true" width="384">
<br />
<i>View of the Fontainebleau Castle in 2017 - Data : photogrammetric model computed at the DHLAB using openMVG and openMVS open source solutions</i>
</p>
<br />

The implemented indexation formalism is then able to store and broadcast wide variety of models through a single and unified formalism creating a spatial and temporal continuity between the models. An example of temporal continuity is shown on the following images showing the same location at different position in time :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-5a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-5b.jpg?raw=true" width="384">
<br />
<i>View of the city of Geneva in 2009 and 2013 - Data : photogrammetric model computed at the DHLAB using SITG aerial images, openMVG and openMVS open source solutions</i>
</p>
<br />

In addition, the indexation formalism applied on point based models offer a natural spatio-temporal grid on which specific operations can be easily performed. For example, logical convolutions can be computed on the fly on point-based models. This allows for example to apply an _AND_ logical operator between two different positions in time to show only the elements identical on both time as illustrated on the following images :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-6a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-6b.jpg?raw=true" width="384">
<br />
<i>View of the AND logical operator applied on the LIDAR models of Geneva in 2005 and 2009 - Data : aerial LIDAR campaigns conducted by the Geneva SITG in 2005 and 2009</i>
</p>
<br />

In a second evolution, the framework and indexation formalism were adapted to be able to ingest, store and broadcast all types of graphical primitives. The temporal and spatial multi-scale architecture were then extended to take into account such type of primitives. As a result, arbitrary large vector models can be handled by the framework :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-7a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-7b.jpg?raw=true" width="384">
<br />
<i>View of a volumetric polygonal of the city of Geneva in 2018 and the land register of the city of Chaux-de-Fonds in 2004 - Data : SITG building model and SITN land register</i>
</p>
<br />

This evolution lead to a much richer framework able to ensure the continuity between space, time and the different types of model. As an example, the following image shows example of communication between different type of data such as vector models and point-based models :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-8a.jpg?raw=true" width="384">
&nbsp;
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image-8b.jpg?raw=true" width="384">
<br />
<i>View of mixed models of the city of Chaux-de-Fonds : 2004 land register and 2018 volumetric model (left) 2004 land register and 2016 colored lidar model (right) - Data : SITN</i>
</p>
<br />

The _eratosthene-suite_ repository stores the front-end elements of the _eratosthene_ framework while the [_liberatosthene_](https://github.com/nils-hamel/liberatosthene) repository hold the library implementing the details of the framework and its generalized _Earth-attached_ space-time indexation formalism.

## eratosthene-suite

Each implemented tool of the suite comes with its own documentation that explains the implemented solution and gives examples of usages and results :

_Server & data injection_

 * [Server instances](src/eratosthene-server)
 * [Models injection](src/eratosthene-inject)

_Graphical client_

 * [Graphical interface](src/eratosthene-client)

_Auxiliary tools_

 * [Times, dates, position and index conversion](src/eratosthene-address)

A detailed documentation of specific file formats used by the tools of this suite can be found of the [format page](FORMAT.md).

## Copyright and License

**eratosthene-suite** - Nils Hamel <br >
Copyright (c) 2016-2020 DHLAB, EPFL

This program is licensed under the terms of the GNU GPLv3. Documentation and illustrations are licensed under the terms of the CC BY 4.0.

## Dependencies

The _eratosthene-suite_ comes with the following package (Ubuntu 20.04 LTS) dependencies ([Instructions](DEPEND.md)) :

* build-essential
* mesa-common-dev
* libsdl2-dev
* libpng-dev

and the following external dependencies provided as sub-modules ([Instructions](DEPEND.md)) :

* liberatosthene 1.3

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
