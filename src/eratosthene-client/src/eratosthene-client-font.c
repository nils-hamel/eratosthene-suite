/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2020 DHLAB, EPFL
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

    # include "eratosthene-client-font.h"

/*
    source - accessor methods
 */

    le_size_t er_font_get_width( er_font_t const * const er_font ) {

        /* return font width */
        return( er_font->ft_w );

    }

    le_size_t er_font_get_height( er_font_t const * const er_font ) {

        /* return font height */
        return( er_font->ft_h );

    }

    le_size_t er_font_get_count( er_font_t const * const er_font ) {

        /* return number of characters */
        return( er_font->ft_c );

    }

/*
    source - display methods
 */

    le_void_t er_font_display_string( er_font_t const * const er_font, le_char_t const * const er_string, le_size_t er_length, le_byte_t const er_value, le_byte_t * const er_buffer, le_size_t const er_width, le_size_t const er_height, le_size_t er_x, le_size_t er_y ) {

        /* buffer position variable */
        le_size_t er_bx = 0;
        le_size_t er_by = 0;

        /* font offset variable */
        le_size_t er_bit = 0;

        /* font corrected height variable */
        le_size_t er_correct = er_font->ft_h - 1;

        /* font period variable */
        le_size_t er_period = er_font->ft_w * er_font->ft_c;

        /* parsing string char */
        for ( le_size_t er_char = 0; er_char < er_length; er_char ++ ) {

            /* check char code */
            if ( er_string[er_char] >= er_font->ft_c ) {

                /* cancel char display */
                continue;

            }

            /* parsing char pixels */
            for ( le_size_t er_u = 0; er_u < er_font->ft_w; er_u ++ ) {

                /* compute buffer position */
                er_bx = er_u + er_x;

                /* buffer condition */
                if ( er_bx < 0 ) {

                    /* cancel column display */
                    continue;

                }

                /* buffer condition */
                if ( er_bx >= er_width ) {

                    /* cancel column display */
                    continue;

                }

                /* parsing char pixels */
                for ( le_size_t er_v = 0; er_v < er_font->ft_h; er_v ++ ) {

                    /* compute buffer position */
                    er_by = er_v + er_y;

                    /* buffer condition */
                    if ( er_by < 0 ) {

                        /* cancel pixel display */
                        continue;

                    }

                    /* buffer condition */
                    if ( er_by >= er_height ) {

                        /* cancel pixel display */
                        continue;

                    }

                    /* compute font offset */
                    er_bit = ( er_period * ( er_correct - er_v ) ) + ( er_font->ft_w * ( le_size_t ) er_string[er_char] ) + er_u;

                    /* check font bitmap */
                    if ( er_font->ft_bits[er_bit >> 3] & ( 1 << ( er_bit & 0x07 ) ) ) {

                        /* assign pixel value */
                        * ( er_buffer + ( ( ( er_by * er_width ) + er_bx ) << 2 ) + 3 ) = er_value;

                    }

                }

            }

            /* update position */
            er_x += er_font->ft_w;

        }

    }

