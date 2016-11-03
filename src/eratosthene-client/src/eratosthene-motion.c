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

    # include "eratosthene-motion.h"

/*
    source - constructor/destructor methods
 */

    er_motion_t er_motion_create( le_void_t ) {

        /* created structure variables */
        er_motion_t er_motion = ER_MOTION_C;

        /* return created structure */
        return( er_motion );

    }

    le_void_t er_motion_delete( er_motion_t * er_motion ) {

        /* deleted structure variables */
        er_motion_t er_delete = ER_MOTION_C;

        /* delete structure */
        ( * er_motion ) = er_delete;

    }

/*
    source - accessor methods
 */

    le_enum_t er_motion_get( er_motion_t const * const er_motion ) {

        /* checks stack size - return answer */
        return( ( er_motion->mt_push > 1 ) ? _LE_TRUE : _LE_FALSE );

    }

