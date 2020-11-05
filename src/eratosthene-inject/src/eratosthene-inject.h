/*
 *  eratosthene-suite - inject
 *
 *      Nils Hamel - nils.hamel@alumni.epfl.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
 *      Copyright (c) 2020 Republic and Canton of Geneva
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
     *  \section license Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2020 DHLAB, EPFL
     *  Copyright (c) 2020 Republic and Canton of Geneva
     *
     *  This program is licensed under the terms of the GNU GPLv3. Documentation    
     *  and illustrations are licensed under the terms of the CC BY 4.0.
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
    # define ER_INJECT ( LE_UV3_CHUNK * LE_ARRAY_DATA )

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

    /*! \brief injection method
     *
     *  This function is responsible of data injection toward the specified
     *  remote server through the provided socket descriptor.
     *
     *  The content of the provided uv3 file is read and written in the socket
     *  toward the remote server. The content is read and send by chunks. The
     *  injection ends as the last chunk of data is sent through the socket.
     *
     *  The provided time value has to be given in UTC seconds through a UNIX
     *  timestamp value.
     *
     *  The execution is blocked during chunks writing on the socket.
     *
     *  \param er_path   uv3 file path
     *  \param er_time   Injection time value - UNIX timestamp
     *  \param er_socket Remote server socket
     *
     *  \return Returns EXIT_SUCCESS on success, EXIT_FAILURE otherwise
     */

    le_enum_t er_inject_uv3( le_char_t const * const er_path, le_time_t er_time, le_sock_t const er_socket );

    /*! \brief main function
     *
     *  The main function is responsible of the injection of the provided model
     *  into the specified remote server :
     *
     *      ./inject --ip/-i --port/-p [remote server address and service]
     *               --time/-t [injection time]
     *               --uv3 [uv3 model file path]
     *
     *  The main function starts by reading the remote server address and
     *  service parameters and creates a connection toward it.
     *
     *  The function then reads and checks the injection model file path before
     *  to call the specialised injection function. The execution is blocked by
     *  the model injection until the entire file is sent to the remote server.
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

