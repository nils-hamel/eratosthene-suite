/*
 *  liberatosthene - geodetic system
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

    # include "eratosthene-client-render.h"

/*
    source - model function
 */

    void er_model( void ) {

        /* Draw template earth */
        glColor3f( 0.8, 0.8, 0.8 );
        glutWireSphere( _EARTH_RAD, 128, 128 );
        glColor3f( 0.1, 0.3, 0.7 );
        glutSolidSphere( _EARTH_RAD - 1.0, 128, 128 );

    }

