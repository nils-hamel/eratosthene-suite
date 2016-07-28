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

    # include "eratosthene-client-geodesy.h"

/*
    source - conversion functions
 */

    le_void_t er_geodesy_cartesian( le_real_t * const er_array, le_size_t const er_count ) {

        /* Optimisation variables */
        le_real_t er_lon = 0.0;
        le_real_t er_lat = 0.0;

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
    source - cell functions
 */

    le_real_t er_geodesy_cell( le_address_t * const er_addr, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Address size variables */
        le_size_t er_size = le_address_get_size( er_addr ) + 1;

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
    source - distance methods
 */

    le_real_t er_geodesy_dist( le_address_t const * const er_cell, le_real_t const er_lon, le_real_t er_lat, le_real_t er_alt ) {

        /* Position array variables */
        le_real_t er_pose1[6] = { 0.0 };
        le_real_t er_pose2[6] = { 0.0, 0.0, 0.0, er_lon, er_lat, er_alt };

        /* Address size variables */
        le_size_t er_size = le_address_get_size( er_cell ) + 1;

        /* Compute planimetric shift */
        le_real_t er_shift = LE_GEODESY_LRAN / pow( 2.0, er_size );

        /* Retrive address position */
        le_address_get_pose( er_cell, er_pose1 + 3 );

        /* Convert position to cartesian coordinates */
        er_pose1[1] = er_pose1[5] + ER_ERA + ( ER_ERA * LE_2P ) / pow( 2.0, er_size );
        er_pose1[0] = er_pose1[1] * cos( er_pose1[4] += er_shift );
        er_pose1[2] = er_pose1[0] * cos( er_pose1[3] += er_shift );
        er_pose1[0] = er_pose1[0] * sin( er_pose1[3] );
        er_pose1[1] = er_pose1[1] * sin( er_pose1[4] );

        /* Convert position to cartesian coordinates */
        er_pose2[1] = er_pose2[5];
        er_pose2[0] = er_pose2[1] * cos( er_pose2[4] );
        er_pose2[2] = er_pose2[0] * cos( er_pose2[3] );
        er_pose2[0] = er_pose2[0] * sin( er_pose2[3] );
        er_pose2[1] = er_pose2[1] * sin( er_pose2[4] );

        /* Compute differences */
        er_pose1[2] -= er_pose2[2];
        er_pose1[0] -= er_pose2[0];
        er_pose1[1] -= er_pose2[1];

        /* Return distance to cell */
        return( sqrt( er_pose1[2] * er_pose1[2] + er_pose1[0] * er_pose1[0] + er_pose1[1] * er_pose1[1] ) );

    }

/*
    source - model functions
 */

    le_real_t er_geodesy_select( le_real_t const er_distance, le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = ( er_altitude - ER_ERA ) / ER_ERA;

        /* Return selection function */
        return( er_altitude * ( 1.0 - 0.75 * exp( - 20.0 * er_normal * er_normal ) ) );

    }

    le_real_t er_geodesy_level( le_real_t const er_distance, le_size_t const er_scale, le_size_t const er_depth ) {

        /* Return level function */        
        //return( 5.0 + ( er_scale - er_depth - 5.0 ) * exp( - 60.0 * ( er_distance / ER_ERA ) ) );

        return( ( er_scale - er_depth ) - log ( er_distance + 1.0 ) / log ( 2.0 ) + 1.0 / log( 2.0 ) );

    }

    le_real_t er_geodesy_distance( le_real_t const er_distance, le_size_t const er_scale_min, le_size_t const er_scale_max ) {

        /* Computation variables */
        le_real_t er_model = er_scale_max - ( log( er_distance + 5.0 ) / log( 2.0 ) ) + log( 5.0 ) / log( 2.0 );

        /* Return scale-distance constraints */
        return( er_model < er_scale_min ? er_scale_min : er_model );

    }

    le_real_t er_geodesy_scale( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = ( er_altitude - ER_ERA ) / ER_ERA;

        /* Return scale factor */
        return( exp( - LE_PI * er_normal * er_normal ) );

    }

    le_real_t er_geodesy_near( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = pow( fabs( er_altitude - ER_ERA ) / ( ER_ERA * 2.0 ), 4 );

        /* Return near plane depth */
        return( 1.0 + ER_ERA * er_normal * er_geodesy_scale( er_altitude ) );

    }

    le_real_t er_geodesy_far( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = er_geodesy_scale( er_altitude );

        /* Return far plane depth */
        return( ( er_altitude - ER_ER2 - ( ER_ERA / 2.5 ) * pow( er_normal, 20 ) ) * er_normal );

    }

