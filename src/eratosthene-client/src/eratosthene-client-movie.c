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

        /* created structure variables */
        er_movie_t er_movie = ER_MOVIE_C;

        /* return created structure */
        return( er_movie );

    }

    le_void_t er_movie_delete( er_movie_t * const er_movie ) {

        /* deleted structure variables */
        er_movie_t er_delete = ER_MOVIE_C;

        /* delete structure */
        * er_movie = er_delete;

    }

/*
    source - accessor methods
 */

    le_enum_t er_movie_get( er_movie_t const * const er_movie ) {

        /* check state - return answer */
        return( er_movie->cm_push > 1 ? _LE_TRUE : _LE_FALSE );

    }

/*
    source - manipulator methods
 */

    le_void_t er_movie_set_clear( er_movie_t * const er_movie ) {

        /* Clear movietic stack */
        er_movie->cm_push = 0;

    }

    le_void_t er_movie_set_push( er_movie_t * const er_movie, le_real_t const er_valt, le_real_t const er_vlon, le_real_t const er_vlat, le_real_t const er_vazm, le_real_t const er_vgam, le_time_t const er_time1, le_time_t const er_time2 ) {

        /* check consistency */
        if ( ( er_movie->cm_push + 1 ) >= ER_MOVIE_STACK ) return;

        /* Push movietic marker - position */
        er_movie->cm_pose[er_movie->cm_push][0] = er_valt;
        er_movie->cm_pose[er_movie->cm_push][1] = er_vlon;
        er_movie->cm_pose[er_movie->cm_push][2] = er_vlat;
        er_movie->cm_pose[er_movie->cm_push][3] = er_vazm;
        er_movie->cm_pose[er_movie->cm_push][4] = er_vgam;

        /* Push movietic marker - time */
        er_movie->cm_time[er_movie->cm_push][0] = er_time1;
        er_movie->cm_time[er_movie->cm_push][1] = er_time2;

        /* Update stack */
        er_movie->cm_push ++;

    }

/*
    source - main methods
 */

    le_void_t er_movie( er_movie_t const * const er_movie ) {

        fprintf( stderr, "movie triggered\n" );

    }

