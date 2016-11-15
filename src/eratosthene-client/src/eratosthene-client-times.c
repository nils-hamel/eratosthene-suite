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

        /* compute buffer width */
        er_times.tm_width = glutGet( GLUT_SCREEN_WIDTH );

        /* assign buffer height */
        er_times.tm_height = 96;

        /* assign buffer position */
        er_times.tm_offset = 64;

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

        /* check buffer memory allocation */
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

        /* garduation display variables */
        le_size_t er_grad = 0;

        /* active time variables */
        le_enum_t er_act = er_view_get_active( er_view );

        /* view point variables */
        le_time_t er_time = er_view_get_time( er_view, er_act );
        le_time_t er_area = er_view_get_area( er_view, er_act );

        /* time boundaries variables */
        le_time_t er_timed = er_time - ( er_area >> 1 );
        le_time_t er_timeu = er_time + ( er_area >> 1 );

        /* scale boundaries variables */
        le_time_t er_scaled = pow( 10.0, floor( log( er_area * 0.02 ) / log( 10.0 ) ) );
        le_time_t er_scaleu = pow( 10.0, floor( log( er_area * 0.75 ) / log( 10.0 ) ) );

        /* position marker variables */
        le_size_t er_midx = er_times->tm_width >> 1;
        le_size_t er_topy = er_times->tm_offset + er_times->tm_height;

        /* clear buffer memory */
        for ( le_size_t er_parse = 3; er_parse < er_times->tm_length; er_parse += 4 ) {

            /* reset alpha component */
            er_times->tm_buffer[er_parse] = 192;

        }

        /* specify date text color */
        glColor3f( 0.18, 0.22, 0.28 );

        /* parsing graduation scales */
        for ( le_size_t er_scale = er_scaled; er_scale <= er_scaleu; er_scale *= 10 ) {

            /* display graduation */
            for ( le_time_t er_parse = ER_TIMES_ROUND( er_timed, er_scale ); er_parse < er_timeu; er_parse += er_scale ) {

                /* compute graduation position x-position */
                er_grad = ( ( ( ( le_real_t ) er_parse ) - er_timed ) / er_area ) * er_times->tm_width;

                /* display graduation increment */
                for ( le_size_t er_pixel = 30; er_pixel < ( er_times->tm_height - 30 ); er_pixel ++ ) {

                    /* update interface buffer alpha channel */
                    er_times->tm_buffer[( ( er_grad + er_pixel * er_times->tm_width ) << 2 ) + 3] -= 32;

                }

                /* detect largest scale */
                if ( er_scale == er_scaleu ) {

                    /* display date text */
                    er_times_display_date( er_parse, er_grad, er_times->tm_offset + 21, ER_TIMES_CENTER );

                }

            }

        }

        /* check activity */
        if ( er_act == 0 ) glColor3f( 0.18, 0.42, 0.68 ); else glColor3f( 0.18, 0.22, 0.28 );

        /* display times */
        er_times_display_date( er_view_get_time( er_view, 0 ), er_midx - 48, er_topy - 4, ER_TIMES_RIGHT );

        /* check activity */
        if ( er_act == 1 ) glColor3f( 0.18, 0.42, 0.68 ); else glColor3f( 0.18, 0.22, 0.28 );

        /* display times */
        er_times_display_date( er_view_get_time( er_view, 1 ), er_midx + 48, er_topy - 4, ER_TIMES_LEFT  );

        /* switch on time mode */
        switch ( er_view_get_mode( er_view ) ) {

        case ( 1 ) : er_times_display_text( "< only  ", er_midx, er_topy - 4, ER_TIMES_CENTER ); break;
        case ( 2 ) : er_times_display_text( "  only >", er_midx, er_topy - 4, ER_TIMES_CENTER ); break;
        case ( 3 ) : er_times_display_text( "< both >", er_midx, er_topy - 4, ER_TIMES_CENTER ); break;

        };

        /* assign buffer position */
        glRasterPos2i( 0, er_times->tm_offset );

        /* display buffer */
        glDrawPixels( er_times->tm_width, er_times->tm_height, GL_RGBA, GL_UNSIGNED_BYTE, er_times->tm_buffer );
        

    }

    le_void_t er_times_display_text( le_char_t const * const er_text, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* check justification - assign shift */
        if ( er_justify == ER_TIMES_RIGHT  ) er_x -= strlen( ( char * ) er_text ) * 8;
        if ( er_justify == ER_TIMES_CENTER ) er_x -= strlen( ( char * ) er_text ) * 4;

        /* assign string position */
        glRasterPos2i( er_x, er_y - 13 );

        /* display string */
        glutBitmapString( GLUT_BITMAP_8_BY_13, er_text );

    }

    le_void_t er_times_display_date( le_time_t const er_time, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* string array variables */
        le_char_t er_string[32] = { 0 };

        /* compose date string */
        lc_time_to_string( er_time, er_string, 32 );

        /* display text */
        er_times_display_text( er_string, er_x, er_y, er_justify );

    }

