/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

        /* font height variable */
        le_size_t er_font = er_font_get_height( & er_times.tm_font );

        /* assign buffer width */
        er_times.tm_width = er_width;

        /* assign buffer height */
        er_times.tm_height = er_font * 5;

        /* assign buffer lower position */
        er_times.tm_offset = er_height - er_times.tm_height - ( er_height * 0.05 );

        /* assign screen heights */
        er_times.tm_sh1 = er_font * 0.50;
        er_times.tm_sh2 = er_font * 3.00;
        er_times.tm_sh3 = er_font * 3.50;

        /* assign buffer heights */
        er_times.tm_bh1 = er_font * 1.75;
        er_times.tm_bh2 = er_font * 3.00;

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

        /* deleted structure variable */
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
    source - mutator methods
 */

    le_void_t er_times_set_buffer( er_times_t * const er_times ) {

        /* buffer period variable */
        le_size_t er_period = er_times->tm_width << 2;

        /* reset buffer */
        for ( le_size_t er_parse = 3; er_parse < er_times->tm_length; er_parse += 4 ) {

            /* buffer height detection */
            if ( ( er_parse / er_period ) < er_times->tm_bh2 ) {

                /* reset buffer alpha */
                er_times->tm_buffer[er_parse] = 232;

            } else {

                /* reset buffer alpha */
                er_times->tm_buffer[er_parse] = 240;

            }

        }

    }

    le_void_t er_times_set_slider( er_times_t * const er_times, le_time_t const er_time, le_time_t const er_area ) {

        /* time boundaries variable */
        le_time_t er_time_l = er_time - ( er_area >> 1 );
        le_time_t er_time_h = er_time + ( er_area >> 1 );

        /* scale boundaries variable */
        le_time_t er_scale_l = pow( 10.0, floor( log( er_area * 0.02 ) / log( 10.0 ) ) );
        le_time_t er_scale_h = pow( 10.0, floor( log( er_area * 0.85 ) / log( 10.0 ) ) );

        /* garduation increment variable */
        le_size_t er_grad = 0;

        /* shading factor variable */
        le_size_t er_shade = 64 / ( er_times->tm_bh2 - er_times->tm_bh1 );

        /* parsing slider scales */
        for ( le_size_t er_scale = er_scale_l; er_scale <= er_scale_h; er_scale *= 10 ) {

            /* parsing slider graduation */
            for ( le_time_t er_parse = er_times_rnd( er_time_l, er_scale ); er_parse < er_time_h; er_parse += er_scale ) {

                /* increment position */
                er_grad = ( ( ( ( le_real_t ) er_parse ) - er_time ) / er_area ) * er_times->tm_width + er_times->tm_middle;

                /* display increment */
                for ( le_size_t er_pixel = er_times->tm_bh1, er_value = 0; er_pixel < er_times->tm_bh2; er_pixel ++, er_value += er_shade ) {

                    /* update interface buffer alpha channel */
                    er_times->tm_buffer[( ( er_grad + er_pixel * er_times->tm_width ) << 2 ) + 3] -= er_value;

                }

                /* detect largest scale */
                if ( er_scale == er_scale_h ) {

                    /* display date text */
                    er_times_display_date( er_times, er_parse, 64, er_grad, er_times->tm_sh1, ER_TIMES_CENTER );

                }


            }

        }

    }

/*
    source - display methods
 */

    le_void_t er_times_display( er_times_t * const er_times, er_view_t const * const er_view ) {

        /* mode variable */
        le_enum_t er_mode = er_view_get_mode( er_view );

        /* activity variable */
        le_enum_t er_active = ( er_mode == 2 ) ? 1 : 0;

        /* alpha variable */
        le_byte_t er_alpha = 0;

        /* text variable */
        le_char_t * er_text[6] = ER_TIMES_MODES;

        /* reset buffer */
        er_times_set_buffer( er_times );

        /* display slider */
        er_times_set_slider( er_times, er_view_get_time( er_view, er_active ), er_view_get_area( er_view, er_active ) );

        /* display cursor */
        er_times_display_text( er_times, er_text[0], 64, er_times->tm_middle, er_times->tm_sh2, ER_TIMES_CENTER );

        /* display mode */
        er_times_display_text( er_times, er_text[er_mode], 64, er_times->tm_middle, er_times->tm_sh3, ER_TIMES_CENTER );

        /* update activity */
        er_alpha = ( er_mode != 2 ) ? 64 : 208;

        /* display time */
        er_times_display_date( er_times, er_view_get_time( er_view, 0 ), er_alpha, er_times->tm_middle - 24, er_times->tm_sh3, ER_TIMES_RIGHT );

        /* update activity */
        er_alpha = ( er_mode >= 2 ) ? 64 : 208;

        /* display time */
        er_times_display_date( er_times, er_view_get_time( er_view, 1 ), er_alpha, er_times->tm_middle + 24, er_times->tm_sh3, ER_TIMES_LEFT );

        /* buffer position */
        glRasterPos2i( 0, er_times->tm_offset );

        /* buffer display */
        glDrawPixels( er_times->tm_width, er_times->tm_height, GL_RGBA, GL_UNSIGNED_BYTE, er_times->tm_buffer );

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

