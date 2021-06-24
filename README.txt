
Helicopter "Game"
-----------------

Zachariah Lichtenberg

Final Submission Checklist:

1. Run the program and fly around the WASD keys, Left Click to pan, 
and PgUp/Dn to change altitude. The city is randomly generated and has many interesting
buildings and other helicopters to look at. You can make the maze bigger by adding -size
and a size up to 10.

2. I like the way the objects and sub-objects worked out and allowed me to easily compose
complex random scenes. Changing to C++ helped this process greatly. Making the project a maze
worked very well for the scene aspect of the project as it allowed me to showcase many types
of buildings easily. Also, finding a way to include the Lorenz function should count for 
something right? 

3. I plan to keep working this project after the submission and turn it into a real game.

This program draws a complex OpenGL scene and lets you fly a helicopter model
in it.  

One of the flying copters is being piloted by a math major.

The modeling has been changed to C++ to make it easier to have different types of
objects in the scene.  Many of the objects are just C++ versions of previous 
assignments.

The classes are defined as follows:

Scene.cpp   	A class containing the list of objects to render in the scene.

GenObject.cpp	The base class for rendered objects.  Anything to be displayed
		in the scene is derived from this base class.   Derived
		classes must implement the 'draw' method.

		Objects can have "sub-objects" so that you can make a GenObject
		from one or more other objects.  This is helpful for combining
		shapes together into one object.

CityMaze.cpp	Using some code from "rosettacode.com", this class generates a
		random maze and returns it as a 2-dimensional array.   We can
		build our city from this array and pave the paths.

TextureMap.cpp	Keeps an array of loaded texture files and their "STL Attributes."
		Autodesk inventor puts unique values into an STL file depending
		on the color of the face.  We can use this to specify which texture
		should be applied to which face.

STL_Object.cpp	Reads an STL file to get the triangles.  Only the triangles
		and the attribute bytes are used, the normals are all calculated
		by the main program.  (normal data from the STL is thrown away).

SkyBox.cpp	Implement the sky box, a large cube with a sky texture
		applied.  The skybox is just a cube.
		Skybox bitmaps found at: https://reije081.home.xs4all.nl/skyboxes/


Classes derived from 'GenObject' are:


Helicopter.cpp	     Not currently used
Helipad.cpp	     A blue circle with an "H" in it
Cylinder.cpp	     A textured cylinder
TrapezodBox.cpp      A box with slanted sides, textured.
TexturedBox.cpp	     A flat box with textures mapped 1:1
HeliRotors.cpp	     A spinning rotor for the helicopter.
FlatArea.cpp	     A flat rectangle (ground) with a texture.
OfficeBuilding.cpp   a multi-story office building, textured walls and roof
Courthouse.cpp	     A building with columns and a pitched roof
Mosque.cpp	     A building with minarets and a dome on top
Dome.cpp	     A Generic dome object
CubeTower.cpp        A twisting cube sculpture
OracleCampus.cpp     Three cylindical shaped buildings of different heights,
Mountains.cpp	     Not currently used, creates the mountain range from old assignmen
STL_Object.cpp	     Reads an STL file to create an object. 


The STL files are stored in the 'STLs' directory.
The Texture files are stored in the 'Textures' directory.


Controls
--------

The controls need to be improved but you can navigate the scene with:

    W,S:     	  Forward and backward
    A,F		  Pan left and right
    Arrows:  	  Yaw left and right
    Pgup/PgDn:	  Increase/decrease altitude
    M             Cycle between Top, ISO, and FP view
    Mouse down:	  Pan scene
    Right mouse:  Move light source


What could be done later
-----------------------

Improve the scene.  Possible additions include:

    - Add roadway markings
    - Add shadows

Add better flight controls
    - Momentum
    - Detect crashing into building

Known bugs:
    - The STL objects do not map their textures properly.  This is harder than it seems.

