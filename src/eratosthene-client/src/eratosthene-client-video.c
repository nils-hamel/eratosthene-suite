/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2019 DHLAB, EPFL
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

    # include "eratosthene-client-video.h"

/*
    source - constructor/destructor methods
 */

    er_video_t er_video_create( le_char_t * const er_path, le_size_t const er_count, le_size_t const er_width, le_size_t const er_height ) {

        /* created structure variable */
        er_video_t er_video = ER_VIDEO_C;

        /* assign path value */
        er_video.vd_path = er_path;

        /* assign frame count */
        er_video.vd_count = er_count;

        /* assign image dimension */
        er_video.vd_width = er_width;

        /* assign image dimension */
        er_video.vd_height = er_height;

        /* allocate buffer memory */
        if ( ( er_video.vd_buffer = ( le_byte_t * ) malloc( er_width * er_height * 3 ) ) == NULL ) {

            /* return created structure */
            return( er_video );

        }

        /* return created structure */
        return( le_set_status( er_video, _LE_TRUE ) );

    }

    le_void_t er_video_delete( er_video_t * const er_video ) {

        /* deleted structure variable */
        er_video_t er_delete = ER_VIDEO_C;

        /* check buffer memory */
        if ( er_video->vd_buffer != NULL ) {

            /* release buffer memory */
            free( er_video->vd_buffer );

        }

        /* delete structure */
        ( * er_video ) = er_delete;

    }

/*
    source - accessor methods
 */

    er_view_t er_video_get( er_video_t const * const er_video ) {

        /* returned structure variable */
        er_view_t er_view = er_video->vd_view[0];

        /* interpolation variable */
        le_size_t er_index = 0;

        /* interpolation variable */
        le_real_t er_local  = 0.0;
        le_real_t er_weight = 0.0;

        /* interpolant parameter variable */
        le_real_t er_param = ( ( le_real_t ) er_video->vd_index / ( le_real_t ) ( er_video->vd_count - 1 ) ) * ( er_video->vd_push - 1 );

        /* interpolant value variable */
        le_real_t er_ilon = 0.0;
        le_real_t er_ilat = 0.0;
        le_real_t er_ialt = 0.0;
        le_real_t er_iazm = 0.0;
        le_real_t er_igam = 0.0;

        /* interpolant value variable */
        le_time_t er_itia = _LE_TIME_NULL;
        le_time_t er_itib = _LE_TIME_NULL;
        le_time_t er_icmb = _LE_TIME_NULL;

        /* position interpolation - accumulation */
        for ( le_size_t er_parse = 0; er_parse < er_video->vd_push; er_parse ++ ) {

            /* compute interpolation parameter and weight accumulation */
            er_weight += ( er_local = exp( - 5.0 * ( er_param - er_parse ) * ( er_param - er_parse ) ) );

            /* interpolant accumulation - position */
            er_ilon += er_local * er_view_get_lon( & er_video->vd_view[er_parse] );
            er_ilat += er_local * er_view_get_lat( & er_video->vd_view[er_parse] );
            er_ialt += er_local * er_view_get_alt( & er_video->vd_view[er_parse] );
            er_iazm += er_local * er_view_get_azm( & er_video->vd_view[er_parse] );
            er_igam += er_local * er_view_get_gam( & er_video->vd_view[er_parse] );

        }

        /* position interpolation - compute */
        er_ilon /= er_weight;
        er_ilat /= er_weight;
        er_ialt /= er_weight;
        er_iazm /= er_weight;
        er_igam /= er_weight;

        /* compute interpolation index */
        er_index = ( le_size_t ) er_param;

        /* range boundary check */
        er_index = ( er_index == er_video->vd_push - 1 ) ? er_index - 1 : er_index;

        /* compute interpolation parameter */
        er_param -= er_index;

        /* compute interpolation parameter */
        er_local = 1.0 - er_param;

        /* time interpolation - compute */
        er_itia = er_view_get_time( & er_video->vd_view[er_index], 0 ) * er_local + er_view_get_time( & er_video->vd_view[er_index + 1], 0 ) * er_param;
        er_itib = er_view_get_time( & er_video->vd_view[er_index], 1 ) * er_local + er_view_get_time( & er_video->vd_view[er_index + 1], 1 ) * er_param;

        /* comb interpolation - compute */
        er_icmb = er_view_get_comb( & er_video->vd_view[er_index] ) * er_local + er_view_get_comb( & er_video->vd_view[er_index + 1] ) * er_param;

        /* assign position and time to view */
        er_view_set( & er_view, er_ilon, er_ilat, er_ialt, er_iazm, er_igam, er_itia, er_itib, er_icmb );

        /* return created structure */
        return( er_view );

    }

    le_enum_t er_video_get_state( er_video_t const * const er_video ) {

        /* check exportation path */
        if ( er_video->vd_path == NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* check frame count */
        if ( er_video->vd_count == 0 ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* check stack state */
        if ( er_video->vd_push < 2 ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* send message */
        return( _LE_TRUE );

    }

/*
    source - mutator methods
 */

    le_enum_t er_video_set( er_video_t * const er_video ) {

        /* path variable */
        le_char_t er_path[_LE_USE_PATH] = { 0 };

        /* compose exportation path */
        sprintf( ( char * ) er_path, "%s/%06" _LE_SIZE_P ".png", er_video->vd_path, er_video->vd_index );

        /* OpenGL buffer selection */
        glReadBuffer( GL_BACK );

        /* read buffer pixels */
        glReadPixels( 0, 0, er_video->vd_width, er_video->vd_height, GL_RGB, GL_UNSIGNED_BYTE, er_video->vd_buffer );

        /* buffer content exportation */
        lc_image_write( ( char * ) er_path, er_video->vd_width, er_video->vd_height, er_video->vd_buffer );

        /* update frame index and check limit */
        if ( ( ++ er_video->vd_index ) == er_video->vd_count ) {

            /* reset index */
            er_video->vd_index = 0;

            /* send message */
            return( ER_COMMON_VIEW );

        } else {

            /* send message */
            return( ER_COMMON_AUTO );

        }

    }

    le_enum_t er_video_set_push( er_video_t * const er_video, er_view_t const * const er_view ) {

        /* check stack state */
        if ( er_video->vd_push == ER_VIDEO_STACK ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* push view on stack */
        er_video->vd_view[er_video->vd_push] = ( * er_view );

        /* update stack state */
        er_video->vd_push ++;

        /* send message */
        return( _LE_TRUE );

    }

    le_void_t er_video_set_reset( er_video_t * const er_video ) {

        /* reset stack state */
        er_video->vd_push = 0;

    }

