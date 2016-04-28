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

    # include "eratosthene-client-geodesy.h"

/*
    source - scale functions
 */

    le_real_t er_geodesy_scale( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = fabs( ( er_altitude - LE_GEODESY_WGS84_A ) / LE_GEODESY_WGS84_A );

        /* Return scale factor */
        return( 1.0 - 0.999 * er_normal );

    }

    le_real_t er_geodesy_near( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_eval = 1.0 + 9999.0 * pow( 1.0 - er_geodesy_scale( er_altitude ), LE_P2 );

        /* Return near plane depth */
        return( er_eval * er_geodesy_scale( er_altitude ) );

    }

    le_real_t er_geodesy_far( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_eval = ( er_altitude - LE_GEODESY_WGS84_A );

        /* Compute absorbtion component */
        er_eval = 1.0 - 0.75 * exp( - ( er_eval * er_eval ) / 1.7719e+12 );

        /* Return far plane depth */
        return( fabs( er_altitude - ER_ER2 ) * er_eval * er_geodesy_scale( er_altitude ) );

    }

