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

    /*! \file   eratosthene-client-common.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  eratosthene-suite - client - common
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_CLIENT_COMMON__
    # define __ER_CLIENT_COMMON__

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
    # include <stdlib.h>
    # include <math.h>
    # include <omp.h>
    # include <GL/gl.h>
    # include <GL/glu.h>
    # include <SDL2/SDL.h>
    # include <common-include.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* define execution modes */
    # define ER_COMMON_EXIT  ( 0x00 )
    # define ER_COMMON_VIEW  ( 0x01 )

    /* define trigonometric factors */
    # define ER_COMMON_D2R   ( 3.1415926535 / 180.0 )
    # define ER_COMMON_R2D   ( 180.0 / 3.1415926535 )

    /* define time range */
    # define ER_COMMON_LIMIT ( _LE_TIME_L( 28382400000000 ) )

    /* define inertial coefficients */
    # define ER_COMMON_INE   ( 1.0 / 100.0 )
    # define ER_COMMON_IML   ( 1.0 / 8.0 )
    # define ER_COMMON_IMU   ( 8.0 )
    # define ER_COMMON_INP   ( 2.0 * LE_PI / ( LE_ADDRESS_WGS_A * 180.0 ) )
    # define ER_COMMON_INR   ( 1.0 / 800.0 )

    /* define altitude clamped range */
    # define ER_COMMON_ALL   ( LE_ADDRESS_WGS_A * 0.75 )
    # define ER_COMMON_ALU   ( LE_ADDRESS_WGS_A * 3.00 )

    /* define model parameters */
    # define ER_COMMON_ENUM  (  4 )
    # define ER_COMMON_SPAN  ( 8 ) //(  7 )
    # define ER_COMMON_PACK  ( 16 )

    /* define keyboard modifiers */
    # define ER_COMMON_KMALL ( KMOD_LCTRL | KMOD_LALT | KMOD_LSHIFT )
    # define ER_COMMON_KMAAC ( KMOD_LCTRL | KMOD_LALT )
    # define ER_COMMON_KMCTL ( KMOD_LCTRL )
    # define ER_COMMON_KMALT ( KMOD_LALT )
    # define ER_COMMON_KMSHF ( KMOD_LSHIFT )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

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

