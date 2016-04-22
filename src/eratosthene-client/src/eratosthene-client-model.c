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

                    /* Vertex and color array configuration */
                    glVertexPointer( 3, GL_DOUBLE, 0, er_cell_get_pose( er_model->md_cell + er_parse ) );
                    glColorPointer ( 3, GL_FLOAT , 0, er_cell_get_data( er_model->md_cell + er_parse ) );

                    /* Trigger cell primitive drawing */
                    glDrawArrays( GL_POINTS, 0, er_cell_get_size( er_model->md_cell + er_parse) / 3 );

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

    void er_model_update( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t er_alt ) {

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Scale variables */
        le_size_t er_depth = 0;

        /* Address variables */
        le_address_t er_refa = LE_ADDRESS_C;
        le_address_t er_qrya = LE_ADDRESS_C;

        /* Scale model variables */
        le_real_t er_scale = ( ( er_alt / 1000.0 ) / ER_ERA ) - 1.0;

        /* Position array */
        le_real_t er_pose[3] = { er_lon, er_lat, 0.0 };

        /* Compute adaptative depth */
        er_scale = 32.0 * ( 1.0 - ( exp( pow( fabs( er_scale ), 0.0905 ) ) / exp( 1.0 ) ) );

        /* Clamp adaptative depth */
        er_depth = ( er_scale < 4.0 ) ? 4 : ( ( er_scale > 14.0 ) ? 14 : er_scale );

        /* Assign address time and depth */
        le_address_set_time ( & er_qrya, er_time );
        le_address_set_depth( & er_qrya, 8 );

        /* Assign address size */
        le_address_set_size( & er_refa, er_depth + 1 );
        le_address_set_size( & er_qrya, er_depth + 1 );

        /* Compute reference address */
        le_address_set_pose( & er_refa, er_pose );

        le_address_cvas( & er_refa, ( er_model->md_cell )->ce_push );
        //fprintf( stderr, "Pushing : %5i %s (Reference)\n", er_depth, ( er_model->md_cell )->ce_push );

        int x = 0, y = 0;

        for ( x = -3; x < 4; x ++ ) {
        for ( y = -3; y < 4; y ++ ) {

            memcpy( er_qrya.as_addr, er_refa.as_addr, _LE_USE_DEPTH );
            le_address_set_shift( & er_qrya, er_depth, x, y, 0 );
            le_address_cvas( & er_qrya, ( er_model->md_cell + er_parse )->ce_push );
            //fprintf( stderr, "Pushing : %5i %5i %s\n", x, y, ( er_model->md_cell + er_parse )->ce_push );
            er_parse ++;

        }
        }

    }

    void er_model_client( le_char_t const * const er_ip, le_sock_t const er_port, er_model_t * const er_model ) {

        /* Parsing variables - static */
        le_size_t er_parse = 0;

        /* Socket handle variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Parsing cells */
        for ( ; er_parse < ER_MODEL_SEG; er_parse ++ ) {

            /* Check update necessities */
            if ( er_cell_get_update( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                // fprintf( stderr, "Update : %s\n", ( er_model->md_cell + er_parse )->ce_push );

                /* Establish server connexion */
                if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {
                    
                    /* Update cell through server query */
                    er_cell_set_query( er_model->md_cell + er_parse, er_socket );

                    /* Close server connexion */
                    er_socket = le_client_delete( er_socket );

                }

            }

        }

    }

