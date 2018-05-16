## Usage and complements

This page gives an overview of the basic usage of the _eratosthene-suite_ softwares. It explains how to instance an _eratosthene_ service and how to feed it with data. It also gives the basic usage of the graphical client that communicates with the service instance.

## Compilation on other plateforms

This first section gives additional information on the compilation of the _eratosthene-suite_ on other plateforms than GNU/Linux. Complements or alternate procedures are welcomed and can be submitted through issues or pull-requests.

### macOS plateforms

The _eratosthene-suite_ can be built through the provided _makefile_ on macOS plateforms. Before compiling the code, make sure the following dependencies are available on the system :

* C compiler (gcc, clang) and make
* OpenGL headers (GL, GLU)
* OpenMP (libomp)
* SDL2

As soon as all dependencies are fulfilled, use the make directive :

    $ make clean-all && make all

in the _eratosthene-suite_ directory.

## Server instance

An _eratosthene_ service instance is obtained using the _eratosthene-server_ software linked to a service number and a storage structure. It can be instanced on any type of computer but computers with enough bandwidth, processing cores and storage space is recommended as _eratosthene_ services are intensive.

In the first place, the storage structure has to be created in order for the service to be linked to it. The storage structure is a simple directory that has to be initialised with a single text file containing two values : the spatial and temporal parameters.

The spatial parameter gives the amount of scales considered in each temporal unit storage. The more scale are used, the more spatial resolution it allows. The following formula gives the size of the spatial equivalence classes for a given scale value _s_ :

    w = ( pi * R ) / ( 2 ^ ( s - 1 ) )

where _R_ gives the WGS84 ellipsoid main radius, in metres. The _w_ value gives the size at the equator of the spatial equivalence classes at the considered scale and so, the spatial resolution it allows. For example, considering _32_ as the number of scales allows a spatial resolution of _0.9 cm_ in the lower scale on the entire earth surface. Of course, considering more spatial scales also increase the amount of storage.

The temporal parameter gives to the server instance the size of the temporal units it has to handle. This value is given in _seconds_ and can be any non-zero positive whole number. It gives the size that is considered to segment the time dimension in temporal units in which data are collapsed. It means that any injected data that falls in the same temporal unit are treated as equivalent by the server.

On an UNIX-like system, you can initialise simply a storage structure for an _eratosthene_ service in the following way :

    $ mkdir /.../server-data

to create the directory and :

    $ echo "32 86400" >> /.../server-data/system

to create the server configuration file. This file has to be an ANSI/ASCII raw text file named _system_ and placed in the storage structure root directory.

As the storage structure is set, the instance of an _eratosthene_ service linked to it can be executed using the following command :

    $ ./eratosthene-server --path /.../server-data --port 12345

It creates an instance of an _eratosthene_ service linked to the created storage structure and listening on TCP/IP port number _12345_.

## Injection client

As an _eratosthene_ service instance is created, it contains no data. The injection client is usually used to inject data in the remote service. In the first place, one has to understand the expected coordinates system and to format used for the data injection.

Point-based data are understood as sets of points with a colour associated to each of them. A point is then described by three spatial coordinates and three colour components. The spatial coordinates are expected to be provided in the WGS84 ellipsoid coordinates frame. The first coordinates has to be the geographic longitude, according to WGS84 ellipsoid, expressed in radian in the [-pi, +pi] range. The second coordinates has to give the geographic latitude (not the geocentric latitude), also according to WGS84 ellipsoid, and expressed in radian in the [-pi/2,+pi/2] range. The last coordinates has to provided the height above the WGS84 ellipsoid, expressed in metre (not the MSL height).

As a point-based model is considered in a given coordinates set, its planimetric coordinates have to be converted in the WGS84 ellipsoidal coordinates while its heights have to be converted in height above WGS84 ellipsoid. Usually, an appropriate geoid model has to be considered to convert MSL heights into WGS84 heights.

The injection client only allows UF3 format for data injection. This format is simply a collection of records stored in a continuous binary sequence. Each records come with six placeholders to carry the three spatial coordinates and the three colour components. The spatial coordinates are store in the record using the IEEE 754 double precision format in its little endian form (x86/amd64). The three colour components are values in the [0,255] range stored using 8-bits long unsigned integers.

    UF3 sequence : [x][y][z][r][g][b][x][y][z][r][g][b] ... [x][y][z][r][g][b]

As the considered model is correctly expressed in the WGS84 coordinates system and stored in a UF3 file, the injection client can be used to send the data in the selected remote _eratosthene_ service :

    $ ./eratosthene-inject --ip 192.168.0.55 --port 12345 --inject --time 1526460005 --uf3 /some/path/data.uf3

The --ip parameter has to give the TCP/IP address of the computer on which the _eratosthene_ service is running while the --port parameter has to provide its service number. The --inject switch indicates the injection client the operation to perform. The --uf3 parameter has to provide the path where the data UF3 file can be found. Finally, the --time parameter has to provide the time associated to the points of the model stored in the provided UF3 file. The time value has to be a UNIX timestamp giving the amount of seconds elapsed since Jan 01 1970 following the UTC frame.

In addition, the injection client also allows to ask the remote _eratosthene_ service to optimise the storage structure of a temporal unit. Optimisation of the storage structure allows to considerably increase the access performances on the temporal unit :

    $ ./eratosthene-inject --ip 192.168.0.55 --port 12345 --inject --time 1526460005

Note that in both injection or optimisation cases, the temporal unit on which the operation is conducted is locked by the service in order to disallow redundant operation to occur. It follows that such unit will remains hidden even in graphical client until the injection or optimisation operation ends.

## Graphical client

As an _eratosthene_ service is available and filled with data, the graphical client can be used to browse the data through an earth scale 4D model. The graphical client is one example of geographic service that can be built using the _eratosthene_ server and its indexation formalism.

To start the graphical client, simply use the following command :

    $ ./eratosthene-client --ip 192.168.0.55 --port 12345

where the --ip parameter gives the TCP/IP address of the computer on which the _eratosthene_ service is available while the --port parameter gives the service number. As the graphical client starts, it should looks like the following :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/doc/image/client-1a.jpg?raw=true" width="640">
<br />
<i>View of the eratosthene graphical client interface</i>
</p>
<br />

Both keyboard and mouse are used to interact with the user. The position of the point of view can be modified by maintaining the left-click while moving the mouse. The altitude of the point of view is modified through the mouse wheel. Note that maintaining the **LEFT-SHIFT** reduces the speed of the motion while the **LEFT-CTRL** increases it. The orientation of the point of view can be modified by maintaining the right-click while moving the mouse.

The interface shows the position in time through the upper timeline. The position in time can also be modified. To do so, first adjust the size of the area shown by the timeline. The velocity of the motion in time is set according to this size. To modify the time area, maintain the **LEFT-CTRL** and **LEFT-ALT** while using the mouse wheel. The position in time can be updated maintaining the **LEFT-ALT** while using the mouse wheel.

The density of the model can be modified to increase or decrease the amount of points per cell queried to the remove server. Use the key **PLUS** and **MINUS** of the _numpad_ or the **S** and **A** keys.

Looking at the interface timeline, one can see that a second time is available. By default, the client starts using only the first time. To switch from the first time to the second, use the key **W**. To return back to the first one, use the **Q** key. As two different temporal point or view are set using the first and second times, one can use the **E**, **R** or **T** keys to ask the server a differential model using the logical OR, AND and XOR, respectively.
