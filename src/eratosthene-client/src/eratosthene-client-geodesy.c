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

    le_real_t er_geodesy_distance( le_address_t const * const er_cell, le_real_t const er_lon, le_real_t er_lat, le_real_t er_alt ) {

        /* Static array variables */
        static le_real_t er_pose[12] = { 0.0 };

        /* Scale half variables */
        le_real_t er_scale = _LE_SIZE_L( 1 ) << ( le_address_get_size( er_cell ) + 1 );;

        /* Scale shift variables */
        le_real_t er_shift = LE_GEODESY_LRAN / er_scale;

        /* Initialise array elements */
        er_pose[ 3] = 0.0, er_pose[ 4] = 0.0, er_pose[ 5] = 0.0;

        /* Initialise array elements */
        er_pose[ 9] = er_lon, er_pose[10] = er_lat, er_pose[11] = er_alt;

        /* Retrieve address position */
        le_address_get_pose( er_cell, er_pose + 3 );

        /* Convert position to cartesian coordinates */
        er_pose[1] = er_pose[ 5] + ER_ERA + ( ER_ERA * LE_2P ) / er_scale;
        er_pose[0] = er_pose[ 1] * cos( er_pose[ 4] += er_shift );
        er_pose[2] = er_pose[ 0] * cos( er_pose[ 3] += er_shift );
        er_pose[0] = er_pose[ 0] * sin( er_pose[ 3] );
        er_pose[1] = er_pose[ 1] * sin( er_pose[ 4] );

        /* Convert position to cartesian coordinates */
        er_pose[7] = er_pose[11];
        er_pose[6] = er_pose[ 7] * cos( er_pose[10] );
        er_pose[8] = er_pose[ 6] * cos( er_pose[ 9] );
        er_pose[6] = er_pose[ 6] * sin( er_pose[ 9] );
        er_pose[7] = er_pose[ 7] * sin( er_pose[10] );

        /* Compute differences */
        er_pose[2] -= er_pose[8], er_pose[0] -= er_pose[6], er_pose[1] -= er_pose[7];

        /* Return distance */
        return( sqrt( er_pose[2] * er_pose[2] + er_pose[0] * er_pose[0] + er_pose[1] * er_pose[1] ) );

    }

/*
    source - model methods
 */

    le_real_t er_geodesy_limit( le_real_t const er_distance, le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = er_altitude / ER_ERA - 1.0;

        /* Return evaluation */
        return( er_altitude * ( 1.0 - 0.75 * exp( - LE_2P * er_normal * er_normal ) ) );

    }

    le_real_t er_geodesy_depth( le_real_t const er_distance, le_size_t const er_scale, le_size_t const er_depth ) {

        /* Return evaluation */
        return( ( er_scale - er_depth ) - log ( er_distance + 1.0 ) / log ( 2.0 ) + 1.0 / log( 2.0 ) );

    }

    le_real_t er_geodesy_scale( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = er_altitude / ER_ERA - 1.0;

        /* Return evaluation */
        return( exp( - LE_PI * er_normal * er_normal ) );

    }

    le_real_t er_geodesy_near( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = pow( fabs( er_altitude - ER_ERA ) / ( ER_ERA * 2.0 ), 4 );

        /* Return evaluation */
        return( ER_ERA * er_normal * er_geodesy_scale( er_altitude ) + 1.0 );

    }

    le_real_t er_geodesy_far( le_real_t const er_altitude ) {

        /* Computation variables */
        le_real_t er_normal = er_geodesy_scale( er_altitude );

        /* Return evaluation */
        return( ( er_altitude - ER_ER2 - ( ER_ERA / 2.5 ) * pow( er_normal, 20 ) ) * er_normal );

    }

