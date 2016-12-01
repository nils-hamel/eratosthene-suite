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

    # include "eratosthene-client-times.h"

/*
    source - constructor/destructor methods
 */

    er_times_t er_times_create( le_void_t ) {

        /* created structure variables */
        er_times_t er_times = ER_TIMES_C;

        /* assign buffer width */
        er_times.tm_width = glutGet( GLUT_SCREEN_WIDTH );

        /* assign buffer height */
        er_times.tm_height = 96-16;

        /* assign buffer lower position */
        er_times.tm_offset = glutGet( GLUT_SCREEN_HEIGHT ) - er_times.tm_height - 64;

        /* assign screen heights */
        er_times.tm_sh1 = er_times.tm_offset - ( ( 6 + 6 ) + 15 ) + 6;
        er_times.tm_sh2 = er_times.tm_offset + 6;
        er_times.tm_sh3 = er_times.tm_offset + er_times.tm_height - ( ( 6 + 6 ) + 15 );

        /* assign buffer heights */
        er_times.tm_bh1 = ( ( 6 + 6 ) + 15 ) - 6;
        er_times.tm_bh2 = er_times.tm_height - ( ( 6 + 6 ) + 15 ) - 6 - 6;

        /* assign middle position */
        er_times.tm_middle = er_times.tm_width >> 1;

        /* compute buffer size */
        er_times.tm_length = er_times.tm_width * er_times.tm_height * 4;

        /* allocate buffer memory */
        if ( ( er_times.tm_buffer = ( le_byte_t * ) malloc( er_times.tm_length ) ) == NULL ) {

            /* send message */
            return( er_times._status = _LE_FALSE, er_times );

        }

        /* initialise buffer memory */
        memset( er_times.tm_buffer, 255, er_times.tm_length );

        /* return created structure */
        return( er_times );

    }

    le_void_t er_times_delete( er_times_t * const er_times ) {

        /* deleted structure variables */
        er_times_t er_delete = ER_TIMES_C;

        /* check buffer memory */
        if ( er_times->tm_buffer != NULL ) {

            /* unallocate buffer memory */
            free( er_times->tm_buffer );

        }

        /* delete structure */
        ( * er_times ) = er_delete;

    }

/*
    source - display methods
 */

    le_void_t er_times_display( er_times_t const * const er_times, er_view_t const * const er_view ) {

        /* interface text variables */
        le_char_t * er_text[3] = { ( le_char_t * ) "< ONLY  ",  ( le_char_t * ) "  ONLY >", ( le_char_t * ) "< BOTH >" };

        /* garduation display variables */
        le_size_t er_grad = 0;

        /* view point variables */
        le_enum_t er_act = er_view_get_active( er_view );

        /* view point variables */
        le_time_t er_time = er_view_get_time( er_view, er_act );
        le_time_t er_area = er_view_get_area( er_view, er_act );

        /* time boundaries variables */
        le_time_t er_timed = er_time - ( er_area >> 1 );
        le_time_t er_timeu = er_time + ( er_area >> 1 );

        /* scale boundaries variables */
        le_time_t er_scaled = pow( 10.0, floor( log( er_area * 0.02 ) / log( 10.0 ) ) );
        le_time_t er_scaleu = pow( 10.0, floor( log( er_area * 0.85 ) / log( 10.0 ) ) );

        /* reset buffer memory */
        for ( le_size_t er_parse = 3; er_parse < er_times->tm_length; er_parse += 4 ) {

            /* check current height */
            if ( ( er_parse / ( er_times->tm_width << 2 ) ) == er_times->tm_bh2 ) {

                /* reset buffer alpha component */
                er_times->tm_buffer[er_parse] = 230 - 3 * 32;

            } else {

                /* reset buffer alpha component */
                er_times->tm_buffer[er_parse] = 230;

            }

        }

        /* specify text color */
        glColor4f( 1.0, 1.0, 1.0, 0.9 - 0.375 );

        /* parsing graduation scales */
        for ( le_size_t er_scale = er_scaled, er_reduce = 8; er_scale <= er_scaleu; er_scale *= 10, er_reduce -= 6 ) {

            /* parsing graduation increments */
            for ( le_time_t er_parse = er_times_rnd( er_timed, er_scale ); er_parse < er_timeu; er_parse += er_scale ) {

                /* compute graduation x-position */
                er_grad = ( ( ( ( le_real_t ) er_parse ) - er_time ) / er_area ) * er_times->tm_width + er_times->tm_middle;

                /* display graduation increment */
                for ( le_size_t er_pixel = er_times->tm_bh1 + er_reduce; er_pixel < er_times->tm_bh2; er_pixel ++ ) {

                    /* update interface buffer alpha channel */
                    er_times->tm_buffer[( ( er_grad + er_pixel * er_times->tm_width ) << 2 ) + 3] -= 32;

                }

                /* detect largest scale */
                if ( er_scale == er_scaleu ) {

                    /* display date text */
                    er_times_display_date( er_parse, er_grad, er_times->tm_sh3, ER_TIMES_CENTER );

                }

            }

        }

        /* display middle marker text */
        er_times_display_text( ( le_char_t * ) "^", er_times->tm_middle + 1, er_times->tm_sh2, ER_TIMES_CENTER );

        /* specify text color */
        glColor4f( 0.9, 0.9, 0.9, 1.0 );

        /* display mode text */
        er_times_display_text( er_text[er_view_get_mode( er_view ) - 1], er_times->tm_middle, er_times->tm_sh1, ER_TIMES_CENTER );

        /* check activity - specify text color */
        if ( er_act == 0 ) glColor4f( 0.9, 0.9, 0.9, 1.0 ); else glColor4f( 0.7, 0.7, 0.7, 1.0 );

        /* display times */
        er_times_display_date( er_view_get_time( er_view, 0 ), er_times->tm_middle - 48, er_times->tm_sh1, ER_TIMES_RIGHT );

        /* check activity - specify text color */
        if ( er_act == 1 ) glColor4f( 0.9, 0.9, 0.9, 1.0 ); else glColor4f( 0.7, 0.7, 0.7, 1.0 );

        /* display times */
        er_times_display_date( er_view_get_time( er_view, 1 ), er_times->tm_middle + 48, er_times->tm_sh1, ER_TIMES_LEFT  );

        /* assign buffer position */
        glRasterPos2i( 0, er_times->tm_offset );

        /* display buffer */
        glDrawPixels( er_times->tm_width, er_times->tm_height, GL_RGBA, GL_UNSIGNED_BYTE, er_times->tm_buffer );

    }

    le_void_t er_times_display_date( le_time_t const er_time, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* string array variables */
        le_char_t er_string[32] = { 0 };

        /* compose date string */
        lc_time_to_string( er_time, er_string, 32 );

        /* display date text */
        er_times_display_text( er_string, er_x, er_y, er_justify );

    }

    le_void_t er_times_display_text( le_char_t const * const er_text, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* check justification - assign shift */
        if ( er_justify == ER_TIMES_RIGHT  ) er_x -= 9.0 * strlen( ( char * ) er_text );
        if ( er_justify == ER_TIMES_CENTER ) er_x -= 4.5 * strlen( ( char * ) er_text );

        /* set string position */
        glRasterPos2i( er_x, er_y );

        /* display text */
        glutBitmapString( GLUT_BITMAP_9_BY_15, er_text );

    }

