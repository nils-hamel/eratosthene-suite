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

    # include "eratosthene-client-model.h"

/*
    source - constructor/destructor methods
 */

    er_model_t er_model_create( le_void_t ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Model variables */
        er_model_t er_model = { 1, { ER_CELL_C } };

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Create cell */
            ( er_model.md_cell )[er_parse] = er_cell_create();

        }

        /* Return constructed model */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) { fprintf( stderr, "DELETE\n" );

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Delete cell */
            er_cell_delete( ( er_model->md_cell ) + er_parse );

        }

    }

/*
    source - accessor methods
 */

    le_size_t er_model_get_psiz( er_model_t const * const er_model ) {

        /* Return model point size */
        return( er_model->md_psiz );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_psiz( er_model_t * const er_model, le_size_t er_psiz ) {

        /* Assign model point size */
        er_model->md_psiz = er_psiz;

    }

/*
    source - model display
 */

    void er_model_main( er_model_t const * const er_model ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Point size configuration */
        glPointSize( 4 ); //er_model->md_psiz );

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Check cell state */
            if ( er_model->md_cell[er_parse].ce_size > 0 ) {

                /* Vertex and color array configuration */
                glVertexPointer( 3, GL_DOUBLE, 0, ( er_model->md_cell + er_parse )->ce_pose );
                glColorPointer ( 3, GL_FLOAT , 0, ( er_model->md_cell + er_parse )->ce_data );

                /* Trigger cell primitive drawing */
                glDrawArrays( GL_POINTS, 0, ( er_model->md_cell + er_parse)->ce_size / 3 );

            }

        }

        /* Earth frame - orientation */
        glRotatef( 90.0, 1.0, 0.0, 0.0 );

        /* Earth frame - color */
        glColor3f( 0.3, 0.32, 0.35 );

        /* Earth frame - model */
        GLUquadricObj * er_earth = gluNewQuadric();
        gluQuadricDrawStyle( er_earth, GLU_LINE );
        gluSphere( er_earth, ER_ERA, 360, 180);
        gluDeleteQuadric( er_earth );

    }

/*
    source - model management
 */

    void er_model_update( le_char_t const * const er_ip, le_sock_t const er_port, er_model_t * const er_model, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* State variables */
        static le_enum_t er_state = _LE_FALSE;

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Socket handle variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Query variables */
        le_char_t er_query[LE_NETWORK_BUFFER_ADDR] = { 0 };

        /* Check state */
        if ( er_state == _LE_FALSE ) {

            /* Update state */
            er_state = _LE_TRUE;

            /* Parsing variables */
            le_size_t er_index = 0;

            /* Buffer variables */
            le_size_t er_stamp = 0;

            /* Initialise cells */
            for ( er_parse = 0; er_parse < ER_MODEL_SEG; er_parse ++ ) {

                /* Initialise cell */
                er_model->md_cell[er_parse].ce_size = 0;
                er_model->md_cell[er_parse].ce_pose = NULL;
                er_model->md_cell[er_parse].ce_data = NULL;
                er_model->md_cell[er_parse].ce_swap = NULL;

                /* Compose static address */
                for ( er_index = 0, er_stamp = er_parse; er_index < 5; er_index ++, er_stamp /= 4 ) {

                    /* Assign digits */
                    er_model->md_cell[er_parse].ce_addr[4-er_index] = ( er_stamp % 4 ) + 48;

                }

            }

        }

        /* Parsing cells */
        for ( er_parse = 0; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Check cell state */
            if ( er_model->md_cell[er_parse].ce_stat == _LE_FALSE ) {

                /* Build query strings */
                sprintf( ( char * ) er_query, "/950486422/%s/8", er_model->md_cell[er_parse].ce_addr );

                /* open server */
                if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {

                    /* Send query to server */
                    er_model_query( er_model->md_cell + er_parse, er_query, er_socket );

                    /* close server */
                    le_client_delete( er_socket );

                }

                /* Update cell state */
                er_model->md_cell[er_parse].ce_stat = _LE_TRUE;

            }

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
        le_real_t * er_celp = NULL;
        le_data_t * er_celd = NULL;

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
            if ( er_cell_set_push( er_cell, ( er_count / LE_ARRAY_LINE ) * 3 ) == LE_ERROR_SUCCESS ) {

                /* Retrieve array pointers */
                er_celp = er_cell_get_pose( er_cell );
                er_celd = er_cell_get_data( er_cell );
                
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

    }

