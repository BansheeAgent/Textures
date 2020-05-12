////////////////////////////////////////////////////////////////////////////////
/*
 * Textures are used by artist and programmers to add more detail to objects.
 * A texture is a 2D image used to add detail to an object; think of a
 * texture as a piece of paper with a nice brick image on it neatly folded
 * over your 3D house so it looks like your house has a stone exterior.
 *
 * Because we can insert a lot of detail in a single image, we can give the
 * illusion the object is extremely detailed without having to specify extra
 * vertices.
 *
 * Also next to images, textures can also be used to store a large collection
 * of arbitrary data to send to the shaders.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * In order to map a texture to the triangle we need to tell each vertex of
 * the triangle which part of the texture it corresponds to. Each vertex
 * should thus have a texture coordinate associated with them that specifies
 * what part of the texture image to sample from. Fragment interpolation then
 * does the rest for the other fragments.
 *
 * Texture coordinates range from 0 to 1 in the x and y axis. Retrieving the
 * texture color using texture coordinates is called sampling. Texture
 * coordinates start at (0, 0)  for the lower left corner of a texture image
 * to (1, 1) for the upper right corner of a texture image. The following
 * image shows how we map texture coordinates to the triangle.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * We specify 3 texture coordinate points for the triangle. We want the
 * bottom-left side of the triangle to correspond with the bottom-left side
 * of the texture so we use the (0, 0) texture coordinate for the triangle's
 * bottom-left vertex. The same applies to the bottom-right side with a (1,0)
 * texture coordinate. The top of the triangle should correspond with the
 * top-center of the texture image so we take (0.5, 1.0) as its texture
 * coordinate. We only have to pass 3 texture coordinates to the vertex
 * shader, which then passes those to the fragment shader that neatly
 * interpolates all the texture coordinates for each fragment.
 *
 * The resulting texture coordinates would then look like this:

 *  float texCoords[] = {
 *    0.0f, 0.0f,  // lower-left corner
 *    1.0f, 0.0f,  // lower-right corner
 *    0.5f, 1.0f   // top-center corner
 *  };
 *
 *  Texture sampling has a loose interpretation and can be done in many
 *  different ways. It is thus our job to tell opengl how it should sample
 *  its textures.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Texture Wrapping
 *  Texture coordinates usually range from (0,0) to (1,1) but what happens if
 *  we specify coordinates outside this range? The default behavior of OpenGL
 *  is to repeat the texture images(we basically ignore the integer part of
 *  the floating point texture coordinate), but there are more options OpenGL
 *  offers:
 *
 *    + GL_REPEAT: the default behavior for textures. Repeats the texture image.
 *    + GL_MIRRORED_REPEAT: Same as GL_REPEAT but morrors the image with each
 *        repeat.
 *    + GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. The result
 *        is that higher coordinates become clamped to the edge , resulting in a
 *        stretched edge pattern.
 *    + GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a
 *        user-specified border color.
 *
 *  Each of the aforementioned options can be set per coordinate axis(s, t
 *  (and r if you're using 3d textures) equivalent to x, y, z) with the
 *  glTexParameter* function:
 *
 *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
 *  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
 *
 *      + The first argument specifies the texture target; we're working with
 *      2D textures so the texture target is GL_TEXTURE_2D. the second
 *      argument requires us to tell what o0pption we want to set and for
 *      which texture axis; we want to configure it for both the S and T axis
 *      . The last argument requires us to pass in the texture wrapping mode
 *        we'd like and in this case OpenGL will set its texture wrapping
 *        option on the currently active texture with GL_MIRRORED_REPEAT.
 *
 *      Note: if we choose the GL_CLAMP_TO_BORDER option we should also
 *      specify a border color. This is done using the fv equivalent of the
 *      glTexParameter function with GL_TEXTURE_BORDER_COLOR as its option
 *      where we pass in a float array of the border's color value.
 *
 *      float borderColor[] = {1,0f, 1.0f, 0.0f, 1.0f};
 *      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Texture Filtering
 *  Texture coordinates do not depend on resolution but can be any floating
 *  point value, thus OpenGL has to figure out which texture pixel( also
 *  known as a texel) to map the texture coordinate to. This becomees
 *  especially important if you have a very large object and a low resolution
 *  texture. You probably guessed by now that OpenGL has options for this
 *  texture filtering as well. Thgere are several options available, we'll
 *  discuss  the most important options: GL_NEAREST and GL_LINEAR.
 *
 *    + GL_NEAREST (also known as nearest neighbor or point filtering) is the
 *      default texture filtering method of OpenGL when set to GL_NEAREST,
 *      OpenGL selects the texel that center is closest to the texture
 *      coordinate. Below you can see 4 pixels where the cross represents the
 *      exact texture coordinate. The upper-left texel has its center closest
 *      to the texture coordinate and is therefore chosen as the sampled color:
 *
 *    + GL_LINEAR (also known as bilinear filtering) takes an interpolated
 *    value from the texture coordinate's neighboring texels, approximating a
 *    color between the texels. The smaller the distance from the texture
 *    coordinate to a texel's center.  the more that texel's color
 *    contributes to sampled color. Below we can see that a mixed color of
 *    the neighboring pixels is returned:
 *
 *    the result of these are with GL_NEAREST resulting in blocked patterns
 *    where we can clearly see the pixels that form the texture while
 *    GL_LINEAR produces a smoother pattern where the individual pixels are
 *    less visible. GL_LINEAR produces a more realistic output, but some
 *    developers prefer a more 8-bit look and as a result pick the GL_NEAREST
 *    option.
 *
 *    Texture filtering can be set for magnifying and minifying operations
 *    (when scaling up or downwards) so you could for example use nearest
 *    neighbor filtering when textures are scaled downwards and linear
 *    filtering for upscaled textures. We  thus have to specify the filtering
 *    method for both options via glTexParameter*. The code should look
 *    similar to setting the wrapping method:
 *
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Mipmaps
 *  Imagine we had a large room with thousands of objects, each with an
 *  attached texture. There will be objects far away that have the same high
 *  resolution texture attached as the objects close to the viewer. Since the
 *  objects are far away and probably only produce a few fragments, OpenGL
 *  has difficulties retrieving the right color value for its fragment form
 *  the high resolution texture, since it has to pick a texture color for a
 *  fragment that spans a large part of the texture. This will produce
 *  visible artifacts on small objects, not to mention the waste of memory
 *  bandwidth using high resolution textures on small objects.
 *
 *  To solve this issue OpenGL uses a concept called mipmaps that is
 *  basically a collection of texture images where each subsequent texture is
 *  twice as small compared to the previous one. The idea behind mipmaps
 *  should be easy to understand: after a certain distance threshold from the
 *  viewer, OpenGL will use a different mipmap texture that best suits the
 *  distance to the object. Because the object is far away, the smaller
 *  resolution will not be noticeable to the user. OpenGL is then able to
 *  sample the correct texels, and there's less cache memory involved when
 *  smapling that part of the mipmaps
 *
 *  Createing a collection of mipmapped textures for each texture image is
 *  cumbersome to do manually, but lubkily OpenGL is able to do all the work
 *  for us with a single call to glGenerateMipmaps after we've created a
 *  texture.
 *
 *  When switching between mipmaps levels during rendering OpenGL might show
 *  some artifacts like sharp edges visible between the two mipmaps layers.
 *  Just like normal texture filtering, it is also possible to filter between
 *  mipmap levels using nearest and linear filtering for switching between
 *  mipmap levels. To specify the filtering method between mipmap levels we
 *  can replace the original filtering methods with one of the following four
 *  options:
 *
 *    + GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the
 *    pixel size and uses nearest neighbor interpolation for texture sampling.
 *
 *    +GL_LINEAR_MIPMAP_NEAREAST: takes the nearest mipmap level and smples
 *    that level using linear interpolation.
 *
 *    +GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two
 *    mipmaps that most closely match the size of a pixel and samples the
 *    interpolated level via nearest neighbor interpolation.
 *
 *    +GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest
 *    mipmaps and samples the interpolated level via linear interpolation.
 *
 *  Just like texture filtering we can set the filtering method to one of the
 *  4 aforementioned methods using glTexParameteri:
 *
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 *
 *  A common mistake is to set one of the mipmap filtering options as the
 *  magnification filter. This doesn't have any effect since mipmaps are
 *  primarily used for when textures get downscaled: texture magnification
 *  doesn't use mipmaps and giving it a mipmap filtering option will generate
 *  an OpenGL GL_INVALID_ENUM error code.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Loading and Creating Textures
 *  The first thing we need to do to actually use textures is to load them
 *  into our application. Texture images can be stored in dozens of file
 *  formats, each with their own structure and ordering of data, so how so we
 *  get those images in our application? One solution would be to choose a
 *  file format we'd like ot use, say .PNG and write our own image loader to
 *  convert the image format into a large array of bytes. While it's not very
 *  hard to write your own image loader it's  still cumbersome and what if
 *  you want to support more file formats? You'd then have to write an image
 *  loader for each format you want to support.
 *
 *  Another solution, and probably a good one, is to use an image-loading
 *  library that supports several popular formats and does all the hard work
 *  for us. A library like stb_image.h
 *
 *  stb_image.h is a very popular single header image loading library by Sean
 *  Barrett that is able to load most popular file formats and is easy to
 *  integrate in your project(s). stb_image.h can be downloaded from
 *  https://github.com/nothings/stb/blob/master/stb_image.h
 *    Simply download the single header file, add it to your project as
 *    stb_image.h, and create an additional C++ file with the following code:
 *
 *  #define STB_IMAGE_IMPLEMNENTATION
 *  #include "stb_image.h"
 *
 *  By defininf STB_IMAGE_IMPLEMENTATION the preprocessor modifies the header
 *  file such that it only contains the relevant definition source code,
 *  effectively turning the header file into a .cpp file, and that's about it
 *  . Now simply include stb_image.h somewhere in your program and compile.
 *
 *  For the following texture sections we're going to use an image of a
 *  wooden container.
 *  https://learnopengl.com/img/textures/container.jpg
 *  To load an image using stb_image.h we use its stbi_load function:
 *
 *  int width, height, nrChannels;
 *  unsigned char *data = stbi_load("container.jph", &width, &height, &nrChannels, 0);
 *
 *  The function first takes as input the location of an image file. It then
 *  expects you to give three ints as its second, third and fourth argument
 *  that stb_image.h will fill with the resulting image's width, height and
 *  number of color channels. We need the image's width and height for
 *  generating textures later on.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Generating a texture
 *  Like any of the previous objects in OpenGL, textures are referenced with
 *  an ID; let's create one:
 *    unsigned int texture;
 *    glGenTextures(1, &texture);
 *
 *  the glGenTextures function first takes as input how many textures we want
 *  to generate and stores them in a unsigned int array given as its second
 *  argument (in our case just a single unsigned int). Just like other
 *  objects we need to bind it so any subsequent texture commands will
 *  configure the currently bound texture:
 *
 *    glBindTexture(GL_TEXTURE_2D, texture);
 *
 *  Now that the texture is bound, we can start generating a texture using
 *  the previously loaded image data. Textures are generated with glTexImage2D:
 *
 *    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
 *    glGenerateMipmap(GL_TEXTURE_2D);
 *
 *  This is a large function with quite a few parameters so we'll walk
 *  through them step-by-step:
 *
 *    + The first argument specifies the texture target; setting this to
 *    GL_TEXTURE_2D means this operation will generate a texture on the
 *    currently bound texture object at the same target(so any textures bound
 *    to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).
 *    + The second argument specifies the mipmap level for which we want to
 *    create a texture for if you want to set each mipmap level manually, but
 *    we'll leave it at the base level which is 0.
 *    + The third argument tells OpenGL in what kind of format we want to
 *    store the texture. Our image has only RGB values so we'll store the
 *    texture with RGB values as well.
 *    + The fourth and fith argument sets the width and height of the
 *    resulting texture. We stored those earlier when loading the image se
 *    we'll use the corresponding variables.
 *    + The sixth argument should always be 0 (some legacy stuff)
 *    + The 7th and 8th argument specify the format and datatype of the
 *    source image. We loaded the image with RGB values and stored them as
 *    chars (bytes) se we'll pass in the corresponding values.
 *    + The last argument is the actual image data.
 *
 *  Once glTExImage2D is called, the currently bound texture object now has
 *  the texture image attached to it. However, currently it only has the
 *  base-level of the texture image loaded and if we want to use mipmaps we
 *  have to specify all the different images manually (by continually
 *  incrementing the second argument) or , we could call glGenerateMipmap
 *  after generating the texture. This will automatically generate all the
 *  required mipmaps for the currently bound texture.
 *
 *  After we're done generating the texture and its corresponding mipmaps, it
 *  is good practice to free the image memory:
 *
 *     stbi_image_free(data);
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Summery of generating a texture
 *
 *  The whole process of generating a texture thus looks something like this:
 *
 *    unsigned int texture;
 *    glGenTextures(1, &texture);
 *    glBindTexture(GL_TEXTURE_2D, texture);
 *    //  set the texture wrapping/filtering options (on the currently bound
 *    //  texture object)
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 *    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 *    //load and generate the texture
 *    int width, height, nrChannels;
 *    unsigned char* data = stbi_load("container.jpg", &widht, &height,
 *    &nrChannels, 0);
 *    if(data)
 *    {
 *      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
 *      glGenerateMipmap(GL_TEXTURE_2D);
 *    }
 *    else
 *    {
 *      std::cout << "Failed to load texture" << std::endl;
 *    }
 *    stbi_image_free(data);
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*
 * Applying textures
 *  For the upcoming sections we will use the rectangle shape drawn with
 *  glDrawElements from the final part of the Hello Triangle Project. We need
 *  to inform OpenGL how to sample the texture so we'll have to update the
 *  vertex data with the texture coordinates;
 */
////////////////////////////////////////////////////////////////////////////////
#if 0
float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * Since we've added an extra vertex attribute we again have to notify OpenGL
 * of the new vertex format:
 *
 * glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
 * glEnableVertexAttribArray(2);
 *
 * Note that we have to adjust the stride parameter of the previous two
 * vertex attributes to 8 * sizeof(float) as well.
 */
////////////////////////////////////////////////////////////////////////////////
#if 0
  glVertexAttibPointer(2, 2, GLfLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * Next we need to alter the vertex shader to accept the texture coordinates
 * as a vertex attribute and then forward the coordinates to the fragment
 * shader:
 */
////////////////////////////////////////////////////////////////////////////////
#if 0
  const char* vertexShaderCode = (
            "#version 330 core\n"
 "layout (location = 0) in vec3 aPos;\n"
 "layout (location = 1) in vec3 aColor;\n"
 "layout (location = 2) in vec2 aTexCoord;\n"
 "\n"
 "out vec3 ourColor;\n"
 "out vec2 TexCoord;\n"
 "\n"
 "void main()\n"
 "{\n"
 "    gl_Position = vec4(aPos, 1.0);\n"
 "    ourColor = aColor;\n"
 "    TexCoord = aTexCoord;\n"
 "}"
          );
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * The fragment shader should then accept the TextCoord output as an input
 * variable.
 *
 * The fragment shader should also have access to the texture object, but how
 * do we pass the texture object to the fragment shader? GLSL has built in
 * data-type for texture objects called a sampler that takes as a postfix the
 * texture type we want e.g. sampler1D, sampler3D or in our case sampler2D.
 * We can then add a texture to the fragment shader by simply declaring a
 * uniform sampler2D that we later assign our texture to
 *
 * To sample the color of a texture we use GLSL's built-in texture function
 * that takes as its first argument a texture sampler and as its second
 * argument the corresponding texture coordinates. The texture function then
 * samples the corresponding color value using the texture parameters we set
 * earlier. The output of this fragment shader is then the (filtered) color
 * of the texture at the (interpolated) texture coordinate.
 */
////////////////////////////////////////////////////////////////////////////////

#if 0
const char* fragmentShaderCode = (
          "#version 330 core\n"
 "out vec4 FragColor;\n"
 "  \n"
 "in vec3 ourColor;\n"
 "in vec2 TexCoord;\n"
 "\n"
 "uniform sampler2D ourTexture;\n"
 "\n"
 "void main()\n"
 "{\n"
 "    FragColor = texture(ourTexture, TexCoord);\n"
 "}"
        );
#endif

////////////////////////////////////////////////////////////////////////////////
/*
 * All that's left to do now is to bind the texture before calling
 * glDrawElements and it will then automatically assign the texture to the
 * fragment shader's sampler:
 *
 */
////////////////////////////////////////////////////////////////////////////////

#if 0
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("4.1.texturevs.txt", "4.1.texturefs.txt");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
          // positions          // colors           // texture coords
          0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
          0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
         -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
         -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
          0, 1, 3, // first triangle
          1, 2, 3  // second triangle
  };
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);


  // load and create a texture
  // -------------------------
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);


  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // render container
    ourShader.use();
    int uniformTime = glGetUniformLocation(ourShader.getProgram(), "time");
    glUniform1f(uniformTime, glfwGetTime());


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}