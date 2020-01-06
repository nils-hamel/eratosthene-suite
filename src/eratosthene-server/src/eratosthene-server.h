/*
 *  eratosthene-suite - server
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

    /*! \file   eratosthene-server.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - server
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section overview Overview
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  software suite offering front-end solutions to the functionalities of
     *  the eratosthene library.
     *
     *  The principal software implements the server itself, allowing to simply
     *  create server instances and to maintain network services. The suite also
     *  offers a front-end software for the data injection in server instances.
     *  It also offers a graphical 4D solution allowing to browse the 4D model
     *  of Earth offered by servers.
     *
     *  \section project Eratosthene Project
     *
     *  The Eratosthene Project was initially designed to address the challenge
     *  of very large point-based models of cities and environments. Indeed,
     *  mapping a whole city at the level of the centimeter induces very large
     *  and heavy models that are difficult to manipulate, explore and exploit.
     *  Moreover, considering the time dimension implies a quick increase of the
     *  size of the models, making them impossible to handle as a single and
     *  monolithic object.
     *
     *  The Eratosthene Project is designed to provide a solution by considering
     *  a space-time indexation formalism. This indexation formalism is used to
     *  implement a server able to manage, store and broadcast over the network
     *  very large 4D models distributed all over the earth surface and covering
     *  large periods of time. In addition to the server, the Eratosthene
     *  Project also implements a graphical client able to communicate with the
     *  remote servers to offer a convenient way to browse the models in both
     *  space and time.
     *
     *  The space-time indexation formalism is designed to efficiently manage
     *  the storage of the incoming 4D models and to offer a simple and
     *  efficient way of addressing queries to the server to allow models
     *  broadcasting. The space-time indexation also allows to perform queries
     *  to the server in an intuitive way giving the possibility to both address
     *  the challenge of the earth structures level of details and time
     *  management. This allows to explore the entire earth surface in terms of
     *  4D structures from large mountain chains in 2000 to very small details
     *  of cities in 1850.
     *
     *  The subsequent indexation formalism used to drive the storage and access
     *  of the data also naturally brings a standardised discretisation of space
     *  and time. Point-based models injected in the Eratosthene information
     *  system are then standardised from both spatial and temporal point of
     *  views. This allows the implementation of binary and logical operators
     *  when considering a model at two different times. The operators allow to
     *  deduce a third model on the fly providing a differential point of view
     *  of the two models separated in time. The evolution of cities and
     *  environments can be enhanced and analysed in an efficient way, opening
     *  the way to more advanced geographic services.
     *
     *  The ability of indexation formalism to consider polygonal models in
     *  addition to large scale point-based models allows to enhance the
     *  possibilities in terms of 4D geographic services. More than the
     *  possibility to automatically manage the polygonal models level of
     *  details and large scale display, the indexation formalism also allows
     *  compositions on the fly of mixed models allowing to compare different
     *  type of models, from lidar point-based models to land registers.
     *
     *  \section license Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2020 DHLAB, EPFL
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_SERVER__
    # define __ER_SERVER__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <eratosthene-include.h>
    # include <common-include.h>
    # include <stdio.h>
    # include <stdlib.h>

/*
    header - preprocessor definitions
 */

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

    /*! \brief main function
     *
     *  The main function instance an eratosthene server that points on the
     *  provided storage structure path :
     *
     *      ./-server --path/-p [storage structure path]
     *                --port/-t [service port]
     *
     *  After arguments and parameters reading, the main function creates the
     *  server main structure. The server main instance function is then called
     *  to establish the services. The services are maintained by the called
     *  specialised function.
     *
     *  As the main server instance exits, the main function deletes the server
     *  structure and exits.
     *
     *  \param argc Standard parameters
     *  \param argv Standard parameters
     *
     *  \return Standard exit code
     */

    int main( int argc, char ** argv );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

