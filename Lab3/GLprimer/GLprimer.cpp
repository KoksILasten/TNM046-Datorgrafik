/*
 * A C++ framework for OpenGL programming in TNM046 for MT1
 *
 * This is a small, limited framework, designed to be easy to use for students in an introductory
 * computer graphics course in the first year of a M Sc curriculum. It uses custom code for some
 * things that are better solved by external libraries like GLM, but the emphasis is on simplicity
 * andreadability, not generality.
 *
 * For the window management, GLFW 3.x is used for convenience.
 * The framework should work in Windows, MacOS X and Linux.
 * GLEW is used for handling platform specific OpenGL extensions.
 * This code depends only on GLFW, GLEW, and OpenGL libraries.
 * OpenGL 3.3 or higher is required.
 *
 * Authors: Stefan Gustavson (stegu@itn.liu.se) 2013-2015
 *          Martin Falk (martin.falk@liu.se) 2021
 *
 * This code is in the public domain.
 */
#if defined(WIN32) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

// File and console I/O for logging and error reporting
#include <iostream>
// Math header for trigonometric functions
#include <cmath>

// glew provides easy access to advanced OpenGL functions and extensions
#include <GL/glew.h>

// GLFW 3.x, to handle the OpenGL window
#include <GLFW/glfw3.h>

#include "Utilities.hpp"

#include <vector>

#include "Shader.hpp"

#include <array>

#include "TriangleSoup.hpp"

GLuint createVertexBuffer(int location, int dimensions, const std::vector<float>& vertices) {
GLuint bufferID;
    // Generate buffer, activate it and copy the data
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),
                   GL_STATIC_DRAW);
// Tell OpenGL how the data is stored in our buffer
// Attribute location (must match layout(location=#) statement in shader)
// Number of dimensions (3 -> vec3 in the shader, 2-> vec2 in the shader),
// type GL_FLOAT, not normalized, stride 0, start at element 0
glVertexAttribPointer(location, dimensions, GL_FLOAT, GL_FALSE, 0, nullptr);
// Enable the attribute in the currently bound VAO
glEnableVertexAttribArray(location);
return bufferID;

}
GLuint createIndexBuffer(const std::vector<unsigned int>& indices) {
    GLuint bufferID;
    // Generate buffer, activate it and copy the data
    glGenBuffers(1, &bufferID);
    // Activate (bind) the index buffer and copy data to it.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    // Present our vertex indices to OpenGL
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),indices.data(), GL_STATIC_DRAW);
    return bufferID;
    
}

// Multiply 4x4 matrices m1 and m2 and return the result
std::array<float, 16> mat4mult(const std::array<float, 16>& m1, const std::array<float, 16>& m2) {
    std::array<float, 16> result;

    result[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
    result[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
    result[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
    result[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

    result[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
    result[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
    result[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
    result[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

    result[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
    result[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
    result[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
    result[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

    result[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
    result[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
    result[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    result[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

    return result;
}

void mat4print(const std::array<float, 16>& m) {
     printf("Matrix:\n");
     printf("%6.2f %6.2f %6.2f %6.2f\n", m[0], m[4], m[8], m[12]);
     printf("%6.2f %6.2f %6.2f %6.2f\n", m[1], m[5], m[9], m[13]);
     printf("%6.2f %6.2f %6.2f %6.2f\n", m[2], m[6], m[10], m[14]);
     printf("%6.2f %6.2f %6.2f %6.2f\n", m[3], m[7], m[11], m[15]);
     printf("\n");
    
}
std::array<float, 16> mat4identity() { 
    std::array<float, 16> result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return result;
}
std::array<float, 16> mat4rotx(float angle) {
    std::array<float, 16> result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cos(angle), -sin(angle), 0.0f,
        0.0f, sin(angle), cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return result;
}
std::array<float, 16> mat4roty(float angle) {
    std::array<float, 16> result = {
        cos(angle), 0.0f, sin(angle), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -sin(angle), 0.0f, cos(angle), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return result;
}
std::array<float, 16> mat4rotz(float angle){
    std::array<float, 16> result = {
        cos(angle), sin(angle), 0.0f, 0.0f,
        -sin(angle), cos(angle), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return result;
}
std::array<float, 16> mat4scale(float scale) {
    std::array<float, 16> result = {
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, scale, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    return result;
}
std::array<float, 16> mat4translate(float x, float y, float z) {
    std::array<float, 16> result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, z, 1.0f
    };
    return result;
}


/*
 * main(int argc, char* argv[]) - the standard C++ entry point for the program
 */
int main(int, char*[]) {
    
     const std::vector<GLfloat> vertexArrayData = {
         /* old code
    -1.0f, -1.0f, -1.0f, // First vertex, xyz
    -1.0f, -1.0f, -1.0f, // Second vertex, xyz
    -1.0f, -1.0f, -1.0f, // Third vertex, xyz

    -1.0f, -1.0f, 1.0f, // First vertex, xyz
    -1.0f, -1.0f, 1.0f, // Second vertex, xyz
    -1.0f, -1.0f, 1.0f, // Third vertex, xyz

    -1.0f, 1.0f, 1.0f, // First vertex, xyz
    -1.0f, 1.0f, 1.0f, // Second vertex, xyz
    -1.0f, 1.0f, 1.0f, // Third vertex, xyz

    -1.0f, 1.0f, -1.0f, // First vertex, xyz
    -1.0f, 1.0f, -1.0f, // Second vertex, xyz
    -1.0f, 1.0f, -1.0f, // Third vertex, xyz

    1.0f, -1.0f, -1.0f, // First vertex, xyz
    1.0f, -1.0f, -1.0f, // Second vertex, xyz
    1.0f, -1.0f, -1.0f, // Third vertex, xyz

    1.0f, -1.0f, 1.0f, // First vertex, xyz
    1.0f, -1.0f, 1.0f, // Second vertex, xyz
    1.0f, -1.0f, 1.0f, // Third vertex, xyz

    1.0f, 1.0f, 1.0f, // First vertex, xyz
    1.0f, 1.0f, 1.0f, // Second vertex, xyz
    1.0f, 1.0f, 1.0f, // Third vertex, xyz

    1.0f, 1.0f, -1.0f, // First vertex, xyz
    1.0f, 1.0f, -1.0f, // Second vertex, xyz
    1.0f, 1.0f, -1.0f // Third vertex, xyz
    */

    };
    const std::vector<GLuint> indexArrayData = {
        /* old code
        0, 3, 9,
        3, 6, 9,
        4, 17, 7,
        17, 20, 7,
        15, 21, 18,
        15, 13, 21, 
        12, 10, 23,
        12, 1, 10,
        8, 22, 11, 
        8, 19, 22,
        2, 16, 5,
        2, 14, 16
         */

    };

        const std::vector<GLfloat> colorArrayData = {
        1.0f, 0.0f, 0.0f,  
        0.0f, 1.0f, 0.0f,  
        1.0f, 0.0f, 1.0f,  

        1.0f,  0.0f,  0.0f,  
        0.0f,  1.0f,  1.0f,  
        1.0f,  0.0f,  1.0f, 

        1.0f,  0.0f,  0.0f,  
        0.0f,  1.0f,  1.0f,  
        0.0f,  0.0f,  1.0f, 

        1.0f,  0.0f,  0.0f,  
        0.0f,  1.0f,  0.0f,  
        0.0f,  0.0f,  1.0f, 

        0.0f,  1.0f,  0.0f,  
        1.0f,  1.0f,  0.0f,  
        1.0f,  0.0f,  1.0f, 

        1.0f,  1.0f,  0.0f,  
        1.0f,  0.0f,  1.0f,  
        0.0f,  1.0f,  1.0f, 

        1.0f,  1.0f,  0.0f,  
        0.0f,  0.0f,  1.0f,  
        0.0f,  1.0f,  1.0f,  

        1.0f,  1.0f,  0.0f,  
        0.0f,  0.0f,  1.0f,  
        0.0f,  1.0f,  0.0f,
    };
    
    Shader myShader;

    // Initialise GLFW
    glfwInit();


    const GLFWvidmode* vidmode;  // GLFW struct to hold information about the display
    // Determine the desktop size
    vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    // Make sure we are getting a GL context of at least version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable the OpenGL core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a square window (aspect 1:1) to fill half the screen height
    GLFWwindow* window =
        glfwCreateWindow(vidmode->height / 2, vidmode->height / 2, "GLprimer", nullptr, nullptr);
    if (!window) {
        std::cout << "Unable to open window. Terminating.\n";
        glfwTerminate();  // No window was opened, so we can't continue in any useful way
        return -1;
    }

    // Make the newly created window the "current context" for OpenGL
    // (This step is strictly required or things will simply not work)
    glfwMakeContextCurrent(window);

    // Initialize glew
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return -1;
    }

    // Show some useful information on the GL context
    std::cout << "GL vendor:       " << glGetString(GL_VENDOR)
              << "\nGL renderer:     " << glGetString(GL_RENDERER)
              << "\nGL version:      " << glGetString(GL_VERSION)
              << "\nDesktop size:    " << vidmode->width << " x " << vidmode->height << "\n";

    // Get window size. It may start out different from the requested size and
    // will change if the user resizes the window
    int width, height;
   // glfwGetWindowSize(window, &width, &height);
    // Set viewport. This is the pixel rectangle we want to draw into
   // glViewport(0, 0, width, height);  // The entire window

    glfwSwapInterval(0);  // Do not wait for screen refresh between frames
    GLuint vertex_array_ID = 0;
    glGenVertexArrays(1, &vertex_array_ID);
    glBindVertexArray(vertex_array_ID);
    
    GLuint vertexBufferID = createVertexBuffer(0, 3, vertexArrayData);
    GLuint colorBufferID = createVertexBuffer(1, 3, colorArrayData);
    // Create the index buffer object (the list of triangles).
    GLuint indexBufferID = createIndexBuffer(indexArrayData);
    glBindVertexArray(0);
    
    myShader.createShader("../Shaders/vertex.glsl", "../Shaders/fragment.glsl");

    GLint locationTime = glGetUniformLocation(myShader.id(), "time");
    if (locationTime == -1) {  // If the variable is not found, -1 is returned
        std::cout << "Unable to locate variable 'time' in shader!\n";
        
    }

   // TriangleSoup myShape;
    //myShape.createTriangle();

    TriangleSoup mySoup;
    mySoup.createSphere(1.0f, 20);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        util::displayFPS(window);


        glfwGetWindowSize(window, &width, &height);
        // Set viewport. This is the pixel rectangle we want to draw into
        glViewport(0, 0, width, height);  // The entire window

        // Set the clear color to a dark gray (RGBA)
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        // Clear the color and depth buffers for drawing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(myShader.id());

        /* ---- Rendering code should go here ---- */
        // ---- Put the following code in the rendering loop

        std::array<GLfloat, 16> matOR = mat4roty(glfwGetTime());
        std::array<GLfloat, 16> matR1 = mat4roty(glfwGetTime());
        std::array<GLfloat, 16> matR2 = mat4rotz(glfwGetTime());
        std::array<GLfloat, 16> cam = mat4rotx(30);
        std::array<GLfloat, 16> matS = mat4scale(0.8f);
        std::array<GLfloat, 16> matOT = mat4translate(0.5f, 0.0f, 0.0f);
      
        std::array<GLfloat, 16> matA = mat4mult(matR1, matS);
        /*
        std::array<GLfloat, 16> matB = mat4mult(matR2, matA);
        std::array<GLfloat, 16> matC = mat4mult(matOT, matB);
        std::array<GLfloat, 16> matD = mat4mult(matOR, matC);
        std::array<GLfloat, 16> matE = mat4mult(cam, matA);
        */

        GLint locationE = glGetUniformLocation(myShader.id(), "E");
        glUseProgram(myShader.id());  // Activate the shader to set its variables
        glUniformMatrix4fv(locationE, 1, GL_FALSE, matA.data());  // Copy the value

        GLint locationOT = glGetUniformLocation(myShader.id(), "OT");
        glUseProgram(myShader.id());  // Activate the shader to set its variables
        glUniformMatrix4fv(locationOT, 1, GL_FALSE, matOT.data());
        
        float time = static_cast<float>(glfwGetTime()); // Number of seconds since the program was started
        glUseProgram(myShader.id());  // Activate the shader to set its variables
        glUniform1f(locationTime,time);  // Copy the value to the shader program
        
        GLint locationR = glGetUniformLocation(myShader.id(), "R");
        glUseProgram(myShader.id());  // Activate the shader to set its variables
        glUniformMatrix4fv(locationR, 1, GL_FALSE, matR1.data());

        //myShape.render();
        mySoup.render();
        
        // Activate the vertex array object we want to draw (we may have several)
         glBindVertexArray(vertex_array_ID);// Draw our triangle with 3 vertices.

          // When the last argument of glDrawElements is nullptr, it means
        // "use the previously bound index buffer". (This is not obvious.)
         // The index buffer is part of the VAO state and is bound with it.

         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
         glEnable(GL_CULL_FACE);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         glCullFace(GL_BACK);

         /*
         glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
         // glEnable(GL_CULL_FACE);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glCullFace(GL_FRONT);
         */


        // Swap buffers, display the image and prepare for next frame
        glfwSwapBuffers(window);

        // Poll events (read keyboard and mouse input)
        glfwPollEvents();

        // Exit if the ESC key is pressed (and also if the window is closed)
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
    // ------------------------------------------------------------------------
    // ---- Put the following code directly after the rendering loop (before glfwDestroyWindow())
    // release the vertex and index buffers as well as the vertex array
    glDeleteVertexArrays(1, &vertex_array_ID);
    glDeleteBuffers(1, &vertexBufferID);
    glDeleteBuffers(1, &indexBufferID);
    glDeleteBuffers(1, &colorBufferID);


    // Close the OpenGL window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}
