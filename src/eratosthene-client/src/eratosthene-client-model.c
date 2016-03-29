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
            ( er_model.md_cell )[er_parse] = er_cell_create( er_parse );

        }

        /* Return constructed model */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

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

        /* Apply limitation */
        if ( ( er_psiz > 0 ) && ( er_psiz <= 8 ) ) {

            /* Assign model point size */
            er_model->md_psiz = er_psiz;

        }

    }

/*
    source - model display
 */

    void er_model_main( er_model_t * const er_model ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Display cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Check cell size */
            if ( er_cell_get_size( er_model->md_cell + er_parse ) > 0 ) {

                /* Check cell state */
                if ( er_cell_get_state( er_model->md_cell + er_parse ) == ER_CELL_IDLE ) {

                    /* Lock cell */
                    er_cell_set_state( er_model->md_cell + er_parse, ER_CELL_DRAW );

                    /* Vertex and color array configuration */
                    glVertexPointer( 3, GL_DOUBLE, 0, ( er_model->md_cell + er_parse )->ce_pose );
                    glColorPointer ( 3, GL_FLOAT , 0, ( er_model->md_cell + er_parse )->ce_data );

                    /* Trigger cell primitive drawing */
                    glDrawArrays( GL_POINTS, 0, ( er_model->md_cell + er_parse)->ce_size / 3 );

                    /* Unlock cell */
                    er_cell_set_state( er_model->md_cell + er_parse, ER_CELL_IDLE );

                }

            }

        }

        /* Earth frame - orientation */
        glRotatef( 90.0, 1.0, 0.0, 0.0 );

        /* Earth frame - color */
        glColor3f( 0.3, 0.32, 0.4 );

        /* Earth model variables */
        GLUquadricObj * er_earth = gluNewQuadric();

        /* Configure quadric */
        gluQuadricDrawStyle( er_earth, GLU_LINE );

        /* Draw quadric */
        gluSphere( er_earth, ER_ERA, 360, 180 );

        /* Delete quadric */
        gluDeleteQuadric( er_earth );

    }

/*
    source - model management
 */

    void er_model_update( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Push cell address */
            sprintf( ( char * ) ( er_model->md_cell + er_parse )->ce_push, "/%" _LE_TIME_P "/%s/9", er_time, ( er_model->md_cell + er_parse )->ce_base );

        }

    }

    void er_model_client( le_char_t const * const er_ip, le_sock_t const er_port, er_model_t * const er_model ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Socket handle variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Check query necessities */
            if ( strcmp( ( char * ) ( er_model->md_cell + er_parse )->ce_addr, ( char * ) ( er_model->md_cell + er_parse )->ce_push ) != 0 ) {

                /* Establish server connexion */
                if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {
                    
                    /* Update cell through server query */
                    er_cell_set_query( er_model->md_cell + er_parse, er_socket );

                    /* Close server connexion */
                    le_client_delete( er_socket );

                }

            }

        }

    }

