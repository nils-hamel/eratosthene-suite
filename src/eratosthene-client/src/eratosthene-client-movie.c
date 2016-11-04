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

    # include "eratosthene-client-movie.h"

/*
    source - constructor/destructor methods
 */

    er_movie_t er_movie_create( le_void_t ) {

        /* create structure variables */
        er_movie_t er_movie = ER_MOVIE_C;

        /* return created structure */
        return( er_movie );

    }

    le_void_t er_movie_delete( er_movie_t * const er_movie ) {

        /* deleted structure variables */
        er_movie_t er_delete = ER_MOVIE_C;

        /* delete structure */
        ( * er_movie ) = er_delete;

    }

/*
    source - manipulator methods
 */

    le_void_t er_movie_set_empty( er_movie_t * const er_movie ) {

        /* empty control point stack */
        er_movie->mv_stack = 0;

    }

    le_void_t er_movie_set_point( er_movie_t * const er_movie, le_real_t const er_vlon, le_real_t const er_vlat, le_real_t const er_valt, le_real_t const er_vazm, le_real_t const er_vgam, le_time_t const er_timea, le_time_t const er_timeb ) {

        /* push point of view - space */
        er_movie->mv_poses[er_movie->mv_stack][0] = er_vlon;
        er_movie->mv_poses[er_movie->mv_stack][1] = er_vlat;
        er_movie->mv_poses[er_movie->mv_stack][2] = er_valt;
        er_movie->mv_poses[er_movie->mv_stack][3] = er_vazm;
        er_movie->mv_poses[er_movie->mv_stack][4] = er_vgam;

        /* push point of view - time */
        er_movie->mv_times[er_movie->mv_stack][0] = er_timea;
        er_movie->mv_times[er_movie->mv_stack][1] = er_timeb;

        /* update stack state */
        er_movie->mv_stack ++;

    }

/*
    source - movie methods
 */

    le_enum_t er_movie( er_movie_t const * const er_movie ) {

        /* check control point stack */
        if ( er_movie->mv_stack < 2 ) {

            /* send message */
            return( _LE_FALSE );

        }

        return( _LE_TRUE );

    }

