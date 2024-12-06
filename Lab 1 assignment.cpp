/*


//
//  main.cpp
//  triangle
//
//  Created by Nazirul Hasan on 26/8/23.
//  modified by Badiuzzaman
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float rotateAngle = 0.0;
float translate_X = -0.42;
float translate_Y = -0.45;
float scale_X = 4.8;
float scale_Y = 3.5;


float rotateAngle2 = 0.0;  // Rotation angle for VAO2
float translate_X2 = -0.47;
float translate_Y2 = -0.75;
float scale_X2 = 5.0;
float scale_Y2 = 8;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Assignment 1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //triangle
    float vertices2[] = {
0.042,0.118,0,
0.042,0.097,0,
0.042,0.102,0,
0.041,0.08,0,
0.05,0.072,0,
0.059,0.07,0,
0.07,0.068,0,
0.08,0.066,0,
0.09,0.065,0,
0.1,0.064,0,
0.11,0.063,0,
0.12,0.064,0,
0.13,0.065,0,
0.14,0.066,0,
0.15,0.068,0,
0.16,0.07,0,
0.17,0.072,0,
0.181,0.08,0,
0.182,0.102,0,
0.182,0.097,0,
0.182,0.118,0,
0.042,0.118,0,
0.051,0.11,0,
0.059,0.109,0,
0.07,0.1075,0,
0.08,0.106,0,
0.09,0.104,0,
0.1,0.102,0,
0.11,0.101,0,
0.12,0.102,0,
0.13,0.104,0,
0.14,0.106,0,
0.15,0.108,0,
0.16,0.11,0,
0.169,0.112,0,
0.182,0.118,0,
0.042,0.118,0,
0.05,0.122,0,
0.059,0.127,0,
0.07,0.129,0,
0.07501,0.13,0,
0.07501,0.1175,0,
0.08,0.1155,0,
0.09,0.113,0,
0.1,0.112,0,
0.105,0.111,0,
0.07501,0.145,0,
0.07501,0.148,0,
0.08,0.15,0,
0.09,0.153,0,
0.1,0.154,0,
0.105,0.155,0,
0.042,0.097,0,
0.05,0.1,0,
0.055,0.09401,0,
0.0599,0.096,0,
0.065,0.09,0,
0.07,0.093,0,
0.07501,0.088,0,
0.08,0.092,0,
0.09,0.087,0,
0.1,0.09,0,
0.11,0.087,0,
0.07501,0.135,0,
0.08,0.122,0,
0.09,0.12,0,
0.108,0.118,0,
0.07501,0.125,0,
0.07501,0.145,0,
0.08499,0.141,0,
0.1,0.14,0,
0.107,0.138,0,
0.07501,0.135,0,
0.08499,0.13,0,
0.1,0.128,0,
0.109,0.127,0,
0.12,0.128,0,
0.135,0.13,0,
0.145,0.135,0,
0.107,0.138,0,
0.113,0.138,0,
0.12,0.14,0,
0.182,0.118,0,
0.17,0.122,0,
0.16,0.127,0,
0.15,0.129,0,
0.145,0.13,0,
0.145,0.1175,0,
0.14,0.1155,0,
0.13,0.113,0,
0.12,0.112,0,
0.115,0.111,0,
0.105,0.111,0,
0.145,0.145,0,
0.145,0.148,0,
0.14,0.15,0,
0.13,0.153,0,
0.12,0.154,0,
0.115,0.155,0,
0.105,0.155,0,
0.182,0.097,0,
0.17,0.1,0,
0.165,0.09401,0,
0.16,0.096,0,
0.155,0.09,0,
0.15,0.093,0,
0.145,0.088,0,
0.14,0.092,0,
0.13,0.087,0,
0.12,0.09,0,
0.11,0.087,0,
0.145,0.135,0,
0.14,0.122,0,
0.13,0.12,0,
0.112,0.118,0,
0.108,0.118,0,
0.145,0.125,0,
0.145,0.145,0,
0.135,0.141,0,
0.12,0.14,0,
0.113,0.138,0,
0.042,0.097,0,
0.05,0.1,0,
0.05,0.072,0,
0.055,0.09401,0,
0.0599,0.07,0,
0.0599,0.096,0,
0.065,0.09,0,
0.07,0.068,0,
0.07,0.093,0,
0.07501,0.088,0,
0.08,0.066,0,
0.08,0.092,0,
0.09,0.087,0,
0.09,0.065,0,
0.1,0.09,0,
0.1,0.064,0,
0.11,0.087,0,
0.11,0.063,0,
0.12,0.09,0,
0.12,0.064,0,
0.13,0.087,0,
0.13,0.065,0,
0.14,0.092,0,
0.14,0.066,0,
0.145,0.088,0,
0.15,0.068,0,
0.15,0.093,0,
0.16,0.07,0,
0.155,0.09,0,
0.17,0.072,0,
0.16,0.096,0,
0.17,0.072,0,
0.165,0.09401,0,
0.17,0.1,0,
0.181,0.08,0,
0.182,0.097,0,
0.17,0.072,0,
0.165,0.09401,0,
0.042,0.102,0,
0.05,0.072,0,
0.041,0.08,0,
0.07,0.068,0,
0.0599,0.07,0,
0.065,0.09,0,
0.07,0.068,0,
0.08,0.066,0,
0.07501,0.088,0,
0.08,0.066,0,
0.09,0.065,0,
0.09,0.087,0
    };


    float  vertices[] = {
0.01,0.08,0,
0.02,0.09,0,
0.03,0.098,0,
0.04,0.102,0,
0.0501,0.105,0,
0.06,0.107,0,
0.0701,0.109,0,
0.08,0.11,0,
0.0899,0.112,0,
0.1,0.113,0,
0.11,0.112,0,
0.12,0.111,0,
0.13,0.11,0,
0.14,0.109,0,
0.15,0.107,0,
0.16,0.105,0,
0.17,0.103,0,
0.18,0.101,0,
0.19,0.095,0,
0.2,0.085,0,
0.195,0.07,0,
0.19,0.065,0,
0.18,0.058,0,
0.175,0.053,0,
0.17,0.05,0,
0.16,0.047,0,
0.15,0.045,0,
0.14,0.042,0,
0.13,0.041,0,
0.12,0.04,0,
0.11,0.039,0,
0.1,0.038,0,
0.0899,0.039,0,
0.08,0.04,0,
0.0701,0.041,0,
0.06,0.042,0,
0.055,0.044,0,
0.0501,0.045,0,
0.042,0.047,0,
0.035,0.05,0,
0.03299,0.056,0,
0.028,0.058,0,
0.02,0.062,0,
0.013,0.07,0,
0.18,0.058,0,
0.17,0.055,0,
0.16,0.052,0,
0.15,0.05,0,
0.14,0.049,0,
0.13,0.047,0,
0.12,0.046,0,
0.11,0.045,0,
0.1,0.045,0,
0.0899,0.046,0,
0.08,0.047,0,
0.0701,0.048,0,
0.06,0.049,0,
0.0501,0.051,0,
0.04,0.053,0,
0.03299,0.056,0,
0.04499,0.08,0,
0.0501,0.087,0,
0.06,0.092,0,
0.0701,0.094,0,
0.08,0.095,0,
0.0899,0.098,0,
0.1,0.098,0,
0.11,0.099,0,
0.12,0.098,0,
0.13,0.096,0,
0.14,0.093,0,
0.15,0.091,0,
0.16,0.087,0,
0.165,0.08,0,
0.16,0.075,0,
0.15,0.069,0,
0.14,0.065,0,
0.13,0.062,0,
0.12,0.061,0,
0.11,0.06,0,
0.1,0.061,0,
0.0899,0.062,0,
0.08,0.064,0,
0.0701,0.067,0,
0.06,0.069,0,
0.0501,0.072,0

    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);



    // Second object VAO and VBO
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
    glBindVertexArray(0); // unbind VAO


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

        // create transformations
        //glm::mat4 trans = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //trans = glm::translate(trans, glm::vec3(translate_X, translate_Y, 0.0f));
        //trans = glm::rotate(trans, glm:: radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        //trans = glm::scale(trans,glm::vec3(scale_X, scale_Y, 1.0));
        glm::mat4 translationMatrix;
        glm::mat4 rotationMatrix;
        glm::mat4 scaleMatrix;
        glm::mat4 modelMatrix;
        glm::mat4 identityMatrix = glm::mat4(1.0f);


        translationMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        //modelMatrix = rotationMatrix * scaleMatrix;




        // get matrix's uniform location and set matrix
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // draw our first triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_LINE_LOOP, 0, 44);
        glDrawArrays(GL_LINE_STRIP, 44, 16);
        glDrawArrays(GL_LINE_LOOP, 60, 26);
        glBindVertexArray(0); // no need to unbind it every time


        // Transformation matrix for VAO2 (object 2)
        glm::mat4 translationMatrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(translate_X2, translate_Y2, 0.0f));
        glm::mat4 rotationMatrix2 = glm::rotate(glm::mat4(1.0f), glm::radians(rotateAngle2), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scaleMatrix2 = glm::scale(glm::mat4(1.0f), glm::vec3(scale_X2, scale_Y2, 1.0f));
        glm::mat4 modelMatrix2 = translationMatrix2 * rotationMatrix2 * scaleMatrix2;

        // Set and apply the transformation for VAO2
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix2));



        glBindVertexArray(VAO2);
        glDrawArrays(GL_LINE_STRIP, 0, 21); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 21, 15); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 36, 10); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 46, 6); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 52, 11); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 63, 4); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 67, 5); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 72, 7); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 79, 3); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 82, 11); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 93, 7); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 100, 11); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 111, 5); // Draw vertices2[] as a line loop
        glDrawArrays(GL_LINE_STRIP, 116, 5); // Draw vertices2[] as a line loop
        glDrawArrays(GL_TRIANGLE_STRIP, 121, 38); // Draw vertices2[] as a line loop
        glDrawArrays(GL_TRIANGLES, 159, 3); // Draw vertices2[] as a line loop
        glDrawArrays(GL_TRIANGLES, 162, 3); // Draw vertices2[] as a line loop
        glDrawArrays(GL_TRIANGLES, 165, 3); // Draw vertices2[] as a line loop
        glDrawArrays(GL_TRIANGLES, 168, 3); // Draw vertices2[] as a line loop
 


        glBindVertexArray(0); // Unbind VAO

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        translate_X -= 0.002f;  // Move both left
        translate_X2 -= 0.002f; // Move plate left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        translate_X += 0.002f;  // Move both right
        translate_X2 += 0.002f; // Move plate right
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        translate_Y += 0.002f;  // Move both up
        translate_Y2 += 0.002f; // Move plate up
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        translate_Y -= 0.002f;  // Move both down
        translate_Y2 -= 0.002f; // Move plate down
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        translate_Y += 0.002f;  // Move up
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        translate_Y -= 0.002f;  // Move down
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        translate_X -= 0.002f;  // Move left
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        translate_X += 0.002f;  // Move right

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        translate_X2 -= 0.002f;  // Move left
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        translate_X2 += 0.002f;  // Move right
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        translate_Y2 += 0.002f;  // Move up
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        translate_Y2 -= 0.002f;  // Move down

    //Rotation controls
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        rotateAngle2 += 0.2f;   // Rotate clockwise
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        rotateAngle2 -= 0.2f;   // Rotate counterclockwise

    // Scaling controls 
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        scale_Y2 += 0.002f;  // Increase vertical scale
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        scale_Y2 = std::max(scale_Y - 0.002f, 0.01f);  // Decrease vertical scale with minimum limit
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        scale_X2 += 0.002f;  // Increase horizontal scale
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        scale_X2 = std::max(scale_X - 0.002f, 0.01f);  // Decrease horizontal scale with minimum limit


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



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float scale_increment = 0.2f;

    // Increase or decrease scale based on scroll direction
    if (yoffset > 0) {
        scale_Y += scale_increment; // Zoom in
        scale_X += scale_increment;
    }
    else {
        scale_Y = std::max(scale_Y - scale_increment, 0.01f); // Zoom out 
        scale_X = std::max(scale_X - scale_increment, 0.01f);
    }
}

float rotation_angle = 0.0f;
bool rotating_cake = false;  // Indicates if rotation is active
bool rotating_plate = false;  // Indicates if rotation is active
double lastX, lastY;    // Last mouse positions


// Mouse button callback to start/stop rotation
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rotating_cake = true;
            //translating_cake = true;
        }
        else if (action == GLFW_RELEASE) {
            rotating_cake = false;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            rotating_plate = true;
            rotating_cake = true;
        }
        else if (action == GLFW_RELEASE) {
            rotating_plate = false;
            rotating_cake = false;
        }
    }


}

void cursor_position_callback(GLFWwindow* window, double xPOS, double yPOS) {
    if (rotating_cake && rotating_plate) {
        // Calculate the difference in mouse position

        float dY = lastY - yPOS;

        rotateAngle += (dY * 0.1f);
        rotateAngle2 += (dY * 0.1f);

        // Keep angles Within 0 to 360 degrees
        if (rotateAngle >= 360.0f) rotateAngle -= 360.0f;
        if (rotateAngle < 0.0f) rotateAngle += 360.0f;
        if (rotateAngle2 >= 360.0f) rotateAngle2 -= 360.0f;
        if (rotateAngle2 < 0.0f) rotateAngle2 += 360.0f;
    }
    else if (rotating_cake) {
        // Calculate the difference in mouse position
        float dY = lastY - yPOS;
        rotateAngle += (dY * 0.1);

        // Keep angle within 0 to 360 degrees
        if (rotateAngle2 >= 360.0f) rotateAngle2 -= 360.0f;
        if (rotateAngle2 < 0.0f) rotateAngle2 += 360.0f;

    }

}



*/