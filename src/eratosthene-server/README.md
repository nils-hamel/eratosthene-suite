## Overview

This tool is the main server instance launcher. Its role is to start an instance of an _eratosthene_ server and service. It then opens a listening socket on which clients can connect to perform queries.

## Usage

In the first place, the storage structure has to be created in order for the service to be linked to it. The storage structure is a simple directory that has to be initialized with a single text file containing two values : the spatial and temporal parameters of the desired _Earth_ representation.

The spatial parameter gives the amount of scales considered in each temporal unit storage. The more scales are used, the more spatial resolution it allows. The following formula gives the size of the spatial equivalence classes for a given scale value _s_ :

    w = ( pi * R ) / ( 2 ^ ( s - 1 ) )

where _R_ gives the _WGS84_ ellipsoid main radius, in meters. The _w_ value gives the size at the equator of the spatial equivalence classes at the considered scale and so, the spatial resolution it allows. For example, considering _32_ as the number of scales allows a spatial resolution of _9 mm_ in the lower scale on the entire earth surface. Of course, considering more spatial scales also increase the amount of required storage space.

The temporal parameter gives to the server instance the size of the temporal units it has to handle. This value is given in _seconds_ and can be any non-zero positive whole number. It gives the size that is considered to segment the time dimension in temporal units in which data are collapsed. It means that any injected data that falls in the same temporal unit are treated as equivalent, from the temporal point of view, by the server.

On a _UNIX-like_ system, you can simply initialize a storage structure for an _eratosthene_ service in the following way :

    mkdir /path/to/server-data

to create the directory and :

    echo "32 86400" > /path/to/server-data/system

to create the server configuration file. This file has to be an _ANSI/ASCII_ raw text file named _system_ and placed in the storage structure root directory.

As the storage structure is set, the instance of an _eratosthene_ service linked to it can be executed using the following command :

    ./eratosthene-server --path /path/to/server-data --port 12345

It creates an instance of an _eratosthene_ service linked to the created storage structure and listening on TCP/IP port number _12345_. At this point, clients can connect to the service to inject or query data.
