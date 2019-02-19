/*
 *  eratosthene-suite - common library
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2019 DHLAB, EPFL
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

    /*! \file   common-time.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - common library - time
     */

/*
    header - inclusion guard
 */

    # ifndef __LC_TIME__
    # define __LC_TIME__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "common.h"

/*
    header - external includes
 */

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

    /*! \brief conversion methods
     *
     *  This function converts the provided UNIX timestamp into human readable
     *  string. The format of the created string follows : YYYY-MM-DD-hh-mm-ss.
     *
     *  \param lc_time   UNIX timestamp
     *  \param lc_string Composed string
     *  \param lc_limit  Maximum length of composed string
     *
     *  \return Returns pointer to composed string
     */

    unsigned char * lc_time_to_string( time_t const lc_time, unsigned char * const lc_string, size_t const lc_limit );

    /*! \brief conversion methods
     *
     *  This function converts the provided human readable date string into a
     *  UNIX timestamp. The format of the date string has to be given to the
     *  function. See strptime() for more information.
     *
     *  \param lc_date   Date string
     *  \param lc_format Date string format
     *
     *  \return Returns converted UNIX timestamp
     */

    time_t lc_time_from_string( unsigned char * const lc_date, unsigned char * const lc_format );

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

