/*
 *  eratosthene-suite - eratosthene indexation server front-end
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016 EPFL CDH DHLAB
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

    /*! \file   eratosthene-isometric.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene isometric projection
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section eratosthene-suite
     *
     *  The _eratosthene-suite_ offers a front-end to the eratosthene indexation
     *  server implemented in the _liberatosthene_ library. It consists in a
     *  software suite implementing front-ends to the functionality of the 
     *  indexation server.
     *
     *  The main software implements the server itself allowing to simply create
     *  instance of it. The suite also offers a front-end software for the data
     *  injection in the servers storage structure. Finally, it also offers a 
     *  graphical client allowing to browse the data contained in the servers
     *  storage structure through a 4 dimensional representation of earth.
     * 
     *  \section Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016 EPFL CDH DHLAB
     *
     *  This program is licensed under the terms of the GNU GPLv3.
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_ISOMETRIC__
    # define __ER_ISOMETRIC__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include <stdio.h>
    # include <stdlib.h>
    # include <stdint.h>
    # include <string.h>
    # include <common-args.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    # define ER_FORMAT " %" _LE_REAL_P \
                       " %" _LE_REAL_P \
                       " %" _LE_REAL_P \
                       " %" _LE_TIME_P \
                       " %" _LE_DATA_P \
                       " %" _LE_DATA_P \
                       " %" _LE_DATA_P \
                       "\n"

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

    /*! \brief query function
     *
     */

    void er_query( le_sock_t const er_socket, le_char_t const * const er_query );

    /*! \brief main function
     *
     *  \param argc Standard parameter
     *  \param argv Standard parameter
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

