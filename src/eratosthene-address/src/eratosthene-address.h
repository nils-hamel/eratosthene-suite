/*
 *  eratosthene-suite - address
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

    /*! \file   eratosthene-address.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - address
     */

    /*! \mainpage eratosthene-suite
     *
     *  \section license Copyright and License
     *
     *  **eratosthene-suite** - Nils Hamel <br >
     *  Copyright (c) 2016-2020 DHLAB, EPFL
     *
     *  This program is licensed under the terms of the GNU GPLv3. Documentation    
     *  and illustrations are licensed under the terms of the CC BY 4.0.
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_ADDRESS__
    # define __ER_ADDRESS__

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

    /* trigonometric conversion factors */
    # define ER_ADDRESS_D2R ( LE_PI / 180.0 )
    # define ER_ADDRESS_R2D ( 180.0 / LE_PI )

    /* display format specifier */
    # define ER_ADDRESS_P   "%" _LE_REAL_P ", %" _LE_REAL_P ", %" _LE_REAL_P "\n"

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

    /*! \brief space conversion methods
     *
     *  This function, that expects the standard main function parameters, reads
     *  the provided geographic coordinates and the scale from them. It then
     *  computes the spatial index obtained using the provided geographical
     *  coordinates. The provided scale is used to determine the spatial index  
     *  length. The index is then converted back to geographical coordinates.
     *
     *  It then displays on the standard output the original coordinates, the
     *  computed spatial index and the coordinates obtained by converting back
     *  the spatial index.
     *
     *  The provided coordinates have to be given in decimal degrees in the
     *  ranges : [-180,+180], [-90,+90]. The heights have to be given in metres
     *  above the WGS84 ellipsoid.
     *
     *  \param argc Standard parameters
     *  \param argv Standard parameters
     */

    le_void_t er_address_space( int argc, char ** argv );

    /*! \brief time conversion methods
     *
     *  This function, that expects the standard main function parameters, reads
     *  the provided date and converts it into a UNIX timestamp. After the
     *  conversion, the function displays the result on the standard output.
     *
     *  The date has to be provided following the pattern : YYYY-MM-DD+hh:mm:ss.
     *
     *  \param argc Standard parameters
     *  \param argv Standard parameters
     */

    le_void_t er_address_date_time( int argc, char ** argv );

    /*! \brief time conversion methods
     *
     *  This function, that expects the standard main function parameters, reads
     *  the provided UNIX timestamp and converts it into a standard date string.
     *  The function then displays both date string and UNIX timestamp.
     *
     *  The computed date follows the pattern : YYYY-MM-DD+hh:mm:ss.
     *
     *  \param argc Standard parameters
     *  \param argv Standard parameters
     */

    le_void_t er_address_time_date( int argc, char ** argv );

    /*! \brief main function
     *
     *  The main function starts by searching in the arguments and parameters
     *  the switch used to determine the execution mode of the software :
     *
     *      ./-address --space/-s --date-time/-t --time-date/-d [switches]
     *                 --time/-s  [UNIX timestamp] 
     *                 --date/-d [standard date string]
     *                 --longitude/-l [geographic coordinates]
     *                 --latitude/-a [geographic coordinates]
     *                 --altitude/-h [geographic coordinates]
     *                 --scale/-c [space index scale]
     *
     *  If the '--space' switch is found, the main function invokes the spatial
     *  index computation function. It expects the 'scale' parameters and a
     *  geographic position through its coordinates. The function converts the
     *  provided coordinates into a spatial index before to display it. the
     *  spatial index is then converted back to geographical coordinates.
     *
     *  If the '--date-time' switch is found, the main function invokes the date
     *  to time conversion execution mode. The provided date is converted into a
     *  UNIX timestamp and displayed.
     *  
     *  If the '--time-date' switch is found, the main function invokes the time
     *  to date conversion execution mode. The provided UNIX timestamp is
     *  converted in standard date string format and displayed.
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

