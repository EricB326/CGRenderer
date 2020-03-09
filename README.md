# OpenGL Rendering Engine
__An OpenGL rendering engine showcasing what I learnt whilst studying graphics programming.__

## About
This project was made during the computer graphics project whilst studing at AIE. The goal of the project was to create an OpenGL rendering engine that could display loaded .obj assets with textuing, materials, and use of lambertian lighting. For the final product, review the _Build_ folder, for the source files, review the _Renderer_ folder.

## Features
* Easily managed shaders, meshes, texture, and material classes for quickly and efficently creating new objects to be rendered.
* Fatal and non-fatal error handling. Allowing the rendering engine to still run even with human error. Displaying error based materials, textures, and/or meshes.

## Libraries
These libraries are either currently implemented, or once were but dropped for the given reason.
* OpenGL 
* GLFW3 - Allows you to create and manage windows.
* Open Asset Importer Lib (assimp) - Previously implemented for importing various 3D model formats. Removed as it was not fully utilized and added an unnecessary overhead for the scope of the project.
* TinyOBJLoader - Currently implemented for importing .obj formatted 3D models.
* stb_image.h - Used to handle loaded texture/map images that go with 3D models.
