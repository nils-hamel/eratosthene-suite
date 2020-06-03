## Overview

This tool implements an example of a graphical client to an _eratosthene_ service that uses dynamic queries to offer the user a way to browse in space and time through a model of the entire _Earth_. It shows that the storage strategy implemented in _eratosthene_ services is sufficiently efficient to create a dynamic representation of the _4D Earth_. As it is a simple example, the features are limited and the interface is kept simple.

## Usage

In order to start the interface, the client has to be started with the information of the remote _eratosthene_ service from with the data are queried, which is an _IP_ address and a service port number :

    ./eratosthene-client --ip 192.168.0.5 --port 12345

If the service is reached, the _4D_ interface shows up and the user can start to navigate through the _4D Earth_ model.

After the connection to an empty _eratosthene_ service, the interface should look as follows :

<br />
<p align="center">
<img src="https://github.com/nils-hamel/eratosthene-suite/blob/master/src/eratosthene-client/doc/interface.jpg?raw=true" width="640">
<br />
<i>Graphical client interface</i>
</p>
<br />

The interface allows navigation through both space and time. A minimal set of information is displayed by the interface. On the first line is the position in time or the convolution between two times when activated by the user. On the second line stands the current time navigator, that can one or two, the query depth value that drive the amount and precision of the displayed model and the type of the query. On the last line are the current boundaries of the time window around the position in time.

Two time navigators are available in order for the user to be able to switch from one to the other to compare two temporal situation of the same location in time. In addition, the _eratosthene_ service allows the clients to ask for logical convolution between the two time.

Two query modes are offered to the user. The first mode, the _NEAR_ mode, ask the server to search only in the most closest position in time where data are found. In this mode, the server does not check if the found data contains actual data for the user position (as a temporal unit most of the time contains data for a limited space range). If the _DEEP_ mode is selected, the server will check if the queried data contains actual data. If not, the sever goes further away from the user time position to find actual data for the position in space. The time window boundaries are there to clamp this search.

Both keyboard and mouse are used to interact with the user. The position of the point of view can be modified by maintaining the left-click while moving the mouse. The altitude of the point of view is modified through the mouse wheel. Note that maintaining the **LEFT-SHIFT** reduces the speed of the motion while the **LEFT-CTRL** increases it. The orientation of the point of view can be modified by maintaining the right-click while moving the mouse.

The query mode can be change through the **D** and **F** keys for the _NEAR_ and _DEEP_ mode, respectively. The density of the model can be modified to increase or decrease the number of elements per cell queried to the remote server. Use the key **PLUS** and **MINUS** of the _numpad_ or the **S** and **A** keys.

The position in time can also be modified. To do so, first adjust the size of the temporal window shown by the interface on the last line. The velocity of the motion in time is set according to this window size. To modify the time area, maintain the **LEFT-CTRL** and **LEFT-ALT** while using the mouse wheel. The position in time can then be updated maintaining the **LEFT-ALT** while using the mouse wheel.

By default, the client starts using only the first time navigator. To switch from the first time to the second, use the key **W**. To return back to the first one, use the **Q** key. As two different temporal point of views are set using the first and second times, one can use the **E**, **R** or **T** keys to ask the server for a convolutional model using the logical OR, AND and XOR convolutions, respectively.

Finally, the keys **F1**, **F2** and **F3** allows to hide the data, the wireframe model of the _Earth_ and the interface information texts, respectively.

## Automatic trajectory

In order to create animation out of the interface, this tool comes with an automatic motion feature. It allows to move automatically and smoothly the position in space and time of the point of view and to save the rendering buffer in a portable network graphic file at regular intervals. In such case, more parameter are required :

    ./eratosthene-client --ip 192.168.0.5 --port 12345 --export /some/export/directory --frame 1800

The exportation directory points to the location where the successive rendering buffers are saved. The amount of frame tells the interface how to scale the motion steps to cover the whole trajectory while exporting the specified amount of frame. Choosing _1800_ as a frame count could for example lead to the creation of an _30_ seconds animation at _60_ frames per second.

As the interface show up, the user is invited to choose a position in space and time as a starting point. To push a position in the trajectory stack, use the **I** key. This process is repeated until the desired trajectory is obtained - at least to point of view have to be pushed. To start the automatic trajectory and exportation, use the **P** key. To reset the stack of pushed point of view, use the **O** key.
