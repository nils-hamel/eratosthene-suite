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

    # include <eratosthene-include.h>
    # include <common-include.h>
    # include <stdio.h>
    # include <stdlib.h>
    # include <math.h>
    # include <omp.h>
    # if defined ( __OS_APPLE )
    # include <OpenGL/gl.h>
    # include <OpenGL/glu.h>
    # endif
    # if defined ( __OS_LINUX )
    # include <GL/gl.h>
    # include <GL/glu.h>
    # endif
    # include <SDL2/SDL.h>

/*
    header - preprocessor definitions
 */

    /* define execution modes */
    # define ER_COMMON_EXIT  ( 0x00 )
    # define ER_COMMON_VIEW  ( 0x01 )
    # define ER_COMMON_AUTO  ( 0x02 )

    /* define model parameters */
    # define ER_COMMON_ENUM  ( 4 )
    # define ER_COMMON_SIZE  ( 16384 )
    # define ER_COMMON_PACK  ( 4 )

    /* define inertial coefficients */
    # define ER_COMMON_INE   ( 1.0 / 100.0 )
    # define ER_COMMON_IML   ( 1.0 / 8.0 )
    # define ER_COMMON_IMU   ( 8.0 )
    # define ER_COMMON_INP   ( 2.0 * LE_PI / ( LE_ADDRESS_WGS_A * 180.0 ) )
    # define ER_COMMON_INR   ( 1.0 / 800.0 )

    /* define limit and default : time */
    # define ER_COMMON_UTIME ( _LE_TIME_MAX )

    /* define limit and default : comb */
    # define ER_COMMON_DCOMB ( _LE_TIME_L( 31536000 ) )
    # define ER_COMMON_LCOMB ( _LE_TIME_L( 60 ) )
    # define ER_COMMON_UCOMB ( _LE_TIME_L( 157788000000 ) )

    /* define limit and default : address span */
    # define ER_COMMON_DSPAN ( 7 )
    # define ER_COMMON_LSPAN ( 4 )
    # define ER_COMMON_USPAN ( 8 )

    /* define limit and default : altitude */
    # define ER_COMMON_LALTI ( LE_ADDRESS_WGS_A * 0.75 )
    # define ER_COMMON_UALTI ( LE_ADDRESS_WGS_A * 3.00 )

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

