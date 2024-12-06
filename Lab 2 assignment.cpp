//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//  modified by Badiuzzaman on 3/11/24.
//


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "basic_camera2.h"

#include <iostream>
#include <vector>
#include <cmath>
#include "pointLight2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
bool f = false;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
//void generateCircleVertices(std::vector<float>& vertices);

// draw object functions
void drawCube(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX = 0.0, float posY = 0.0, float posZ = 0.0, float rotX = 0.0, float rotY = 0.0, float rotZ = 0.0, float scX = 1.0, float scY = 1.0, float scZ = 1.0);
void drawCylender(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scX, float scY, float scZ);
void drawTable(Shader ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Center);
void drawChair(Shader ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Center);
void drawTableandChair(Shader  ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Modify);
void drawSofa(Shader  ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Modify);
void drawAlmira(Shader  ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Modify, bool almira);
void drawFan(Shader ourShader, glm::mat4 identityMatrix, unsigned int VAO, unsigned int VAO2, glm::mat4 Model_Center);
glm::mat4 myRotate(float angleRadians, const glm::vec3& axis);
void orthogonalProjection(float left, float right, float bottom, float top, float near, float far, float matrix[16]);
glm::mat4 perspectiveProjection(float fov, float aspect, float near, float far);
void calculateVertexNormals(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, std::vector<float>& normals);
glm::vec3 computeSurfaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
void generateCylinder(float radius, float height, int segments, std::vector<float>& vertices, std::vector<unsigned int>& indices);
void generateCone(float radius, float height, int segments, std::vector<float>& vertices, std::vector<unsigned int>& indices);
void generateSphere(float radius, int sectorCount, int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0;
float rotateAngle_Y = 0;
float rotateAngle_Z = 0;

float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;


float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 0.5;
float scale_Y = 0.5;
float scale_Z = 0.5;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = -5.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float cameraSpeed = 0.005f;


const float PI = 3.14f;
const float ROTATION_STEP = 1.0f; // Step in radians

bool fanon;


// lighting
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-0.75f,  1.5f,  1.0f),
    glm::vec3(-1.40f,  1.5f,  1.0f)

};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       //�light�number
);

//directional light
bool directionLightOn = true;
bool directionalAmbient = true;
bool directionalDiffuse = true;
bool directionalSpecular = true;

//spot light
bool spotLightOn = true;

// light settings
bool pointLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;

//point light
bool point1 = true;
bool point2 = true;



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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    Shader constantShader("vertexShader.vs", "fragmentShaderV2.fs");

    Shader FanShader("vertexShaderSimple.vs", "fragmentShaderV2.fs");

    Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions          // normals
        // Front face
        0.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
        0.5f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, 0.0f,    0.0f, 0.0f, -1.0f,
        0.0f, 0.5f, 0.0f,    0.0f, 0.0f, -1.0f,

        // Right face
        0.5f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f,    1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,    1.0f, 0.0f, 0.0f,

        // Back face
        0.0f, 0.0f, 0.5f,    0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f,    0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f,

        // Left face
        0.0f, 0.0f, 0.5f,    -1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f,    -1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,    -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,

        // Top face
        0.5f, 0.5f, 0.5f,    0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,    0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f,    0.0f, 1.0f, 0.0f,

        // Bottom face
        0.0f, 0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, 0.5f,    0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 0.5f,    0.0f,-1.0f,0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    //float cube_vertices[] = {
    //    0.0f, 0.0f, 0.0f, 0.3f, 0.8f, 0.5f,
    //    0.5f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f,
    //    0.5f, 0.5f, 0.0f, 0.2f, 0.7f, 0.3f,
    //    0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.8f,
    //    0.0f, 0.0f, 0.5f, 0.8f, 0.3f, 0.6f,
    //    0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.8f,
    //    0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.6f,
    //    0.0f, 0.5f, 0.5f, 0.7f, 0.5f, 0.4f
    //};
    /*unsigned int cube_indices[] = {
       0, 3, 2,
       2, 1, 0,

       1, 2, 6,
       6, 5, 1,

       5, 6, 7,
       7 ,4, 5,

       4, 7, 3,
       3, 0, 4,

       6, 2, 3,
       3, 7, 6,

       1, 5, 4,
       4, 0, 1
    };*/



    std::vector<float> cylinder_vertix = {
    1,0.5,0,
    1,0,0,
    0.913,0.5,0.406,
    0.913,0,0.406,
    0.669,0.5,0.743,
    0.669,0,0.743,
    0.309,0.5,0.951,
    0.309,0,0.951,
    -0.104,0.5,0.994,
    -0.104,0,0.994,
    -0.5,0.5,0.866,
    -0.5,0,0.866,
    -0.809,0.5,0.588,
    -0.809,0,0.588,
    -0.978,0.5,0.208,
    -0.978,0,0.208,
    -0.978,0.5,-0.208,
    -0.978,0,-0.208,
    -0.809,0.5,-0.588,
    -0.809,0,-0.588,
    -0.5,0.5,-0.866,
    -0.5,0,-0.866,
    -0.104,0.5,-0.994,
    -0.104,0,-0.994,
    0.309,0.5,-0.951,
    0.309,0,-0.951,
    0.669,0.5,-0.743,
    0.669,0,-0.743,
    0.913,0.5,-0.406,
    0.913,0,-0.406,
    };

    std::vector<unsigned int> cylinder_indices = {
        // Top cap
        1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,

        // Bottom cap
        0,28,26,24,22,20,18,16,14,12,10,8,6,4,2,

        //side bar
        //1,2,0,1,3,2,3,4,2,3,5,4,5,6,4,5,7,6,7,8,6,
         0, 2, 1,   2, 3, 1,
         2, 4, 3,   4, 5, 3,
         4, 6, 5,   6, 7, 5,
         6, 8, 7,   8, 9, 7,
         8, 10, 9,  10, 11, 9,
         10, 12, 11, 12, 13, 11,
         12, 14, 13, 14, 15, 13,
         14, 16, 15, 16, 17, 15,
         16, 18, 17, 18, 19, 17,
         18, 20, 19, 20, 21, 19,
         20, 22, 21, 22, 23, 21,
         22, 24, 23, 24, 25, 23,
         24, 26, 25, 26, 27, 25,
         26, 28, 27, 28, 29, 27,
         28, 0, 29,   0, 1, 29,
    };

    // Generate cylinder vertices and indices
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    generateCylinder(0.25f, 0.5f, 36, vertices, indices);

    // Generate cone data
    std::vector<float> vertices_k;
    std::vector<unsigned int> indices_k;
    generateCone(1.0f, 2.0f, 36, vertices_k, indices_k);

    // Generate sphere data
    std::vector<float> vertices_s;
    std::vector<unsigned int> indices_s;
    generateSphere(1.0f, 36, 18, vertices_s, indices_s);

    std::vector<float> normals;
    calculateVertexNormals(cylinder_vertix, cylinder_indices, normals);
    // Combine vertices and normals into a single array for rendering
    std::vector<float> combinedVertices;
    for (size_t i = 0; i < cylinder_vertix.size() / 3; ++i) {
        combinedVertices.push_back(cylinder_vertix[i * 3]);     // x
        combinedVertices.push_back(cylinder_vertix[i * 3 + 1]); // y
        combinedVertices.push_back(cylinder_vertix[i * 3 + 2]); // z
        combinedVertices.push_back(normals[i * 3]);      // nx
        combinedVertices.push_back(normals[i * 3 + 1]);  // ny
        combinedVertices.push_back(normals[i * 3 + 2]);  // nz
    }


    // //cube
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // //cylinder
    unsigned int VBO2, VAO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, combinedVertices.size() * sizeof(float), combinedVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder_indices.size() * sizeof(unsigned int), cylinder_indices.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  // Normal
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //glBindVertexArray(VAO2);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_vertix), cylinder_vertix, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinder_indices), cylinder_indices, GL_STATIC_DRAW);

    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //glBindVertexArray(0);


    // // sphere
        // Create VAO_S, VBO_S, and EBO_S
    unsigned int VAO_S, VBO_S, EBO_S;
    glGenVertexArrays(1, &VAO_S);
    glGenBuffers(1, &VBO_S);
    glGenBuffers(1, &EBO_S);

    // Bind VAO
    glBindVertexArray(VAO_S);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_S);
    glBufferData(GL_ARRAY_BUFFER, vertices_s.size() * sizeof(float), vertices_s.data(), GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_S);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_s.size() * sizeof(unsigned int), indices_s.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);


    // //cone
        // Create VAO, VBO, and EBO
    unsigned int VAO_K, VBO_K, EBO_K;
    glGenVertexArrays(1, &VAO_K);
    glGenBuffers(1, &VBO_K);
    glGenBuffers(1, &EBO_K);

    // Bind VAO
    glBindVertexArray(VAO_K);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO_K);
    glBufferData(GL_ARRAY_BUFFER, vertices_k.size() * sizeof(float), vertices_k.data(), GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_K);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_k.size() * sizeof(unsigned int), indices_k.data(), GL_STATIC_DRAW);



    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // Unbind VAO
    glBindVertexArray(0);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //ourShader.use();

    float speed = 0.0;
    glm::vec3 color;
    //constantShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // be sure to activate shader when setting uniforms/drawing objects
         //lightingShader.use();
        lightingShader.setVec3("viewPos", basic_camera.eye);

        //point lights set up
        pointlight1.setUpPointLight(lightingShader);
        pointlight2.setUpPointLight(lightingShader);

        //directional light set up
        lightingShader.setVec3("directionalLight.direction", 0.0f, 0.0f, -0.5f);
        lightingShader.setVec3("directionalLight.ambient", 0.1f, 0.1f, 0.1f);
        lightingShader.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setBool("directionLightOn", directionLightOn);

        //spot light set up
        lightingShader.setVec3("spotLight.position", 0.0f, 0.0f, .80f);
        lightingShader.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShader.setVec3("spotLight.ambient", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.k_c", 1.0f);
        lightingShader.setFloat("spotLight.k_l", 0.09);
        lightingShader.setFloat("spotLight.k_q", 0.032);
        lightingShader.setFloat("spotLight.cos_theta", glm::cos(glm::radians(50.0f)));
        lightingShader.setBool("spotLightOn", spotLightOn);


        //handle for changes in directional light directly from shedder
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
            if (directionLightOn) {
                lightingShader.setBool("ambientLight", !directionalAmbient);
                directionalAmbient = !directionalAmbient;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
            if (directionLightOn) {
                lightingShader.setBool("diffuseLight", !directionalDiffuse);
                directionalDiffuse = !directionalDiffuse;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            if (directionLightOn) {
                lightingShader.setBool("specularLight", !directionalSpecular);
                directionalSpecular = !directionalSpecular;
            }
        }



        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(basic_camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 projection = perspectiveProjection(50.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 50.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        constantShader.setMat4("projection", projection);
        FanShader.setMat4("projection", projection);
        lightingShader.setMat4("projection", projection);



        // camera/view transformation
        glm::mat4 view = basic_camera.createViewMatrix();

        ourShader.setMat4("view", view);
        constantShader.setMat4("view", view);
        FanShader.setMat4("view", view);
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //drawCube(ourShader, VAO, identityMatrix, translate_X, translate_Y, translate_Z, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, scale_X, scale_Y, scale_Z);

        glm::mat4 translateMatrix, rotationMatrix, scaleMatrix, modelCenter, model;


        modelCenter = glm::scale(identityMatrix, glm::vec3(0.7, 0.7, 1));

        drawTableandChair(lightingShader, identityMatrix, VAO, modelCenter);

        modelCenter = identityMatrix;

        drawSofa(lightingShader, identityMatrix, VAO, modelCenter);
        //almira

        //rotationMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rotationMatrix = myRotate(glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.59f, 0.1f, 0.0f));
        modelCenter = translateMatrix * rotationMatrix * identityMatrix;

        drawAlmira(lightingShader, identityMatrix, VAO, modelCenter, 1);

        //wall

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(9, 8, 2.5));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.8f, +1.0f, 0.0f));
        model = glm::translate(identityMatrix, glm::vec3(-2.0f, -1.5f, 0.0f));

        modelCenter = model * scaleMatrix * translateMatrix;

        drawAlmira(lightingShader, identityMatrix, VAO, modelCenter, 0);

        // tv cavinet

        //rotationMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rotationMatrix = myRotate(glm::radians(-180.0f), glm::vec3(0.0, 0.0, 1.0));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4, 1.1, .35));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.8f, +1.0f, 0.0f));
        model = glm::translate(identityMatrix, glm::vec3(1.0f, 1.5f, 0.0f));

        modelCenter = model * scaleMatrix * rotationMatrix * translateMatrix;

        drawAlmira(lightingShader, identityMatrix, VAO, modelCenter, 1);


        //tv

        lightingShader.use();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 1.45f, 0.475f));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.6, 0.05, 1));

        model = translateMatrix * scaleMatrix;

        color = glm::vec3(0.294f, 0.23f, 0.18f);

        lightingShader.setVec3("material.ambient", color);
        lightingShader.setVec3("material.diffuse", color);
        lightingShader.setVec3("material.specular", color);
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setMat4("model", model);
        //ourShader.setVec4("color", glm::vec4(0.294f, 0.23f, 0.18f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //book shelf 1

        lightingShader.use();


        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.395f, 0.0f, 0.5f));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 1.4, 0.05));

        model = translateMatrix * scaleMatrix;

        color = glm::vec3(0.6f, 0.243f, 0.047f);

        lightingShader.setVec3("material.ambient", color);
        lightingShader.setVec3("material.diffuse", color);
        lightingShader.setVec3("material.specular", color);
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setMat4("model", model);
        //ourShader.setVec4("color", glm::vec4(0.6f, 0.243f, 0.047f, 1.0f));


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        //book shelf 2

        lightingShader.use();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.395f, 0.0f, 0.8f));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 1.4, 0.05));

        model = translateMatrix * scaleMatrix;

        color = glm::vec3(0.6f, 0.243f, 0.047f);

        lightingShader.setVec3("material.ambient", color);
        lightingShader.setVec3("material.diffuse", color);
        lightingShader.setVec3("material.specular", color);
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setMat4("model", model);
        //ourShader.setVec4("color", glm::vec4(0.6f, 0.243f, 0.047f, 1.0f));



        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        float y = 0.0;

        for (int i = 1; i < 7; i++)
        {
            //book for shelf 1

            lightingShader.use();
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.395f, y, 0.525f));

            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.15, 0.35));

            model = translateMatrix * scaleMatrix;

            color = glm::vec3(0.427f, y * 0.498f, y + 0.592f);

            lightingShader.setVec3("material.ambient", color);
            lightingShader.setVec3("material.diffuse", color);
            lightingShader.setVec3("material.specular", color);
            lightingShader.setFloat("material.shininess", 32.0f);

            lightingShader.setMat4("model", model);
            //ourShader.setVec4("color", glm::vec4(0.427f, y * 0.498f, 0.592f, y+0.2));


            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            //book for shelf 2

            lightingShader.use();
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.395f, y, 0.825f));
            y += 0.12;

            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.15, 0.35));

            model = translateMatrix * scaleMatrix;

            color = glm::vec3(0.769f, 0.616f, y + 0.655f);

            lightingShader.setVec3("material.ambient", color);
            lightingShader.setVec3("material.diffuse", color);
            lightingShader.setVec3("material.specular", color);
            lightingShader.setFloat("material.shininess", 32.0f);

            lightingShader.setMat4("model", model);
            //ourShader.setVec4("color", glm::vec4(0.769f, 0.616f, 0.655f, y + 0.2));


            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

        }

        //floor

        lightingShader.use();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, -1.625f, -0.01f));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(8, 6.5, 0.02));

        model = translateMatrix * scaleMatrix;

        color = glm::vec3(0.612f, 0.71f, 0.694f);

        lightingShader.setVec3("material.ambient", color);
        lightingShader.setVec3("material.diffuse", color);
        lightingShader.setVec3("material.specular", color);
        lightingShader.setFloat("material.shininess", 32.0f);


        lightingShader.setMat4("model", model);
        //ourShader.setVec4("color", glm::vec4(0.612f, 0.71f, 0.694f, 1.0f));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //draw Fan


        //rotationMatrix = glm::rotate(identityMatrix, glm::radians(speed), glm::vec3(0.0f, 0.0f, 1.0f));
        rotationMatrix = myRotate(glm::radians(speed), glm::vec3(0.0, 0.0, 1.0));
        modelCenter = rotationMatrix;
        drawFan(lightingShader, identityMatrix, VAO, VAO2, modelCenter);

        if (fanon)
        {
            speed -= 1;
        }


        //draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);

        for (unsigned int i = 0; i < 2; i++)
        {
            translateMatrix = glm::translate(identityMatrix, pointLightPositions[i]);
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -0.2f, 0.2f));
            model = translateMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glad_glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        basic_camera.move(BACKWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        basic_camera.move(FORWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        basic_camera.move(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        basic_camera.move(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        basic_camera.move(YAW_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        basic_camera.move(YAW_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        basic_camera.move(PITCH_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        basic_camera.move(PITCH_L, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        basic_camera.move(ROLL_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        basic_camera.move(ROLL_L, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        basic_camera.eye -= cameraSpeed * basic_camera.direction; // Forward
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        basic_camera.eye += cameraSpeed * basic_camera.direction;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;


    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.5;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }


    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.5;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.eye = glm::vec3(eyeX, eyeY, eyeZ);
    }



    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        directionLightOn = !directionLightOn;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if (pointlight1.ambientOn > 0 && pointlight1.diffuseOn > 0 && pointlight1.specularOn > 0) {
            pointlight1.turnOff();
            point1 = false;
        }
        else {
            pointlight1.turnOn();
            point1 = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if (pointlight2.ambientOn > 0 && pointlight2.diffuseOn > 0 && pointlight2.specularOn > 0) {
            pointlight2.turnOff();
            point2 = false;
        }
        else {
            pointlight2.turnOn();
            point2 = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        spotLightOn = !spotLightOn;
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        if (pointlight1.ambientOn > 0 || pointlight2.ambientOn > 0) {

            pointlight1.turnAmbientOff();

            pointlight2.turnAmbientOff();
        }
        else {

            pointlight1.turnAmbientOn();

            pointlight2.turnAmbientOn();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        if (pointlight1.diffuseOn > 0 || pointlight2.diffuseOn > 0) {

            pointlight1.turnDiffuseOff();

            pointlight2.turnDiffuseOff();
        }
        else {

            pointlight1.turnDiffuseOn();

            pointlight2.turnDiffuseOn();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        if (pointlight1.specularOn > 0 || pointlight2.specularOn > 0) {

            pointlight1.turnSpecularOff();

            pointlight2.turnSpecularOff();
        }
        else {

            pointlight1.turnSpecularOn();

            pointlight2.turnSpecularOn();
        }
    }


    /*if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        lookAtZ -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }*/

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
    }


    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (fanon)
            fanon = false;
        else
            fanon = true;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called, do as you please with it
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //basic_camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    basic_camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// The parentTrans parameter is here for hiererchical modeling,
// If you are confused with it's usage, then pass an identity matrix to it, and everything will be fine 

void drawCube(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scX, float scY, float scZ)
{
    shaderProgram.use();

    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, model, modelCentered;
    translateMatrix = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    //rotateXMatrix = glm::rotate(translateMatrix, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    //rotateYMatrix = glm::rotate(rotateXMatrix, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    //rotateZMatrix = glm::rotate(rotateYMatrix, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(translateMatrix, glm::vec3(scX, scY, scZ));
    modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", modelCentered);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawFan(Shader ourShader, glm::mat4 identityMatrix, unsigned int VAO, unsigned int VAO2, glm::mat4 Model_Center)
{
    glm::mat4 translateMatrix, rotationMatrix, scaleMatrix, model;
    glm::vec3 color;

    // stick of fan

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.025f, -0.025f, 2.115f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 1.75));
    model = translateMatrix * scaleMatrix;

    color = glm::vec3(0.58f, 0.58f, 0.529f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);
    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.58f, 0.58f, 0.529f, 0.851f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //fan

    ourShader.use();
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 2.0));
    //rotationMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = myRotate(glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 0.2));

    model = translateMatrix * rotationMatrix * scaleMatrix;

    color = glm::vec3(0.49f, 0.467f, 0.443f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.49f, 0.467f, 0.443f, 1.0f));


    glBindVertexArray(VAO2);
    glDrawElements(GL_TRIANGLE_FAN, 15, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLE_FAN, 15, GL_UNSIGNED_INT, (const void*)(15 * sizeof(unsigned int)));
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (const void*)(30 * sizeof(unsigned int)));
    glDrawElements(GL_TRIANGLE_STRIP, 90, GL_UNSIGNED_INT, (const void*)(36 * sizeof(unsigned int)));
    //glDrawElements(GL_TRIANGLE_FAN, 30, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    // fan wing 1

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, 2.0625f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 1.2, 0.05));
    model = Model_Center * translateMatrix * scaleMatrix;

    color = glm::vec3(0.4f, 0.4f, 0.365f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.4f, 0.4f, 0.365f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);



    // fan wing 2

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, 2.0625f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 1.2, 0.05));
    //rotationMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = myRotate(glm::radians(-120.0f), glm::vec3(0.0, 0.0, 1.0));
    model = Model_Center * rotationMatrix * translateMatrix * scaleMatrix;
    color = glm::vec3(0.4f, 0.4f, 0.365f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.4f, 0.4f, 0.365f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    // fan wing 3

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, 2.0625f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 1.2, 0.05));
    //rotationMatrix = glm::rotate(identityMatrix, glm::radians(240.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = myRotate(glm::radians(-240.0f), glm::vec3(0.0, 0.0, 1.0));
    model = Model_Center * rotationMatrix * translateMatrix * scaleMatrix;

    color = glm::vec3(0.4f, 0.4f, 0.365f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.4f, 0.4f, 0.365f, 1.0f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void drawChair(Shader ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Center)
{
    glm::mat4 translateMatrix, rotationMatrix, scaleMatrix, sit_modelCenter;
    glm::vec3 color;
    //chair sit

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5 - .15 - 0.2f, +0.1f, 0.0f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.3, .4, .7));

    sit_modelCenter = Model_Center * translateMatrix * scaleMatrix;

    drawTable(ourShader, identityMatrix, VAO, sit_modelCenter);


    //chair head
    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5 - .15 - 0.3f, 0.0f, 0.35 + 0.08f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.075, .4, .5));

    sit_modelCenter = Model_Center * translateMatrix * scaleMatrix;

    color = glm::vec3(0.49f, 0.357f, 0.224f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", sit_modelCenter);
    //ourShader.setVec4("color", glm::vec4(0.49f, 0.357f, 0.224f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawTable(Shader ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Center)
{
    glm::mat4 translateMatrix, rotationMatrix, scaleMatrix, model;
    glm::vec3 color;
    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, -0.25f, 0.5f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2, 1, .2));

    //modelCentered = glm::translate(model, glm::vec3(0, 0, 0));
    model = Model_Center * translateMatrix * scaleMatrix;

    color = glm::vec3(0.831f, 0.847f, 0.859f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(, , , 1.0f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    // Table leg 1

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, -0.25f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, .2, 1));

    //modelCentered = glm::translate(model, glm::vec3(0, 0, 0));
    model = Model_Center * translateMatrix * scaleMatrix;

    color = glm::vec3(0.173f, 0.651f, 0.91f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.173f, 0.651f, 0.91f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    // Table leg 2

    ourShader.use();


    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, +.15f, 0.0f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, .2, 1));

    //modelCentered = glm::translate(model, glm::vec3(0, 0, 0));
    model = Model_Center * translateMatrix * scaleMatrix;

    color = glm::vec3(0.173f, 0.651f, 0.91f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.173f, 0.651f, 0.91f, 1.0f));



    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Table leg 3

    ourShader.use();


    translateMatrix = glm::translate(identityMatrix, glm::vec3(+0.4f, -0.25f, 0.0f));


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, .2, 1));

    //modelCentered = glm::translate(model, glm::vec3(0, 0, 0));
    model = Model_Center * translateMatrix * scaleMatrix;

    color = glm::vec3(0.173f, 0.651f, 0.91f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.173f, 0.651f, 0.91f, 1.0f));



    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Table leg 4

    ourShader.use();


    translateMatrix = glm::translate(identityMatrix, glm::vec3(+0.4f, +0.15f, 0.0f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, .2, 1));

    //modelCentered = glm::translate(model, glm::vec3(0, 0, 0));
    model = Model_Center * translateMatrix * scaleMatrix;

    color = glm::vec3(0.173f, 0.651f, 0.91f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.173f, 0.651f, 0.91f, 1.0f));



    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawTableandChair(Shader  ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Modify)
{
    glm::mat4 translateMatrix, rotationMatrix, scaleMatrix, modelCenter, model;

    //modelCenter = glm::mat4(1.0f);
    modelCenter = Model_Modify;

    // Table
    drawTable(ourShader, identityMatrix, VAO, modelCenter);


    //modelCenter = glm::mat4(1.0f);
    modelCenter = Model_Modify;
    //chair 1
    drawChair(ourShader, identityMatrix, VAO, modelCenter);

    //chair 2

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, -0.2 - 0.05f, 0.0f));
    modelCenter = Model_Modify * translateMatrix;

    drawChair(ourShader, identityMatrix, VAO, modelCenter);

    //chair 3
    //rotationMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = myRotate(glm::radians(-180.0f), glm::vec3(0.0, 0.0, 1.0));
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0, -0.05f, 0.0f));

    modelCenter = Model_Modify * rotationMatrix;

    drawChair(ourShader, identityMatrix, VAO, modelCenter);

    //chair 4
    //rotationMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = myRotate(glm::radians(-180.0f), glm::vec3(0.0, 0.0, 1.0));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, +0.2 + 0.05f, 0.0f));

    modelCenter = Model_Modify * translateMatrix * rotationMatrix;

    drawChair(ourShader, identityMatrix, VAO, modelCenter);

    //chair 5
    //rotationMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = myRotate(glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.3f, 0.0f));
    modelCenter = Model_Modify * translateMatrix * rotationMatrix;

    drawChair(ourShader, identityMatrix, VAO, modelCenter);

    //chair 6
    //rotationMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = myRotate(glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3f, 0.3f, 0.0f));
    modelCenter = Model_Modify * translateMatrix * rotationMatrix;

    drawChair(ourShader, identityMatrix, VAO, modelCenter);

    //chair 7
    //rotationMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = myRotate(glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(+0.1f, -0.3f, 0.0f));
    modelCenter = Model_Modify * translateMatrix * rotationMatrix;

    drawChair(ourShader, identityMatrix, VAO, modelCenter);

    //chair 8
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = myRotate(glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, -0.3f, 0.0f));
    modelCenter = Model_Modify * translateMatrix * rotationMatrix;

    drawChair(ourShader, identityMatrix, VAO, modelCenter);
}


void drawSofa(Shader  ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Modify)
{
    glm::mat4 translateMatrix, rotationMatrix, scaleMatrix, modelCenter, model;
    glm::vec3 color;

    //sofa
    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2 - 1.2f, -0.5f, 0.0f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.8, 2, .2));

    model = Model_Modify * translateMatrix * scaleMatrix;

    color = glm::vec3(0.922f, 0.706f, 0.651f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.922f, 0.706f, 0.651f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //sofa head
    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2 - 1.2f, -0.5f, 0.1f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.3, 2, .5));

    model = Model_Modify * translateMatrix * scaleMatrix;

    color = glm::vec3(0.961f, 0.867f, 0.753f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.961f, 0.867f, 0.753f, 0.851f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //sofa hand left side

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2 - 1.2 + 0.15f, -0.5f, 0.1f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .3, .5));

    model = Model_Modify * translateMatrix * scaleMatrix;

    color = glm::vec3(0.82f, 0.784f, 0.745f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.82f, 0.784f, 0.745f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //sofa hand right side

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2 - 1.2 + 0.15f, -0.5 + 1.0 - 0.15f, 0.1f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .3, .5));

    model = Model_Modify * translateMatrix * scaleMatrix;

    color = glm::vec3(0.82f, 0.784f, 0.745f);

    ourShader.setVec3("material.ambient", color);
    ourShader.setVec3("material.diffuse", color);
    ourShader.setVec3("material.specular", color);
    ourShader.setFloat("material.shininess", 32.0f);

    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.82f, 0.784f, 0.745f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void drawAlmira(Shader  ourShader, glm::mat4 identityMatrix, unsigned int VAO, glm::mat4 Model_Modify, bool almira)
{
    glm::mat4 translateMatrix, rotationMatrix, scaleMatrix, modelCenter, model;
    glm::vec3 color;

    //almira left side
    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8f, -1.0f, 0.0f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.05, 0.8, 2.4));

    model = Model_Modify * translateMatrix * scaleMatrix;

    ourShader.setMat4("model", model);
    if (almira == true)
    {
        color = glm::vec3(0.82f, 0.569f, 0.314f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);
        //ourShader.setVec4("color", glm::vec4(0.82f, 0.569f, 0.314f, 1.0f));
    }
    else
    {
        color = glm::vec3(0.835f, 0.835f, 0.835f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);
        //ourShader.setVec4("color", glm::vec4(0.835f, 0.835f, 0.835f, 1.0f));

    }


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //almira right side
    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.2f, -1.0f, 0.0f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.05, 0.8, 2.4));

    model = Model_Modify * translateMatrix * scaleMatrix;

    ourShader.setMat4("model", model);
    if (almira == true)
    {
        color = glm::vec3(0.82f, 0.569f, 0.314f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);
        //ourShader.setVec4("color", glm::vec4(0.82f, 0.569f, 0.314f, 1.0f));
    }
    else
    {
        color = glm::vec3(0.835f, 0.835f, 0.835f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);
        //ourShader.setVec4("color", glm::vec4(0.835f, 0.835f, 0.835f, 1.0f));
    }


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);



    //almira back

    if (almira == true)
    {
        ourShader.use();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8 + 0.025f, -1.0f, 0.0f));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.75, 0.05, 2.4));

        model = Model_Modify * translateMatrix * scaleMatrix;

        color = glm::vec3(0.82f, 0.569f, 0.314f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);

        ourShader.setMat4("model", model);
        //ourShader.setVec4("color", glm::vec4(0.82f, 0.569f, 0.314f, 1.0f));


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    if (almira == true)
    {
        //almira front 1st half
        ourShader.use();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8 + 0.025f, -1.0 + 0.375f, 0.0f));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.375, 0.05, 2.4));

        model = Model_Modify * translateMatrix * scaleMatrix;

        color = glm::vec3(0.78f, 0.569f, 0.408f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);

        ourShader.setMat4("model", model);

        //ourShader.setVec4("color", glm::vec4(0.451f, 0.255f, 0.059f, 1.0f));


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //almira front 2nd half

        ourShader.use();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8 + 0.1875 + 0.025f, -1.0 + 0.375f, 0.0f));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.375, 0.05, 2.4));

        model = Model_Modify * translateMatrix * scaleMatrix;

        color = glm::vec3(0.749f, 0.498f, 0.247f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);

        ourShader.setMat4("model", model);
        //ourShader.setVec4("color", glm::vec4(0.749f, 0.498f, 0.247f, 1.0f));


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }
    else
    {
        // front
        ourShader.use();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8 + 0.025f, -1.0 + 0.375f, 0.0f));

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.75, 0.05, 2.4));

        model = Model_Modify * translateMatrix * scaleMatrix;

        color = glm::vec3(0.941f, 0.937f, 0.859f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);

        ourShader.setMat4("model", model);
        //ourShader.setVec4("color", glm::vec4(0.941f, 0.937f, 0.859f, 1.0f));


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }


    //almira top half

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.8 + 0.025f, -1.0 + 0.025f, 1.2f));

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.75, 0.7, 0.05));

    model = Model_Modify * translateMatrix * scaleMatrix;

    ourShader.setMat4("model", model);

    if (almira == false)
    {
        color = glm::vec3(0.822f, 0.922f, 0.875f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);
        //ourShader.setVec4("color", glm::vec4(0.822f, 0.922f, 0.875f, 1.0f));

    }
    else
    {
        color = glm::vec3(0.4f, 0.243f, 0.051f);

        ourShader.setVec3("material.ambient", color);
        ourShader.setVec3("material.diffuse", color);
        ourShader.setVec3("material.specular", color);
        ourShader.setFloat("material.shininess", 32.0f);
        //ourShader.setVec4("color", glm::vec4(0.4f, 0.243f, 0.051f, 0.922f));

    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void drawCylender(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scX, float scY, float scZ)
{
    shaderProgram.use();

    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, model, modelCentered;
    translateMatrix = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    //rotateXMatrix = glm::rotate(translateMatrix, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateXMatrix = myRotate(glm::radians(-rotX), glm::vec3(1.0, 0.0, 0.0));
    //rotateYMatrix = glm::rotate(rotateXMatrix, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateYMatrix = myRotate(glm::radians(-rotY), glm::vec3(0.0, 1.0, 0.0));
    //rotateZMatrix = glm::rotate(rotateYMatrix, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    rotateZMatrix = myRotate(glm::radians(-rotZ), glm::vec3(0.0, 0.0, 1.0));
    model = glm::scale(rotateZMatrix, glm::vec3(scX, scY, scZ));
    modelCentered = glm::translate(model, glm::vec3(-0.25, -0.25, -0.25));

    shaderProgram.setMat4("model", modelCentered);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 15, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLE_FAN, 15, GL_UNSIGNED_INT, (const void*)(15 * sizeof(unsigned int)));
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (const void*)(30 * sizeof(unsigned int)));
    glDrawElements(GL_TRIANGLE_STRIP, 90, GL_UNSIGNED_INT, (const void*)(36 * sizeof(unsigned int)));
    //glDrawElements(GL_TRIANGLE_FAN, 30, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

glm::mat4 myRotate(float angleRadians, const glm::vec3& axis) {

    glm::mat4 IdentityMatrix(1.0f);
    // Check the axis to determine the rotation matrix
    if (axis == glm::vec3(1.0f, 0.0f, 0.0f)) { // X-axis rotation
        IdentityMatrix[1][1] = cos(angleRadians);
        IdentityMatrix[1][2] = -sin(angleRadians);
        IdentityMatrix[2][1] = sin(angleRadians);
        IdentityMatrix[2][2] = cos(angleRadians);
    }
    else if (axis == glm::vec3(0.0f, 1.0f, 0.0f)) { // Y-axis rotation
        IdentityMatrix[0][0] = cos(angleRadians);
        IdentityMatrix[0][2] = sin(angleRadians);
        IdentityMatrix[2][0] = -sin(angleRadians);
        IdentityMatrix[2][2] = cos(angleRadians);
    }
    else if (axis == glm::vec3(0.0f, 0.0f, 1.0f)) { // Z-axis rotation
        IdentityMatrix[0][0] = cos(angleRadians);
        IdentityMatrix[0][1] = -sin(angleRadians);
        IdentityMatrix[1][0] = sin(angleRadians);
        IdentityMatrix[1][1] = cos(angleRadians);
    }
    return IdentityMatrix;
}


glm::mat4 perspectiveProjection(float fov, float aspect, float near, float far) {
    glm::mat4 matrix(0.0f); // Initialize all elements to 0

    float tanHalfFov = tanf(glm::radians(fov) / 2.0f);

    matrix[0][0] = 1.0f / (aspect * tanHalfFov);  // (1 / (aspect * tan(fov / 2)))
    matrix[1][1] = 1.0f / tanHalfFov;            // (1 / tan(fov / 2))
    matrix[2][2] = -(far + near) / (far - near);  // (-(far + near) / (far - near))
    matrix[2][3] = -1.0f;                         // -1
    matrix[3][2] = -(2.0f * far * near) / (far - near);

    return matrix;
}

void orthogonalProjection(float left, float right, float bottom, float top, float near, float far, float matrix[16]) {
    // Compute the orthogonal projection matrix
    float r_l = right - left;
    float t_b = top - bottom;
    float n_f = far - near;

    // Set all elements of the matrix to 0
    for (int i = 0; i < 16; ++i)
        matrix[i] = 0.0f;

    // The orthogonal projection matrix (M_orth) structure:
    matrix[0] = 2.0f / r_l;  // (2 / (right - left))
    matrix[5] = 2.0f / t_b;  // (2 / (top - bottom))
    matrix[10] = 2.0f / n_f; // (2 / (near - far))
    matrix[12] = (right + left) / r_l;   // (right + left) / (right - left)
    matrix[13] = (top + bottom) / t_b;   // (top + bottom) / (top - bottom)
    matrix[14] = (far + near) / n_f;     // (far + near) / (near - far)
    matrix[15] = 1.0f;

    // Set other elements to 0 (already default, but to be explicit)
    matrix[1] = matrix[2] = matrix[3] = 0.0f;
    matrix[4] = matrix[6] = matrix[7] = 0.0f;
    matrix[8] = matrix[9] = matrix[11] = 0.0f;
}


void generateCylinder(float radius, float height, int segments, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    float angleStep = 2.0f * M_PI / segments;

    // Generate vertices and normals
    for (int i = 0; i <= segments; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Bottom circle vertex
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);

        // Bottom circle normal (pointing down the y-axis)
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);

        // Top circle vertex
        vertices.push_back(x);
        vertices.push_back(height);
        vertices.push_back(z);

        // Top circle normal (pointing up the y-axis)
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);

        // Side vertex (bottom)
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);

        // Side normal
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);

        // Side vertex (top)
        vertices.push_back(x);
        vertices.push_back(height);
        vertices.push_back(z);

        // Side normal
        vertices.push_back(x);
        vertices.push_back(0.0f);
        vertices.push_back(z);
    }

    // Generate indices for the sides
    for (int i = 0; i < segments; ++i) {
        int bottom1 = 4 * i + 2;
        int top1 = bottom1 + 1;
        int bottom2 = 4 * (i + 1) + 2;
        int top2 = bottom2 + 1;

        // Side triangles
        indices.push_back(bottom1);
        indices.push_back(top1);
        indices.push_back(bottom2);

        indices.push_back(top1);
        indices.push_back(top2);
        indices.push_back(bottom2);
    }

    // Center vertex for the top circle
    int topCenterIndex = vertices.size() / 6;
    vertices.push_back(0.0f);
    vertices.push_back(height);
    vertices.push_back(0.0f);

    // Top center normal (pointing up the y-axis)
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    // Generate indices for the top circle
    for (int i = 0; i < segments; ++i) {
        int top1 = 4 * i + 1;
        int top2 = 4 * ((i + 1) % segments) + 1;

        indices.push_back(topCenterIndex);
        indices.push_back(top1);
        indices.push_back(top2);
    }

    // Center vertex for the bottom circle
    int bottomCenterIndex = vertices.size() / 6;
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Bottom center normal (pointing down the y-axis)
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    // Generate indices for the bottom circle
    for (int i = 0; i < segments; ++i) {
        int bottom1 = 4 * i;
        int bottom2 = 4 * ((i + 1) % segments);

        indices.push_back(bottomCenterIndex);
        indices.push_back(bottom2);
        indices.push_back(bottom1);
    }
}


void generateCone(float radius, float height, int segments, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    float angleStep = 2.0f * M_PI / segments;

    // Generate vertices and normals for the base circle
    for (int i = 0; i < segments; ++i) {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Base circle vertex
        vertices.push_back(x);
        vertices.push_back(0.0f); // y = 0 for the base
        vertices.push_back(z);

        // Base circle normal (pointing downwards)
        vertices.push_back(0.0f);
        vertices.push_back(-1.0f);
        vertices.push_back(0.0f);
    }

    // Add the tip vertex of the cone
    int tipIndex = vertices.size() / 6; // Index of the tip
    vertices.push_back(0.0f);
    vertices.push_back(height); // Tip is at y = height
    vertices.push_back(0.0f);

    // Tip normal (pointing upwards)
    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    // Add the center vertex of the base circle
    int baseCenterIndex = vertices.size() / 6; // Index of the base center
    vertices.push_back(0.0f);
    vertices.push_back(0.0f); // Center is at y = 0
    vertices.push_back(0.0f);

    // Base center normal (pointing downwards)
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);

    // Generate indices and normals for the sides
    for (int i = 0; i < segments; ++i) {
        int nextIndex = (i + 1) % segments;

        // Side triangle
        indices.push_back(i);
        indices.push_back(nextIndex);
        indices.push_back(tipIndex);

        // Calculate normals for the side vertices
        glm::vec3 v1(vertices[6 * i], vertices[6 * i + 1], vertices[6 * i + 2]);
        glm::vec3 v2(vertices[6 * nextIndex], vertices[6 * nextIndex + 1], vertices[6 * nextIndex + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, glm::vec3(0.0f, height, 0.0f) - v1));

        // Add normals for the side vertices
        vertices.push_back(v1.x);
        vertices.push_back(v1.y);
        vertices.push_back(v1.z);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);

        vertices.push_back(v2.x);
        vertices.push_back(v2.y);
        vertices.push_back(v2.z);
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
    }

    // Generate indices for the base circle
    for (int i = 0; i < segments; ++i) {
        int nextIndex = (i + 1) % segments;

        indices.push_back(baseCenterIndex);
        indices.push_back(i);
        indices.push_back(nextIndex);
    }
}




void generateSphere(float radius, int sectorCount, int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal, lenginv is the inverse of the radius


    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    // generate indices
    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void calculateVertexNormals(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, std::vector<float>& normals) {
    // Create a container for the accumulated normals
    std::vector<glm::vec3> tempNormals(vertices.size() / 3, glm::vec3(0.0f));

    // Loop through the indices to compute surface normals
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int index0 = indices[i];
        unsigned int index1 = indices[i + 1];
        unsigned int index2 = indices[i + 2];

        glm::vec3 v0(vertices[index0 * 3], vertices[index0 * 3 + 1], vertices[index0 * 3 + 2]);
        glm::vec3 v1(vertices[index1 * 3], vertices[index1 * 3 + 1], vertices[index1 * 3 + 2]);
        glm::vec3 v2(vertices[index2 * 3], vertices[index2 * 3 + 1], vertices[index2 * 3 + 2]);

        glm::vec3 normal = computeSurfaceNormal(v0, v1, v2);

        // Accumulate normals for each vertex of the triangle
        tempNormals[index0] += normal;
        tempNormals[index1] += normal;
        tempNormals[index2] += normal;
    }

    // Normalize the accumulated normals and store them
    normals.resize(vertices.size());
    for (size_t i = 0; i < tempNormals.size(); ++i) {
        glm::vec3 normalized = glm::normalize(tempNormals[i]);
        normals[i * 3] = normalized.x;
        normals[i * 3 + 1] = normalized.y;
        normals[i * 3 + 2] = normalized.z;
    }
}
glm::vec3 computeSurfaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    glm::vec3 edge1 = b - a;
    glm::vec3 edge2 = c - a;
    return glm::normalize(glm::cross(edge1, edge2));
}
