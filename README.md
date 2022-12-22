# Raytracer

## Example Images

![ex1](/ray_output/shadow_test.png)

![ex2](/ray_output/spot_light_1.png)

![ex2](/ray_output/refract_spot.png)

![ex3](/ray_output/astronaut.png)

![ex4](/ray_output/chess2.png)

## Implementation and Features

### Setup

I used initializer lists and constructors to setup camera data and other necessary data
when starting up the ray tracer.

### Implicit Functions

`raytracer/implicit.h`

I used lambdas and composed solver functions and constraint functions in order to create
a set of functions used for implicit functions. I also did the same thing with normals.
There is a function for each face of each implicit function as well as for its normal.

### Phong Illumination Model

`shader/phong.h`, `shader/phong.cpp`

I created a function named `Phong()` that handled pixel generation for a single pixel.
This function utilized a few helper functions to ensure good coding practice.

### Ray Tracing

`raytracer/raytracer.cpp`

The goal is to shoot a ray from the camera through each point on the view plane
w/ respect to an image coordinate to find where this ray intersects, if any, with
an object on the scene and then light that point on the image based off the the lights
in the scene and that intersected object's properties. I followed this idea
in the `render` function, creating helper functions to handle the shooting of rays,
multithreading, and other extra credit features.

### Bounding Volume Hierarchy

`raytracer/bvh.h`

I used the bottom-up approach to build a bounding volume hierarchy. I used
C++'s inheritance and derived class to get this to work. I had an abstract class,
called `BoundingVolume` that was derived from `BoundingVolumeBox` and `BoundingVolumePrimitive`.
`BoundingVolumeBox` contained two `BoundingVolume`'s, and `BoundingVolumePrimitive`
contained just a primitive, with both having their own bounding box.
More details on the implementation can be found in the `bvh.h` for the thought process
behind constructing it.

With the BVH, speed-ups are noticeable, but **please note that `recursiveSpheres6` and
above do not work on my personal computer as it does not have enough memory.**

### Multithreading

I used `std::threads` to split up the image between a few threads to run each part separately.
More specifically, I split up each image to have a section of rows in the image
because this is more efficient for cache memory.

### Super-sampling

I followed the given process for super sampling by shooting more than one ray per pixel in the
view space coordinates. I shot 9 rays per pixel and took the average. However, this
did not seem to reduce jaggies by that much, if at all?


### Point Lights and Spot lights

`shader/phong.h`, `shader/phong.cpp`

Point lights required the use of attenuation as well as the light having a position.
I utilized the Lab 6 code I wrote for this to find the correct directional vectors for the Phong illumination model.

For spot lights, I utilized the dot product to two vectors to find the angle, which was needed to find the falloff.

### Shadows

`shader/shadow.h`, `shader/shadow.cpp`

When illuminating a scene for a light, shoot a shadow ray to the light. If the ray does not reach the light (the returned t-value is less than that of the light) then we do not count that light.
For directional lights, the direction to the light is the opposite of the light's direction and for the point and spot lights we have the light's position to calculate.

To avoid self-shadowing, start the position a bit off the intersection point in the light's direction.


### Reflection

`raytracer.cpp:reflectionRay()`

Send a reflection ray starting from the intersection point. I make sure that the reflection only happens on reflective objects, and with every reflection, the reflective weight `k_s` is compounded on each successive reflection. When reflecting, I make sure to utilize the previous object's reflective index in calculating the lighting.


### Texture Mapping

`raytracer/implicit.h`, `shader/texture.h`, `shader/texture.cpp`

I load in each image as a vector of float triplets representing the texture map. I do this for each unique filename, and create a map of filepath to `TextureMap` struct that contains a unique texture map. If multiple scene objects have the same texture map, I assign it the `TextureMap` pointer. Each `TextureMap` struct is also dynamically allocated in heap memory so that each object can use it freely.

For UV mapping, I implemented a UV mapping function for each surface type with the implicit functions and integrated this into my `PrimitiveSolver`. This way, each object could use its own generic UV mapping function when the primitive is returned.

When deciding a color, the texture map is consulted if the object has one (and if the setting is on) and blends with the diffuse part of the Phong model.


### Bilinear interpolation for texture mapping

See `texture.cpp` and `texture.h`, I utilized the bilinear interpolation method learned in Lab 7 to interpolate between neighbors for the texture mapping.

### Refraction

`raytracer/raytracer.cpp:refractionRay()`, `raytracer/utils.cpp`

I utilized Snell's law to calculate the refracted ray direction. I cite a vector arithmetic implementation of this formula and tested it against `glm`'s own refract function.

When refracting, I need to reverse the surface normal on the next recursive iteration if I find that the dot product of the refracted direction and the surface normal is positive. This is because refracting requires that the light hits through an object, which means the surface normal is inverted. I also make sure that if this happens, the next index of transmission is 1, for air, as we are leaving the refracted object.
Verify that the refraction results match that of the demo.

### Soft Shadows

I have attempted to implement what I believe is the soft shadow with area lights, but I have not been able to see any results. My implementations are in `shadow.h` and `shadow.cpp` and I have created a way to manually test a light.

# Realtime Renderer

## Implementation and Features

### FBOs

To add FBO's I repurposed the Lab 11 code to make and generate FBOs. This can be found in `renderer/render.cpp`.
I drew to the renderbuffer initializing, and then drew the renderbuffer with any post-processing effects, just like in the lab.
I passed in the necessary uniforms for the filtering effects.

### Camera Movement

For translation effects, I accumulated a given direction vector and applied it to the current camera position vector.
For rotation effects, I used Rodrigues's formula to perform axis angle rotation. I found the theta for this rotation by taking the dragged amount as a proportion of the screen height/width, and then converting to radians.

After any new movement effects were applied, I regenerated the camera matrices.


### Extra filters

I implemented two more per-pixel and kernel-based filters:

For per-pixel:
- Color Swap: change and adjust color channels
- Darken: reduce intensity by a constant scale factor < 1

For kernel-based:
- Gradient based on g(0) of the Kirsch edge detection
- Laplacian

I also integrated by raytracer code so that pressing the button in the top after loading a scene would render it with the raytracer at that camera location.

All extra credit can be toggled in the QWidget.


**Note:** I use a fair bit of dynamic memory allocation. This is to ensure that I don't unnecessarily copy items from place to place and that ensures that my objects do not go out-of-scope.

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

### Adaptive Level of Detail

`renderer/primitive.cpp`

I implemented adaptive level of detail based on distance to the camera. For a given primitive, I used the CTM to find the world point center and took the distance from that to the camera. Then, I applied a scale factor that gives a >1 multiple of the tesselation parameters if it is closer than a given distance threshold, and a multiple <1 if it is farther than the threshold. You can toggle this with the extra credit 1 checkbox. See implementation in the given file for more details.


### Custom OBJ Mesh loader

`trimeshes/obj_mesh.cpp`

I implemented a custom .obj mesh loader. I read the file line by line to parse vertices and normals.
However, it seems like when trying to run the tests for meshes with the buddha and the dragon, my mesh loader seems to be unable to read past a certain line number and throwing a parsing error (even though the lines are fine in the .obj files). I'm wondering if this is another running out of memory issue? These meshes don't show up in my implementation.

I am able to get a rather poor version of the piano scene file and the pdiddy scene file working, granted certain meshes do not show up and the lighting and texture mapping effects are not implemented.
