/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@alumni.epfl.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
 *      Copyright (c) 2020 Republic and Canton of Geneva
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

    le_real_t er_geodesy_distance( le_address_t const * const er_cell, er_view_t const * const er_view ) {

        /* computation array variables */
        le_real_t er_pose[12] = { 0.0 };

        /* half-cell distance variables */
        le_real_t er_scale = ( le_real_t ) ( 1 << ( le_address_get_size( er_cell ) + 1 ) );

        /* half-cell shift variables */
        le_real_t er_shift = LE_ADDRESS_RAN_L / er_scale;

        /* retrieve address edge */
        le_address_get_pose( er_cell, er_pose + 3 );

        /* retrieve view position */
        er_view_get_pose( er_view, er_pose + 9 );

        /* convert cell position - ellipsoidal to cartesian */
        er_pose[1] = er_pose[ 5] + LE_ADDRESS_WGS_A + ( LE_ADDRESS_WGS_A * LE_2P ) / er_scale;
        er_pose[0] = er_pose[ 1] * cos( er_pose[ 4] += er_shift );
        er_pose[2] = er_pose[ 0] * cos( er_pose[ 3] += er_shift );
        er_pose[0] = er_pose[ 0] * sin( er_pose[ 3] );
        er_pose[1] = er_pose[ 1] * sin( er_pose[ 4] );

        /* convert view position - ellipsoidal to cartesian */
        er_pose[7] = er_pose[11];
        er_pose[6] = er_pose[ 7] * cos( er_pose[10] );
        er_pose[8] = er_pose[ 6] * cos( er_pose[ 9] );
        er_pose[6] = er_pose[ 6] * sin( er_pose[ 9] );
        er_pose[7] = er_pose[ 7] * sin( er_pose[10] );

        /* compute dimensional differences */
        er_pose[2] -= er_pose[8], er_pose[0] -= er_pose[6], er_pose[1] -= er_pose[7];

        /* return computed distance */
        return( sqrt( er_pose[2] * er_pose[2] + er_pose[0] * er_pose[0] + er_pose[1] * er_pose[1] ) );

    }

/*
    source - geodetic methods
 */

    le_real_t er_geodesy_angle( le_real_t const er_altitude ) {

        /* computation variable */
        le_real_t er_normal = er_altitude < LE_ADDRESS_WGS_A ? 2.0 * LE_ADDRESS_WGS_A - er_altitude : er_altitude;

        /* compute and return angle */
        return( asin( LE_ADDRESS_WGS_A / er_normal ) );

    }

    le_real_t er_geodesy_face( le_real_t const er_altitude ) {

        /* largest tile radius variable */
        le_real_t er_radius = ( LE_ADDRESS_WGS_A * LE_2P ) / pow( 2.0, ER_COMMON_ENUM );

        /* computation variable */
        le_real_t er_normal = sqrt( fabs( er_altitude * er_altitude - LE_ADDRESS_WGS_A * LE_ADDRESS_WGS_A ) ) + er_radius;

        /* clamp and return cutting radius */
        return( er_normal < ER_COMMON_CFACE ? ER_COMMON_CFACE : er_normal );

    }

    le_enum_t er_geodesy_select( le_real_t const er_distance, er_view_t const * const er_view, le_size_t const er_scfg, le_size_t const er_scale ) {

        /* clamping variables */
        le_real_t er_clamp = er_scfg - er_view_get_span( er_view ) - 2;

        /* computation variables */
        le_real_t er_normal = log( ( LE_ADDRESS_WGS_A / 2.0 ) / ( er_distance * 30.0 ) ) / M_LN2 + 9.5;

        /* check range */
        er_normal = ( er_normal < 5.0 ? 5.0 : ( er_normal > er_clamp ? er_clamp : er_normal ) );

        /* check selection condition */
        return( er_normal - er_scale < 1.0 ? _LE_TRUE : _LE_FALSE );

    }

    le_real_t er_geodesy_scale( le_real_t const er_altitude ) {

        /* computation variable */
        le_real_t er_normal = er_altitude / LE_ADDRESS_WGS_A - 1.0;

        /* compute scale value */
        return( ER_COMMON_SCALE + ( 1.0 - ER_COMMON_SCALE ) * exp( - LE_PI * er_normal * er_normal ) );

    }

    le_real_t er_geodesy_near( le_real_t const er_altitude, le_real_t const er_scale ) {

        /* computation variable */
        le_real_t er_normal = fabs( er_altitude - LE_ADDRESS_WGS_A );

        /* compute near plane position */
        er_normal = er_scale * pow( er_normal, 0.99 ) * ( 1.0 - exp( - pow( er_normal / 19568.0, 8.0 ) ) );

        /* clamp, scale and return near plane distance */
        return( er_normal < 1.0 ? 1.0 : er_normal );

    }

    le_real_t er_geodesy_far( le_real_t const er_altitude, le_real_t const er_gamma, le_real_t const er_scale ) {

        /* computation variable */
        le_real_t er_normal = sqrt( fabs( er_altitude * er_altitude - LE_ADDRESS_WGS_A * LE_ADDRESS_WGS_A ) );

        /* earth center-tengant angle variable */
        le_real_t er_angle = er_geodesy_angle( er_altitude );

        /* corrected tilt variable */
        le_real_t er_correct = er_gamma > 90.0 ? 180.0 - er_gamma : er_gamma;

        /* compute tilt angle correction factor */
        er_normal *= cos( er_angle - er_correct * LE_D2R );

        /* clamp, scale and return far plane distance */
        return( er_scale * ( er_normal < ER_COMMON_LIMIT ? ER_COMMON_LIMIT : er_normal ) );

    }

