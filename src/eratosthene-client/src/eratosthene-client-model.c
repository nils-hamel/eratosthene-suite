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

        /* created structure variables */
        er_model_t er_model = ER_MODEL_C;

        /* array variables */
        le_array_t er_array = LE_ARRAY_C;

        /* array access variables */
        le_array_cf_t er_access = LE_ARRAY_CF_C;

        /* assign server network configuration */
        er_model.md_svip = er_ip;
        er_model.md_port = er_port;

        /* query server configuration array */
        if ( le_client_array( er_ip, er_port, LE_MODE_CMOD, & er_array ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( er_model._status = _LE_FALSE, er_model );

        } else {

            /* compute configuration array mapping */
            le_array_cf( le_array_get_byte( & er_array ), 0, er_access );

            /* retrieve configuration */
            er_model.md_sparam = er_access.as_size[0];
            er_model.md_tparam = er_access.as_time[0];

        }

        /* allocate cells stack memory */
        if ( ( er_model.md_cell = ( er_cell_t * ) malloc( ( er_model.md_size = ER_MODEL_STACK ) * sizeof( er_cell_t ) ) ) == NULL ) {

            /* send message */
            return( er_model._status = _LE_FALSE, er_model );

        } else {

            /* create model cells */
            for ( le_size_t er_parse = 0; er_parse < er_model.md_size; er_parse ++ ) {

                /* initialise cell */
                er_model.md_cell[er_parse] = er_cell_create();

            }

        }

        /* return created model */
        return( er_model );

    }

    le_void_t er_model_delete( er_model_t * const er_model ) {

        /* deleted structure variables */
        er_model_t er_reset = ER_MODEL_C;

        /* check stack state */
        if ( er_model->md_cell != NULL ) {

            /* delete model cells */
            for ( le_size_t er_parse = 0; er_parse < er_model->md_size; er_parse ++ ) {

                /* delete cell */
                er_cell_delete( er_model->md_cell + er_parse );

            }

            /* unallocate stack memory */
            free( er_model->md_cell );

        }

        /* delete structure */
        ( * er_model ) = er_reset;

    }

/*
    source - accessor methods
 */

    le_size_t er_model_get_cell( er_model_t const * const er_model ) {

        /* parsing cells stack */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* check cell state - return index */
            if ( er_cell_get_flag( er_model->md_cell + er_parse ) == _LE_FALSE ) return( er_parse );

        }

        /* return index */
        return( er_model->md_size );

    }

/*
    source - mutator methods
 */

    le_void_t er_model_set_update_cell( er_model_t * const er_model, le_address_t * const er_enum, er_view_t const * const er_view ) {

        /* enumerator size variables */
        le_size_t er_scale = le_address_get_size( er_enum );

        /* scale base variables */
        le_size_t er_base = LE_ADDRESS_BASE( er_scale );

        /* distance variables */
        le_real_t er_dist = 0.0;

        /* push address variables */
        le_address_t er_push = LE_ADDRESS_C;

        /* parsing scale digits */
        for ( le_size_t er_digit = 0; er_digit < er_base; er_digit ++ ) {

            /* update enumerator size */
            le_address_set_size( er_enum, er_scale + 1 );

            /* assign enumerator digit */
            le_address_set_digit( er_enum, er_scale, er_digit );

            /* check enumeration constraint */
            if ( er_scale > ER_MODEL_ENUM ) {

                /* compute distance */
                er_dist = er_geodesy_distance( er_enum, er_view );

                /* check selection criterion */
                if ( er_dist < er_geodesy_limit( er_view_get_alt( er_view ) ) ) {

                    /* check depth criterion */
                    if ( fabs( er_geodesy_depth( er_dist, er_model->md_sparam, ER_MODEL_DEPTH ) - ( le_real_t ) er_scale ) < 1.0 ) {

                        /* check cells stack */
                        if ( er_model->md_push < er_model->md_size ) {

                            /* set zero cell address */
                            er_cell_set_addr( er_model->md_cell, er_enum );

                            /* perform query for time reduction on zero cell */
                            er_cell_io_query( er_model->md_cell, er_model->md_svip, er_model->md_port );

                            /* retrieve time-reduced address from zero cell */
                            er_push = er_cell_get_addr( er_model->md_cell );

                            /* set address depth */
                            le_address_set_depth( & er_push, ER_MODEL_DEPTH );
                            
                            /* address to push address stack */
                            er_cell_set_push( er_model->md_cell + ( er_model->md_push ++ ), & er_push );

                        }

                    } else {

                        /* check enumeration boundary */
                        if ( ( er_scale + ER_MODEL_DEPTH + 2 ) < er_model->md_sparam ) {

                            /* set zero cell address */
                            er_cell_set_addr( er_model->md_cell, er_enum );

                            /* check parent cell */
                            if ( er_cell_io_query( er_model->md_cell, er_model->md_svip, er_model->md_port ) > 0 ) {

                                /* constrained enumeration */
                                er_model_set_update_cell( er_model, er_enum, er_view ); 

                            }

                        }

                    }

                }

            /* constraintless enumeration */
            } else { er_model_set_update_cell( er_model, er_enum, er_view ); }

        }

    }

    le_void_t er_model_set_update_query( er_model_t * const er_model ) {

        /* parsing variables */
        le_size_t er_parse = 0;
        le_size_t er_inner = 0;

        /* searching variables */
        le_size_t er_found  = 0;

        /* parsing model cells */
        for ( er_parse = 1; er_parse < er_model->md_push; er_parse ++ ) {

            /* check pushed address */
            if ( er_cell_get_push( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* reset cell search */
                er_inner = 1, er_found = er_model->md_size;

                /* searching allocation */
                while ( ( er_inner < er_model->md_size ) && ( er_found == er_model->md_size ) ) {

                    /* compare address and pushed address */
                    if ( er_cell_get_match( er_model->md_cell + er_inner, er_model->md_cell + er_parse ) == _LE_TRUE ) {

                        /* reset pushed address */
                        er_cell_set_pop( er_model->md_cell + er_parse );

                        /* update cell flag */
                        er_cell_set_flag( er_model->md_cell + er_inner, _LE_TRUE );

                        /* update cell flag */
                        er_cell_set_draw( er_model->md_cell + er_inner, _LE_TRUE );

                    /* update cell search */
                    er_found = er_inner; } else { er_inner ++; }

                }

            }

        }

        /* parsing model cells */
        for ( er_parse = 1; er_parse < er_model->md_push; er_parse ++ ) {

            /* check pushed address */
            if ( er_cell_get_push( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* check search results */
                if ( ( er_found = er_model_get_cell( er_model ) ) != er_model->md_size ) {

                    /* swap address and pushed address */
                    er_cell_set_swap( er_model->md_cell + er_found, er_model->md_cell + er_parse );

                    /* update cell array */
                    er_cell_io_query( er_model->md_cell + er_found, er_model->md_svip, er_model->md_port );

                    /* update cell flag */
                    er_cell_set_flag( er_model->md_cell + er_found, _LE_TRUE );

                    /* update cell state */
                    er_cell_set_draw( er_model->md_cell + er_found, _LE_TRUE );

                }

                /* reset pused address */
                er_cell_set_pop( er_model->md_cell + er_parse );

            }

        }

    }

    le_void_t er_model_set_update_terminate( er_model_t * const er_model ) {

        /* parsing model cells */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* check cell flag */
            if ( er_cell_get_flag( er_model->md_cell + er_parse ) == _LE_FALSE ) {

                /* update cell state */
                er_cell_set_draw( er_model->md_cell + er_parse, _LE_FALSE );

            } else {

                /* reset cell state */
                er_cell_set_flag( er_model->md_cell + er_parse, _LE_FALSE );

            }

        }

        /* reset update index */
        er_model->md_push = 1;

    }

/*
    source - model display methods
 */

    le_void_t er_model_display_cell( er_model_t const * const er_model, er_view_t const * const er_view ) {

        /* view position variables */
        le_real_t er_lon = er_view_get_lon( er_view );
        le_real_t er_lat = er_view_get_lat( er_view );

        /* optimisation variables */
        le_real_t er_cosl = cos( - er_lon * ER_COMMON_D2R );
        le_real_t er_sinl = sin( - er_lon * ER_COMMON_D2R );
        le_real_t er_cosa = cos( + er_lat * ER_COMMON_D2R );
        le_real_t er_sina = sin( + er_lat * ER_COMMON_D2R );

        /* edge array variables */
        le_real_t * er_edge = NULL;

        /* motion management - tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGSA );

        /* motion management - azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* display model cells */
        for ( le_size_t er_parse = 1; er_parse < er_model->md_size; er_parse ++ ) {

            /* check cell state */
            if ( er_cell_get_draw( er_model->md_cell + er_parse ) == _LE_TRUE ) {

                /* vertex and color pointer assignation */
                glVertexPointer( 3, ER_MODEL_VERTEX, 0, er_cell_get_pose( er_model->md_cell + er_parse ) );
                glColorPointer ( 3, ER_MODEL_COLORS, 0, er_cell_get_data( er_model->md_cell + er_parse ) );

                /* cell matrix */
                glPushMatrix(); {

                    /* retrieve edge array */
                    er_edge = er_cell_get_edge( er_model->md_cell + er_parse );

                    /* motion management - cell edge translation */
                    glTranslated( er_cosl * er_edge[0] + er_sinl * er_edge[2], er_sina * er_sinl * er_edge[0] + er_cosa * er_edge[1] - er_sina * er_cosl * er_edge[2], er_cosa * er_cosl * er_edge[2] + er_sina * er_edge[1] - er_cosa * er_sinl * er_edge[0] - LE_ADDRESS_WGSA );

                    /* motion management - planimetric rotation */
                    glRotated( + er_lat, 1.0, 0.0, 0.0 );
                    glRotated( - er_lon, 0.0, 1.0, 0.0 );

                    /* display graphical primitives */
                    glDrawArrays( GL_POINTS, 0, er_cell_get_size( er_model->md_cell + er_parse ) / 3 );

                /* cell matrix */
                } glPopMatrix();

            }

        }

    }

    le_void_t er_model_display_earth( er_view_t const * const er_view ) {

        /* motion management - tilt rotation */
        glRotated( - er_view_get_gam( er_view ), 1.0, 0.0, 0.0 );

        /* motion management - altimetric translation */
        glTranslated( 0.0, 0.0, - er_view_get_alt( er_view ) + LE_ADDRESS_WGSA );

        /* motion management - azimuth rotation */
        glRotated( + er_view_get_azm( er_view ), 0.0, 0.0, 1.0 );

        /* earth wireframe - centering */
        glTranslated( 0.0, 0.0, - LE_ADDRESS_WGSA );

        /* motion management - planimetric rotation */
        glRotated( + er_view_get_lat( er_view ), 1.0, 0.0, 0.0 );
        glRotated( - er_view_get_lon( er_view ), 0.0, 1.0, 0.0 );

        /* earth wireframe - orientation */
        glRotated( 90.0, 1.0, 0.0, 0.0 );

        /* create quadric */
        GLUquadricObj * er_earth = gluNewQuadric();

        /* configure quadric */
        gluQuadricDrawStyle( er_earth, GLU_LINE );

        /* earth wireframe - color */
        glColor3f( 0.18, 0.22, 0.28 );

        /* display quadric */
        gluSphere( er_earth, LE_ADDRESS_WGSA, 360, 180 );

        /* delete quadric */
        gluDeleteQuadric( er_earth );

    }

