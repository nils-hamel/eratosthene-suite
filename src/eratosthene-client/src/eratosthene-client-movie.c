/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 DHLAB, EPFL
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

    er_view_t er_movie_get( er_movie_t const * const er_movie ) {

        /* return computed view */
        return( er_view_get_inter( er_movie->mv_views, er_movie->mv_stack, er_movie->mv_parse, er_movie->mv_param ) );

    }

/*
    source - mutator methods
 */

    le_void_t er_movie_set( er_movie_t * const er_movie, er_view_t const * const er_view ) {

        /* check stack state */
        if ( er_movie->mv_stack == 0 ) {

            /* reset exportation index */
            er_movie->mv_index = 0;

            /* reset interpolation parameters */
            er_movie->mv_parse = 0;
            er_movie->mv_param = 0.0;

        }

        /* check stack state */
        if ( er_movie->mv_stack < ER_MOVIE_STACK ) {

            /* stacking view */
            er_movie->mv_views[er_movie->mv_stack ++] = ( * er_view );

        }

    }

    le_void_t er_movie_set_clear( er_movie_t * const er_movie ) {

        /* unstacking views */
        er_movie->mv_stack = 0;

    }

/*
    source - movie methods
 */

    le_enum_t er_movie( er_movie_t * const er_movie ) {

        /* exportation path variables */
        le_char_t er_path[_LE_USE_STRING] = { 0 };

        /* check control point stack */
        if ( er_movie->mv_stack < 2 ) {

            /* send message */
            return( ER_COMMON_VIEW );

        }

        /* compose exportation path */
        sprintf( ( char * ) er_path, "/tmp/era-%05" _LE_SIZE_P ".png", er_movie->mv_index ++ );

        /* opengl read buffer */
        glReadBuffer( GL_BACK );

        /* read buffer content */
        glReadPixels( 0, 0, er_movie->mv_wbuffer, er_movie->mv_hbuffer, GL_RGBA, GL_UNSIGNED_BYTE, er_movie->mv_pbuffer );

        /* buffer content exportation */
        lc_image_png_write( ( char * ) er_path, er_movie->mv_wbuffer, er_movie->mv_hbuffer, LC_IMAGE_RGBA, er_movie->mv_pbuffer );

        /* update interpolation parameter */
        if ( ( er_movie->mv_param += 0.0025 ) < 1.0 ) {

            /* send message */
            return( ER_COMMON_MOVIE );

        }

        /* update interpolation parameter */
        er_movie->mv_param -= 1.0;

        /* update interpolation parameter */
        if ( ( ++ er_movie->mv_parse ) < ( er_movie->mv_stack - 1 ) ) {

            /* send message */
            return( ER_COMMON_MOVIE );

        }

        /* unstacking views */
        er_movie->mv_stack = 0;

        /* send message */
        return( ER_COMMON_VIEW );

    }

