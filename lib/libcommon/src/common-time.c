/*
 *  eratosthene-suite - geodetic system
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

    # include "common-image.h"

/*
    source - conversion methods
 */

    unsigned char * lc_time_to_string( time_t const lc_time, unsigned char * const lc_string, size_t const lc_limit ) {

        /* Time decomposition variable */
        struct tm lc_struct = * gmtime( & lc_time );

        /* Compose date string */
        strftime( ( char * ) lc_string, lc_limit, "%Y/%m/%d %H%M%S", & lc_struct );

        /* Return filled string */
        return( lc_string );

    }

