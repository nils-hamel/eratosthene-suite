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
        er_times.tm_sh1 = er_font * 0.5;
        er_times.tm_sh2 = er_font * 3.5;

        /* assign buffer heights */
        er_times.tm_bh1 = er_font * 2.0;
        er_times.tm_bh2 = er_font * 3.0;

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
        le_char_t * er_text[5] = ER_TIMES_MODES;

        /* garduation display variables */
        le_size_t er_grad = 0;

        /* text display value variable */
        le_byte_t er_alpha = 0;

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
            if ( ( er_parse / ( er_times->tm_width << 2 ) ) < er_times->tm_bh2 ) {

                /* reset buffer alpha component */
                er_times->tm_buffer[er_parse] = 224;

            } else {

                /* reset buffer alpha component */
                er_times->tm_buffer[er_parse] = 240;

            }

        }

        /* parsing graduation scales */
        for ( le_size_t er_scale = er_scaled; er_scale <= er_scaleu; er_scale *= 10 ) {

            /* parsing graduation increments */
            for ( le_time_t er_parse = er_times_rnd( er_timed, er_scale ); er_parse < er_timeu; er_parse += er_scale ) {

                /* compute graduation x-position */
                er_grad = ( ( ( ( le_real_t ) er_parse ) - er_time ) / er_area ) * er_times->tm_width + er_times->tm_middle;

                /* display graduation increment */
                for ( le_size_t er_pixel = er_times->tm_bh1; er_pixel < er_times->tm_bh2; er_pixel ++ ) {

                    /* update interface buffer alpha channel */
                    er_times->tm_buffer[( ( er_grad + er_pixel * er_times->tm_width ) << 2 ) + 3] -= 56;

                }

                /* detect largest scale */
                if ( er_scale == er_scaleu ) {

                    /* display date text */
                    er_times_display_date( er_times, er_parse, er_grad, er_times->tm_sh1, 0, ER_TIMES_CENTER );

                }

            }

        }

        /* display mode text */
        er_times_display_text( er_times, er_text[er_view_get_mode( er_view ) - 1], er_times->tm_middle, er_times->tm_sh2, 64, ER_TIMES_CENTER );

        /* assign text color */
        er_alpha = ( er_act == 0 ) ? 64 : 192;

        /* display times */
        er_times_display_date( er_times, er_view_get_time( er_view, 0 ), er_times->tm_middle - 48, er_times->tm_sh2, er_alpha, ER_TIMES_RIGHT );

        /* assign text color */
        er_alpha = ( er_act == 1 ) ? 64 : 192;

        /* display times */
        er_times_display_date( er_times, er_view_get_time( er_view, 1 ), er_times->tm_middle + 48, er_times->tm_sh2, er_alpha, ER_TIMES_LEFT  );

        /* assign buffer position */
        glRasterPos2i( 0, er_times->tm_offset );

        /* display buffer */
        glDrawPixels( er_times->tm_width, er_times->tm_height, GL_RGBA, GL_UNSIGNED_BYTE, er_times->tm_buffer );

    }

    le_void_t er_times_display_date( er_times_t const * const er_times, le_time_t const er_date, le_size_t const er_x, le_size_t const er_y, le_byte_t const er_value, le_enum_t const er_justify ) {

        /* string array variables */
        le_char_t er_string[32] = { 0 };

        /* compose date string */
        lc_time_to_string( er_date, er_string, 32 );

        /* display date text */
        er_times_display_text( er_times, er_string, er_x, er_y, er_value, er_justify );

    }

    le_void_t er_times_display_text( er_times_t const * const er_times, le_char_t const * const er_text, le_size_t er_x, le_size_t er_y, le_byte_t const er_value, le_enum_t const er_justify ) {

        /* string length variable */
        le_size_t er_length = strlen( ( char * ) er_text );

        /* pixel offset variable */
        le_byte_t * er_offset = NULL;

        /* position variable */
        le_size_t er_r = 0;
        le_size_t er_s = 0;

        /* bit coordinates variable */
        le_size_t er_bitx = 0;
        le_size_t er_bity = 0;

        /* check justification */
        if ( er_justify == ER_TIMES_RIGHT ) {

            /* correct x-position */
            er_x -= er_length * ( er_times->tm_font.ft_w );

        } else if ( er_justify == ER_TIMES_CENTER ) {

            /* correct x-position */
            er_x -= er_length * ( er_times->tm_font.ft_w >> 1 );

        }

        /* parsing text string */
        for ( le_size_t er_parse = 0; er_parse < er_length; er_parse ++ ) {

            /* parsing pixels */
            for ( le_size_t er_u = 0; er_u < er_times->tm_font.ft_w; er_u ++ ) {

                /* parsing pixels */
                for ( le_size_t er_v = 0; er_v < er_times->tm_font.ft_h; er_v ++ ) {

                    /* compute position */
                    er_r = er_x + er_u;
                    er_s = er_y + er_v;

                    /* check boundaries */
                    if ( er_r < 0 ) continue;
                    if ( er_s < 0 ) continue;

                    /* check boundaries */
                    if ( er_r >= er_times->tm_width  ) continue;
                    if ( er_s >= er_times->tm_height ) continue;

                    /* compute pixel offset */
                    er_offset = er_times->tm_buffer + ( ( ( er_y + er_v ) * er_times->tm_width + ( er_x + er_u ) ) * 4 );

                    /* compute bit coordinate */
                    er_bitx = er_times->tm_font.ft_w * ( le_size_t ) ( er_text[er_parse] ) + er_u;

                    /* compute bit coordinate */
                    er_bity = er_times->tm_font.ft_w * er_times->tm_font.ft_c;

                    /* compute bit coordinate */
                    er_bitx = er_bity * ( er_times->tm_font.ft_h - er_v - 1 ) + er_bitx;

                    /* check font bitmap */
                    if ( er_times->tm_font.ft_bits[er_bitx >> 3] & ( 1 << ( er_bitx % 8 ) ) ) {

                        /* assign value */
                        er_offset[3] = er_value;

                    }

                }

            }

            /* update head */
            er_x += er_times->tm_font.ft_w;

        }

    }

