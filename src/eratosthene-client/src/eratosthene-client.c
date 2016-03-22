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

    # include "eratosthene-client.h"

/*
    source - main function
 */

    int main( int argc, char ** argv ) {

        /* Server port variables */
        unsigned int er_port = lc_read_uint( argc, argv, "--port", "-t", _LE_USE_PORT );

        /* Socket handle variables */
        le_sock_t er_client = _LE_SOCK_NULL;

        /* Create client handle */
        if ( ( er_client = le_client_create( ( le_char_t * ) lc_read_string( argc, argv, "--ip", "-i" ), er_port ) ) == _LE_SOCK_NULL ) {

            /* Display message */
            fprintf( stderr, "eratosthene-client : error : unable to connect to server\n" );

        } else {

            /* Initialise GL/GLU/GLUT */
            glutInit( & argc, argv );

            /* Rendering engine */
            er_engine( er_client );

            /* Delete client handle */
            le_client_delete( er_client );

        }        

        /* Return to system */
        return( EXIT_SUCCESS );

    }

