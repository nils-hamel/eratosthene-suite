/*
 *  eratosthene-suite - eratosthene indexation server front-end
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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

    /*! \file   eratosthene-client-times.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite client - times module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_TIMES__
    # define __ER_CLIENT_TIMES__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "eratosthene-client-common.h"
    # include "eratosthene-client-view.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_TIMES_C           { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, _LE_TRUE }

    /* define string justification */
    # define ER_TIMES_LEFT        ( 0 )
    # define ER_TIMES_RIGHT       ( 1 )
    # define ER_TIMES_CENTER      ( 2 )

/*
    header - preprocessor macros
 */

    /* define rounding macro */
    # define er_times_rnd( t, r ) ( ( ( ( t ) / ( r ) ) * ( r ) ) + ( r ) )

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct er_times_struct {

        le_size_t   tm_width;
        le_size_t   tm_height;
        le_size_t   tm_length;
        le_size_t   tm_offset;

        le_size_t   tm_sh1;
        le_size_t   tm_sh2;
        le_size_t   tm_sh3;
        le_size_t   tm_bh1;
        le_size_t   tm_bh2;
        le_size_t   tm_middle;

        le_byte_t * tm_buffer;

    le_enum_t _status; } er_times_t;

/*
    header - function prototypes
 */

    er_times_t er_times_create( le_void_t );

    le_void_t er_times_delete( er_times_t * const er_times );

    le_void_t er_times_display( er_times_t const * const er_times, er_view_t const * const er_view );

    le_void_t er_times_display_date( le_time_t const er_time, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify );

    le_void_t er_times_display_text( le_char_t const * const er_text, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

