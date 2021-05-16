# 3D graphics display

This program gives the user control of a 3D tank model.

![Display example](/images/3d_tank.gif)

The program is written in C++ and developed using the OpenGL API (Core Profile). GLFW (for creating an OpenGL context) and GLM (for handling the linear algebra) libraries are used, as well as the GLAD loader (for finding OpenGL functions in memory).

The tank model was created by transforming several copies of a singular cube.

The custom fragment shader implements Phong shading.

A seperate vertex and fragment shader is used to create ray-traced shadows. Some linear algebra is used to calculate where the shadows would intersect the plane defined by a given surface. The shadow is then cropped to fit within that surface's boundaries.

I added an orbiting camera mechanic that is restriced to be only located within the defined walls. Collision detection is implemented between the tank and the walls.
