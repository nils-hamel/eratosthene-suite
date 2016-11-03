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

    /*! \file   eratosthene-motion.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Eratosthene client - motion (parallel system)
     */

/*
    header - inclusion guard
 */

    # ifndef __ER_MOTION__
    # define __ER_MOTION__

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

    # include <GL/freeglut.h>
    # include <common-include.h>
    # include <eratosthene-include.h>

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define ER_MOTION_C     { 0, { { 0.0 } }, { { 0 } }, _LE_TRUE }

    /* Define stack maximum size */
    # define ER_MOTION_STACK ( 32 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct er_motion_struct {

        le_size_t mt_push;

        le_real_t mt_pose[ER_MOTION_STACK][5];
        le_time_t mt_time[ER_MOTION_STACK][2];

    le_enum_t _status; } er_motion_t;

/*
    header - function prototypes
 */

    er_motion_t er_motion_create( le_void_t );

    le_void_t er_motion_delete( er_motion_t * er_motion );

    le_enum_t er_motion_get( er_motion_t const * const er_motion );

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

