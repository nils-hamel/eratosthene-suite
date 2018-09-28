/*
 *  eratosthene-suite - client
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

    # include "eratosthene-client-cell.h"

/*
    source - constructor/destructor methods
 */

    er_cell_t er_cell_create( le_void_t ) {

        /* created structure variables */
        er_cell_t er_cell = ER_CELL_C;

        /* create array structure */
        er_cell.ce_data = le_array_create();

        /* return created structure */
        return( er_cell );

    }

    le_void_t er_cell_delete( er_cell_t * const er_cell ) {

        /* deleted structure variables */
        er_cell_t er_delete = ER_CELL_C;

        /* delete array structure */
        le_array_delete( & er_cell->ce_data );

        /* delete structure */
        ( * er_cell ) = er_delete;

    }

/*
    source - accessor methods
 */


    le_byte_t er_cell_get_flag( er_cell_t const * const er_cell, le_byte_t const er_state ) {

        /* return state */
        return( er_cell->ce_flag & er_state );

    }

    le_enum_t er_cell_get_equal( er_cell_t const * const er_cell, er_cell_t const * const er_targ ) {

        /* compare addresses */
        return( le_address_get_equal( & er_cell->ce_addr, & er_targ->ce_addr ) );

    }

    le_enum_t er_cell_get_drop( er_cell_t const * const er_cell, le_address_t const * const er_addr ) {

        /* compare addresses */
        return( le_address_get_equal( & er_cell->ce_addr, er_addr ) );

    }

    le_size_t er_cell_get_record( er_cell_t const * const er_cell ) {

        /* return number of uf3 records */
        return( le_array_get_size( & er_cell->ce_data ) / LE_ARRAY_DATA );

    }

    le_size_t er_cell_get_size( er_cell_t const * const er_cell ) {

        /* return address length */
        return( le_address_get_size( & er_cell->ce_addr ) );

    }

    le_size_t er_cell_get_sync( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset ) {

        /* de-serialise address */
        return( le_address_serial( & er_cell->ce_addr, er_array, er_offset, _LE_SET ) );

    }

    le_void_t er_cell_get_render( er_cell_t * const er_cell, le_real_t const er_lon, le_real_t const er_lat, le_real_t const er_cl, le_real_t const er_sl, le_real_t const er_ca, le_real_t const er_sa ) {

        /* buffer pointer variable */
        le_byte_t * er_base = le_array_get_byte( & er_cell->ce_data );

        /* compute cell translation */
        er_cell->ce_edge[4] = er_cell->ce_edge[0] * er_sl - er_cell->ce_edge[2] * er_cl;
        er_cell->ce_edge[5] = er_cell->ce_edge[1] * er_sa - er_cell->ce_edge[4] * er_ca;
        er_cell->ce_edge[4] = er_cell->ce_edge[1] * er_ca + er_cell->ce_edge[4] * er_sa;
        er_cell->ce_edge[3] = er_cell->ce_edge[0] * er_cl + er_cell->ce_edge[2] * er_sl;

        /* cell translation */
        glTranslated( er_cell->ce_edge[3], er_cell->ce_edge[4], er_cell->ce_edge[5] - LE_ADDRESS_WGS_A );

        /* cell rotation - planimetric rotation */
        glRotated( + er_lat, 1.0, 0.0, 0.0 );
        glRotated( - er_lon, 0.0, 1.0, 0.0 );

        /* cell primitive count */
        if ( er_cell->ce_type[0] ) {

            /* assign vertex array pointer */
            glVertexPointer( 3, GL_DOUBLE, LE_ARRAY_DATA, er_base );

            /* assign color array pointer */
            glColorPointer( 3, GL_UNSIGNED_BYTE, LE_ARRAY_DATA, er_base + LE_ARRAY_DATA_POSE + LE_ARRAY_DATA_TYPE );

            /* display cell primitive */
            glDrawArrays( GL_POINTS, 0, er_cell->ce_type[0] );

        }

        /* cell primitive count */
        if ( er_cell->ce_type[1] | er_cell->ce_type[2] ) {

            /* assign vertex array pointer */
            glVertexPointer( 3, GL_DOUBLE, LE_ARRAY_DATA, er_base + er_cell->ce_type[0] * LE_ARRAY_DATA );

            /* assign color array pointer */
            glColorPointer( 3, GL_UNSIGNED_BYTE, LE_ARRAY_DATA, er_base + LE_ARRAY_DATA_POSE + LE_ARRAY_DATA_TYPE );

            /* cell primitive index pointer */
            er_base = le_array_get_byte( & er_cell->ce_list );

            /* cell primitive count */
            if ( er_cell->ce_type[1] ) {

                /* display cell primitive */
                glDrawElements( GL_LINES, er_cell->ce_type[1], GL_UNSIGNED_INT, er_base );

            }

            /* cell primitive count */
            if ( er_cell->ce_type[2] ) {

                /* assign normal array */
                glNormalPointer( GL_DOUBLE, 0, le_array_get_byte( & er_cell->ce_norm ) );

                /* enable model light */
                glEnable( GL_LIGHTING );

                /* enable normal array */
                glEnableClientState( GL_NORMAL_ARRAY );

                /* display cell primitive */
                glDrawElements( GL_TRIANGLES, er_cell->ce_type[2], GL_UNSIGNED_INT, er_base + er_cell->ce_type[1] * sizeof( GLuint ) );

                /* disable normal array */
                glDisableClientState( GL_NORMAL_ARRAY );

                /* disable model light */
                glDisable( GL_LIGHTING );

            }

        }

    }

/*
    source - mutator methods
 */

    le_void_t er_cell_set_flag( er_cell_t * const er_cell, le_byte_t const er_state ) {

        /* update cell state */
        er_cell->ce_flag |= er_state;

    }

    le_void_t er_cell_set_zero( er_cell_t * const er_cell, le_byte_t const er_state ) {

        /* update cell state */
        er_cell->ce_flag &= ~ er_state;

    }

    le_void_t er_cell_set_push( er_cell_t * const er_cell, le_address_t const * const er_addr ) {

        /* assign address */
        er_cell->ce_addr = ( * er_addr );

    }

    le_size_t er_cell_set_sync( er_cell_t * const er_cell, le_array_t * const er_array, le_size_t const er_offset ) {

        /* serialise address */
        return( le_address_serial( & er_cell->ce_addr, er_array, er_offset, _LE_GET ) );

    }

    le_void_t er_cell_set_array( er_cell_t * const er_cell, le_sock_t const er_socket ) {

        /* read cell socket-array */
        le_array_io_read( & er_cell->ce_data, er_socket );

    }

    le_size_t er_cell_set_data( er_cell_t * const er_cell ) {

        /* pointer variables */
        le_byte_t * er_head = NULL;
        le_byte_t * er_base = NULL;

        /* optimisation variables */
        le_real_t er_optima = 0.0;
        le_real_t er_optimb = 0.0;

        /* size variables */
        le_size_t er_size = 0;

        /* reset primitive count */
        er_cell->ce_type[0] = 0;
        er_cell->ce_type[1] = 0;
        er_cell->ce_type[2] = 0;

        /* retrieve array size */
        if ( ( er_size = le_array_get_size( & er_cell->ce_data ) ) == 0 ) {

            /* return cell size */
            return( er_size );

        }

        /* retrieve array pointer */
        er_head = er_base = le_array_get_byte( & er_cell->ce_data );

        /* cell edge computation */
        er_cell->ce_edge[2] = ( ( le_real_t * ) er_head )[2] + LE_ADDRESS_WGS_A;

        /* cell edge computation */
        er_cell->ce_edge[1] = er_cell->ce_edge[2] * sin( ( ( le_real_t * ) er_head )[1] );
        er_cell->ce_edge[2] = er_cell->ce_edge[2] * cos( ( ( le_real_t * ) er_head )[1] );
        er_cell->ce_edge[0] = er_cell->ce_edge[2] * sin( ( ( le_real_t * ) er_head )[0] );
        er_cell->ce_edge[2] = er_cell->ce_edge[2] * cos( ( ( le_real_t * ) er_head )[0] );

        /* parsing socket array */
        while ( ( er_head - er_base ) < er_size ) {

            /* coordinates conversion */
            ( ( le_real_t * ) er_head )[2] += LE_ADDRESS_WGS_A;

            /* coordinates conversion */
            er_optima = ( ( le_real_t * ) er_head )[0];
            er_optimb = ( ( le_real_t * ) er_head )[1];

            /* coordinates conversion */
            ( ( le_real_t * ) er_head )[1] = ( ( le_real_t * ) er_head )[2] * sin( er_optimb ) - er_cell->ce_edge[1];
            ( ( le_real_t * ) er_head )[2] = ( ( le_real_t * ) er_head )[2] * cos( er_optimb );
            ( ( le_real_t * ) er_head )[0] = ( ( le_real_t * ) er_head )[2] * sin( er_optima ) - er_cell->ce_edge[0];
            ( ( le_real_t * ) er_head )[2] = ( ( le_real_t * ) er_head )[2] * cos( er_optima ) - er_cell->ce_edge[2];

            /* update primitive count */
            er_cell->ce_type[er_head[LE_ARRAY_DATA_POSE] - 1] ++;

            /* update head */
            er_head += LE_ARRAY_DATA;

        }

        /* check multivertex count */
        if ( ( er_cell->ce_type[1] | er_cell->ce_type[2] ) != 0 ) {

            /* multivertex rendering complement */
            er_cell_set_render( er_cell );

        }

        /* return cell size */
        return( er_size );

    }

    le_void_t er_cell_set_render( er_cell_t * const er_cell ) {

        /* count variable */
        le_size_t er_count = er_cell->ce_type[1] + er_cell->ce_type[2];

        /* size variable */
        le_size_t er_size = le_array_get_size( & er_cell->ce_data );

        /* buffer pointer variable */
        le_byte_t * er_dhead = NULL;
        le_byte_t * er_dbase = NULL;

        /* buffer pointer variable */
        le_real_t * er_nbase = NULL;

        /* buffer pointer variable */
        GLuint * er_lbase = NULL;
        GLuint * er_tbase = NULL;

        /* vector variable */
        le_real_t er_vector[6];

        /* offset variable */
        le_size_t er_offset = 0;

        /* modular variable */
        le_size_t er_module = LE_UV3_POINT;

        /* compute buffer pointer */
        er_dhead = ( er_dbase = le_array_get_byte( & er_cell->ce_data ) + er_cell->ce_type[0] * LE_ARRAY_DATA );

        /* update array size */
        le_array_set_size( & er_cell->ce_list, sizeof( GLuint ) * er_count );

        /* compute buffer pointer */
        er_tbase = ( er_lbase = ( GLuint * ) le_array_get_byte( & er_cell->ce_list ) ) + er_cell->ce_type[1];

        /* update array size */
        le_array_set_size( & er_cell->ce_norm, sizeof( le_real_t ) * er_count * 3 );

        /* compute buffer pointer */
        er_nbase = ( le_real_t * ) le_array_get_byte( & er_cell->ce_norm );

        /* parsing data socket-array */
        while ( ( er_offset = ( er_dhead - er_dbase ) ) < er_size ) {

            /* check modular variable */
            if ( ( -- er_module ) == 0 ) {

                /* update modular variable */
                er_module = er_dhead[LE_ARRAY_DATA_POSE];

            }

            /* switch on primitive */
            if ( er_dhead[LE_ARRAY_DATA_POSE] == LE_UV3_LINE ) {

                /* compute render index */
                ( * ( er_lbase ++ ) ) = er_offset / LE_ARRAY_DATA;

            } else {

                /* compute render index */
                ( * ( er_tbase ++ ) ) = er_offset / LE_ARRAY_DATA;

                /* normal computation */
                if ( er_module == 3 ) {

                    /* edge computation */
                    er_vector[0] = ( ( le_real_t * ) er_dhead )[0];
                    er_vector[1] = ( ( le_real_t * ) er_dhead )[1];
                    er_vector[2] = ( ( le_real_t * ) er_dhead )[2];

                } else if ( er_module == 2 ) {

                    /* edge computation */
                    er_vector[0] += ( er_vector[3] = - ( ( le_real_t * ) er_dhead )[0] );
                    er_vector[1] += ( er_vector[4] = - ( ( le_real_t * ) er_dhead )[1] );
                    er_vector[2] += ( er_vector[5] = - ( ( le_real_t * ) er_dhead )[2] );

                } else {

                    /* edge computation */
                    er_vector[3] += ( ( le_real_t * ) er_dhead )[0];
                    er_vector[4] += ( ( le_real_t * ) er_dhead )[1];
                    er_vector[5] += ( ( le_real_t * ) er_dhead )[2];

                    /* compute normal vector */
                    er_nbase[0] = er_vector[2] * er_vector[4] - er_vector[1] * er_vector[5];
                    er_nbase[1] = er_vector[0] * er_vector[5] - er_vector[2] * er_vector[3];
                    er_nbase[2] = er_vector[1] * er_vector[3] - er_vector[0] * er_vector[4];

                    /* compute normal norm */
                    er_vector[0] = 1.0 / sqrtf( er_nbase[0] * er_nbase[0] + er_nbase[1] * er_nbase[1] + er_nbase[2] * er_nbase[2] );

                    /* normal correction */
                    er_nbase[0] *= er_vector[0];
                    er_nbase[1] *= er_vector[0];
                    er_nbase[2] *= er_vector[0];

                    /* normal backward broadcasting */
                    er_nbase[-6] = er_nbase[0];
                    er_nbase[-5] = er_nbase[1];
                    er_nbase[-4] = er_nbase[2];
                    er_nbase[-3] = er_nbase[0];
                    er_nbase[-2] = er_nbase[1];
                    er_nbase[-1] = er_nbase[2];

                }

            }

            /* update pointer */
            er_nbase += 3;

            /* update pointer */
            er_dhead += LE_ARRAY_DATA;

        }

    }

