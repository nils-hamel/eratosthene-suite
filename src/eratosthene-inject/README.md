## Overview

This tool allows to inject data in an instance of an _eratosthene_ service. It takes a three dimensional model and a time value to inject the data in the pointed service through the network.

## Usage

The three dimensional model to inject in any instance of _eratosthene_ service has to be in _UV3_ format. For a detailed description of the format, see the [format](../../FORMAT.md) page.

The spatial coordinates of the model to inject have to be expressed in the _WGS84_ coordinates frame for longitudes and latitudes. Their values have to be expressed in radian in the [-pi, +pi] range for the longitudes and in radian in the [-pi/2, +pi/2] range for latitudes. The latitude is understood as the geographical latitude, not the geocentric latitude. The altitude has to be expressed in metres and as a height above the _WGS84_ ellipsoid. The form of a vertex has then to follows :

    x : longitude [Radian], y : latitude [Radian], z : height above ellipsoid [Metres]

with the longitude coming first.

Has an _UV3_ file is ready to be injected in a remote _eratosthene_ service, the following command can be used :

    ./eratosthene-inject --ip 192.168.1.5 --port 12345 --uv3 /path/to/model.uv3 --time 1526460005

The provided _IP_ address and service port have to correspond to an active remote _erastosthene_ service reachable by the local machine network.

The time value provided along with the _UV3_ model to inject gives the time associated with the model. It follows that the server, as it receives the model, will consider all the primitives of the model to belong to this position in time and stored them accordingly. The time value is provided through a simple _UNIX_ timestamp expressed outside of any regional parameters.

During data injection, the server temporal unit that receives the incoming data is locked. It means that only a single injection client can be used on the same temporal unit at the same time.