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

    # include "eratosthene-client-movie.h"

/*
    source - constructor/destructor methods
 */

    er_movie_t er_movie_create( le_void_t ) {

        /* create structure variables */
        er_movie_t er_movie = ER_MOVIE_C;

        /* align buffer size to screen resolution */
        er_movie.mv_wbuffer = glutGet( GLUT_SCREEN_WIDTH  );
        er_movie.mv_hbuffer = glutGet( GLUT_SCREEN_HEIGHT );

        /* allocate buffer memory */
        if ( ( er_movie.mv_pbuffer = ( le_byte_t * ) malloc( er_movie.mv_wbuffer * er_movie.mv_hbuffer * 4 ) ) == NULL ) {

            /* send message - return created structure */
            return( er_movie._status = _LE_FALSE, er_movie );

        }

        /* return created structure */
        return( er_movie );

    }

    le_void_t er_movie_delete( er_movie_t * const er_movie ) {

        /* deleted structure variables */
        er_movie_t er_delete = ER_MOVIE_C;

        /* check buffer memory */
        if ( er_movie->mv_pbuffer != NULL ) {

            /* release buffer memory */
            free( er_movie->mv_pbuffer );

        }

        /* delete structure */
        ( * er_movie ) = er_delete;

    }

/*
    source - accessor methods
 */

    le_real_t er_movie_get_value( er_movie_t const * const er_movie, le_size_t const er_index ) {

        /* returned value variables */
        le_real_t er_value = 0.0;

        /* global parameter variables */
        le_real_t er_global = 0.0;

        /* interpolation weight variables */
        le_real_t er_local = 0.0;
        le_real_t er_total = 0.0;

        /* parsing control points */
        for ( le_size_t er_parse = 0; er_parse < er_movie->mv_stack; er_parse ++ ) {

            /* compute global parameter */
            er_global = ( le_real_t ) er_parse - er_movie->mv_parse - er_movie->mv_param;

            /* compute interpolation weight */
            er_total += ( er_local = exp( - 3.0 * er_global * er_global ) );

            /* compute interpolation value */
            er_value += er_local * er_movie->mv_poses[er_parse][er_index];

        }

        /* returned interpolated value */
        return( er_value / er_total );

    }

/*
    source - manipulator methods
 */

    le_void_t er_movie_set_reset( er_movie_t * const er_movie ) {

        /* reset exportation index */
        er_movie->mv_index = 0;

        /* reset intepolation parameters */
        er_movie->mv_parse = 0;
        er_movie->mv_param = 0.0;

    }

    le_void_t er_movie_set_empty( er_movie_t * const er_movie ) {

        /* empty control point stack */
        er_movie->mv_stack = 0;

    }

    le_void_t er_movie_set_point( er_movie_t * const er_movie, le_real_t const er_vlon, le_real_t const er_vlat, le_real_t const er_valt, le_real_t const er_vazm, le_real_t const er_vgam ) {

        /* push point of view - space */
        er_movie->mv_poses[er_movie->mv_stack][0] = er_vlon;
        er_movie->mv_poses[er_movie->mv_stack][1] = er_vlat;
        er_movie->mv_poses[er_movie->mv_stack][2] = er_valt;
        er_movie->mv_poses[er_movie->mv_stack][3] = er_vazm;
        er_movie->mv_poses[er_movie->mv_stack][4] = er_vgam;

        /* update stack state */
        er_movie->mv_stack ++;

    }

/*
    source - movie methods
 */

    le_enum_t er_movie( er_movie_t * const er_movie ) {

        /* check control point stack */
        if ( er_movie->mv_stack < 2 ) {

            /* send message */
            return( ER_COMMON_VIEW );

        }

        /* opengl read buffer */
        glReadBuffer( GL_BACK );

        /* read buffer content */
        glReadPixels( 0, 0, er_movie->mv_wbuffer, er_movie->mv_hbuffer, GL_RGBA, GL_UNSIGNED_BYTE, er_movie->mv_pbuffer );

        /* compose exportation path */
        sprintf( ( char * ) er_movie->mv_path, "/tmp/era-%05" _LE_SIZE_P ".png", er_movie->mv_index ++ );

        /* buffer content exportation */
        lc_image_png_write( ( char * ) er_movie->mv_path, er_movie->mv_wbuffer, er_movie->mv_hbuffer, LC_IMAGE_RGBA, er_movie->mv_pbuffer );

        /* update parameter */
        //if ( ( er_movie->mv_param += 0.0025 ) > 1.0 ) {
        if ( ( er_movie->mv_param += 0.05 ) > 1.0 ) {

            /* reset interpolation parameter */
            er_movie->mv_param -= 1.0;

            /* update interpolation parameter */
            if ( ( ++ er_movie->mv_parse ) >= er_movie->mv_stack ) {

                /* send message */
                return( ER_COMMON_VIEW );

            } else {

                /* send message */
                return( ER_COMMON_MOVIE );

            }

        } else {

            /* send message */
            return( ER_COMMON_MOVIE );

        }

    }

