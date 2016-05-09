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

        /* Optimisation variables */
        le_real_t er_lon = 0.0, er_lat = 0.0;

        /* Parsing array */
        for ( le_size_t er_parse = 0; er_parse < er_count; er_parse += 3 ) {

            /* Push coordinates angles */
            er_lon = er_array[er_parse  ];
            er_lat = er_array[er_parse+1];

            /* Optimised coordinates conversion */
            er_array[er_parse+1] = er_array[er_parse+2] + ER_ERA;
            er_array[er_parse  ] = er_array[er_parse+1] * cos( er_lat );
            er_array[er_parse+2] = er_array[er_parse  ] * cos( er_lon );
            er_array[er_parse  ] = er_array[er_parse  ] * sin( er_lon );
            er_array[er_parse+1] = er_array[er_parse+1] * sin( er_lat );

        }

    }

/*
    source - distance functions
 */

    le_real_t er_geodesy_cell( le_address_t * const er_addr, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Address size variables */
        le_size_t er_size = le_address_get_size( er_addr );

        /* Position array variables */
        le_real_t er_pose[] = { er_lon, er_lat, er_alt, 0.0, 0.0, 0.0 };

        /* Planimetric shift variables */
        le_real_t er_shiftp = ( LE_GEODESY_LMAX - LE_GEODESY_LMIN ) / pow( 2.0, er_size );

        /* Altimetric shift variables */
        le_real_t er_shifta = ( LE_2P * LE_GEODESY_WGS84_A ) / pow( 2.0, er_size );

        /* Compute cell position */
        le_address_get_pose( er_addr, er_pose + 3 );

        /* Compute cell center */
        er_pose[3] += er_shiftp;
        er_pose[4] += er_shiftp;
        er_pose[5] += er_shifta;

        /* Convert position to cartesian coordinates */
        er_geodesy_cartesian( er_pose, 6 );

        /* Optimisation step */
        er_pose[0] = er_pose[0] - er_pose[3];
        er_pose[1] = er_pose[1] - er_pose[4];
        er_pose[2] = er_pose[2] - er_pose[5];

        /* Compute and return distance to cell center */
        return( sqrt( er_pose[0] * er_pose[0] + er_pose[1] * er_pose[1] + er_pose[2] * er_pose[2] ) );

    }

/*
    source - scale functions
 */

    le_real_t er_geodesy_distance( le_real_t const er_distance, le_size_t const er_scale_min, le_size_t const er_scale_max ) {

        /* Computation variables */
        le_real_t er_model = er_scale_max - ( log( er_distance + 25.0 ) / log( 1.82 ) ) + log( 25.0 ) / log( 1.82 );

        /* Return scale-distance constraints */
        return( er_model < er_scale_min ? er_scale_min : er_model );

    }

    le_real_t er_geodesy_depth( le_real_t const er_distance, le_size_t const er_depth_min, le_size_t const er_depth_max ) {

        /* Computation variables */
        le_real_t er_normal = ( er_distance * er_distance ) * 1.28e-7; //6.4e-8;

        /* Return depth-distance constraints */
        return( er_depth_min + ( er_depth_max - er_depth_min ) * exp( - er_normal ) );

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

