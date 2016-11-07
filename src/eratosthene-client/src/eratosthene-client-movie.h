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

    /*! \file   eratosthene-client-times.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - times module
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_MOVIE__
    # define __ER_CLIENT_MOVIE__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

/*
    header - external includes
 */

    # include <stdio.h>
    # include <math.h>
    # include <GL/freeglut.h>
    # include <common-include.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define ER_MOVIE_C     { 0, 0, NULL, { 0 }, 0, 0, 0.0, 0, { { 0.0 } }, _LE_TRUE }

    /* define stack size */
    # define ER_MOVIE_STACK 32

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct er_movie_struct {

        le_size_t   mv_wbuffer;
        le_size_t   mv_hbuffer;
        le_byte_t * mv_pbuffer;

        le_char_t   mv_path[256];

        le_size_t   mv_index;
        le_size_t   mv_parse;
        le_real_t   mv_param;

        le_size_t   mv_stack;

        le_real_t   mv_poses[ER_MOVIE_STACK][5];

    le_enum_t _status; } er_movie_t;


/*
    header - function prototypes
 */

    er_movie_t er_movie_create( le_void_t );

    le_void_t er_movie_delete( er_movie_t * const er_movie );

    le_real_t er_movie_get_value( er_movie_t const * const er_movie, le_size_t const er_index );

    le_void_t er_movie_set_reset( er_movie_t * const er_movie );

    le_void_t er_movie_set_empty( er_movie_t * const er_movie );

    le_void_t er_movie_set_point( er_movie_t * const er_movie, le_real_t const er_vlon, le_real_t const er_vlat, le_real_t const er_valt, le_real_t const er_vazm, le_real_t const er_vgam );

    le_enum_t er_movie( er_movie_t * const er_movie );

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

