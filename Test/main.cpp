#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <cmath>

const std::string VERTEX_SHADER_PATH = "glsl\\vertex_shader.txt";
const std::string FRAGMENT_SHADER_PATH = "glsl\\fragment_shader.txt";

void framebuffer_size_callback(GLFWwindow*,int,int);
void get_inputs(GLFWwindow*);

int main()
{
    // GLFW initialization and context set up.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window creation.
    GLFWwindow *window = glfwCreateWindow(800,600, "Current Window", nullptr,nullptr);
    if (window == nullptr)
    {
        std::cout << "Window failed to open\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    // Initializes GLAD.
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "GLAD failed to initialize\n";
        glfwTerminate();
        return -1;
    }

    // Custom shader instance.
    Shader main_shader(VERTEX_SHADER_PATH,FRAGMENT_SHADER_PATH);

    // VERTEX DATA
    float vertex_data[] 
    {
        // Position             //Color             // Texture
        -0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,    0.0f, 1.0f, // Top Left
        0.5f, 0.5f, 0.0f,      0.0f, 1.0f, 0.0f,    1.0f, 1.0, // Top Right
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, // Bot Left
        0.5f,-0.5f, 0.0f,      0.5f, 0.0f, 0.5f,    1.0f, 0.0f  // Bot Right
    };

    // ELEMENT BUFFER OBJECT INDICES
    unsigned int indices[]
    {
        2, 3, 0,
        0, 1, 3
    };
    
    // Vertex buffer object creation, Vertex array object creation.
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    // Buffer object binding
    glBindVertexArray(VAO); // <- Begins storing VBO and EBO data and configurations.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Config buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind VAO
    glBindVertexArray(0); // <- Set state back to 0. Stopping VAO from storing/using anymore data and configurations.


    // Texture set up.
    unsigned int back_texture;
    glGenTextures(1, &back_texture);
    glBindTexture(GL_TEXTURE_2D, back_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    float border_color[] {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    int height, width, nr_channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load("content\\textures\\brrook.png", &width, &height, &nr_channels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "\nCouldn't load data.\n";
    stbi_image_free(data);


    unsigned int front_texture;
    glGenTextures(1, &front_texture);
    glBindTexture(GL_TEXTURE_2D, front_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    data = stbi_load("content\\textures\\troll_1f9cc.png", &width, &height, &nr_channels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "\nCouldn't load data.\n";
    stbi_image_free(data);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,back_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,front_texture);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        get_inputs(window);
        
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        main_shader.use();

        main_shader.set_uniform("tex_back_data",0);
        main_shader.set_uniform("tex_front_data",1);
        float seconds = static_cast<float>(glfwGetTime());

        // main_shader.set_uniform("offset_x", (std::cos(seconds))/2);
        // main_shader.set_uniform("offset_y", (std::sin(8 * seconds))/2);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free buffer objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &back_texture);
    glDeleteTextures(1, &front_texture);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0,0,width,height);
}

void get_inputs(GLFWwindow* window)
{
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}