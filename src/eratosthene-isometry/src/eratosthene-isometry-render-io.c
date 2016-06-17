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

    # include "eratosthene-isometry-render-io.h"

/*
    source - portable network graphics i/o methods
 */

    le_enum_t er_png_write( le_char_t const * const er_path, le_size_t const er_width, le_size_t const er_height, le_byte_t * const er_bytes ) {

        /* Stream variables */
        FILE * er_stream = NULL;

        /* Portable network graphics variables */
        png_structp er_pngp = NULL;
        png_infop   er_info = NULL;

        /* Buffer variables */
        le_byte_t * er_rbuf = NULL;

        /* Create buffer */
        if ( ( er_rbuf = malloc( er_width * 4 * sizeof( png_byte ) ) ) == NULL ) {

            /* Send message */
            return( _LE_FALSE );

        }

        /* Create output stream */
        if ( ( er_stream = fopen( ( char * ) er_path, "wb" ) ) == NULL ) {

            /* Delete buffer */
            free( er_rbuf );

            /* Send message */
            return( _LE_FALSE );

        }

        /* Create portable network graphics structure */
        if ( ( er_pngp = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL ) ) == NULL ) {

            /* Delete output stream */
            fclose( er_stream );

            /* Delete buffer */
            free( er_rbuf );

            /* Send message */
            return( _LE_FALSE );

        }

        /* Create portable network graphics info */
        if ( ( er_info = png_create_info_struct( er_pngp ) ) == NULL ) {

            /* Delete portable network graphics structure */
            png_destroy_write_struct( & er_pngp, ( png_infopp ) NULL );

            /* Delete output stream */
            fclose( er_stream );

            /* Delete buffer */
            free( er_rbuf );

            /* Send message */
            return( _LE_FALSE );

        }

        /* Setup exception management */
        if ( setjmp( png_jmpbuf( er_pngp ) ) ) {

            /* Create portable network graphics info */
            png_free_data( er_pngp, er_info, PNG_FREE_ALL, -1 );

            /* Delete portable network graphics structure */
            png_destroy_write_struct( & er_pngp, ( png_infopp ) NULL );

            /* Delete output stream */
            fclose( er_stream );

            /* Delete buffer */
            free( er_rbuf );

            /* Send message */
            return( _LE_FALSE );

        }

        /* Bind output stream */
        png_init_io( er_pngp, er_stream );

        /* Setup portable network format */
        png_set_IHDR( er_pngp, er_info, er_width, er_height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        /* Write portable network graphics header */
        png_write_info( er_pngp, er_info );

        /* Export portable network graphics rows */
        for ( int er_parse = 0; er_parse < er_height; er_parse ++ ) png_write_row( er_pngp, er_bytes + ( er_height - er_parse - 1 ) * ( er_width * 4 ) );

        /* Write portable network graphics footer */
        png_write_end( er_pngp, NULL );

        /* Create portable network graphics info */
        png_free_data( er_pngp, er_info, PNG_FREE_ALL, -1 );

        /* Delete portable network graphics structure */
        png_destroy_write_struct( & er_pngp, ( png_infopp ) NULL );

        /* Delete output stream */
        fclose( er_stream );

        /* Delete buffer */
        free( er_rbuf );

        /* Send message */
        return( _LE_TRUE );

    }

