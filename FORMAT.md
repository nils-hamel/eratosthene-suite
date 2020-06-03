## Specific formats

This documentation gives a presentation of the formats that are used in these sotfware solutions.

## UV3 format

The _uv3 (universal vector + three information bytes)_ format is defined to store large scale point and vector based models in the most simplest expression. It allows to store points, lines and triangles associated with color to each vertex in arbitrary amount. It uses the double precision to store the vertex coordinates in order to deal with any type of model aligned in a geographical frame, including the Cartesian frames associated to Earth.

The format of the _uv3_ file has a very simple structure made of records, each record encoding a vertex of the model stored in it. A vertex is defined as three coordinates for spatial position encoding, a number giving the type of the vertex (point, line or triangle vertex) and three numbers encoding the associated information. The associated information is traditionally the color of the vertex (red, green and blue components).

A structure of an _uv3_ record can then be summarized as follows :

    [x][y][z][type][red][green][blue]

The three spatial coordinates are encoded using the IEEE 754 double precision standard using eight bytes in little endian. The type number is coded using a single byte and as to be :

    1 for a point vertex
    2 for a line vertex
    3 for a triangle

The three color component, in red-green-blue order, are encoding using whole number form _0_ to _255_ each encoded on one byte. The size of a record is then equal to _28_ bytes.

A _uv3_ file is then a collection of records :

    [x][y][z][type][red][green][blue][x][y][z][type][red][green][blue] ...

The amount of record in an _uv3_ file can be retrieved by dividing the size in bytes of the file by _28_.

There is not imposed order for the storage of the primitives, but for poly-vertex primitives (lines and triangles), the vertex of the primitive have to be contiguous in the file :

    [x][y][z][1][red][green][blue]
    [x][y][z][1][red][green][blue]
    [x][y][z][2][red][green][blue]
    [x][y][z][2][red][green][blue]
    [x][y][z][1][red][green][blue]
    [x][y][z][2][red][green][blue]
    [x][y][z][2][red][green][blue]
    [x][y][z][3][red][green][blue]
    [x][y][z][3][red][green][blue]
    [x][y][z][3][red][green][blue]
    [x][y][z][1][red][green][blue]
    ...

The amount of primitive stored in an _uv3_ file is not limited. It follows that any tool using the _uv3_ format has to implement methodologies to deal with potentially very large file without exhausting the available volatile memory.

### Note on UV3 format and geographical coordinates

In case an _uv3_ file is used to store model expressed in geographical coordinates system, the coordinates stored through the records vertex have to follow the strict following order :

    x : dimension associated to longitude direction
    y : dimension associated to latitude direction
    z : dimension associated to height or altitude

In addition, in case of angular coordinates, the _uv3_ impose to store all the angle in radians (decimal degrees are not allowed).

## UF3 Format (Predecessor of UV3)

The _uf3_ format was the initial version of the _uv3_ format. It was also introduce to be able to store very large amount of data expressed in any type of coordinates system. It was nevertheless limited to point-based model storage.

It structure was similar to _UV3_ but without the vertex _type_ information. A record of the an _uf3_ can then be summarized as follows :

    [x][y][z][red][green][blue]

The same encoding standard were used to store these records. As a result, an _uf3_ record contains one byte less that an _uv3_ record, making it _27_ bytes long.

The _uf3_ format is no longer in used and should not be considered for any futur purpose.