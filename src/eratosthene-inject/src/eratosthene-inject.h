/*
 *  eratosthene-suite - inject
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

    /*! \file   eratosthene-inject.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - inject
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section overview Overview
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  softwares suite offering front-end solutions to the functionalities of
     *  the eratosthene library.
     *
     *  The principal software implements the server itself allowing to simply
     *  create server instances and to maintain services. The suite also offers
     *  a front-end software for the data injection in the available servers. It
     *  also offers a graphical 4D solution allowing to browse the 4D earth
     *  model offered by the available servers.
     *  
     *  In addition to the server, client data injection and the 4D graphical
     *  client softwares, the suite also provides a tools allowing to massively
     *  query 4D earth cells, following the _liberatosthene_ indexation
     *  formalism, to build large 3D-raster datasets. Such datasets can be used
     *  to train and validate machine learning algorithms working on 3D/4D
     *  geographic information.
     *
     *  \section project Eratosthene Project
     *
     *  The Eratosthene Project was initially imagined to answer the question of
     *  very large point-based models of cities and environments. Indeed,
     *  mapping a whole city at the level of the centimetre induces very large
     *  and heavy models that are difficult to manipulate, explore and exploit.
     *  Moreover, considering the time dimension implies a quick increase of the
     *  size of the models, making them impossible to handle as a single and
     *  monolithic object.
     *
     *  The Eratosthene Project is designed to answer this question by
     *  implementing an space-time indexation formalism able to handle, store
     *  and access very large 4D models of cities and environments. The
     *  indexation formalism is used to implement a server able to manage, store
     *  and broadcast very large 4D models distributed all over the earth
     *  surface and covering large period of time. In addition to the server,
     *  the Eratosthene Project also implements a graphical client able to
     *  communicate with the server and to offer a convenient way of browsing
     *  the models in both space and time.

     *  The space-time indexation formalism is designed to efficiently manage
     *  the storage of the incoming 4D models and to offer and simple and
     *  efficient ways of addressing queries to the server allowing models
     *  broadcasting. The space-time indexation also allows to perform queries
     *  to the server in a intuitive way giving the possibility to both address
     *  the questions of the earth structures level of detail and time
     *  management. This allows to explore the entire earth surface in terms of
     *  4D structures from large mountain chains in 2000 to very small details
     *  of cities in 1850.
     *
     *  The subsequent indexation formalism used to drive the storage and access
     *  of the data also naturally brings a standardised discretisation of space
     *  and time. Models injected in the information system are then
     *  standardised from both spatial and temporal point of view. This allows
     *  the implementation of binary and logical operators when considering a
     *  model at two different times. The operators allows to deduce a third
     *  model providing a differential point of view of the two models separated
     *  in time. The evolution of cities and environments can be enhanced and
     *  analysed in an efficient way, opening the way to more advanced
     *  geographic services.
     *
     *  \section license Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2018 DHLAB, EPFL
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_INJECT__
    # define __ER_INJECT__

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

    /* define segmentation size */
    # define ER_INJECT ( LE_UV3_CHUNK * LE_ARRAY_UV3 )

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

    /*! \brief injection method - uf3
     *
     *  This function is responsible of data injection in the specified server
     *  that come from a file of format uf3 (universal format 3-bytes).
     *
     *  It opens and checks the provided uf3 file. It also prepare the injection
     *  array containing the injection time, needed at each injection segment.
     *  After the function receives the server confirmation array, it starts to
     *  read the file content by chunks and sends them to the remote server
     *  using the provided socket.
     *
     *  The injection time has to be provided in UTC through a UNIX timestamps
     *  format.
     *
     *  \param er_path   UF3 stream path
     *  \param er_time   Injection time
     *  \param er_socket Remote server socket
     *
     *  \return Returns EXIT_SUCCESS on success, EXIT_FAILURE otherwise
     */

    le_enum_t er_inject_uv3_beta( le_char_t const * const er_path, le_time_t er_time, le_sock_t const er_socket );

    le_enum_t er_inject_uf3( le_char_t const * const er_path, le_time_t er_time, le_sock_t const er_socket );

    /*! \brief injection method - optimisation
     *
     *  This function is responsible of asking the optimisation of the storage
     *  of data pointed by the provided timestamps in the remote server.
     *
     *  The function prepares the optimisation query by packing the provided
     *  time and sends it to the remote server to trigger the time storage unit
     *  optimisation. The function waits the server confirmation array before
     *  to exit.
     *
     *  \param er_time   Injection time
     *  \param er_socket Remote server socket
     *
     *  \return Returns EXIT_SUCCESS on success, EXIT_FAILURE otherwise
     */

    le_enum_t er_inject_optimise( le_time_t er_time, le_sock_t const er_socket );

    /*! \brief main function
     *
     *  The main function is responsible of models injection and optimisation in
     *  the remote server :
     *
     *      ./-inject --inject/-j, --optimise/-o [execution switch]
     *                 --ip/-i, --port/-p [remote server access]
     *                 --time/-t [time specification]
     *                 --uf3 [model file path specification]
     *
     *  The main function starts by reading the remote server access elements
     *  and creates a connection toward it.
     *
     *  If the '--inject' execution switch is found, the main function invokes
     *  the specialised injection process. This process expects the time and
     *  model path specification to perform the injection.
     *
     *  If the '--optimise' execution switch is found, the main function invokes
     *  the specialised optimisation function. The procedure only expects a time
     *  specification.
     *
     *  Considering a specified time value and a model path, both injection and
     *  optimisation switch can be provided. The main function starts by the
     *  injection of the model and the triggers the optimisation process in a
     *  single execution. If no execution switch is provided, the connection is
     *  created and immediately closed, doing nothing toward the server.
     *
     *  \param argc Main function parameters
     *  \param argv Main function parameters
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

