/*
 *  eratosthene-suite - storage
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

    # include "eratosthene-storage.h"

/*
    source - i/o function
 */

    le_size_t le_storage_io_server( le_char_t * le_root ) {

        /* string variable */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* stream variable */
        le_file_t le_stream = NULL;

        /* returned value variable */
        le_size_t le_return = _LE_SIZE_NULL;

        /* compose configuration path */
        sprintf( ( char * ) le_path, "%s/system", le_root );

        /* create stream */
        if ( ( le_stream = fopen( ( char * ) le_path, "r" ) ) == NULL ) {

            /* send message */
            return( _LE_SIZE_NULL );

        }

        /* read server configuration */
        if ( fscanf( le_stream, "%" _LE_SIZE_S, & le_return ) != 1 ) {

            /* delete stream */
            fclose( le_stream );

            /* send message */
            return( _LE_SIZE_NULL );

        }

        /* delete stream */
        fclose( le_stream );

        /* return spatial parameter */
        return( le_return );

    }

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* path variable */
        le_char_t * le_path = ( le_char_t * ) lc_read_string( argc, argv, "--path", "-p" );

        /* parameter variable */
        le_size_t le_scfg = le_storage_io_server( le_path );

        /* time variable */
        le_time_t le_time = ( le_time_t ) lc_read_signed( argc, argv, "--time", "-t", _LE_TIME_NULL );

        /* unit structure variable */
        le_unit_t le_unit = LE_UNIT_C;

        /* check spatial parameter */
        if ( le_scfg == _LE_SIZE_NULL ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to access server configuration\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* check unit creation */
        if ( le_get_status( le_unit = le_unit_create( le_path, le_time, LE_UNIT_READ, le_scfg, LE_UNIT_BDEF ) ) != LE_ERROR_SUCCESS ) {

            /* display message */
            fprintf( stderr, "eratosthene-suite : error : unable to access storage unit\n" );

            /* send message */
            return( EXIT_FAILURE );

        }

        /* process storage unit */
        //le_storage( & le_unit, le_path, le_time );
        le_unit_set_optimise( & le_unit, le_path );

        /* delete unit */
        le_unit_delete( & le_unit );

        /* send message */
        return( EXIT_SUCCESS );

    }

