# Projects 5 & 6: Lights, Camera & Action!

All project handouts can be found [here](https://cs1230.graphics/projects).


# Action

## Implementation

To add FBO's I repurposed the Lab 11 code to make and generate FBOs. This can be found in



## Extra Credit (CS1234)

I didn't time to do too much extra credit as I am starting the final project.


# Lights, Camera

## Implementation

**Note:** I use a fair bit of dynamic memory allocation in this assignment. This is to ensure that I don't unnecessarily copy items from place to place and that ensures that my objects do not go out-of-scope.

`trimeshes/`
I used an object-oriented approach to my trimeshes. I utilized an abstract class called `Trimesh` for a trimesh primitive and then implemented general functions that were known to be shared through all trimeshes, such as building a tile, a updating parameters, getters for types, etc.

Then, each trimesh could implement their own way of building the trimesh as well as their surface normal.

`camera/`
The camera was copied and repurposed from my raytracer to include support for the projection matrix.

`renderer/primitive.cpp`

Includes functions to create primitive mesh objects, destroy them, and update tesselation parameters. I also optimize for creating new VBOs by assigning previous VBOs to the primitives with the same type and tesselation parameters. This is done using a map using a key of ``"<m_param1>-<m_param2>-<primitive_type>"`` mapped to a VBO ID.


`renderer/render.cpp`

This includes the `gl___` function calls. Includes functions to initialize buffers, draw buffers, destroy buffers, pass in uniform variables.

`realtime.cpp`

Calls the functions written above in the right places.

For example, `paintGL()` creates buffers on the first initialization, and then sends each uniform to the shader, then draws each object. `initializeGL()` creates the shader.
On scene changed, old buffers and meshes are wiped and a new scene is parsed and loaded. On parameter updates, necessary buffers are recreated with new parameters.



## Extra Credit (CS1234)

### Adaptive Level of Detail

`renderer/primitive.cpp`

I implemented adaptive level of detail based on distance to the camera. For a given primitive, I used the CTM to find the world point center and took the distance from that to the camera. Then, I applied a scale factor that gives a >1 multiple of the tesselation parameters if it is closer than a given distance threshold, and a multiple <1 if it is farther than the threshold. You can toggle this with the extra credit 1 checkbox. See implementation in the given file for more details.


### Custom OBJ Mesh loader

`trimeshes/obj_mesh.cpp`

I implemented a custom .obj mesh loader. I read the file line by line to parse vertices and normals.
However, it seems like when trying to run the tests for meshes with the buddha and the dragon, my mesh loader seems to be unable to read past a certain line number and throwing a parsing error (even though the lines are fine in the .obj files). I'm wondering if this is another running out of memory issue? These meshes don't show up in my implementation.

I am able to get a rather poor version of the piano scene file and the pdiddy scene file working, granted certain meshes do not show up and the lighting and texture mapping effects are not implemented.
