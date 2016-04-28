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

    er_model_t er_model_create( le_size_t er_cells ) {

        /* Model variables */
        er_model_t er_model = ER_MODEL_C;

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Allocate cell array memory */
        if ( ( er_model.md_cell = ( er_cell_t * ) malloc( er_cells * sizeof( er_cell_t ) ) ) != NULL ) {

            /* Assign cell array size */
            er_model.md_size = er_cells;

            /* Parsing cells array */
            for ( ; er_parse < er_model.md_size; er_parse ++ ) {

                /* Initialise cell */
                ( er_model.md_cell )[er_parse] = er_cell_create();

            }

        }

        /* Return constructed model */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* Model variables */
        er_model_t er_reset = ER_MODEL_C;

        /* Parsing variables */
        le_size_t er_parse = 0;

        /* Check cells array state */
        if ( er_model->md_size > 0 ) {

            /* Parsing cells array */
            for ( ; er_parse < er_model->md_size; er_parse ++ ) {

                /* Delete cell */
                er_cell_delete( ( er_model->md_cell ) + er_parse );

            }

            /* Unallocate cells array memory */
            free( er_model->md_cell );

        }

        /* Clear model fields */
        * ( er_model ) = er_reset;

    }

/*
    source - model display methods
 */

    le_void_t er_model_main( er_model_t * const er_model ) {

        /* Parsing and count variables */
        le_size_t er_parse = 0;
        le_size_t er_count = 0;

        /* Display cells content */
        for ( ; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check cell content */
            if ( ( er_count = er_cell_get_size( ( er_model->md_cell ) + er_parse ) ) > 0 ) {

                /* Vertex and color pointer to cell arrays */
                glVertexPointer( 3, ER_MODEL_VA, 0, er_cell_get_pose( ( er_model->md_cell ) + er_parse ) );
                glColorPointer ( 3, ER_MODEL_CA, 0, er_cell_get_data( ( er_model->md_cell ) + er_parse ) );

                /* Display graphical primitives */
                glDrawArrays( GL_POINTS, 0, er_count / 3 );

            }

        }

    }

    le_void_t er_model_earth( le_void_t ) {

        /* Earth frame - orientation */
        glRotated( 90.0, 1.0, 0.0, 0.0 );

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
    source - model update methods
 */

    le_void_t er_model_update( er_model_t * const er_model, le_time_t const er_time, le_real_t const er_lon, le_real_t const er_lat, le_real_t er_alt ) {

    }

/*
    source - model query methods
 */

    le_void_t er_model_query( er_model_t * const er_model, le_char_t const * const er_ip, le_sock_t const er_port ) {

        /* Parasing variables */
        le_size_t er_parse = 0;
        le_size_t er_find  = 0;

        /* Flag variables */
        le_enum_t er_state = _LE_FALSE;

        /* Socket variables */
        le_sock_t er_socket = _LE_SOCK_NULL;

        /* Parsing cells array */
        for ( er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check pushed address */
            if ( er_cell_get_push( ( er_model->md_cell ) + er_parse ) == _LE_TRUE ) {

                /* Parsing cells array */
                er_find = 0; do {

                    /* Comapre addresses */
                    er_state = er_cell_set_swap( ( er_model->md_cell ) + er_parse, ( er_model->md_cell ) + er_find );

                /* Ending condition */
                } while ( ( er_state == _LE_FALSE ) && ( ( ++ er_find ) < er_model->md_size ) );

            }

        }

        /* Parsing cells array */
        for ( er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check update necessities */
            if ( er_cell_get_update( ( er_model->md_cell ) + er_parse ) == _LE_TRUE ) {

                /* Establish server connexion */
                if ( ( er_socket = le_client_create( er_ip, er_port ) ) != _LE_SOCK_NULL ) {
                    
                    /* Update cell through server query */
                    er_cell_set_query( ( er_model->md_cell ) + er_parse, er_socket );

                    /* Close server connexion */
                    er_socket = le_client_delete( er_socket );

                }

            }

        }

    }

