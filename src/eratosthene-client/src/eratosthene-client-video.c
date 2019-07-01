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

    er_video_t er_video_create( le_char_t * const er_path, le_size_t const er_width, le_size_t const er_height ) {

        /* created structure variable */
        er_video_t er_video = ER_VIDEO_C;

        /* assign path value */
        er_video.vd_path = er_path;

        /* assign image dimension */
        er_video.vd_width = er_width;

        /* assign image dimenstion */
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

/*
    source - mutator methods
 */


