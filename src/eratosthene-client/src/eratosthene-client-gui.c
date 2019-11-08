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

    # include "eratosthene-client-gui.h"

/*
    source - constructor/destructor methods
 */

    er_gui_t er_gui_create( le_size_t const er_width, le_size_t const er_height ) {

        /* created structure variable */
        er_gui_t er_gui = ER_GUI_C;

        /* assign buffer sizes */
        er_gui.gu_width  = 968;
        er_gui.gu_height = er_font_get_height( & er_gui.gu_font ) * 5;

        /* assign buffer position - horizontal */
        er_gui.gu_left = ( er_width  >> 1 ) - ( er_gui.gu_width  >> 1 );

        /* assign buffer position - vertical */
        er_gui.gu_top  = er_height - ( er_height >> 4 ) - er_gui.gu_height;

        /* compute buffer size */
        er_gui.gu_length = er_gui.gu_width * er_gui.gu_height * 4;

        /* allocate buffer memory */
        if ( ( er_gui.gu_buffer = ( le_byte_t * ) malloc( er_gui.gu_length ) ) == NULL ) {

            /* return created structure */
            return( er_gui );

        }

        /* initialise buffer */
        memset( er_gui.gu_buffer, 255, er_gui.gu_length );

        /* return created structure */
        return( le_set_status( er_gui, _LE_TRUE ) );

    }

    le_void_t er_gui_delete( er_gui_t * const er_gui ) {

        /* deleted structure variable */
        er_gui_t er_delete = ER_GUI_C;

        /* check buffer memory */
        if ( er_gui->gu_buffer != NULL ) {

            /* release buffer memory */
            free( er_gui->gu_buffer );

        }

        /* delete structure */
        ( * er_gui ) = er_delete;

    }

/*
    source - mutator methods
 */

    le_void_t er_gui_set_buffer( er_gui_t * const er_gui ) {

        /* reset buffer alpha channel */
        for ( le_size_t er_parse = 3; er_parse < er_gui->gu_length; er_parse += 4 ) {

            /* reset alpha value */
            er_gui->gu_buffer[er_parse] = 0;

        }

    }

/*
    source - render methods
 */

    le_void_t er_gui_display( er_gui_t * const er_gui, er_view_t const * const er_view ) {

        /* middle position variable */
        le_size_t er_middle = er_gui->gu_width >> 1;

        /* hieght position variable */
        le_size_t er_height = 0;

        /* height step variable */
        le_size_t er_step = er_font_get_height( & er_gui->gu_font ) * 2;

        /* mode variable */
        le_enum_t er_mod = er_view_get_mode( er_view );

        /* query variable */
        le_enum_t er_qry = er_view_get_query( er_view );

        /* time variable */
        le_time_t er_tia = er_view_get_time( er_view, 0 );
        le_time_t er_tib = er_view_get_time( er_view, 1 );

        /* time variable */
        le_time_t er_tac = er_view_get_time( er_view, ( er_mod == 2 ) ? 1 : 0 );

        /* comb variable */
        le_time_t er_cmb = er_view_get_comb( er_view );

        /* span variable */
        le_size_t er_spn = er_view_get_span( er_view );

        /* reset buffer */
        er_gui_set_buffer( er_gui );

        /* display comb value */
        er_gui_range( er_gui, er_cmb, 255, er_middle, er_height, ER_GUI_CENTER );

        /* update comb value */
        er_cmb >>= 1;

        /* display time range limit */
        er_gui_display_date( er_gui, er_tac - er_cmb, 255, er_middle - 104, er_height, ER_GUI_RIGHT );
        er_gui_display_date( er_gui, er_tac + er_cmb, 255, er_middle + 104, er_height, ER_GUI_LEFT  );

        /* update text height */
        er_height += er_step;

        /* display query span */
        er_gui_display_text( er_gui, er_gui->gu_span[er_spn], 255, er_middle, er_height, ER_GUI_CENTER );

        /* compose mode/query text */
        er_gui_display_text( er_gui, er_gui->gu_time [er_mod], 255, er_middle - 52, er_height, ER_GUI_RIGHT );
        er_gui_display_text( er_gui, er_gui->gu_query[er_qry], 255, er_middle + 52, er_height, ER_GUI_LEFT  );

        /* update text height */
        er_height += er_step;

        /* check mode value */
        if ( er_mod < 3 ) {

            /* display time */
            er_gui_display_date( er_gui, er_tac, 255, er_middle, er_height, ER_GUI_CENTER );

        } else {

            /* display time */
            er_gui_display_date( er_gui, er_tia, 255, er_middle - 32, er_height, ER_GUI_RIGHT );

            /* display time */
            er_gui_display_date( er_gui, er_tib, 255, er_middle + 32, er_height, ER_GUI_LEFT );

            /* display mode */
            er_gui_display_text( er_gui, er_gui->gu_mode[er_mod], 255, er_middle, er_height, ER_GUI_CENTER );

        }

        /* buffer position */
        glRasterPos2i( er_gui->gu_left, er_gui->gu_top );

        /* enable blending */
        glEnable( GL_BLEND );

        /* display buffer */
        glDrawPixels( er_gui->gu_width, er_gui->gu_height, GL_RGBA, GL_UNSIGNED_BYTE, er_gui->gu_buffer );

        /* disable blending */
        glDisable( GL_BLEND );

    }

    le_void_t er_gui_range( er_gui_t * const er_gui, le_time_t er_range, le_byte_t const er_value, le_size_t const er_x, le_size_t const er_y, le_enum_t const er_justify ) {

        /* step array variable */
        static le_size_t er_step[5] = { 60, 60, 24, 365, 0 };

        /* suffix array variable */
        static le_char_t er_char[5] = { 'S', 'M', 'H', 'D', 'Y' };

        /* parsing variable */
        le_size_t er_parse = 0;

        /* detect relevant range */
        while ( _LE_TRUE ) {

            /* check for relevant or last step */
            if ( ( er_range < er_step[er_parse] ) || ( er_parse == 4 ) ) {

                /* compose step string */
                sprintf( ( char * ) er_gui->gu_text, "(%" _LE_TIME_P "%c)", er_range, er_char[er_parse] );

                /* display step string */
                er_gui_display_text( er_gui, er_gui->gu_text, er_value, er_x, er_y, er_justify );

                /* end detection */
                return;

            } else {

                /* compute next step */
                er_range /= er_step[er_parse];

                /* update index */
                er_parse ++;

            }

        }

    }

    le_void_t er_gui_display_date( er_gui_t * const er_gui, le_time_t const er_date, le_byte_t const er_value, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* compose date string */
        lc_time_to_string( er_date, er_gui->gu_text, 32 );

        /* display date text */
        er_gui_display_text( er_gui, er_gui->gu_text, er_value, er_x, er_y, er_justify );

    }

    le_void_t er_gui_display_text( er_gui_t * const er_gui, le_char_t const * const er_text, le_byte_t const er_value, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify ) {

        /* string length variable */
        le_size_t er_length = strlen( ( char * ) er_text );

        /* check justification */
        if ( er_justify == ER_GUI_RIGHT ) {

            /* correct x-position */
            er_x -= er_length * ( er_gui->gu_font.ft_w );

        } else if ( er_justify == ER_GUI_CENTER ) {

            /* correct x-position */
            er_x -= er_length * ( er_gui->gu_font.ft_w >> 1 );

        }

        /* display string */
        er_font_display_string( & er_gui->gu_font, er_text, er_length, er_value, er_gui->gu_buffer, er_gui->gu_width, er_gui->gu_height, er_x, er_y );

    }

