/*
 *  eratosthene-suite - geodetic system
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

    # include "eratosthene-client-render.h"

/*
    source - model display
 */

    void er_model_display2( er_model2_t const * const er_model ) {

        glPointSize( 2 );

        glVertexPointer(3, GL_FLOAT, 0, er_model->md_vert );
        glColorPointer(3, GL_FLOAT, 0, er_model->md_colo );

        glDrawArrays(GL_POINTS, 0, er_model->md_size );

        /* Draw template earth */ {
            glRotatef( 90, 1, 0, 0 );
            glColor3f( 0.3, 0.32, 0.35 );
            GLUquadricObj*quadric;
            quadric=gluNewQuadric();
            gluQuadricDrawStyle(quadric,GLU_LINE);
            gluSphere(quadric,ER_ERA,360,180);
            gluDeleteQuadric(quadric);
        }

    }

/*
    source - model management
 */

    void er_model_update2( er_model2_t * const er_model, le_sock_t const er_socket ) {

        static int er_flag = 0;

        if ( er_flag == 0 ) { er_flag = 1;

            er_model_query2( er_model, er_socket,
                //( le_char_t * ) "/950616034/122001/10"
                //( le_char_t * ) "/950486422/1/14"
                ( le_char_t * ) "/950486422/1/14"
            );

            fprintf( stderr, "%i\n", er_model->md_size );

            /* Validation query 
            er_model_query( er_model, er_socket,
                //( le_char_t * ) "/950616034/122001/10"
                //( le_char_t * ) "/950486422/1/14"
                ( le_char_t * ) "/950486422/0/14"
            );

            fprintf( stderr, "%i\n", er_model->md_size ); */

        }

    }

/*
    source - query management
 */

    void er_model_query( er_cell_t * const er_cell, le_char_t const * const er_query, le_sock_t const er_socket ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Tracking variables */
        le_size_t er_track = 0;

        /* Socket i/o count variables */
        le_size_t er_count = 0;

        /* Socket i/o buffer variables */
        le_byte_t er_buffer[LE_NETWORK_BUFFER_SYNC] = LE_NETWORK_BUFFER_C;

        /* Array pointer variables */
        le_real_t * er_ptrp = NULL;
        le_time_t * er_ptrt = NULL;
        le_data_t * er_ptrd = NULL;

        /* Cell pointer variables */
        le_real_t * er_celp = er_cell_get_pose( er_cell );
        le_data_t * er_celd = er_cell_get_data( er_cell );

        /* Client/server query handshake */
        if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_QMOD ) != LE_ERROR_SUCCESS ) {

            /* Abort query */
            return;

        }

        /* Query string to socket buffer */
        strcpy( ( char * ) er_buffer, ( char * ) er_query );

        /* Write query address */
        if ( write( er_socket, er_buffer, LE_NETWORK_BUFFER_ADDR ) != LE_NETWORK_BUFFER_ADDR ) {

            /* Abort query */
            return;

        }

        /* Reading query elements */
        while( ( er_count = read( er_socket, er_buffer, LE_NETWORK_BUFFER_SYNC ) ) > 0 ) {

            /* Retrieve cell size */
            er_track = er_cell_get_size( er_cell );

            /* Resize cell arrays */
            er_cell_set_push( er_cell, ( er_count / LE_ARRAY_LINE ) * 3 );
            
            /* Parsing received elements */
            for ( er_parse = 0; er_parse < er_count; er_parse += LE_ARRAY_LINE, er_track += 3 ) {

                /* Compute pointers */
                er_ptrp = ( le_real_t * ) ( er_buffer + er_parse );
                er_ptrt = ( le_time_t * ) ( er_ptrp + 3 );
                er_ptrd = ( le_data_t * ) ( er_ptrt + 1 );

                /* Assign vertex */
                er_celp[ er_track + 2 ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * cos( er_ptrp[1] ) * cos( er_ptrp[0] );
                er_celp[ er_track     ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * cos( er_ptrp[1] ) * sin( er_ptrp[0] );
                er_celp[ er_track + 1 ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * sin( er_ptrp[1] );

                /* Assign color */
                er_celd[ er_track     ] = er_ptrd[0];
                er_celd[ er_track + 1 ] = er_ptrd[1];
                er_celd[ er_track + 2 ] = er_ptrd[2];

            }

        }

    }

    void er_model_query2( er_model2_t * const er_model, le_sock_t const er_socket, le_char_t const * const er_query ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Socket i/o count variables */
        le_size_t er_count = 0;

        /* Socket i/o buffer variables */
        le_byte_t er_buffer[LE_NETWORK_BUFFER_SYNC] = LE_NETWORK_BUFFER_C;

        /* Array pointer variables */
        le_real_t * er_ptrp = NULL;
        le_time_t * er_ptrt = NULL;
        le_data_t * er_ptrd = NULL;

        /* Client/server query handshake */
        if ( le_client_handshake_mode( er_socket, LE_NETWORK_MODE_QMOD ) != LE_ERROR_SUCCESS ) {

            /* Abort query */
            return;

        }

        /* Query string to socket buffer */
        strcpy( ( char * ) er_buffer, ( char * ) er_query );

        /* Write query address */
        if ( write( er_socket, er_buffer, LE_NETWORK_BUFFER_ADDR ) != LE_NETWORK_BUFFER_ADDR ) {

            /* Abort query */
            return;

        }

        /* Reading query elements */
        while( ( er_count = read( er_socket, er_buffer, LE_NETWORK_BUFFER_SYNC ) ) > 0 ) {

            /* Update model structure - vertex array */
            if ( ( er_model->md_swap = ( float * ) realloc( ( void * ) er_model->md_vert, ( er_model->md_size + ( er_count / LE_ARRAY_LINE ) ) * sizeof( float ) * 3 ) ) == NULL ) {

                /* Abort query */
                return;

            } else {

                /* Assign reallocated memory block */
                er_model->md_vert = er_model->md_swap;

            }

            if ( ( er_model->md_swap = ( float * ) realloc( ( void * ) er_model->md_colo, ( er_model->md_size + ( er_count / LE_ARRAY_LINE ) ) * sizeof( float ) * 3 ) ) == NULL ) {

                /* Abort query */
                return;

            } else {

                /* Assign reallocted memory block */
                er_model->md_colo = er_model->md_swap;

            }
            
            /* Parsing received elements */
            for ( er_parse = 0; er_parse < er_count; er_parse += LE_ARRAY_LINE ) {

                /* Compute pointers */
                er_ptrp = ( le_real_t * ) ( er_buffer + er_parse );
                er_ptrt = ( le_time_t * ) ( er_ptrp + 3 );
                er_ptrd = ( le_data_t * ) ( er_ptrt + 1 );

                /* Assign vertex */
                ( er_model->md_vert )[ er_model->md_size + 2 ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * cos( er_ptrp[1] ) * cos( er_ptrp[0] );
                ( er_model->md_vert )[ er_model->md_size     ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * cos( er_ptrp[1] ) * sin( er_ptrp[0] );
                ( er_model->md_vert )[ er_model->md_size + 1 ] = ( ( er_ptrp[2] * 0.001 ) + ER_ERA ) * sin( er_ptrp[1] );

                /* Assign color */
                ( er_model->md_colo )[ er_model->md_size     ] = er_ptrd[0];
                ( er_model->md_colo )[ er_model->md_size + 1 ] = er_ptrd[1];
                ( er_model->md_colo )[ er_model->md_size + 2 ] = er_ptrd[2];

                /* Update array size */
                er_model->md_size += 3;

            }

        }

    }

