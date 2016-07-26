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

    # include "eratosthene-client-server.h"

/*
    header - external includes
 */

    # include <time.h>
    # include <GL/freeglut.h>
    # include <common-include.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_TIMES_C           { NULL, _LE_USE_PORT, _LE_TRUE, _LE_TIME_NULL, LE_ARRAY_C, _LE_TIME_NULL, ER_TIMES_YEAR, 0, { _LE_SIZE_NULL, _LE_SIZE_NULL }, 0, NULL, _LE_TRUE }

    /* Define modification modes */
    # define ER_TIMES_DECREASE    ( 0 )
    # define ER_TIMES_INCREASE    ( 1 )

    /* Define standard intervalle */
    # define ER_TIMES_DAY         ( _LE_TIME_L( 86400 ) )
    # define ER_TIMES_YEAR        ( _LE_TIME_L( 86400 ) * _LE_TIME_L( 30 ) * _LE_TIME_L ( 12 ) )

    /* Define graduation configuration */
    # define ER_TIMES_GRAD_SCALE  ( _LE_TIME_L( 1000000000 ) )
    # define ER_TIMES_GRAP_DEPTH  ( 5 )

    /* Define zoom limitations */
    # define ER_TIMES_ZOOM_MIN    ( ER_TIMES_DAY * _LE_TIME_L(     5 ) )
    # define ER_TIMES_ZOOM_MAX    ( ER_TIMES_DAY * _LE_TIME_L( 91250 ) )

    /* Define string justification flags */
    # define ER_TIMES_JUST_LEFT   ( 0 )
    # define ER_TIMES_JUST_RIGHT  ( 1 )
    # define ER_TIMES_JUST_CENTER ( 2 )

/*
    header - preprocessor macros
 */

    /* Define rounding macro */
    # define ER_TIMES_ROUND(t,r)  ( ( ( t / r ) * r ) + r )

    /* Define SRTM macro (default) */
    # define ER_TIMES_SRTM(t)     ( ( _LE_TIME_L( 950486422 ) / t ) * t )

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct er_times_struct {

        le_char_t * tm_svip;
        le_sock_t   tm_port;

        le_enum_t   tm_update;
        le_time_t   tm_tparam;
        le_array_t  tm_tarray;

        le_time_t   tm_pose;
        le_time_t   tm_zoom;
        le_size_t   tm_near;
        le_size_t   tm_view[2];

        le_size_t   tm_size;
        le_time_t * tm_time;

        //le_array_t  tm_stack;

    le_enum_t _status; } er_times_t;

/*
    header - function prototypes
 */

    er_times_t er_times_create( le_char_t * const er_ip, le_sock_t const er_port );

    le_void_t er_times_delete( er_times_t * const er_times );

    le_time_t er_times_get( er_times_t const * const er_times );

    le_enum_t er_times_get_update( er_times_t * const er_times );

    le_void_t er_times_set( er_times_t * const er_times, le_size_t const er_index );

    le_void_t er_times_set_default( er_times_t * const er_times );

    le_void_t er_times_set_nearest( er_times_t * const er_times );

    le_void_t er_times_set_zoom( er_times_t * const er_times, le_size_t const er_mode );

    le_void_t er_times_set_pose( er_times_t * const er_times, le_size_t const er_mode );

    le_void_t er_times_display( er_times_t const * const er_times );

    le_void_t er_times_display_date( le_time_t const er_time, le_size_t er_x, le_size_t er_y, le_enum_t const er_justify, le_enum_t const er_color );

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

