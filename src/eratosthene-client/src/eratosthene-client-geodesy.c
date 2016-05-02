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
    source - conversion functions
 */

    le_void_t er_geodesy_cartesian( le_real_t * const er_array, le_size_t const er_count ) {

        /* Computation variables */
        le_real_t er_sinl = 0.0;
        le_real_t er_cosl = 0.0;
        le_real_t er_sina = 0.0;
        le_real_t er_cosa = 0.0;

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Parsing array */
        for ( ; er_parse < er_count; er_parse += 3 ) {

            /* Compute trigonometric values */
            er_sinl = sin( er_array[er_parse    ] );
            er_cosl = cos( er_array[er_parse    ] );
            er_sina = sin( er_array[er_parse + 1] );
            er_cosa = cos( er_array[er_parse + 1] );

            /* Compute and assign cartesian coordinates */
            er_array[er_parse    ] = ( er_array[er_parse + 2] + ER_ERA ) * er_cosa * er_sinl;
            er_array[er_parse + 1] = ( er_array[er_parse + 2] + ER_ERA ) * er_sina;
            er_array[er_parse + 2] = ( er_array[er_parse + 2] + ER_ERA ) * er_cosa * er_cosl;

        }

    }

/*
    source - scale functions
 */

    le_real_t er_geodesy_distance( le_real_t const er_distance, le_size_t const er_scale_min, le_size_t const er_scale_max ) {

        /* Scale saturation management */
        if ( er_distance <= 1.0e0 ) {

            /* Return saturation scale */
            return( er_scale_max );

        } else if ( er_distance >= 1.0e5 ) {

            /* Return saturation scale */
            return( er_scale_min );

        } else {

            /* Return progressive scale */
            return( ( ( 1.0e5 - er_distance ) / 1.0e5 ) * ( le_real_t ) ( er_scale_min - er_scale_max ) );

        }

    }

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

