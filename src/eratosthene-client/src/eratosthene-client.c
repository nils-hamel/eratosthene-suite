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

    # include "eratosthene-client.h"

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* Server address variables */
        le_char_t * le_addr = ( le_char_t * ) lc_read_string( argc, argv, "--ip"  , "-i" );

        /* Server port variables */
        le_size_t le_port = ( le_size_t   ) lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* Initialise GL/GLU/GLUT */
        glutInit( & argc, argv );

        /* Create rendering engine */
        if ( er_engine_create( le_addr, le_port ) == _LE_FALSE ) {

            /* Display message */
            fprintf( stderr, "eratosthene-suite : error : unable to create model\n" );

        } else {

            /* Engine loops */
            er_engine_loops();

            /* Delete rendering engine */
            er_engine_delete();

        }

        /* Return to system */
        return( EXIT_SUCCESS );

    }

