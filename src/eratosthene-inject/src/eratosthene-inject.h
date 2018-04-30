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
     *  create instances and maintain services. The suite also offers a
     *  front-end software for the data injection in the available servers. It
     *  also offers a graphical software allowing to browse the 4D worlds
     *  offered by the available servers.
     *
     *  Other tools are also available as the isometric projection creation and
     *  the address and time conversion softwares.
     *
     *  \section project Eratosthene Project
     *
     *  The Eratosthene Project was initially imagined to answer the question of
     *  very large point-based models of cities and environments. Indeed, mapping
     *  a whole city at the level of the centimetre induces very large and heavy
     *  models that are difficult to manipulate, explore and exploit. Moreover,
     *  considering the time dimension implies a quick increase of the size of
     *  the models, making them almost impossible to handle as a single object.
     *
     *  The Eratosthene Project is designed to answer this question by
     *  implementing an space-time indexation formalism able to handle, store
     *  and access very large 4D models of cities and environments. The
     *  indexation formalism is used to implement a server able to manage,
     *  store and broadcast very large 4D models distributed all over the earth
     *  surface and covering large period of time. In addition to the server,
     *  the Eratosthene Project also implements a graphical client able to
     *  communicate with the server and to offer a convenient way of browsing
     *  the models in both space and time.
     *
     *  The space-time indexation formalism is designed to efficiently manage
     *  the storage of the incoming 4D models and to offer and simple and
     *  efficient ways of addressing queries to the server allowing models
     *  broadcasting. The space-time indexation also allows to perform queries
     *  to the server in a intuitive way giving the possibility to both address
     *  the questions of the earth structures levels of detail and time
     *  management. This allows to explore the entire earth surface in terms of
     *  4D structures from large mountain chains in 2000 to very small details
     *  of cities in 1850.
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

    # include <stdio.h>
    # include <stdlib.h>
    # include <common-include.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* define segmentation size */
    # define ER_INJECT ( 621378 * LE_ARRAY_UF3 )

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
     *  It opens and checks the provided uf3. It also prepare the injection
     *  array containing the injection time, needed at each injection segment.
     *  It then starts to read the file content by chunks and sends them to
     *  the remote server using the provided socket.
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

    le_enum_t er_inject_uf3( le_char_t const * const er_path, le_time_t er_time, le_sock_t const er_socket );

    /*! \brief injection method - uf3
     *
     *  This function is responsible of asking the optimisation of the storage
     *  of data pointed by the provided timestamps in the remote server.
     *
     *  The function prepares the optimisation query by packing the provided
     *  time and sends it to the remote server to trigger the time storage unit
     *  optimisation. The function exits immediately after sending the query.
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
     *      ./*-inject --inject/-j, --optimise/-o [execution switch]
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

