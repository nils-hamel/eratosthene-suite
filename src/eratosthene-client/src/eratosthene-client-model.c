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

    # include "eratosthene-client-model.h"

/*
    source - constructor/destructor methods
 */

    er_model_t er_model_create( le_char_t * const er_ip, le_sock_t const er_port ) {

        /* Returned structure variables */
        er_model_t er_model = ER_MODEL_C;

        /* Assign server network configuration */
        er_model.md_svip = er_ip;
        er_model.md_port = er_port;

        /* Retrieve server configuration */
        if ( ( er_model.md_sparam = er_client_server_sparam( er_ip, er_port ) ) == _LE_SIZE_NULL ) {

            /* Send message */
            return( er_model._status = _LE_FALSE, er_model );

        }

        /* Retrieve server configuration */
        if ( ( er_model.md_tparam = er_client_server_tparam( er_ip, er_port ) ) == _LE_TIME_NULL ) {

            /* Send message */
            return( er_model._status = _LE_FALSE, er_model );

        }

        /* Allocate cells stack memory */
        if ( ( er_model.md_cell = ( er_cell_t * ) malloc( ( er_model.md_size = ER_MODEL_STACK ) * sizeof( er_cell_t ) ) ) == NULL ) {

            /* Send message */
            return( er_model._status = _LE_FALSE, er_model );

        }

        /* Initialise model cells */
        for ( le_size_t er_parse = 0; er_parse < er_model.md_size; er_parse ++ ) {

            /* Initialise cell */
            er_model.md_cell[er_parse] = er_cell_create();

        }

        /* Return created model */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* Model variables */
        er_model_t er_reset = ER_MODEL_C;

        /* Check cells stack state */
        if ( er_model->md_size > 0 ) {

            /* Delete model cells */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* Delete cell */
                er_cell_delete( er_model->md_cell + er_parse );

            }

            /* Unallocate cells stack memory */
            free( er_model->md_cell );

        }

        /* Clear model fields */
        * ( er_model ) = er_reset;

    }

/*
    source - accessor methods
 */

    le_size_t er_model_get_cell( er_model_t const * const er_model ) {

        /* Parsing cell array */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check cell state - returned index */
            if ( er_cell_get_flag( er_model->md_cell + er_parse ) == _LE_FALSE ) return( er_parse );

        /* Return index */
        } return( er_model->md_size );

    }

    le_enum_t er_model_get_update( er_model_t * const er_model, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Static position memory variables */
        static le_real_t er_mlon = 0.0, er_mlat = 0.0, er_malt = 0.0;

        /* Returned value variables */
        le_enum_t er_return = _LE_FALSE;

        /* Check update necessities - update return */
        if ( er_mlon != er_lon ) er_mlon = er_lon, er_return = _LE_TRUE;
        if ( er_mlat != er_lat ) er_mlat = er_lat, er_return = _LE_TRUE;
        if ( er_malt != er_alt ) er_malt = er_alt, er_return = _LE_TRUE;

        /* Send answer */
        return( er_return );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_update_prepare( er_model_t * const er_model ) {

        /* Reset update index */
        er_model->md_push = 1;

        /* Parsing model cells */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* Reset cell flag */
            er_cell_set_flag( er_model->md_cell + er_parse, _LE_FALSE );

        }

    }

    le_void_t er_model_set_update_cell( er_model_t * const er_model, le_address_t * const er_enum, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt ) {

        /* Enumerator size variables */
        le_size_t er_scale = le_address_get_size( er_enum );

        /* Scale base variables */
        le_size_t er_base = LE_ADDRESS_BASE( er_scale );

        /* Distance variables */
        le_real_t er_dist = 0.0;

        /* Parsing scale digits */
        for ( le_size_t er_digit = 0; er_digit < er_base; er_digit ++ ) {

            /* Update enumerator size */
            le_address_set_size( er_enum, er_scale + 1 );

            /* Assign enumerator digit */
            le_address_set_digit( er_enum, er_scale, er_digit );

            /* Check enumeration constraint */
            if ( er_scale >= 4 ) {

                /* Compute distance */
                er_dist = er_geodesy_distance( er_enum, er_lon, er_lat, er_alt );

                /* Check selection criterion */
                if ( er_dist < er_geodesy_limit( er_alt ) ) {

                    /* Check depth criterion */
                    if ( fabs( er_geodesy_depth( er_dist, er_model->md_sparam, ER_MODEL_DEPTH ) - ( le_real_t ) er_scale ) < 1.0 ) {

                        /* Check cells stack */
                        if ( er_model->md_push < er_model->md_size ) {

                            /* Set address depth */
                            le_address_set_depth( er_enum, ER_MODEL_DEPTH );

                            /* Set cell address */
                            er_cell_set_push( er_model->md_cell + ( er_model->md_push ++ ), er_enum );

                        }

                    } else {

                        /* Check enumeration boundary */
                        if ( ( er_scale + ER_MODEL_DEPTH + 2 ) < er_model->md_sparam ) {

                            /* Set address depth */
                            le_address_set_depth( er_enum, 0 );

                            /* Set cell address */
                            er_cell_set_push( er_model->md_cell, er_enum );

                            /* Swap addresses */
                            er_cell_set_swap( er_model->md_cell, er_model->md_cell );

                            /* Check parent cell */
                            if ( er_cell_io_query( er_model->md_cell, er_model->md_svip, er_model->md_port ) > 0 ) {

                                /* Constrained enumeration */
                                er_model_set_update_cell( er_model, er_enum, er_lon, er_lat, er_alt ); 

                            }

                        }

                    }

                }

            /* Constraintless enumeration */
            } else { er_model_set_update_cell( er_model, er_enum, er_lon, er_lat, er_alt ); }

        }

    }

    le_void_t er_model_set_update_query( er_model_t * const er_model ) {

        /* Parsing variables */
        le_size_t er_parse = 0;
        le_size_t er_inner = 0;

        /* Searching variables */
        le_size_t er_found  = 0;

        /* Parsing model cells */
        for ( er_parse = 1; er_parse < er_model->md_push; er_parse ++ ) {

            /* Check pushed address */
            if ( er_cell_get_push( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* Reset cell search */
                er_inner = 1, er_found = er_model->md_size;

                /* Searching allocation */
                while ( ( er_inner < er_model->md_size ) && ( er_found == er_model->md_size ) ) {

                    /* Compare address and pushed address */
                    if ( er_cell_get_match( er_model->md_cell + er_inner, er_model->md_cell + er_parse ) == _LE_TRUE ) {

                        /* Reset pushed address */
                        er_cell_set_pop( er_model->md_cell + er_parse );

                        /* Update cell flag */
                        er_cell_set_flag( er_model->md_cell + er_inner, _LE_TRUE );

                        /* Update cell flag */
                        er_cell_set_draw( er_model->md_cell + er_inner, _LE_TRUE );

                    /* Update cell search */
                    er_found = er_inner; } else { er_inner ++; }

                }

            }

        }

        /* Parsing model cells */
        for ( er_parse = 1; er_parse < er_model->md_push; er_parse ++ ) {

            /* Check pushed address */
            if ( er_cell_get_push( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* Check search results */
                if ( ( er_found = er_model_get_cell( er_model ) ) != er_model->md_size ) {

                    /* Swap address and pushed address */
                    er_cell_set_swap( er_model->md_cell + er_found, er_model->md_cell + er_parse );

                    /* Update cell array */
                    er_cell_io_query( er_model->md_cell + er_found, er_model->md_svip, er_model->md_port );

                    /* Update cell flag */
                    er_cell_set_flag( er_model->md_cell + er_found, _LE_TRUE );

                    /* Update cell state */
                    er_cell_set_draw( er_model->md_cell + er_found, _LE_TRUE );

                }

                /* Reset pused address */
                er_cell_set_pop( er_model->md_cell + er_parse );

            }

        }

    }

    le_void_t er_model_set_update_terminate( er_model_t * const er_model ) {

        /* Parsing model cells */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check cell flag */
            if ( er_cell_get_flag( er_model->md_cell + er_parse ) == _LE_FALSE ) {

                /* Update cell state */
                er_cell_set_draw( er_model->md_cell + er_parse, _LE_FALSE );

            }

        }

    }

/*
    source - model display methods
 */

    le_void_t er_model_display_cell( er_model_t * const er_model, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt, le_real_t const er_azm, le_real_t const er_gam ) {

        /* Rotation vector variables */
        le_real_t * er_vcell = NULL;

        /* Optimisation variables */
        le_real_t er_cosl = cos( - er_lon * ER_D2R );
        le_real_t er_sinl = sin( - er_lon * ER_D2R );
        le_real_t er_cosa = cos( + er_lat * ER_D2R );
        le_real_t er_sina = sin( + er_lat * ER_D2R );

        /* Rotation matrix variables */
        le_real_t er_rcell[3][3] = {
        { + er_cosl          , +     0.0, + er_sinl           },
        { + er_sina * er_sinl, + er_cosa, - er_sina * er_cosl },
        { - er_cosa * er_sinl, + er_sina, + er_cosa * er_cosl }
        };

        /* Motion management - tilt rotation */
        glRotated( - er_gam, 1.0, 0.0, 0.0 );

        /* Motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_alt + ER_ERA );

        /* Motion management - azimuth rotation */
        glRotated( + er_azm, 0.0, 0.0, 1.0 );

        /* Display cells content */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* Check cell content */
            if ( er_cell_get_draw( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* Vertex and color pointer to cell arrays */
                glVertexPointer( 3, ER_MODEL_VERTEX, 0, er_cell_get_pose( ( er_model->md_cell ) + er_parse ) );
                glColorPointer ( 3, ER_MODEL_COLORS, 0, er_cell_get_data( ( er_model->md_cell ) + er_parse ) );

                /* Cell matrix */
                glPushMatrix(); {

                    /* Assign cell edge array */
                    er_vcell = ( ( er_model->md_cell ) + er_parse )->ce_edge;

                    /* Motion management - cell edge translation */
                    glTranslated( 
                    er_rcell[0][0] * er_vcell[0] + er_rcell[0][1] * er_vcell[1] + er_rcell[0][2] * er_vcell[2],
                    er_rcell[1][0] * er_vcell[0] + er_rcell[1][1] * er_vcell[1] + er_rcell[1][2] * er_vcell[2],
                    er_rcell[2][0] * er_vcell[0] + er_rcell[2][1] * er_vcell[1] + er_rcell[2][2] * er_vcell[2] - ER_ERA
                    );

                    /* Motion management - planimetric rotation */
                    glRotated( + er_lat, 1.0, 0.0, 0.0 );
                    glRotated( - er_lon, 0.0, 1.0, 0.0 );

                    /* Display graphical primitives */
                    //glDrawArrays( GL_POINTS, 0, er_count / 3 );
                    glDrawArrays( GL_POINTS, 0, er_cell_get_size( er_model->md_cell + er_parse ) / 3 );

                /* Cell matrix */
                } glPopMatrix();

            }

        }

    }

    le_void_t er_model_display_earth( le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_alt, le_real_t const er_azm, le_real_t const er_gam ) {

        /* Motion management - tilt rotation */
        glRotated( - er_gam, 1.0, 0.0, 0.0 );

        /* Motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_alt + ER_ERA );

        /* Motion management - azimuth rotation */
        glRotated( + er_azm, 0.0, 0.0, 1.0 );

        /* Earth wireframe - centering */
        glTranslated( 0.0, 0.0, - ER_ERA );

        /* Motion management - planimetric rotation */
        glRotated( + er_lat, 1.0, 0.0, 0.0 );
        glRotated( - er_lon, 0.0, 1.0, 0.0 );

        /* Earth wireframe - orientation */
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

