

# **Texture Wrapping**

In OpenGL textures coordinates begin at the bottom left of the texture image (0,0) and end at the top right (1,1). For this reason texture wrapping exists to determine what happens when the coordinates leave the coordinate range.

Types of texture wrapping in OpenGL:

* Repeat - Repeats image forever.
* Mirrored repeat - Repeats a mirrored image forever.
* Clamp to edge - Creates a continuous border from the edge coordinates of an image.
* Clamp to border - Creates a continuous border of any color.

One other thing to mention is that a texture's axis are referred to as s, t and r. This is equivalent to x, y and z.

# **Texture Filtering**

Each texture has a set amount of texture pixels (also known as texels) that are interpolated by OpenGL during the rasterization stage of the graphics pipeline. There are a few options OpenGL allows the programmer to choose from in order to determine the texel colors.



Texture filtering methods:

* Nearest neighbor filtering - Essentially determines the color of a texel based on the center closest to a coordinate.
* Bilinear filtering - Determines the color of texels from interpolated values that gradually change as each texel gets closer to a center texture coordinate.



# **Mipmaps**

OpenGL has to interpolate each texel during the rasterization stage in the graphics pipeline. When rendering a lot of texels from far distances it can cause aliasing and various other visual issues the farther the image is from the viewport. Additionally, it is unnecessary to render texels that are not visible to the user. This is why mipmaps exist. Mipmaps are a collection of images that get smaller each iteration, this is helpful because OpenGL will not have to render more texels than necessary when an image is far from the viewport.



# **Putting The Concepts To Use**

Running my code will render the image below.
<img width="787" height="632" alt="Window run" src="https://github.com/user-attachments/assets/a5c90165-987c-4ff1-9c27-ce838eed20e4" />

As you can see there are two images, in the front there's a troll and in the back is a pile of stones with a gradient color. This is possible by using texture units and setting up two different texture objects. The two texture objects will have their own texture unit index and the sampler2D data type in glsl will point to a specific texture unit depending on the index it's set to. By default it will look at texture unit 0. The sampler2D data types will need to be uniform, which will allow the programmer to modify it anywhere in the code. Finally, in glsl there are two functions used here, texture() and mix(). The texture function takes two parameters, one is the texture and the other is the texture coordinates (which are defined in my vertex\_data array). This just let's glsl send out a texture with given data. The mix() function allows two different textures to be rendered on top of each other and takes three parameters, the first texture, the second texture and the opacity of the textures.



One additional thing about the texture() function, if it were to be multiplied by specified color coordinates they will overlay that texture (Like in the example above).



# **One Thing I Found Really Cool**

This is sort of off topic regarding textures, in my vertex shader I am multiplying the vertex coordinates by two uniform floats offset\_x and offset\_y. In my render loop there are two commented out lines of code.



// main\_shader.set\_uniform("offset\_x", (std::cos(seconds))/2);

// main\_shader.set\_uniform("offset\_y", (std::sin(8 \* seconds))/2);



This is what happens when I uncomment them.

https://github.com/user-attachments/assets/afd585e5-16ad-4f33-bf2c-5f5f14c04539


# **Additional Information**


Key bindings

* esc - closes application.
* 0 - sets rendering to wireframe mode.
* 1 - sets wireframe mode off.



Open-source files and headers used

* stb\_image.h
* Findglfw3.cmake

Dependencies

* glfw3.h
* glfw3native.h
* khrplatform.h
* glad.h
* glad.c
