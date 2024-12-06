/*

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"

#include <iostream>
#include <cmath>

#include "sphere.h"
#include "cone.h"


//#define PI 3.14159265358979323846

using namespace std;




// Function to calculate the perspective projection matrix
void perspectiveProjection(float fov, float aspect, float near, float far, float matrix[16]) {
    float tanHalfFov = tanf(fov / 2.0f);

    // Initialize all elements to zero
    for (int i = 0; i < 16; ++i)
        matrix[i] = 0.0f;

    matrix[0] = 1.0f / (aspect * tanHalfFov);  // (1 / (aspect * tan(fov / 2)))
    matrix[5] = 1.0f / tanHalfFov;  // (1 / tan(fov / 2))
    matrix[10] = -(far + near) / (far - near);  // (-(far + near) / (far - near))
    matrix[11] = -1.0f;  // -1
    matrix[14] = -(2 * far * near) / (far - near);  // (-(2 * far * near) / (far - near))
    matrix[15] = 0.0f;

    // Set the rest of the matrix elements to zero (by default, they are already zero)
    matrix[1] = matrix[2] = matrix[3] = 0.0f;
    matrix[4] = matrix[6] = matrix[7] = 0.0f;
    matrix[8] = matrix[9] = matrix[12] = matrix[13] = matrix[15] = 0.0f;
}
/*
  1/(aspect * tan(fov / 2))  0                         0                                   0
  0                          1/tan(fov / 2)            0                                   0
  0                          0                         -(far + near) / (far - near)        -1
  0                          0                         -(2 * far * near) / (far - near)     0
*/

/*

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




void drawTrainBogie(unsigned int& cubeVAO, unsigned int& cylinderVAO, Shader& lightingShader);
void drawRailTrack(unsigned int& cubeVAO, Shader& lightingShader);
std::vector<float> generateCirclePoints(float centerX, float centerY, float centerZ, float radius, int numSegments);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawTableChair(unsigned int VAO, Shader ourShader, glm::mat4 group_Tran);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void classroom(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void drawBoard(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void drawFan(Shader ourShader, glm::mat4 identityMatrix, unsigned int VAO, unsigned int VAO2, glm::mat4 Model_Center, Shader shaderProgram, glm::mat4 altogether);
void drawCylender(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scX, float scY, float scZ, glm::mat4 modelcenter);
glm::vec3 computeSurfaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
void calculateVertexNormals(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, std::vector<float>& normals);
void drawCarPart(Shader& lightingShader, unsigned int VAO, unsigned int VAO2, glm::mat4 identityMatrix, glm::mat4 modelCenter, glm::mat4 altogether);
void drawCarpart2(Shader& lightingShader, unsigned int VAO, unsigned int VAO2, glm::mat4 identityMatrix, glm::mat4 modelCenter, glm::mat4 altogether);
std::vector<float> generateSpherePoints(float centerX, float centerY, float centerZ, float radius, int numSegments, int heightSegments);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

int fanon = 0;

// camera
Camera camera(glm::vec3(-5.0f, 0.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 0.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


//---------lights----------
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(2.0f,  0.0f,  0.0f),
    glm::vec3(-2.0f,  0.0f,  0.0f)
};


PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.052f, //k_q
    2       // light number
);




// light settings
bool onOffToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool point1Toggle = true;
bool point2Toggle = true;
bool directionToggle = true;
bool spotToggle = true;




// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
int glob = 0;
bool moveCar = false;
float car_wheel_anglee = 0.0f;




int main(){

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs"); //lightshader

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    float cube_vertices[] = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,

    0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    0.5f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
    0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
    0.0f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
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

    //float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f, radius = 1.0f;
    //int numSegments = 36;      // Smoothness of the circles
    //int heightSegments = 18;   // Vertical segments (smoothness of sphere)

    //// Generate the sphere points
    //std::vector<float> spherePoints = generateSpherePoints(centerX, centerY, centerZ, radius, numSegments, heightSegments);

    //// Print the sphere points
    //for (size_t i = 0; i < spherePoints.size(); i += 3) {
    //    std::cout << "Point " << (i / 3) + 1 << ": ("
    //        << spherePoints[i] << ", "
    //        << spherePoints[i + 1] << ", "
    //        << spherePoints[i + 2] << ")\n";
    //}


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


    unsigned int VBO2, VAO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, combinedVertices.size() * sizeof(float), combinedVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder_indices.size() * sizeof(unsigned int), cylinder_indices.data(), GL_STATIC_DRAW);

    // position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  // Normal
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);




    //------------light-----------------------
    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Sphere sphere = Sphere(
        10.0f,                        // radius
        18,                          // sector count
        6,                           // stack count
        glm::vec3(0.2f, 0.6f, 0.9f), // ambient (soft sky blue)
        glm::vec3(0.3f, 0.7f, 0.2f), // diffuse (brighter sky blue)
        glm::vec3(0.9f, 0.9f, 1.0f), // specular (almost white for sunlight reflection)
        32.0f                        // shininess
    );


    Cone cone = Cone();




    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    int xx = 0;
    //cout << "Use basic camera functions?(0/1)" << endl;
    //cin >> xx;
    glob = xx;
    float r = 0.0f;
    float speed = 0.0;
    float moving = 0.0;


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
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);


        lightingShader.setVec3("diectionalLight.direction", 0.0f, 10.0f, 0.0f);
        lightingShader.setVec3("diectionalLight.ambient", .2, .2, .2);
        lightingShader.setVec3("diectionalLight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("diectionalLight.specular", 1.0f, 1.0f, 1.0f);



        if (directionToggle == true)
        {
            lightingShader.setBool("dlighton", true);
        }
        else
        {
            lightingShader.setBool("dlighton", false);
        }



        //lightingShader.setVec3("spotlight.position", -0.5, 1, -0.5);
        lightingShader.setVec3("spotlight.position", -2.5, 1, -1.5);
        lightingShader.setVec3("spotlight.direction", 0, -1, 0);
        lightingShader.setVec3("spotlight.ambient", .2, .2, .2);
        lightingShader.setVec3("spotlight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotlight.k_c", 1.0f);
        lightingShader.setFloat("spotlight.k_l", 0.09);
        lightingShader.setFloat("spotlight.k_q", 0.032);
        lightingShader.setFloat("cos_theta", glm::cos(glm::radians(5.5f)));

        if (spotToggle == true)
        {
            lightingShader.setBool("spotlighton", true);
        }
        else
            lightingShader.setBool("spotlighton", false);

        if (ambientToggle)
            lightingShader.setInt("amb", 0);
        else
            lightingShader.setInt("amb", 1);
        if (diffuseToggle)
            lightingShader.setInt("diff", 0);
        else
            lightingShader.setInt("diff", 1);
        if (specularToggle)
            lightingShader.setInt("spec", 0);
        else
            lightingShader.setInt("spec", 1);




        lightingShader.use();



        float cameraZoom = camera.Zoom;  // Example camera zoom (adjust as needed)
        float SCR_WIDTH = 800.0f;  // Screen width
        float SCR_HEIGHT = 600.0f;  // Screen height
        float near = 0.1f;  // Near clipping plane
        float far = 100.0f;  // Far clipping plane

        // Call the perspective function and calculate the projection matrix

            // Field of view (FOV) in radians
        float fov = glm::radians(cameraZoom);

        // Aspect ratio (width / height)
        float aspect = SCR_WIDTH / SCR_HEIGHT;

        // Create a matrix to store the result
        float projectionMatrix[16];

        // Calculate the perspective matrix manually
        perspectiveProjection(fov, aspect, near, far, projectionMatrix);

        // Get the uniform location for the 'projection' matrix in your shader
        GLint projectionLoc = glGetUniformLocation(lightingShader.ID, "projection");

        // Send the orthogonal projection matrix to the shader
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projectionMatrix);

        // pass projection matrix to shader (note that in this case it could change every frame)--default

       // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        //lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view;

        if (xx == 0)
        {
            view = camera.GetViewMatrix();
        }
        else
        {
            view = basic_camera.createViewMatrix();
        }
        lightingShader.setMat4("view", view);





        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 modelForSphere = glm::mat4(1.0f);
        glm::mat4 modelForCone = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotationMatrix,rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, modelCenter, altogether;
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
        //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 1, 1));
        //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-15.0f, -15.0f, -15.0f));
        scaleMatrix = glm::translate(identityMatrix, glm::vec3(15.0f, 15.0f, 15.0f));
        modelForSphere = translateMatrix * scaleMatrix ;
        sphere.drawSphere(lightingShader, modelForSphere);

        //modelForCone = identityMatrix;
        //cone.drawCone(lightingShader, modelForCone);


        float rotateAngle = -90;
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        rotateAngle = 90;
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = rotateYMatrix * rotateZMatrix;

        if (moveCar)
            altogether = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, moving));
        else
            altogether = identityMatrix;

        drawCarPart(lightingShader, VAO, VAO2, identityMatrix, model, altogether);
        model = identityMatrix;
        drawCarpart2(lightingShader, VAO, VAO2, identityMatrix, model, altogether);
        

        ourShader.use();
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, -0.06f, -3.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01, 3.0, 15));
        rotateAngle = 90;
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateZMatrix *scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.231f, 0.231f, 0.204f, 0.91f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        drawRailTrack(VAO, lightingShader);
        //drawTrainBogie(VAO, VAO2, lightingShader);


        //draw Fan
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(speed), glm::vec3(0.0f, 0.0f, 1.0f));
        model = rotationMatrix;
        drawFan(ourShader, identityMatrix, VAO, VAO2, model, lightingShader,identityMatrix);

        if (moveCar)
        {
            moving -= 0.001;
            car_wheel_anglee += 1;
            if (moving < -3.5)
            {
                moving = 4;
            }
        }

        if (fanon)
        {
            speed -= 1;
        }




        // activate shader
        ourShader.use();

        GLint projectionLoc1 = glGetUniformLocation(ourShader.ID, "projection");

        // Send the orthogonal projection matrix to the shader
        glUniformMatrix4fv(projectionLoc1, 1, GL_FALSE, projectionMatrix);


        if (xx == 0)
        {
            view = camera.GetViewMatrix();
        }
        else
        {
            view = basic_camera.createViewMatrix();
        }

        ourShader.setMat4("view", view);


        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 2; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.4f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);




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
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
void drawTrainBogie(unsigned int& cubeVAO, unsigned int& cylinderVAO, Shader& lightingShader) {
    // Base model matrix
    glm::mat4 baseModel = glm::mat4(1.0f);

    // Bogie body color
    float bodyR = 0.2f, bodyG = 0.2f, bodyB = 0.8f;

    // Wheel color
    float wheelR = 0.1f, wheelG = 0.1f, wheelB = 0.1f;

    // Draw bogie body
    glm::mat4 bodyModel = glm::translate(baseModel, glm::vec3(0.0f, 0.0f, 0.0f));
    bodyModel = glm::scale(bodyModel, glm::vec3(1.0f, 0.2f, 2.0f)); // Main bogie body dimensions
    drawCube(cubeVAO, lightingShader, bodyModel, bodyR, bodyG, bodyB);

    // Draw wheels
    float wheelRadius = 0.2f, wheelThickness = 0.1f;
    float wheelPositions[4][3] = {
        {-0.5f, -0.2f, 0.8f}, // Front left
        {0.5f, -0.2f, 0.8f},  // Front right
        {-0.5f, -0.2f, -0.8f}, // Back left
        {0.5f, -0.2f, -0.8f}  // Back right
    };

    for (int i = 0; i < 4; i++) {
        glm::mat4 wheelModel = glm::translate(baseModel, glm::vec3(wheelPositions[i][0], wheelPositions[i][1], wheelPositions[i][2]));
        wheelModel = glm::rotate(wheelModel, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate to align with the ground
        drawCylender(
            lightingShader,
            cylinderVAO,
            baseModel,
            wheelPositions[i][0],
            wheelPositions[i][1],
            wheelPositions[i][2],
            90.0f, // Rotate around X-axis
            0.0f,
            0.0f,
            wheelRadius,
            wheelRadius,
            wheelThickness,
            baseModel
        );
    }
}

void drawRailTrack(unsigned int& cubeVAO, Shader& lightingShader) {
    // Base model matrix
    glm::mat4 baseModel = glm::mat4(1.0f);

    // Rail colors
    float railR = 0.6f, railG = 0.6f, railB = 0.6f;

    // Sleeper colors
    float sleeperR = 0.4f, sleeperG = 0.2f, sleeperB = 0.1f;

    // Draw left rail
    glm::mat4 leftRailModel = glm::translate(baseModel, glm::vec3(-0.2f, 0.0f, -2.5f));
    leftRailModel = glm::scale(leftRailModel, glm::vec3(0.05f, 0.05f, 12.0f)); // Scale to make it long and thin
    drawCube(cubeVAO, lightingShader, leftRailModel, railR, railG, railB);

    // Draw right rail
    glm::mat4 rightRailModel = glm::translate(baseModel, glm::vec3(0.2f, 0.0f, -2.5f));
    rightRailModel = glm::scale(rightRailModel, glm::vec3(0.05f, 0.05f, 12.0f)); // Scale to make it long and thin
    drawCube(cubeVAO, lightingShader, rightRailModel, railR, railG, railB);

    // Draw sleepers (ties)
    for (float z = -2.5f; z <= 2.5f; z += 0.3f) { // Adjust spacing by modifying the step size
        glm::mat4 sleeperModel = glm::translate(baseModel, glm::vec3(-0.175f, -0.05f, z));
        sleeperModel = glm::scale(sleeperModel, glm::vec3(0.75f, 0.05f, 0.1f)); // Scale to make it short and wide
        drawCube(cubeVAO, lightingShader, sleeperModel, sleeperR, sleeperG, sleeperB);
    }
}


void drawCarPart(Shader& lightingShader, unsigned int VAO, unsigned int VAO2, glm::mat4 identityMatrix, glm::mat4 modelCenter, glm::mat4 altogether)
{
    glm::mat4 translateMatrix, rotationMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    //a bottom lower
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, 1));
    model = altogether * modelCenter* scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //a bottom-middle lower
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, .5));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, 0.125));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //a middle lower
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.25, 0));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //a top-middle lower 
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, .5));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.375, 0.125));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end


    //a top lower
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .35, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.5, 0));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.025, 0));
    model = altogether * translateMatrix * modelCenter;

    drawCylender(lightingShader, VAO2, identityMatrix, 0, 0.185, 0.025, 90, 0, 0, .075, .15, .0625, model);
    drawCylender(lightingShader, VAO2, identityMatrix, 0, 0.185, 0.4, 90, 0, 0, .075, .15, .0625, model);
    drawCylender(lightingShader, VAO2, identityMatrix, 0, 0.435, 0.025, 90, 0, 0, .075, .15, .0625, model);
    drawCylender(lightingShader, VAO2, identityMatrix, 0, 0.435, 0.4, 90, 0, 0, .075, .15, .0625, model);
    drawCylender(lightingShader, VAO2, identityMatrix, 0, 0.185, 0.025, 90, 0, 0, 0.01, 0.75, 0.01, model);
    drawCylender(lightingShader, VAO2, identityMatrix, 0, 0.435, 0.025, 90, 0, 0, 0.01, 0.75, 0.01, model);

    //a lower upper
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, 1.35, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0, 0));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //a left back
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0, 0));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //a left middle
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.25, 0));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //a left top
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.5, 0));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end




    //a right back
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0, .5));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //a right middle
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.25, .5));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //right top 
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.5, .5));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end


    //a back lower to upper
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, 0.01, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0, 0));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end

    //a front lower to upper
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, 0.01, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0.675, 0));
    model = altogether * modelCenter * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //simple cube end
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, 0.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawBoard(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 0.0f, float g = 0.0f, float b = 0.0f)
{
    lightingShader.use();

    // Set the material properties for a blackboard (black color)
    lightingShader.setVec3("material.ambient", glm::vec3(0.3, 0.2, 0.3)); // Set ambient to black (0, 0, 0)
    lightingShader.setVec3("material.diffuse", glm::vec3(0.3, 0.3, 0.3)); // Set diffuse to black
    lightingShader.setVec3("material.specular", glm::vec3(0.4, 0.4f, 0.4f)); // Set specular to black (no shine)
    lightingShader.setFloat("material.shininess", 1.0f); // Keep the shininess moderate
    lightingShader.setVec3("material.emission", glm::vec3(0.0f, 0.0f, 0.0f)); // No emission (blackboard is not glowing)

    // Set the model matrix to position/rotate the cube as needed
    lightingShader.setMat4("model", model);

    // Bind the cube's VAO and draw it
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawWood(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 0.0f, float g = 0.0f, float b = 0.0f)
{
    lightingShader.use();

    // Set the material properties for a blackboard (black color)
    lightingShader.setVec3("material.ambient", glm::vec3(0.4 * r, 0.4 * g, 0.4 * b)); // Set ambient to black (0, 0, 0)
    lightingShader.setVec3("material.diffuse", glm::vec3(r, 0.5 * g, b)); // Set diffuse to black
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.7f, 0.9f)); // Set specular to black (no shine)
    lightingShader.setFloat("material.shininess", 3.0f); // Keep the shininess moderate
    lightingShader.setVec3("material.emission", glm::vec3(0.0f, 0.0f, 0.0f)); // No emission (blackboard is not glowing)

    // Set the model matrix to position/rotate the cube as needed
    lightingShader.setMat4("model", model);

    // Bind the cube's VAO and draw it
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawWall(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 0.0f, float g = 0.0f, float b = 0.0f)
{
    lightingShader.use();

    // Set the material properties for a blackboard (black color)
    lightingShader.setVec3("material.ambient", glm::vec3(r * 0.7, 0.7 * g, 0.7 * b)); // Set ambient to black (0, 0, 0)
    lightingShader.setVec3("material.diffuse", glm::vec3(r * 0.8, 0.9, b)); // Set diffuse to black
    lightingShader.setVec3("material.specular", glm::vec3(r, g, b)); // Set specular to black (no shine)
    lightingShader.setFloat("material.shininess", 1.0f); // Keep the shininess moderate
    lightingShader.setVec3("material.emission", glm::vec3(0.3f, 0.3f, 0.3f)); // No emission (blackboard is not glowing)

    // Set the model matrix to position/rotate the cube as needed
    lightingShader.setMat4("model", model);

    // Bind the cube's VAO and draw it
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void classroom(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    //blackboard

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5f, 0.1f, 3.9f));  // Move wall to the back, adjust the z-value as per room depth

    // Scale the wall to be tall and wide enough to cover the width of the room
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.5f, 2.2f, 0.05f));  // Adjust width, height, and thickness

    // Combine transformations
    model = alTogether * translateMatrix * scaleMatrix;

    drawBoard(cubeVAO, lightingShader, model, (0.0f, 0.0f, 0.0f));


    //dias
     // lightingShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.9f, 0.0f, 2.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.8f, -1.8f, 1.0f));
    model = alTogether * translateMatrix * scaleMatrix;
    drawWood(cubeVAO, lightingShader, model, (0.76f, 0.6f, 0.42f));



    //wall 4 left

       // Calculate the translation matrix to move the wall to the left side of the room
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, -1.0f, 4.0f));  // Adjust this value based on room dimensions

    // Scale the wall to be tall and wide enough to fill the side of the room
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 5.0f, -23.0f));  // Adjust thickness, height, and width appropriately

    // Combine transformations
    model = alTogether * translateMatrix * scaleMatrix;
    //1.0f, 0.5f, 0.5f
    drawWood(cubeVAO, lightingShader, model, (1.0f, 0.5f, 0.5f));


    //backwall
    // 
    // Calculate the translation matrix to move the wall to the back side of the room
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, -1.0f, -7.5f));  // Move wall to the back, adjust the z-value as per room depth

    // Scale the wall to be tall and wide enough to cover the width of the room
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(18.0f, 5.0f, 0.1f));  // Adjust width, height, and thickness

    // Combine transformations
    model = alTogether * translateMatrix * scaleMatrix;
    drawWall(cubeVAO, lightingShader, model, (1.0f, 1.0f, 0.9f));

    //frontwall
    // 
    // Calculate the translation matrix to move the wall to the back side of the room
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, -1.0f, 4.0f));  // Move wall to the back, adjust the z-value as per room depth

    // Scale the wall to be tall and wide enough to cover the width of the room
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(18.0f, 5.0f, 0.1f));  // Adjust width, height, and thickness

    // Combine transformations
    model = alTogether * translateMatrix * scaleMatrix;
    drawWall(cubeVAO, lightingShader, model, (1.0f, 0.9f, 0.9f));


    //wall right

    // Calculate the translation matrix to move the wall to the left side of the room
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0f, -1.0f, 4.0f));  // Adjust this value based on room dimensions

    // Scale the wall to be tall and wide enough to fill the side of the room
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 5.0f, -23.0f));  // Adjust thickness, height, and width appropriately

    // Combine transformations
    model = alTogether * translateMatrix * scaleMatrix;
    drawWood(cubeVAO, lightingShader, model, (1.0f, 0.5f, 0.5f));


    //floor

    // Calculate the translation matrix to move the wall to the left side of the room
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-7.0f, -1.0f, 4.0f));  // Adjust this value based on room dimensions

    // Scale the wall to be tall and wide enough to fill the side of the room
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(18.0f, 0.1f, -23.0f));  // Adjust thickness, height, and width appropriately

    // Combine transformations
    model = alTogether * translateMatrix * scaleMatrix;
    drawWall(cubeVAO, lightingShader, model, (0.53f, 0.81f, 0.92f));

    //table-chair



    translateMatrix = alTogether * identityMatrix;
    drawTableChair(cubeVAO, lightingShader, translateMatrix);
    // Outer loop for rows
    for (int j = 0; j < 4; j++) // Adjusted to loop for 4 rows
    {
        for (int i = 0; i < 4; i++) // Loop for 4 columns
        {
            // Calculate the translation for each object based on row and column
            glm::mat4 translateMatrix = alTogether *
                glm::translate(identityMatrix, glm::vec3(-2.0f * i, 0.0f, -2.0f * j));

            // Draw each object
            drawTableChair(cubeVAO, lightingShader, translateMatrix);
        }
    }

}



void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{


    float baseHeight = 0.3;
    float width = 1;
    float length = 2;
    float pillowWidth = 0.3;
    float pillowLength = 0.15;
    float blanketWidth = 0.8;
    float blanketLength = 0.7;
    float headHeight = 0.6;

    //base
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

    //foam
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight, 0));
    scale = glm::scale(model, glm::vec3(width, 0.06, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);

    //pillow 1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3((width / 2) - (0.1 + pillowWidth / 2), baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0);

    //pillow 2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3((-width / 2) + (0.1 + pillowWidth / 2), baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0);

    //blanket
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight + 1 * 0.06, -(length / 2 - 0.025) + blanketLength / 2));
    scale = glm::scale(model, glm::vec3(blanketWidth, 0.015, blanketLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.541, 0.169, 0.886);

    //head
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0, (length / 2 - 0.02 / 2) + 0.02));
    scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

}



void drawTableChair1(unsigned int VAO, Shader ourShader, glm::mat4 group_Tran)
{


    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //table body
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2f, 2.0f));
    model = group_Tran * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.33f, 0.19f, 0.09f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //table leg1
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.8f, 0.2f));
    model = group_Tran * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));  // Typical wood color


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //table leg2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.8f, 0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //table leg3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.8f, 0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //table leg4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.8f, 0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair headboard
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.3f, -0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.5f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair seat
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.3f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, -1.0f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));
    ourShader.setVec4("color", glm::vec4(0.532f, 0.42f, 0.294f, 1.0f));



    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //chair leg1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.3f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.3f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));



    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // chair leg2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.3f, -0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.3f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // chair leg3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.65f, -0.3f, -0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.3f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // chair leg4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.65f, -0.3f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.3f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void drawTableChair(unsigned int VAO, Shader lightingShader, glm::mat4 group_Tran)
{


    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //table body
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.2f, 2.0f));
    model = group_Tran * scaleMatrix;
    //ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.33f, 0.19f, 0.09f, 1.0f));
    drawWood(VAO, lightingShader, model, 0.33f, 0.19f, 0.09f);



    //table leg1
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.8f, 0.2f));
    model = group_Tran * scaleMatrix;
    //ourShader.setMat4("model", model);
   // ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));  // Typical wood color
    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);



    //table leg2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.8f, 0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));
    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);



    //table leg3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.8f, 0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));
    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);



    //table leg4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.8f, 0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    // ourShader.setMat4("model", model);
    // ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));
    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);



    //chair headboard
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.3f, -0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.5f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    // ourShader.setMat4("model", model);
    // ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));
    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);



    //chair seat
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.3f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, -1.0f));
    model = group_Tran * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));
   // ourShader.setVec4("color", glm::vec4(0.532f, 0.42f, 0.294f, 1.0f));
    drawWood(VAO, lightingShader, model, 0.532f, 0.42f, 0.294f);




    //chair leg1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.3f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.3f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", model);
    //ourShader.setVec4("color", glm::vec4(0.76f, 0.6f, 0.42f, 1.0f));

    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);




    // chair leg2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.3f, -0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.3f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);

    // chair leg3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.65f, -0.3f, -0.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.3f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);

    // chair leg4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.65f, -0.3f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -1.3f, -0.2f));
    model = group_Tran * translateMatrix * scaleMatrix;
    drawWall(VAO, lightingShader, model, 0.76f, 0.6f, 0.42f);
}










// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{

    /*


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glob == 0)
    {
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            // Move the camera high above the scene
            camera.Position = glm::vec3(-3.0f, 8.0f, 8.0f); // Eye position (above the scene)

            // Look straight down
            camera.Front = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f)); // Front direction

            // Reset the Up vector to maintain orientation
            //camera.Up = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera.ProcessKeyboard(UP, deltaTime); // Fly upwards
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            camera.ProcessKeyboard(DOWN, deltaTime); // Fly downwards
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(YAW_L, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            camera.ProcessKeyboard(YAW_R, deltaTime);
        }
        // Pitch (vertical rotation)
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            camera.ProcessKeyboard(PITCH_UP, deltaTime); // Pitch up
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            camera.ProcessKeyboard(PITCH_DOWN, deltaTime); // Pitch down
        }

        // Roll (rotation around the forward axis)
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            camera.ProcessKeyboard(ROLL_R, deltaTime); // Roll right
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            camera.ProcessKeyboard(ROLL_L, deltaTime); // Roll left
        }
        //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        //{
        //    static float angle = 0.0f; // Static to retain value between frames
        //    angle += 45.0f * deltaTime; // Increment angle over time for smooth rotation

        //    // Radius of the circular path
        //    float radius = 10.0f; // Adjust this based on your scene size

        //    // Fixed look-at point (set to the center of the scene or desired target)
        //    glm::vec3 lookAt = camera.Position + camera.Front;

        //    // Compute new camera position along the circular path
        //    float eyeX = lookAt.x + radius * cos(glm::radians(angle)); // X position
        //    float eyeZ = lookAt.z + radius * sin(glm::radians(angle)); // Z position

        //    // Update camera position
        //    camera.Position = glm::vec3(eyeX, camera.Position.y, eyeZ); // Keep Y constant for horizontal rotation

        //    // Recalculate the Front vector to always point toward the look-at point
        //    camera.Front = glm::normalize(lookAt - camera.Position);

        //    // Reset the Up vector to maintain orientation
        //    camera.Up = glm::vec3(0.0f, 1.0f, 0.0f);
        //}






    }

    else
    {
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        {
            static float angle = 0.0f; // Static to retain value between frames
            angle += 45.0f * deltaTime; // Increase the angle based on time

            // Radius of the circular path
            float radius = 10.0f; // Adjust the radius as needed for the scene

            // Compute new camera position around the look-at point
            eyeX = lookAtX + radius * cos(glm::radians(angle));
            eyeZ = lookAtZ + radius * sin(glm::radians(angle));
            // Keep the eyeY constant for horizontal rotation
            // eyeY could also change for vertical rotations if desired.

            // Update the camera
            basic_camera.changeEye(eyeX, eyeY, eyeZ);
            basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
        }

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            // Set predefined bird's eye position
            eyeX = 0.0f;      // Centered horizontally
            eyeY = 8.0f;     // Elevated for bird's eye view
            eyeZ = -18.0f;      // Centered along Z-axis

            // Set predefined look-at point (center of the scene)
            lookAtX = 0.0f;
            lookAtY = 0.0f;
            lookAtZ = 0.0f;

            basic_camera.changeEye(eyeX, eyeY, eyeZ);
            basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);


        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        {
            eyeX += 2.5 * deltaTime;
            basic_camera.changeEye(eyeX, eyeY, eyeZ);
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            eyeX -= 2.5 * deltaTime;
            basic_camera.changeEye(eyeX, eyeY, eyeZ);
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            eyeZ += 2.5 * deltaTime;
            basic_camera.changeEye(eyeX, eyeY, eyeZ);
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            eyeZ -= 2.5 * deltaTime;
            basic_camera.changeEye(eyeX, eyeY, eyeZ);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            eyeY += 2.5 * deltaTime;
            basic_camera.changeEye(eyeX, eyeY, eyeZ);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            eyeY -= 2.5 * deltaTime;
            basic_camera.changeEye(eyeX, eyeY, eyeZ);
        }



        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            lookAtX += 2.5 * deltaTime;
            basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            lookAtX -= 2.5 * deltaTime;
            basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            lookAtY += 2.5 * deltaTime;
            basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);


        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            lookAtY -= 2.5 * deltaTime;
            basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
        }


        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        {
            lookAtZ += 2.5 * deltaTime;
            basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        {
            lookAtZ -= 2.5 * deltaTime;
            basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
        }
        //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        //{
        //    basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
        //}
        //if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        //{
        //    basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
        //}
        //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //{
        //    basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
        //}



    }



    //if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    //{
    //    if (rotateAxis_X) rotateAngle_X -= 1;
    //    else if (rotateAxis_Y) rotateAngle_Y -= 1;
    //    else rotateAngle_Z -= 1;
    //}

    //if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.01;




    //if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    //{
    //    rotateAngle_X += 1;
    //    rotateAxis_X = 1.0;
    //    rotateAxis_Y = 0.0;
    //    rotateAxis_Z = 0.0;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    //{
    //    rotateAngle_Y += 1;
    //    rotateAxis_X = 0.0;
    //    rotateAxis_Y = 1.0;
    //    rotateAxis_Z = 0.0;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    //{
    //    rotateAngle_Z += 1;
    //    rotateAxis_X = 0.0;
    //    rotateAxis_Y = 0.0;
    //    rotateAxis_Z = 1.0;
    //}






    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (fanon)
            fanon = false;
        else
            fanon = true;
    }

    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    {
        if (moveCar)
            moveCar = false;
        else
            moveCar = true;
    }


    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        ///Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
        if (directionToggle == true)
        {

            // lightingShader.setBool("dlighton", true);

            directionToggle = false;
        }
        else
        {
            //lightingShader.setBool("dlighton", false);

            directionToggle = true;

        }

        /*  pointlight1.turnOff();
        pointlight2.turnOff();
        */

        //pointlight3.turnOff();
        //pointlight4.turnOff();
   /* }
   
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        
        pointlight1.turnOn();
        // pointlight2.turnOn();



    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {

        pointlight1.turnOff();
        // pointlight2.turnOff();

    }

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        
        pointlight2.turnOn();

    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
    
        pointlight2.turnOff();



    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        if (spotToggle == true)
            spotToggle = false;
        else
            spotToggle = true;
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        if (ambientToggle)
        {

            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            ambientToggle = !ambientToggle;

        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }

    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        if (diffuseToggle)
        {

            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            //pointlight3.turnDiffuseOff();
            //pointlight4.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {

            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            //pointlight3.turnDiffuseOn();
            //pointlight4.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        if (specularToggle)
        {

            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            //pointlight3.turnSpecularOff();
            //pointlight4.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {

            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            //pointlight3.turnSpecularOn();
            //pointlight4.turnSpecularOn();
            specularToggle = !specularToggle;
        }
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


// glfw: whenever the mouse moves, this callback is called
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

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void drawCylender(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float scX, float scY, float scZ, glm::mat4 modelcenter)
{
    shaderProgram.use();

    glm::mat4 translateMatrix, rotationMatrix,rotateXMatrix, rotateYMatrix, rotateZMatrix,scaleMatrix, model, rotating_in_own;
    translateMatrix = glm::translate(parentTrans, glm::vec3(posX, posY, posZ));
    rotateXMatrix = glm::rotate(parentTrans, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    //rotateYMatrix = glm::rotate(parentTrans, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    //rotateZMatrix = glm::rotate(parentTrans, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(parentTrans, glm::vec3(scX, scY, scZ));
    rotationMatrix = rotateXMatrix;

    if (moveCar)
    {
        rotating_in_own = glm::rotate(parentTrans, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
        model = modelcenter * translateMatrix  * rotationMatrix * rotating_in_own * scaleMatrix;
    }
    else
        model = modelcenter * translateMatrix * rotationMatrix * scaleMatrix;


    shaderProgram.setMat4("model", model);

    // Set material properties
    shaderProgram.setVec3("material.ambient", glm::vec3(0.2f, 0.2f, 0.6f));
    shaderProgram.setVec3("material.diffuse", glm::vec3(0.4f, 0.4f, 0.8f));
    shaderProgram.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shaderProgram.setFloat("material.shininess", 32.0f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 15, GL_UNSIGNED_INT, 0);
    glDrawElements(GL_TRIANGLE_FAN, 15, GL_UNSIGNED_INT, (const void*)(15 * sizeof(unsigned int)));
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, (const void*)(30 * sizeof(unsigned int)));
    glDrawElements(GL_TRIANGLE_STRIP, 90, GL_UNSIGNED_INT, (const void*)(36 * sizeof(unsigned int)));
    glBindVertexArray(0);
}




void drawFan(Shader ourShader, glm::mat4 identityMatrix, unsigned int VAO, unsigned int VAO2, glm::mat4 Model_Center, Shader shaderProgram, glm::mat4 altogether)
{
    glm::mat4 translateMatrix, rotationMatrix, scaleMatrix, model;

    // stick of fan

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.025f, -0.025f, 2.115f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 1.75));
    model = altogether * translateMatrix * scaleMatrix;
  
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.58f, 0.58f, 0.529f, 0.851f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //fan

    shaderProgram.use();
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 2.0));
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 0.2));

    model = altogether * translateMatrix * rotationMatrix * scaleMatrix;

    shaderProgram.setMat4("model", model);

    // Set material properties
    shaderProgram.setVec3("material.ambient", glm::vec3(0.2f, 0.2f, 0.6f));
    shaderProgram.setVec3("material.diffuse", glm::vec3(0.4f, 0.4f, 0.8f));
    shaderProgram.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shaderProgram.setFloat("material.shininess", 32.0f);

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
    model = altogether * Model_Center * translateMatrix * scaleMatrix;

    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.4f, 0.4f, 0.365f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);



    // fan wing 2

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, 2.0625f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 1.2, 0.05));
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = altogether * Model_Center * rotationMatrix * translateMatrix * scaleMatrix;

    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.4f, 0.4f, 0.365f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    // fan wing 3

    ourShader.use();

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, 2.0625f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 1.2, 0.05));
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(240.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = altogether * Model_Center * rotationMatrix * translateMatrix * scaleMatrix;

    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.4f, 0.4f, 0.365f, 1.0f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}


// Helper function to compute the cross product of two vectors
glm::vec3 computeSurfaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    glm::vec3 edge1 = b - a;
    glm::vec3 edge2 = c - a;
    return glm::normalize(glm::cross(edge1, edge2));
}

// Function to calculate vertex normals
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

void drawCarpart2(Shader& lightingShader, unsigned int VAO, unsigned int VAO2, glm::mat4 identityMatrix, glm::mat4 modelCenter, glm::mat4 altogether)
{
    float rotateAngle;
    glm::mat4 translateMatrix, rotationMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    //car curve 1
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    rotateAngle = -90;
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateAngle = 180;
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    rotateAngle = -90;
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .4));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, -0.6));
    model = altogether * modelCenter * translateMatrix * scaleMatrix * rotateXMatrix * rotateZMatrix * rotateYMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //car curve 2
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    rotateAngle = -90;
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateAngle = 180;
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    rotateAngle = -90;
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .4));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.125, -0.6));
    model = altogether * modelCenter * translateMatrix * scaleMatrix * rotateXMatrix * rotateZMatrix * rotateYMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //car curve 3
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    rotateAngle = -90;
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateAngle = 180;
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    rotateAngle = -90;
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .2));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.125, 0));
    rotateAngle = 180;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    model = altogether * modelCenter * translateMatrix * rotationMatrix * scaleMatrix * rotateXMatrix * rotateZMatrix * rotateYMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //car curve 4
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    rotateAngle = -90;
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateAngle = 180;
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    rotateAngle = -90;
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .2));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, 0));
    rotateAngle = 180;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    model = altogether * modelCenter * translateMatrix * rotationMatrix * scaleMatrix * rotateXMatrix * rotateZMatrix * rotateYMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //car head
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .01, 0.605));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.3, -0.4));
    rotateAngle = 90;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));

    model = altogether * modelCenter * translateMatrix * scaleMatrix * rotationMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //car window front
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .01, 0.525));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, -0.6));
    rotateAngle = -40;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));

    model = altogether * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;

    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //car window back
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 1.0, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .01, 0.405));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, 0));
    rotateAngle = -120;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));

    model = altogether * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;

    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //car door front
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 0.75, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01, 0.35, 0.60));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, -0.4));
    rotateAngle = 90;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    model = altogether * modelCenter * translateMatrix * scaleMatrix * rotationMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //car door front
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(1.0, 0.75, 0.0));
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0, 0.0, 0.0));
    lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.0f, 0.0f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emission", glm::vec3(0.5f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01, 0.35, 0.60));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.125, -0.4));
    rotateAngle = 90;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    model = altogether * modelCenter * translateMatrix * scaleMatrix * rotationMatrix;
    lightingShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    

}

// Function to calculate circle points
std::vector<float> generateCirclePoints(float centerX, float centerY, float centerZ, float radius, int numSegments) {
    std::vector<float> circlePoints;

    // Calculate the angle between consecutive points
    float angleIncrement = 2.0f * 3.1416 / numSegments;

    for (int i = 0; i < numSegments; ++i) {
        float angle = i * angleIncrement;

        // Compute x and y coordinates
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);

        // Add the coordinates to the vector
        circlePoints.push_back(x);
        circlePoints.push_back(y);
        circlePoints.push_back(centerZ);
    }

    return circlePoints;
}

std::vector<float> generateSpherePoints(float centerX, float centerY, float centerZ, float radius, int numSegments, int heightSegments) {
    std::vector<float> spherePoints;

    // Iterate through height segments
    for (int i = 1; i <= heightSegments/2; ++i) {
        // Calculate the z offset and the current radius of the circle at this height
        
        float phi = 3.1416 * i / heightSegments; // Polar angle

        float z = centerZ + radius * cos(phi); // Z-coordinate for this layer
        float currentRadius = radius * sin(phi); // Radius for this layer

        // Generate circle points for this layer
        std::vector<float> circlePoints = generateCirclePoints(centerX, centerY, z, currentRadius, numSegments);

        // Add circle points to sphere points
        spherePoints.insert(spherePoints.end(), circlePoints.begin(), circlePoints.end());
    }

    return spherePoints;
}
*/