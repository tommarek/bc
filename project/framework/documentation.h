/**
\file
Main page of the doxygen documentation.

\mainpage Framework  overview

\author Tomáš  Marek <xmarek11@stud.fit.vutbr.cz>

\section Introduction Introduction

 This framework was designed for creating 64kB demos or intros as there are only a few simillar frameworks for this purpose.
 It is made to be as easily extendable and understandable as possible.

 This framework consists of two separate directories.
 \li /framework - this part provides all functions and algorithms
 \li /demo - this part on the other hand is only describing the demo using algorithms and functions from framework

\section FWPART Framework 
 Framework direcotry contains source code of whole framework. There are implemented necessary algorithms, which may be used for programming a new demo.
 Each logical part of the framework is separated into one directory inside /framework directory.
 Those directories  are:

 \li /framework/materialmanager
 \li /framework/meshmanager
 \li /framework/musicmanager
 \li /framework/scenemanager
 \li /framework/sequencemanager
 \li /framework/texturemanager
 \li /framework/scripts
 
 \n
 \subsection FMODELS Mesh manager
 \subsubsection FMODFILES Files
 \li /framework/meshmanager/meshmanager.[ch]

 Mesh manager is taking care of all models in the demo.
 It implements functions for creating new models, loading them from generated files and saving them to VBO.
 Mesh manager also contains Catmull-Clark subdivision function, model normal calculation and texture uv coordinates calculation functions.

 Each model is represented as a structure
 \code
 struct S_3DmodelUnpacked
 \endcode


 \subsubsection FMODCORESFILES Corresponding files in Demo
 \li /demo/meshes/models/*.[ch] - contains all models compressed with model compression script
 \li /demo/meshes/mesh.[ch] - all meshes are described here, new operations with models may be added here

 \n
 \subsection FMATERIALS Material manager
 \subsubsection FMATFILES Files
 \li /framework/materialmanager/materialmanager.[ch]

 Material manager implements operations with materials and shaders used in demo.
 It changes active materials and shaders during rendering process.
 Each shader is represented as a structure
 \code
 struct S_shader
 \endcode
 Material is stored in a structure
 \code
 struct S_material
 \endcode
 It contains material information such as shininess, color, etc. and pointer to a target shader.

 \subsubsection FMATCORFILES Corresponding files in Demo
 \li /demo/materials/material.[ch] - contains descriptions of all materials used in the demo
 \li /demo/materials/shaders.[ch] - all shaders are stored here as strings.

 \n
 \subsection FMUSIC Music manager
 \subsubsection FMUSFILES Files
 \li /framework/musicmanager/musicmanager.[ch] - contains main player features and player callback function
 \li /framework/musicmanager/synth.[ch] - audio synthesizer source is located here

 Each song is represented as a structure
 \code
 struct S_song
 \endcode
 This structure contains pointer to patterns
 \code
 struct S_songPattern
 \endcode
 which is in fact array of 16 notes. Those patterns are then used in tracks
 \code
 struct S_songTrack
 \endcode
 Tracks are defying  order of patterns in final song.
 Synthesizer replays stored songs via SDL Audio library.

 \subsubsection FMATCORFILES Corresponding files in Demo
 \li /demo/music/song.[ch] - contains initialisation function for all songs in the demo
 \li /demo/music/songs/*.[ch] - all songs are stored here
 \li /demo/music/instruments/*.[ch] - all instruments  are stored here


 \n
 \subsection FSCENE Scene manager
 \subsubsection FSCENEFILES Files
 \li /framework/scenemanager/bezier.[ch] - basic bézier curves operations are defined here
 \li /framework/scenemanager/scenemanager.[ch] -  contains operations with scene tree, rendering nodes, etc.
 \li /framework/scenemanager/sceneoperations.[ch] - contains specific operation with scene nodes such as camera operation, transformations, etc.

 Scene is built as a tree. Every node of that tree is an object  (mesh, camera, root, ...).
 Scene manager defines all operations with the scene. It can create, delete,  rotate, scale or transform any node in the scene.
 It also contains various node-specific functions such as camera lookAt, set perspective, etc.

 Each node of scene is represented as a structure
 \code
 struct S_sceneNode
 \endcode
 
 There has to be stored one root node only to render the scene but it is alwas a good idea to save other nodes too due to rotations and other operations that might be applied to them.

 \subsubsection FSCENECORFILES Corresponding files in Demo
 \li /demo/scene/scene.[ch] - contains scenes and their initialisation fuctions

 \n
 \subsection FSEQAUENCE Sequence manager
 \subsubsection FSEQFILES Files
 \li /framework/sequencemanager/sequencemanager.[ch] - contains all operations for sequence
 \li /framework/sequencemanager/timer.[ch] - takes care of timing operations in the demo

 Sequence manager defines all time depending operations in demo.  Every possible event has a callback function, which is called when event should be executed.
 Events are descrbed as a structure
 \code
 struct S_sceneNode
 \endcode
 which contains callback function of an event, start and stop time of an event and 6 optional parameters.
 These events are stored in an array. Sequence manager reads this array and executes all events according to time.

 \subsubsection FSEQCORFILES Corresponding files in Demo
 \li /demo/sequence/sequence.[ch] - this file contains exact description of sequence in the demo.  It also contains curves, angles and noises used in the events. May be also extended by new event handling functions.

 \n
 \subsection FTEXTURE Texture manager
 \subsubsection FSEQFILES Files
 \li /framework/texturemanager/texturemanager.[ch] - contains all operations with textures and noises

 Texture manager is  taking care of texture creation. Those textures are later used by material manager.
 Textures may be generated as a two dimensional array of pixels(RGBA) in
 \code
 struct S_texture
 \endcode
 Those textures (layers) may be also merged together using various operations implemented in the texture manager.
 Texture manager can generate Noise textures, sine textures etc.

 \subsubsection FTEXCORFILES Corresponding files in Demo
 \li /demo/textures/texture.[ch] - these files contain all textures generated for this demo

 \n
 \subsection FSCRIPTS Scripts
 \subsubsection FSCRIPTFILES Script
 \li /framework/scripts/raw2c.py - converts model in *.raw format to *.[ch] files.
 \li /framework/scripts/instrumentsFFT.py - converts *.wav file to *.[ch] files.

 Scripts are written in Python language.  Both of them generates C source files which are usable with this framework.
 Makefile takes care of copying them to the right location.

 \n
 \subsection FOTHER Other
 \subsubsection FOTHERFILES Files
 \li /framework/draw.[ch] - rendering functions and OpenGL initialisation
 \li /framework/extensions.[ch] - initialisation of OpenGL extensions
 \li /framework/util.[ch] - utilities used the in demo, missing functions due to -no-stdlib make flag, math macros and other mathematical functions are located here
 \li /framework/window.[ch] - takes care of opening new window
 \li /framework/main.c] - main file 
 
 All these files are used by all managers and they take care of running the whole demo.

\n\n
For a better understanding have a look  at the  example  demo implemented in this framework,  it is comprehensively commented.
 Various algorithms and problems are documented in the technical report.
*/

