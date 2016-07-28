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
        return( ( er_scale - er_depth ) - log ( er_distance + 1.0 ) / log ( 2.0 ) + 1.0 / log( 2.0 ) );

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

