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

    # include "eratosthene-client-times.h"

/*
    source - constructor/destructor methods
 */

    er_times_t er_times_create( le_size_t const er_width, le_size_t const er_height ) {

        /* created structure variable */
        er_times_t er_times = ER_TIMES_C;

        /* assign buffer sizes */
        er_times.tm_width  = 968;
        er_times.tm_height = er_font_get_height( & er_times.tm_font ) * 5;

        /* assign buffer position - horizontal */
        er_times.tm_left = ( er_width  >> 1 ) - ( er_times.tm_width  >> 1 );

        /* assign buffer position - vertical */
        er_times.tm_top  = er_height - ( er_height >> 4 ) - er_times.tm_height;

        /* compute buffer size */
        er_times.tm_length = er_times.tm_width * er_times.tm_height * 4;

        /* allocate buffer memory */
        if ( ( er_times.tm_buffer = ( le_byte_t * ) malloc( er_times.tm_length ) ) == NULL ) {

            /* return created structure */
            return( er_times );

        }

        /* initialise buffer */
        memset( er_times.tm_buffer, 255, er_times.tm_length );

        /* return created structure */
        return( le_set_status( er_times, _LE_TRUE ) );

    }

    le_void_t er_times_delete( er_times_t * const er_times ) {

        /* deleted structure variable */
        er_times_t er_delete = ER_TIMES_C;

        /* check buffer memory */
        if ( er_times->tm_buffer != NULL ) {

            /* release buffer memory */
            free( er_times->tm_buffer );

        }

        /* delete structure */
        ( * er_times ) = er_delete;

    }

/*
    source - mutator methods
 */

    le_void_t er_times_set_buffer( er_times_t * const er_times ) {

        /* reset buffer alpha channel */
        for ( le_size_t er_parse = 3; er_parse < er_times->tm_length; er_parse += 4 ) {

            /* reset alpha value */
            er_times->tm_buffer[er_parse] = 0;

        }

    }

/*
    source - display methods
 */

    le_void_t er_times_display( er_times_t * const er_times, er_view_t const * const er_view ) {

        /* middle position variable */
        le_size_t er_middle = er_times->tm_width >> 1;

        /* hieght position variable */
        le_size_t er_height = 0;

        /* height step variable */
        le_size_t er_step = er_font_get_height( & er_times->tm_font ) * 2;

        /* mode text variable */
        le_char_t * er_mode[6] = ER_TIMES_MODES;

        /* query text variable */
        le_char_t * er_query[2] = { ( le_char_t * ) "NEAR QUERY", ( le_char_t * ) "DEEP QUERY" };

        le_char_t * er_dual[3] = { ( le_char_t * ) "(1)", ( le_char_t * ) "(2)", ( le_char_t * ) "BOTH" };

        le_char_t er_text[32] = { 0 };

        /* mode variable */
        le_enum_t er_mod = er_view_get_mode( er_view );

        /* query variable */
        le_enum_t er_qry = er_view_get_query( er_view );

        /* time variable */
        le_time_t er_tia = er_view_get_time( er_view, 0 );
        le_time_t er_tib = er_view_get_time( er_view, 1 );

        /* active time variable */
        le_size_t er_act = ( er_mod == 2 ) ? 1 : 0;

        /* time variable */
        le_time_t er_tac = er_view_get_time( er_view, er_act );

        /* comb variable */
        le_time_t er_cmb = er_view_get_comb( er_view );

        /* reset buffer */
        er_times_set_buffer( er_times );

        /* display comb value */
        er_times_range( er_times, er_cmb, 255, er_middle, er_height, ER_TIMES_CENTER );

        /* update comb value */
        er_cmb >>= 1;

        /* display time range limit */
        er_times_display_date( er_times, er_tac - er_cmb, 128, er_middle - 96, er_height, ER_TIMES_RIGHT );
        er_times_display_date( er_times, er_tac + er_cmb, 128, er_middle + 96, er_height, ER_TIMES_LEFT );

        /* update text height */
        er_height += er_step;

        sprintf( ( char * ) er_text, "%s - %s", er_dual[(er_mod>=3?3:er_mod)-1], er_query[er_qry] );

        /* display query */
        er_times_display_text( er_times, er_text, 255, er_middle, er_height, ER_TIMES_CENTER );

        /* update text height */
        er_height += er_step;

        if ( er_view_get_mode( er_view ) < 3 ) {

            /* display time */
            er_times_display_date( er_times, er_tac, 255, er_middle, er_height, ER_TIMES_CENTER );

        } else {

            /* display time */
            er_times_display_date( er_times, er_tia, 255, er_middle - 32, er_height, ER_TIMES_RIGHT );

            /* display time */
            er_times_display_date( er_times, er_tib, 255, er_middle + 32, er_height, ER_TIMES_LEFT );

            /* display mode */
            er_times_display_text( er_times, er_mode[er_mod], 255, er_middle, er_height, ER_TIMES_CENTER );

        }

        /* buffer position */
        glRasterPos2i( er_times->tm_left, er_times->tm_top );

        /* enable blending */
        glEnable( GL_BLEND );

        /* display buffer */
        glDrawPixels( er_times->tm_width, er_times->tm_height, GL_RGBA, GL_UNSIGNED_BYTE, er_times->tm_buffer );

        /* disable blending */
        glDisable( GL_BLEND );

    }

    le_void_t er_times_range( er_times_t * const er_times, le_time_t er_range, le_byte_t const er_value, le_size_t const er_x, le_size_t const er_y, le_enum_t const er_justify ) {

        /* string array variable */
        static le_char_t er_string[32] = { 0 };

        if ( er_range < 60 ) {

            sprintf( ( char * ) er_string, "%" _LE_TIME_P "S", er_range );

            er_times_display_text( er_times, er_string, er_value, er_x, er_y, er_justify );

            return;

        }

        er_range /= 60;

        if ( er_range < 60 ) {

            sprintf( ( char * ) er_string, "%" _LE_TIME_P "M", er_range );

            er_times_display_text( er_times, er_string, er_value, er_x, er_y, er_justify );

            return;

        }

        er_range /= 60;

        if ( er_range < 24 ) {

            sprintf( ( char * ) er_string, "%" _LE_TIME_P "H", er_range );

            er_times_display_text( er_times, er_string, er_value, er_x, er_y, er_justify );

            return;

        }

        er_range /= 24;

        if ( er_range < 365 ) {

            sprintf( ( char * ) er_string, "%" _LE_TIME_P "D", er_range );

            er_times_display_text( er_times, er_string, er_value, er_x, er_y, er_justify );

            return;

        }

        er_range /= 365;

        sprintf( ( char * ) er_string, "%" _LE_TIME_P "Y", er_range );

        er_times_display_text( er_times, er_string, er_value, er_x, er_y, er_justify );

    }

    le_void_t er_times_display_date( er_times_t * const er_times, le_time_t const er_date, le_byte_t const er_value, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* string array variable */
        le_char_t er_string[32] = { 0 };

        /* compose date string */
        lc_time_to_string( er_date, er_string, 32 );

        /* display date text */
        er_times_display_text( er_times, er_string, er_value, er_x, er_y, er_justify );

    }

    le_void_t er_times_display_text( er_times_t * const er_times, le_char_t const * const er_text, le_byte_t const er_value, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* string length variable */
        le_size_t er_length = strlen( ( char * ) er_text );

        /* check justification */
        if ( er_justify == ER_TIMES_RIGHT ) {

            /* correct x-position */
            er_x -= er_length * ( er_times->tm_font.ft_w );

        } else if ( er_justify == ER_TIMES_CENTER ) {

            /* correct x-position */
            er_x -= er_length * ( er_times->tm_font.ft_w >> 1 );

        }

        /* display string */
        er_font_display_string( & er_times->tm_font, er_text, er_length, er_value, er_times->tm_buffer, er_times->tm_width, er_times->tm_height, er_x, er_y );

    }

