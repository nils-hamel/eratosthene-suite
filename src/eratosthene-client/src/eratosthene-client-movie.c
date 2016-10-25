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
    # include "eratosthene-client.h"

/*
    source - extern variables
 */

    extern er_client_t er_client;
    le_byte_t * er_buffer = NULL;

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

    le_enum_t er_movie_get_reco( er_movie_t const * const er_movie ) {

        return( er_movie->cm_reco );

    }

/*
    source - manipulator methods
 */

    le_void_t er_movie_set_void( er_movie_t * const er_movie ) {

        /* empty marker stack */
        er_movie->cm_push = 0;

    }

    le_void_t er_movie_set_push( er_movie_t * const er_movie, le_real_t const er_valt, le_real_t const er_vlon, le_real_t const er_vlat, le_real_t const er_vazm, le_real_t const er_vgam, le_time_t const er_time1, le_time_t const er_time2 ) {

        /* check consistency */
        if ( ( er_movie->cm_push + 1 ) >= ER_MOVIE_STACK ) return;

        /* push marker - position */
        er_movie->cm_pose[er_movie->cm_push][0] = er_valt;
        er_movie->cm_pose[er_movie->cm_push][1] = er_vlon;
        er_movie->cm_pose[er_movie->cm_push][2] = er_vlat;
        er_movie->cm_pose[er_movie->cm_push][3] = er_vazm;
        er_movie->cm_pose[er_movie->cm_push][4] = er_vgam;

        /* push marker - time */
        er_movie->cm_time[er_movie->cm_push][0] = er_time1;
        er_movie->cm_time[er_movie->cm_push][1] = er_time2;

        /* update stack */
        er_movie->cm_push ++;

    }

/*
    source - main methods
 */

    le_real_t er_movie_dist( le_real_t er_l1, le_real_t er_a1, le_real_t er_h1, le_real_t er_l2, le_real_t er_a2, le_real_t er_h2 ) {

        /* cartesian position variables */
        le_real_t er_p1[3] = { 0.0 };
        le_real_t er_p2[3] = { 0.0 };

        /* convert angles to radians */
        er_l1 *= ER_D2R;
        er_a1 *= ER_D2R;
        er_l2 *= ER_D2R;
        er_a2 *= ER_D2R;

        /* convert to spherical coordinates */
        er_h1 += ER_ERA;
        er_h2 += ER_ERA;

        /* convert to cartesian coordinates */
        er_p1[0] = er_h1 * cos( er_a1 ) * cos( er_l1 );
        er_p1[1] = er_h1 * cos( er_a1 ) * sin( er_l1 );
        er_p1[2] = er_h1 * sin( er_a1 );
        er_p2[0] = er_h2 * cos( er_a2 ) * cos( er_l2 );
        er_p2[1] = er_h2 * cos( er_a2 ) * sin( er_l2 );
        er_p2[2] = er_h2 * sin( er_a2 );

        /* compute and return distance */
        return( sqrt( ( er_p1[0] - er_p2[0] ) * ( er_p1[0] - er_p2[0] ) + ( er_p1[1] - er_p2[1] ) * ( er_p1[1] - er_p2[1] ) + ( er_p1[2] - er_p2[2] ) * ( er_p1[2] - er_p2[2] ) ) );

    }

    le_void_t er_movie( er_movie_t * const er_movie ) {

        /* dynamic weights variables */
        le_real_t er_value[6] = { 0.0 };

        /* weighting function variables */
        le_real_t er_func = 0.0;

        /* weighting total variables */
        le_real_t er_total = 0.0;

        /* exportation index variables */
        le_size_t er_index = 0;

        /* exportation path variables */
        le_char_t er_export[256] = "";

        /* buffer size variables */
        le_size_t er_bw = glutGet( GLUT_SCREEN_WIDTH  );
        le_size_t er_bh = glutGet( GLUT_SCREEN_HEIGHT );

        /* buffer bytes variables */
        er_buffer = ( le_byte_t * ) malloc( er_bw * er_bh * 3 );

        /* recording ... */
        er_movie->cm_reco = _LE_TRUE;

        /* parsing stack */
        for ( le_size_t er_parse = 0; er_parse < er_movie->cm_push; er_parse ++ ) {

            /* parametric progression */
            for ( le_real_t er_param = 0.0; er_param < 1.0; er_param += 0.005 ) {

                /* initialise weighting */
                er_value[0] = 0.0;
                er_value[1] = 0.0;
                er_value[2] = 0.0;
                er_value[3] = 0.0;
                er_value[4] = 0.0;

                /* reset total weight */
                er_total = 0.0;

                /* accumulate weights */
                for ( le_size_t er_accum = 0; er_accum < er_movie->cm_push; er_accum ++ ) {

                    le_real_t er_normal = ( le_real_t ) er_accum - er_parse - er_param;

                    /* compute weighting funtion */
                    er_total += ( er_func = exp( - 3.0 * er_normal * er_normal ) );

                    /* accumulate weights */
                    er_value[0] += er_func * er_movie->cm_pose[er_accum][0];
                    er_value[1] += er_func * er_movie->cm_pose[er_accum][1];
                    er_value[2] += er_func * er_movie->cm_pose[er_accum][2];
                    er_value[3] += er_func * er_movie->cm_pose[er_accum][3];
                    er_value[4] += er_func * er_movie->cm_pose[er_accum][4];

                }

                /* compute weighted values */
                er_value[0] /= er_total;
                er_value[1] /= er_total;
                er_value[2] /= er_total;
                er_value[3] /= er_total;
                er_value[4] /= er_total;

                /* assign client values */
                er_client.cl_valt = er_value[0];
                er_client.cl_vlon = er_value[1];
                er_client.cl_vlat = er_value[2];
                er_client.cl_vazm = er_value[3];
                er_client.cl_vgam = er_value[4];

                le_address_t er_enum = LE_ADDRESS_C;

                /* compose address */
                le_address_set_time( & er_enum, 0, er_movie->cm_time[er_parse][0] );
                le_address_set_time( & er_enum, 1, er_movie->cm_time[er_parse][1] );

                /* Update model cells */
                er_model_set_update_cell( & er_client.cl_model, & er_enum, er_client.cl_vlon * ER_D2R, er_client.cl_vlat * ER_D2R, er_client.cl_valt );

                /* Server queries */
                er_model_set_update_query( & er_client.cl_model );

                /* Terminate model update */
                er_model_set_update_terminate( & er_client.cl_model );

                usleep( 100000 );

                /* compose frame exportation path */
                sprintf( ( char * ) er_export, "/tmp/export-%06" _LE_SIZE_P ".png", er_index ++ );

                /* export frame */
                lc_image_png_write( ( char * ) er_export, er_bw, er_bh, LC_IMAGE_RGB, er_buffer );

            }

        }

        glutLeaveMainLoop();

    }

