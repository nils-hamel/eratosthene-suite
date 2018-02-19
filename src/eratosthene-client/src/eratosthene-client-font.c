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

    le_size_t er_font_get_period( er_font_t const * const er_font ) {

        /* return font bitmap width */
        return( er_font->ft_c * er_font->ft_w );

    }
