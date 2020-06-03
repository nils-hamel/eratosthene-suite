## Overview

This tool allows to perform simple date to time conversion and inversely. It implements a procedure that avoid the consideration of the computer local time parameters. In addition, this tool allows to convert a geographical position in the three dimensional space to an _eratosthene_ index.

## Usage

Starting with the date and time conversion, using the command :

    ./eratosthene-address --date-time --date 2020-06-01+12:12:12

the following result is obtained :

    2020-06-01+12:12:12
    1591013532

giving the _UNIX_ timestamp of the provided date. The date is repeated to confirm the correctness of the entry. Using the invert command :

    ./eratosthene-address --time-date --time 1591013532

should give the identical result :

    2020-06-01+12:12:12
    1591013532

The time value is also repeated for the same reason.

Ending with the spatial to index conversion, the command :

    ./eratosthene-address --space --longitude 6.8 --latitude 45.2 --altitude 370 --scale 32

produces the following output :

    6.80000000000000e+00, 4.52000000000000e+01, 3.70000000000000e+02
    6.79999999701978e+00, 4.51999999210238e+01, 3.69999257766158e+02
    12200100116103015334615075713640, 32

The provided position coordinates are repeated and their corresponding _eratosthene_ index is given. The index is converted back to a spatial position by the tool and displayed as the second position. This allows to check how the number of scale, provided by the _--scale_ parameter, influence the precision of the position. For example, reducing the scale value :

    ./eratosthene-address --space --longitude 6.8 --latitude 45.2 --altitude 370 --scale 16


gives :

    6.80000000000000e+00, 4.52000000000000e+01, 3.70000000000000e+02
    6.79504394531251e+00, 4.51977539062500e+01, 0.00000000000000e+00
    1220010011610301, 16

One can see how the scale value decrease reduces the precision of the position.