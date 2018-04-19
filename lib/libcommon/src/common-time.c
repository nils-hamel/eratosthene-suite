/*
 *  eratosthene-suite - common library
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

    # include "common-time.h"

/*
    source - conversion methods
 */

    unsigned char * lc_time_to_string( time_t const lc_time, unsigned char * const lc_string, size_t const lc_limit ) {

        /* time decomposition variable */
        struct tm lc_struct = * gmtime( & lc_time );

        /* compose date string */
        strftime( ( char * ) lc_string, lc_limit, "%Y-%m-%d+%H:%M:%S", & lc_struct );

        /* return filled string */
        return( lc_string );

    }

    time_t lc_time_from_string( unsigned char * const lc_date, unsigned char * const lc_format ) {

        /* time structure variable */
        struct tm lc_time;

        /* convert string into time structure */
        strptime( ( char * ) lc_date, ( char * ) lc_format, & lc_time );

        /* convert time structure in timestamps */
        return(  mktime( & lc_time ) );

    }

