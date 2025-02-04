#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cube.h"
#include "shader.h"
#include "camera.h"
#include "sphere.h"
#include "Shape.h"
#include "tree.h"
#include "bush.h"
#include "original_tree.h"
#include "torus.h"
#include "sphere2.h"
#include "GrassField.h"
#include "Sphi.h"


#include <iostream>

using namespace std;
using namespace glm;


bool moveCar = false;


float angleofwindow2nd = 210;
float angleofwindow1st = 0;
bool window_open = false;
float carRotate = 0.0;


unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices);
void read_file(std::string file_name, std::vector<float>& vec);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void checkLighting(Shader& shader);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
long long nCr(int n, int r);
void FerrisWheel(Shader ourShader, glm::mat4 moveMatrix, Shape Cube_necessary);
void FerrisWheelSeat(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z, unsigned int Army_texture, Shape Cube_necessary);

#include <glm/glm.hpp>
#include <cmath>
float carlightleftx = -1.85, carlightlefty = -2.5, carlightleftz = 9.8;
float carlightrightx = -1.85, carlightrighty = -2.5, carlightrightz = 7.6;
float robotZTranslation = 0.0f;
float oscillationAngle = 0.0f;  // Tracks the oscillation angle (in degrees)
float robotRotationY = 0.0f;       // Tracks robot's Y-axis rotation angle (in degrees)
float robotFixingArmLegRotation = 0.0f;


glm::vec3 carLightPositions[] = {
    glm::vec3(carlightleftx, carlightlefty, carlightleftz), // Left car light
    glm::vec3(carlightrightx, carlightrighty, carlightrightz)  // Right car light
};

glm::mat4 transformation(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    return model;
}

void cylinder_custom(Shader& ourShader, unsigned int& VAOdec, glm::mat4& model)
{
    //glm::mat4 identityMatrix = glm::mat4(1.0f);
    //glm::mat4 rot = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    //model = model * rot;
    ourShader.setMat4("model", model);
    glBindVertexArray(VAOdec);
    glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
}
float baseZ = 4.0f;

void robot_key_move(
    Sphi& sphere,
    Shader& lightingShaderWithTexture,
    unsigned int VAOdec,
    unsigned int diffMaps,
    unsigned int specMaps) {

    // Base z-position
    baseZ = 4.0f + robotZTranslation;
    glm::mat4 numun = transformation(3.0,1.0,-7.0     ,0,90,0,      1,1,1);
    // Oscillation parameters
    float armLegOscillation = sin(glm::radians(oscillationAngle)) * 30.0f; // Oscillation in degrees

    // Base model rotation about the Y-axis
    glm::mat4 baseModel = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Robot serving
    // Head
    glm::mat4 model = numun * transformation(1.5f, 0.5f + 0.3f, baseZ, 0.0f, robotRotationY, 0.0f, 0.3f, 0.3f, 0.3f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);

    // Body
    model = numun * transformation(1.5f, -0.8f + 0.3f, baseZ, 0.0f, robotRotationY, 0.0f, 0.1f, 1.0f, 0.1f);
    cylinder_custom(lightingShaderWithTexture, VAOdec, model);

    // Left leg
    model = numun * baseModel * transformation(1.3f, -0.7f, baseZ, armLegOscillation, 0.0f, 0.0f, 0.05f, 0.3f, 0.05f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);

    // Right leg
    model = numun * baseModel * transformation(1.7f, -0.7f, baseZ, -armLegOscillation, 0.0f, 0.0f, 0.05f, 0.3f, 0.05f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);

    // Left arm
    model = numun * baseModel * transformation(1.08f, 0.1f, baseZ, armLegOscillation, 0.0f, -30.0f, 0.05f, 0.3f, 0.05f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);

    // Right arm
    model = numun * baseModel * transformation(1.92f, 0.1f, baseZ, -armLegOscillation, 0.0f, 30.0f, 0.05f, 0.3f, 0.05f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);
}


//void setRoadSpotLight(Shader& shader, int idx, glm::vec3 position, glm::vec3 direction) {
//    std::string base = "spotLights[" + std::to_string(idx) + "].";
//    shader.setVec3(base + "position", position);
//    shader.setVec3(base + "direction", direction);
//    shader.setFloat(base + "constant", 1.0f);
//    shader.setFloat(base + "linear", 0.09f);
//    shader.setFloat(base + "quadratic", 0.032f);
//    shader.setFloat(base + "cutOff", glm::cos(glm::radians(20.0f)));
//    shader.setFloat(base + "outerCutOff", glm::cos(glm::radians(25.0f)));
//    shader.setVec3(base + "ambient", 0.2f, 0.2f, 0.2f);
//    shader.setVec3(base + "diffuse", 0.8f, 0.8f, 0.8f);
//    shader.setVec3(base + "specular", 1.0f, 1.0f, 1.0f);
//    shader.setBool(base + "spotlightthatemitOn", RoadspotLightOn);
//}

glm::mat4 manualPerspective(float fov, float aspect, float near, float far) {
    glm::mat4 result(0.0f);

    float tanHalfFovy = tan(fov / 2.0f);

    result[0][0] = 1.0f / (aspect * tanHalfFovy);
    result[1][1] = 1.0f / tanHalfFovy;
    result[2][2] = -(far + near) / (far - near);
    result[2][3] = -1.0f;
    result[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}

glm::mat4 manualPerspective6(float fov, float aspect, float near, float far) {
    float tanHalfFov = tan(fov / 2.0f);

    float top = near * tanHalfFov;
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    glm::mat4 result(0.0f);

    result[0][0] = 2.0f * near / (right - left);
    result[1][0] = 0.0f;
    result[2][0] = (right + left) / (right - left);
    result[3][0] = 0.0f;

    result[0][1] = 0.0f;
    result[1][1] = 2.0f * near / (top - bottom);
    result[2][1] = (top + bottom) / (top - bottom);
    result[3][1] = 0.0f;

    result[0][2] = 0.0f;
    result[1][2] = 0.0f;
    result[2][2] = -(far + near) / (far - near);
    result[3][2] = -(2.0f * far * near) / (far - near);

    result[0][3] = 0.0f;
    result[1][3] = 0.0f;
    result[2][3] = -1.0f;
    result[3][3] = 0.0f;

    return result;
}


const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

float eX = 0.0f, eY = 5.0f, eZ = 28.0f;

Camera camera(glm::vec3(eX, eY, eZ));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float car_wheel_anglee = 0.0f;

const int nt = 40;
const int ntheta = 30;

vector <float> cntrlPoints, cntrlPointsBoat, cntrlPointsRotor, cntrlPointsCylinder, cntrlPointsCarousal, cntrlPointsHead;
vector <float> coordinates, coordinatesBoat, coordinatesRotor, coordinatesCylinder, coordinatesCarousal, coordinatesHead;
vector <float> normals, normalsBoat, normalsRotor, normalsCylinder, normalsCarousal, normalsHead;
vector <int> indices, indicesBoat, indicesRotor, indicesCylinder, indicesCarousal, indicesHead;
vector <float> vertices, verticesBoat, verticesRotor, verticesCylinder, verticesCarousal, verticesHead;

vec3 color;
vec3 lightPosition = vec3(2.0f, 2.0f, 2.0f);
bool carheadlightOn = true;
bool RoadspotLightOn = true;
bool dirLightOn = true;
bool pointLight1On = true, pointLight2On = true, pointLight3On = true, pointLight4On = true;
bool spotLightOn = true;

bool ambientLightOn = true, diffuseLightOn = true, specularLightOn = true;

bool _textureNoColorNoLight = false, _textureColorNoLight = false;
bool _textureNoColorPhong = true, _textureColorPhong = false;
bool _textureNoColorGouraud = false, _textureColorGouraud = false;
bool _colorNoTexture = false;
unsigned int bezierVAO;

float ferrisWheelAngle = 0.0f;
float ferrisWheelSpeed = 0.02f;
float ferrisIncrement = 0.012f;
bool isFerrisWheelOn = true;

void drawFractalTree(Shader& shader, const glm::mat4& model, int depth, Shape& CubeGreen, const glm::mat4& a_translate_mat, Bush& Leaf_Obj, const glm::mat4& model2) {
    if (depth == 0) return;

    // Draw the current cube

    CubeGreen.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, a_translate_mat * model);
    if (depth == 1)
    {
        Leaf_Obj.drawWithTexture(shader, a_translate_mat * model2);
    }

    // Scaling and translation for the branches
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.75f, 0.5f));

    float deg = 0;
    glm::mat4 rotateY = rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    // Left branch 1
    glm::mat4 translateLeft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.05f));
    glm::mat4 rotateLeft = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 leftBranchModel = translateLeft * rotateY * rotateLeft * scaleMatrix * model;
    glm::mat4 leftBranchforLeaf = translateLeft * rotateY * rotateLeft * model2;
    drawFractalTree(shader, leftBranchModel, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, leftBranchforLeaf);
    // Right branch 1
    glm::mat4 translateRight = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.05f));
    glm::mat4 rotateRight = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rightBranchModel = translateRight * rotateY * rotateRight * scaleMatrix * model;
    glm::mat4 rightBranchforLeaf = translateRight * rotateY * rotateRight *  model2;
    drawFractalTree(shader, rightBranchModel, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, rightBranchforLeaf);

    deg = 90;
    rotateY = rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    // Left branch 2
    translateLeft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.15f));
    glm::mat4 leftBranchModel2 = translateLeft * rotateY * rotateLeft * scaleMatrix * model;
    glm::mat4 leftBranchforLeaf2 = translateLeft * rotateY * rotateLeft *  model2;
    drawFractalTree(shader, leftBranchModel2, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, leftBranchforLeaf2);
    translateRight = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.15f));
    glm::mat4 rightBranchModel2 = translateRight * rotateY * rotateRight * scaleMatrix * model;
    glm::mat4 rightBranchforLeaf2 = translateRight * rotateY * rotateRight * model2;
    drawFractalTree(shader, rightBranchModel2, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, rightBranchforLeaf2);

}

bool carback=false, carFront=false;

unsigned int load_texture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Model City", NULL , NULL);//glfwGetPrimaryMonitor()
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    
    Shader shader;
    Shader lightShader("lightShader.vs", "lightShader.fs");

    Shader shader_textureNoColorNoLight("textureNoColorNoLight.vs", "textureNoColorNoLight.fs");
    Shader shader_textureColorNoLight("textureColorNoLight.vs", "textureColorNoLight.fs");
    Shader shader_textureNoColorPhong("textureNoColorPhong.vs", "textureNoColorPhong.fs");
    Shader shader_textureColorPhong("textureColorPhong.vs", "textureColorPhong.fs");
    Shader shader_textureNoColorGouraud("textureNoColorGouraud.vs", "textureNoColorGouraud.fs");
    Shader shader_textureColorGouraud("textureColorGouraud.vs", "textureColorGouraud.fs");
    Shader shader_colorNoTexture("colorNoTexture.vs", "colorNoTexture.fs");

    unsigned int pavingStones = loadTexture("PavingStones136_1K-JPG_Color.jpg");
    unsigned int Highwaytex = loadTexture("Roadimage.jpg");
    unsigned int bricks051 = loadTexture("Bricks051_1K-JPG_Color.jpg");
    unsigned int thatchedRoof001a = loadTexture("ThatchedRoof001A_1K-JPG_Color.jpg");
    unsigned int porcelain003 = loadTexture("ThatchedRoof001A_1K-JPG_Color.jpg");
    unsigned int metal009 = loadTexture("Metal009_1K-JPG_Color.jpg");
    unsigned int metal001 = loadTexture("Metal001_1K-JPG_Color.jpg");
    unsigned int fabric020 = loadTexture("Fabric020_1K-JPG_Color.jpg");
    unsigned int diamondPlate008c = loadTexture("DiamondPlate008C_1K-JPG_Color.jpg");
    unsigned int fabric008 = loadTexture("Fabric008_1K-JPG_Color.jpg");
    unsigned int fabric051 = loadTexture("Fabric051_1K-JPG_Color.jpg");
    unsigned int fabric015 = loadTexture("Fabric015_1K-JPG_Color.jpg");
    unsigned int fabric004 = loadTexture("Fabric004_1K-JPG_Color.jpg");
    unsigned int christmasTreeOrnament002 = loadTexture("ChristmasTreeOrnament002_1K-JPG_Color.jpg");
    unsigned int CarTopIMG = loadTexture("CarTop.png");
    unsigned int CarTopIMGR = loadTexture("CarBack.png");
    unsigned int CarFrontIMG = loadTexture("CarFront.png");
    unsigned int CarWheelIMG = loadTexture("CarWheel.png");
    unsigned int CarTrackIMG = loadTexture("cartrack.jpg");
    unsigned int CarSideIMG = loadTexture("CarSide.png");
    unsigned int CarSideIMG2 = loadTexture("CarSide2.png");
    unsigned int CarSideIMG3 = loadTexture("CarSide3.png");
    unsigned int Car_back_window = loadTexture("Car_Back_with_window.png");
    unsigned int PureRed = loadTexture("PureRed.png");
    unsigned int Car_Door = loadTexture("CarDoor.png");
    unsigned int Car_BackSmall_window = loadTexture("backSmallWindow.png");
    unsigned int Car_Front_side_window = loadTexture("Car_Front_window.png");
    unsigned int Car_Front_side_angle = loadTexture("Car_Front_side_angle.png");
    unsigned int Cube_home_front_wall = loadTexture("Home_Front_wall.jpg");

    unsigned int Home_window_Left = loadTexture("LeftWindow.png");
    unsigned int Home_window_Right = loadTexture("RightWindow.png");
    unsigned int Door_Texture = loadTexture("Door_Texture.png");
    unsigned int Army_texture = loadTexture("army.png");
    unsigned int Leaf_texture = loadTexture("TreeTexture.png");//TreeTexture.png
    unsigned int Tree_Textureroot = loadTexture("TreeRoot.png");
    unsigned int Building_Texture = loadTexture("buildingimage.png");
    unsigned int Building_Texture2 = loadTexture("Bulding2.png");
    unsigned int Helipad_Texture = loadTexture("Helipad.png");
    unsigned int Wine_Image = loadTexture("Wine.jpg");
    //unsigned int Walkway = loadTexture("Footpath.jpg");//TreeTexture.png


    //string diffuseMapPaths = "army.png";
    //string specularMapPaths = "army.png";

    //// Load textures
    //unsigned int diffMaps = loadTexture(diffuseMapPaths.c_str());
    //unsigned int specMaps = loadTexture(specularMapPaths.c_str());

    //// Create a sphere object
    //float radius = 1.0f;
    //int sectorCount = 36;
    //int stackCount = 18;
    //glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    //glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    //glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    //float shininess = 32.0f;

    //Sphere2 sphere(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess);


    Shape floor = Shape("Rectangle");
    floor.initializeRectangle(10.0f, 0.0f, 10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, pavingStones);
    Shape roof = Shape("Rectangle");
    roof.initializeRectangle(10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f, 0.0f, 6.0f, 0.0f, thatchedRoof001a);
    Shape leftWall = Shape("Rectangle");
    leftWall.initializeRectangle(0.0f, 6.0f, 10.0f, 1.0f, 0.0f, 0.0f, -5.0f, 3.0f, 0.0f, bricks051);
    Shape rightWall = Shape("Rectangle");
    rightWall.initializeRectangle(0.0f, 6.0f, 10.0f, -1.0f, 0.0f, 0.0f, 5.0f, 3.0f, 0.0f, bricks051);
    Shape frontWall = Shape("Rectangle");
    frontWall.initializeRectangle(10.0f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 3.0f, 5.0f, bricks051);
    Shape backWall = Shape("Rectangle");
    backWall.initializeRectangle(10.0f, 6.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 3.0f, -5.0f, bricks051);
    Shape Cube_necessary = Shape("Cube");
    Cube_necessary.initializeCube(0.25f, 0.25f, 0.25f, porcelain003, porcelain003, porcelain003, porcelain003, porcelain003, porcelain003);
    Shape Cube_Car_Top = Shape("Cube");
    Cube_Car_Top.initializeCube(0.25f, 0.25f, 0.25f, CarTopIMG, CarTopIMG, CarTopIMG, CarTopIMG, CarTopIMG, CarTopIMG);
    Shape Cube_Car_Top_R = Shape("Cube");
    Cube_Car_Top_R.initializeCube(0.25f, 0.25f, 0.25f, CarTopIMGR, CarTopIMGR, CarTopIMGR, CarTopIMGR, CarTopIMGR, CarTopIMGR);
    Shape Cube_Car_Front = Shape("Cube");
    Cube_Car_Front.initializeCube(0.25f, 0.25f, 0.25f, CarFrontIMG, CarFrontIMG, CarFrontIMG, CarFrontIMG, CarFrontIMG, CarFrontIMG);
    Shape Car_wheel = Shape("Cylinder");
    Car_wheel.initializeCylinder(0.75f, 0.75f, 0.002, 100, 36, CarTrackIMG, CarWheelIMG, CarWheelIMG);
    Shape Cube_Car_Side = Shape("Cube");
    Cube_Car_Side.initializeCube(0.25f, 0.25f, 0.25f, CarSideIMG, CarSideIMG, CarSideIMG, CarSideIMG, CarSideIMG, CarSideIMG);

    Shape Cube_Car_Side2 = Shape("Cube");
    Cube_Car_Side2.initializeCube(0.25f, 0.25f, 0.25f, CarSideIMG2, CarSideIMG2, CarSideIMG2, CarSideIMG2, CarSideIMG2, CarSideIMG2);

    Shape Cube_Car_Side3 = Shape("Cube");
    Cube_Car_Side3.initializeCube(0.25f, 0.25f, 0.25f, CarSideIMG3, CarSideIMG3, CarSideIMG3, CarSideIMG3, CarSideIMG3, CarSideIMG3);

    Shape Cube_Car_Back_window = Shape("Cube");
    Cube_Car_Back_window.initializeCube(0.25f, 0.25f, 0.25f, Car_Front_side_angle, Car_Front_side_angle, Car_back_window, Car_BackSmall_window, Car_BackSmall_window, Car_BackSmall_window);

    Shape Cube_Car_Door = Shape("Cube");
    Cube_Car_Door.initializeCube(0.25f, 0.25f, 0.25f, Car_Door, Car_Door, Car_Door, Car_Door, Car_Door, Car_Door);

    Shape Cube_Car_Front_window = Shape("Cube");
    Cube_Car_Front_window.initializeCube(0.25f, 0.25f, 0.25f, Car_Front_side_window, Car_Front_side_window, Car_Front_side_window, Car_Front_side_window, Car_Front_side_window, Car_Front_side_window);

    Shape Cube_Car_Front_side = Shape("Cube");
    Cube_Car_Front_side.initializeCube(0.25f, 0.25f, 0.25f, Car_Front_side_angle, Car_Front_side_angle, Car_Front_side_angle, Car_Front_side_angle, Car_Front_side_angle, Car_Front_side_angle);

    Shape Cubefront = Shape("Cube");
    Cubefront.initializeCube(0.25f, 0.25f, 0.25f, Cube_home_front_wall, Cube_home_front_wall, Cube_home_front_wall, Cube_home_front_wall, Cube_home_front_wall, Cube_home_front_wall);

    Shape Cubegrill = Shape("Cube");
    Cubegrill.initializeCube(0.25f, 0.25f, 0.25f, metal009, metal009, metal009, metal009, metal009, metal009);

    Shape CubeWindow_L = Shape("Cube");
    CubeWindow_L.initializeCube(0.25f, 0.25f, 0.25f, Home_window_Left, Home_window_Left, Home_window_Left, Home_window_Left, Home_window_Left, Home_window_Left);

    Shape CubeWindow_R = Shape("Cube");
    CubeWindow_R.initializeCube(0.25f, 0.25f, 0.25f, Home_window_Right, Home_window_Right, Home_window_Right, Home_window_Right, Home_window_Right, Home_window_Right);

    Shape CubeDoor = Shape("Cube");
    CubeDoor.initializeCube(0.25f, 0.25f, 0.25f, Door_Texture, Door_Texture, Door_Texture, Door_Texture, Door_Texture, Door_Texture);

    Shape Highway = Shape("Cube");
    Highway.initializeCube(0.25f, 0.25f, 0.25f, Highwaytex, Highwaytex, Highwaytex, Highwaytex, Highwaytex, Highwaytex);

    Shape Building_1 = Shape("Cube");
    Building_1.initializeCube(0.25f, 0.25f, 0.25f, Building_Texture, Building_Texture, Building_Texture, Helipad_Texture  , Building_Texture, Building_Texture);

    Shape Building_2 = Shape("Cube");
    Building_2.initializeCube(0.25f, 0.25f, 0.25f, Building_Texture2, Building_Texture2, Building_Texture2, Helipad_Texture, Building_Texture2, Building_Texture2);

    Sphere pointLight1 = Sphere(0.1f, 36, 100);
    Sphere pointLight2 = Sphere(0.1f, 36, 100);
    Sphere pointLight3 = Sphere(0.1f, 36, 100);
    Sphere pointLight4 = Sphere(0.1f, 36, 100);

    Shape spotLight = Shape("Cylinder");
    spotLight.initializeCylinder(0.0f, 0.1f, 0.1f, 100, 36, -1, -1, -1);

    Tree Helicopter = Tree(Army_texture, Army_texture, 32.0f);
    Bush Leaf_Obj = Bush(Wine_Image, Wine_Image,32.0f);
    OTree Mytree = OTree(Leaf_texture, Leaf_texture, 32.0f);
    //outer = 1.0f, float inner = 0.4f, unsigned int dMap = 0, unsigned int sMap = 0
    Torus a_high_road = Torus(1.0, 0.4, Leaf_texture, Leaf_texture);

    GRASS_Field Grass_fence = GRASS_Field(pavingStones, pavingStones, 32.0f);

    Shape Flield_G = Shape("Cube");
    Flield_G.initializeCube(0.25f, 0.25f, 0.25f, Leaf_texture, Leaf_texture, Leaf_texture, Leaf_texture, Leaf_texture, Leaf_texture);

    Shape WalkwayCube = Shape("Cube");
    WalkwayCube.initializeCube(0.25f, 0.25f, 0.25f, bricks051, bricks051, bricks051, bricks051, bricks051, bricks051);





    Shape Tree_root = Shape("Cylinder");
    Tree_root.initializeCylinder(0.75f, 0.75f, 0.6, 100, 36, Tree_Textureroot, Tree_Textureroot, Tree_Textureroot);


    read_file("ferris_wheel_points.txt", cntrlPoints);
    bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1, coordinates, normals, indices, vertices);
    
    // Exercise Bike start

    /*
    Shape exercise_bike_xhandle = Shape("Cylinder");
    exercise_bike_xhandle.initializeCylinder(0.05f, 0.05f, 1.0f, 100, 36, fabric004, fabric004, fabric004);

    Shape exercise_bike_stadium = Shape("Stadium");
    exercise_bike_stadium.initializeStadium(1.5f, 0.08f, 0.4f, 0.1f, 36, fabric020, fabric020, fabric020);*/

    //Sphereaf balle1 = Sphereaf();
    //Sphere ball2 = Sphere(0.4f, 36, 100, fabric004);

    Shape cone1 = Shape("Cylinder");
    cone1.initializeCylinder(0.0f, 0.4f, 1.0f, 100, 36, Leaf_texture, Leaf_texture, Leaf_texture);
    float moving = 0.0;
    // Exercise bike end

    glEnable(GL_DEPTH_TEST);

    float decagon[] = {
0.0f, 0.0f, 0.0f,  0.992f, 0.8588f, 0.1529f, // Center point
-1.0f, 0.0f, -3.0f,  0.992f, 0.8588f, 0.1529f, // RED
1.0f, 0.0f, -3.0f,    0.992f, 0.8588f, 0.1529f,// GREEN
2.25f, 0.0f, -1.75f,     0.992f, 0.8588f, 0.1529f, // BLUE
3.0f, 0.0f, 0.0f,    0.992f, 0.8588f, 0.1529f, // WHITE
2.25f, 0.0f, 1.75f,    0.992f, 0.8588f, 0.1529f, // PINK
1.0f, 0.0f, 3.0f,     0.992f, 0.8588f, 0.1529f, // KHOYERI
-1.0f, 0.0f, 3.0f,     0.992f, 0.8588f, 0.1529f, // SKY BLUE
-2.25f, 0.0f, 1.75f,     0.992f, 0.8588f, 0.1529f, // YELLOW
-3.0f, 0.0f, 0.0f,    0.992f, 0.8588f, 0.1529f, // purple
-2.25f, 0.0f, -1.75f,     0.992f, 0.8588f, 0.1529f, // orange

0.0f, 1.0f, 0.0f,    0.0f, 0.694f, 0.824f, // Center point
-1.0f, 1.0f, -3.0f,    0.0f, 0.694f, 0.824f, // RED
1.0f, 1.0f, -3.0f,      0.0f, 0.694f, 0.824f, // GREEN
2.25f, 1.0f, -1.75f,    0.0f, 0.694f, 0.824f,// BLUE
3.0f, 1.0f, 0.0f,     0.0f, 0.694f, 0.824f, // WHITE
2.25f, 1.0f, 1.75f,    0.0f, 0.694f, 0.824f, // PINK
1.0f, 1.0f, 3.0f,     0.0f, 0.694f, 0.824f, // KHOYERI
-1.0f, 1.0f, 3.0f,     0.0f, 0.694f, 0.824f, // SKY BLUE
-2.25f, 1.0f, 1.75f,     0.0f, 0.694f, 0.824f,// YELLOW
-3.0f, 1.0f, 0.0f,     0.0f, 0.694f, 0.824f, // purple
-2.25f, 1.0f, -1.75f,     0.0f, 0.694f, 0.824f,// orange
    };

    unsigned int deca_indices[] = {
    0,1,2,
    0,2,3,
    0,3,4,
    0,4,5,
    0,5,6,
    0,6,7,
    0,7,8,
    0,8,9,
    0,9,10,
    0,10,1,

    11,12,13,
    11,13,14,
    11,14,15,
    11,15,16,
    11,16,17,
    11,17,18,
    11,18,19,
    11,19,20,
    11,20,21,
    11,21,12,


    2,12,13,
    2,12,1,

    3,13,14,
    3,13,2,

    4,14,15,
    4,14,3,

    5,15,16,
    5,15,4,

    6,16,17,
    6,16,5,

    7,17,18,
    7,17,6,

    8,18,19,
    8,18,7,

    9,19,20,
    9,19,8,

    10,20,21,
    10,20,9,

    1,21,12,
    1,21,10
    };




    //for decagon
    unsigned int VBOdec, VAOdec, EBOdec;
    glGenVertexArrays(1, &VAOdec);
    glGenBuffers(1, &VBOdec);
    glGenBuffers(1, &EBOdec);
    glBindVertexArray(VAOdec);
    glBindBuffer(GL_ARRAY_BUFFER, VBOdec);
    glBufferData(GL_ARRAY_BUFFER, sizeof(decagon), decagon, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deca_indices), deca_indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    string diffuseMapPaths = "wine.jpg";
    string specularMapPaths = "wine.jpg";

    // Load textures    unsigned int Wine_Image = 
    unsigned int diffMaps = loadTexture("Wine.jpg"); //load_texture(diffuseMapPaths.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMaps = loadTexture("Wine.jpg"); // load_texture(specularMapPaths.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Create a sphere object
    float radius = 1.0f;
    int sectorCount = 36;
    int stackCount = 18;
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 32.0f;

    Sphi sphere(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess);


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        if (_textureNoColorNoLight)
        {
            shader = shader_textureNoColorNoLight;
        }
        else if (_textureColorNoLight)
        {
            shader = shader_textureColorNoLight;
        }
        else if (_textureNoColorPhong)
        {
            shader = shader_textureNoColorPhong;
        }
        else if (_textureColorPhong)
        {
            shader = shader_textureColorPhong;
        }
        else if (_textureNoColorGouraud)
        {
            shader = shader_textureNoColorGouraud;
        }
        else if (_textureColorGouraud)
        {
            shader = shader_textureColorGouraud;
        }
        else if (_colorNoTexture)
        {
            shader = shader_colorNoTexture;
        }

        shader.use();

        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 projection = manualPerspective6(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();


        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        
        shader.setVec3("viewPos", camera.Position);

        shader.setVec3("dirLight.direction", 0.0f, -1.0f, 0.0f);

        checkLighting(shader);

        shader.setVec3("pointLights[0].position", -5.0f, 6.0f, -5.0f);
        
        shader.setFloat("pointLights[0].constant", 1.0f);
        shader.setFloat("pointLights[0].linear", 0.09f);
        shader.setFloat("pointLights[0].quadratic", 0.032f);

        shader.setVec3("pointLights[1].position", 5.0f, 6.0f, -5.0f);
        
        shader.setFloat("pointLights[1].constant", 1.0f);
        shader.setFloat("pointLights[1].linear", 0.09f);
        shader.setFloat("pointLights[1].quadratic", 0.032f);

        shader.setVec3("pointLights[2].position", (17.5 + 12.5 - 5.0), (0.05 + 20.0), (-13.0 + 1.0 + 10.0));
        
        shader.setFloat("pointLights[2].constant", 1.0f);
        shader.setFloat("pointLights[2].linear", 0.09f);
        shader.setFloat("pointLights[2].quadratic", 0.032f);
        shader.setVec3("pointLights[3].position", (-17.5 - 12.5 - 15.0), 15.0, (-13.0 + 6.0));
        
        shader.setFloat("pointLights[3].constant", 1.0f);
        shader.setFloat("pointLights[3].linear", 0.09f);
        shader.setFloat("pointLights[3].quadratic", 0.032f);

        shader.setVec3("spotLight.position", -20.0f, 10.0f, -4.5 );
        shader.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
        
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.09f);
        shader.setFloat("spotLight.quadratic", 0.032f);
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(60.0f)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(60.0f)));
        




        
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrixinit, translateMatrix, rotationMatrix, scaleMatrix, modelCenter, model, carWheelMatrix;

        translateMatrixinit = glm::translate(identityMatrix, glm::vec3(0.25, 0.25, 0.25));

        robot_key_move(sphere, shader, VAOdec, diffMaps, specMaps);

        //road
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-50.0, 0.05, 5.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(200, 0.01, 25));
        model = translateMatrix * scaleMatrix * translateMatrixinit;
        Highway.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        //road 2nd
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-50.0, 0.05, -25.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(200, 0.01, 25));
        model = translateMatrix * scaleMatrix * translateMatrixinit;
        Highway.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        //for (int i = 0; i < 15; i++)
        //{
        //    translateMatrix = glm::translate(identityMatrix, glm::vec3(-52.5 - (2.5) * i, 0.05, 5.5 - 1.0 * i));
        //    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5, 0.01, 25));
        //    model = translateMatrix * scaleMatrix  * translateMatrixinit;
        //    Highway.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        //}

        //for (int i = 0; i < 15; i++)
        //{
        //    translateMatrix = glm::translate(identityMatrix, glm::vec3(-52.5 - (2.5) * i, 0.05, -25.5 + 1.0 * i));
        //    scaleMatrix = glm::scale(identityMatrix, glm::vec3(5, 0.01, 25));
        //    model = translateMatrix * scaleMatrix * translateMatrixinit;
        //    Highway.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        //}

        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-100.0, 0.05, -8.5));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(50, 0.01, 25));
        //model = translateMatrix * scaleMatrix * translateMatrixinit;
        //Highway.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        float rotateAngle = 90;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.5, 0.05, -13.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20, 0.01, 37.5));
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
        Highway.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-33.5, 0.05, -13.0));
        model = translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
        Highway.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //building
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.5+12.5, 0.05, -13.0+1.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20, 40.0, 33.25));
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
        Building_1.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-17.5 - 12.5 - 15.0, -1.0, -13.0 + 1.0));
        model = translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
        Building_2.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        

        //Helicopter
        translateMatrix = glm::translate(identityMatrix, glm::vec3(17.5 + 12.5-5.0, 0.05 + 20.0, -13.0 + 1.0+10.0));
        model = translateMatrix * scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2));
        Helicopter.drawWithTexture(shader, model);


        //Field
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-8.0 , 0.05, -13.0 + 2.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(48, 0.01, 28.25));
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
        Flield_G.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        for (int i = 0; i < 8; i++)
        {
            if (i == 4) continue;
            //Field Bench
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-9.5 - i*3.0, 0.05, 3.125));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 1.0, 1.0));
            model = translateMatrix * scaleMatrix;
            Grass_fence.drawWithTexture(shader, model);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-9.5 - i * 3.0, 0.05, -11.00));
            model = translateMatrix * scaleMatrix;
            Grass_fence.drawWithTexture(shader, model);
        }
        for (int i = 0; i < 5; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-32.0, 0.05, 2.0 - i * 3.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 1.0, 1.0));
            rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            model = translateMatrix * rotationMatrix * scaleMatrix;
            Grass_fence.drawWithTexture(shader, model);
        }

        //Walkway
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-32.0, 0.05, 4.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(52, 0.01, 2.25));
        model = translateMatrix * scaleMatrix   * translateMatrixinit;
        Flield_G.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);





        // Lampposts
        for (int idx = 0; idx < 10; idx+=2) {
            // Calculate the position of each lamppost along the road
            float lampX = -48.0 + (idx * 10.5); // Place lampposts further apart than dividers
            float lampY = 0.05;            // Same y-level as the road
            float lampZ = 5.5+6.25;           // Slightly above the road



            // Vertical pole of the lamppost
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX, lampY, lampZ));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 7.5, 0.2)); // Tall and thin pole
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX-1.0, lampY+1.40, lampZ+6.65));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 1.5, 1.5)); // Tall and thin pole
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            cone1.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX - 0.6, lampY + 1.44 - 1.45, lampZ + 7.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.175, 2.0, 0.175)); // Tall and thin pole
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            Tree_root.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
            //Mytree.drawWithTexture(shader, model);

            // Horizontal arm of the lamppost
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX, lampY + 3.75 - 0.05, lampZ )); // Top of the pole
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2 , 4.5)); // Short horizontal arm
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

            // Horizontal arm of the lamppost other side
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX, lampY + 3.75 - 0.05, lampZ-2.25)); // Top of the pole
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 4.5)); // Short horizontal arm
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


            //lamp
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX + 0.05, lampY + 3.75 , lampZ + 2.25 + 0.125)); // End of the arm
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 0.5)); // Small cube for the lamp
            model = translateMatrix * scaleMatrix;

            Cubegrill.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX + 0.05, lampY + 3.75 , lampZ - 2.25 - 0.125)); // End of the arm
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 0.5)); // Small cube for the lamp
            model = translateMatrix * scaleMatrix;

            Cubegrill.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


            glm::vec3 lightPos(lampX, lampY + 3.75 - 0.1, lampZ + 2.25 + 0.125); // Lamp bulb position 
            glm::vec3 lightDir(0.0f, -1.0f, 0.0f);               // Pointing downward

            std::string i = std::to_string(idx);
            shader.use();
            shader.setVec3("spotLightsRoads[" + i + "].position", lightPos[0], lightPos[1], lightPos[2]);
            shader.setVec3("spotLightsRoads[" + i + "].direction", lightDir[0], lightDir[1], lightDir[2] );

            shader.setFloat("spotLightsRoads[" + i + "].constant", 1.0f);
            shader.setFloat("spotLightsRoads[" + i + "].linear", 0.09f);
            shader.setFloat("spotLightsRoads[" + i + "].quadratic", 0.022f);
            shader.setFloat("spotLightsRoads[" + i + "].cutOff", glm::cos(glm::radians(30.0f)));
            shader.setFloat("spotLightsRoads[" + i + "].outerCutOff", glm::cos(glm::radians(30.0f)));

            shader.setVec3("spotLightsRoadsRight[" + i + "].position", lightPos[0], lightPos[1], lightPos[2] - 2.25 - 0.125 - 2.25 - 0.125);
            shader.setVec3("spotLightsRoadsRight[" + i + "].direction", lightDir[0], lightDir[1], lightDir[2]);

            shader.setFloat("spotLightsRoadsRight[" + i + "].constant", 1.0f);
            shader.setFloat("spotLightsRoadsRight[" + i + "].linear", 0.09f);
            shader.setFloat("spotLightsRoadsRight[" + i + "].quadratic", 0.022f);
            shader.setFloat("spotLightsRoadsRight[" + i + "].cutOff", glm::cos(glm::radians(30.0f)));
            shader.setFloat("spotLightsRoadsRight[" + i + "].outerCutOff", glm::cos(glm::radians(30.0f)));

            shader.setBool("spotlightthatemitOn", RoadspotLightOn);

            if (RoadspotLightOn)
            {

                if (ambientLightOn)
                {
                    shader.setVec3("spotLightsRoads[" + i + "].ambient", 1.0f, 1.0f, 1.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].ambient", 1.0f, 1.0f, 1.0f);
                }
                else
                {
                    shader.setVec3("spotLightsRoads[" + i + "].ambient", 0.0f, 0.0f, 0.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].ambient", 0.0f, 0.0f, 0.0f);
                }

                if (diffuseLightOn)
                {
                    shader.setVec3("spotLightsRoads[" + i + "].diffuse", 1.0f, 1.0f, 1.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].diffuse", 1.0f, 1.0f, 1.0f);
                }
                else
                {
                    shader.setVec3("spotLightsRoads[" + i + "].diffuse", 0.0f, 0.0f, 0.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].diffuse", 0.0f, 0.0f, 0.0f);
                }

                if (specularLightOn)
                {
                    shader.setVec3("spotLightsRoads[" + i + "].specular", 1.0f, 1.0f, 1.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].specular", 1.0f, 1.0f, 1.0f);
                }
                else
                {
                    shader.setVec3("spotLightsRoads[" + i + "].specular", 0.0f, 0.0f, 0.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].specular", 0.0f, 0.0f, 0.0f);
                }
            }
            else
            {
                shader.setVec3("spotLightsRoads[" + i + "].ambient", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoads[" + i + "].diffuse", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoads[" + i + "].specular", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoadsRight[" + i + "].ambient", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoadsRight[" + i + "].diffuse", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoadsRight[" + i + "].specular", 0.0f, 0.0f, 0.0f);
            }

        }


        // Lampposts
        for (int idx = 1; idx < 10; idx+=2) {
            // Calculate the position of each lamppost along the road
            float lampX = -48.0 + (idx * 10.5); // Place lampposts further apart than dividers
            float lampY = 0.05;            // Same y-level as the road
            float lampZ = -25.5 + 6.25;           // Slightly above the road



            // Vertical pole of the lamppost
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX, lampY, lampZ));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 7.5, 0.2)); // Tall and thin pole
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

            //translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX - 1.0, lampY + 1.44, lampZ - 6.7));
            //model = translateMatrix * translateMatrixinit;
            //Mytree.drawWithTexture(shader, model);


            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX - 1.0, lampY + 1.44, lampZ - 6.65));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 1.5, 1.5)); // Tall and thin pole
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            cone1.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX - 0.6, lampY + 1.44 - 1.45, lampZ - 7.0 + 0.6));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.175, 2.0, 0.175)); // Tall and thin pole
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            Tree_root.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


            //more
            if (idx == 3 || idx == 5) {
                //translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX - 1.0, lampY + 1.44, lampZ + 6.3));
                //model = translateMatrix * translateMatrixinit;
                //Mytree.drawWithTexture(shader, model);

                translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX - 1.0, lampY + 1.44, lampZ + 6.3));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 1.5, 1.5)); // Tall and thin pole
                model = translateMatrix * scaleMatrix * translateMatrixinit;
                cone1.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
                translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX - 0.6, lampY + 1.44 - 1.45, lampZ + 7.0 - 0.3));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.175, 2.0, 0.175)); // Tall and thin pole
                model = translateMatrix * scaleMatrix * translateMatrixinit;
                Tree_root.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
            }

            // Horizontal arm of the lamppost
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX, lampY + 3.75 - 0.05, lampZ)); // Top of the pole
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 4.5)); // Short horizontal arm
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

            // Horizontal arm of the lamppost other side
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX, lampY + 3.75 - 0.05, lampZ - 2.25)); // Top of the pole
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 4.5)); // Short horizontal arm
            model = translateMatrix * scaleMatrix * translateMatrixinit;
            Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


            //lamp
            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX + 0.05, lampY + 3.75, lampZ + 2.25 + 0.125)); // End of the arm
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 0.5)); // Small cube for the lamp
            model = translateMatrix * scaleMatrix;

            Cubegrill.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(lampX + 0.05, lampY + 3.75, lampZ - 2.25 - 0.125)); // End of the arm
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2, 0.2, 0.5)); // Small cube for the lamp
            model = translateMatrix * scaleMatrix;

            Cubegrill.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


            glm::vec3 lightPos(lampX, lampY + 3.75 - 0.1, lampZ + 2.25 + 0.125); // Lamp bulb position 
            glm::vec3 lightDir(0.0f, -1.0f, 0.0f);               // Pointing downward

            std::string i = std::to_string(idx);
            shader.use();
            shader.setVec3("spotLightsRoads[" + i + "].position", lightPos[0], lightPos[1], lightPos[2]);
            shader.setVec3("spotLightsRoads[" + i + "].direction", lightDir[0], lightDir[1], lightDir[2]);

            shader.setFloat("spotLightsRoads[" + i + "].constant", 1.0f);
            shader.setFloat("spotLightsRoads[" + i + "].linear", 0.09f);
            shader.setFloat("spotLightsRoads[" + i + "].quadratic", 0.022f);
            shader.setFloat("spotLightsRoads[" + i + "].cutOff", glm::cos(glm::radians(30.0f)));
            shader.setFloat("spotLightsRoads[" + i + "].outerCutOff", glm::cos(glm::radians(30.0f)));

            shader.setVec3("spotLightsRoadsRight[" + i + "].position", lightPos[0], lightPos[1], lightPos[2] - 2.25 - 0.125 - 2.25 - 0.125);
            shader.setVec3("spotLightsRoadsRight[" + i + "].direction", lightDir[0], lightDir[1], lightDir[2]);

            shader.setFloat("spotLightsRoadsRight[" + i + "].constant", 1.0f);
            shader.setFloat("spotLightsRoadsRight[" + i + "].linear", 0.09f);
            shader.setFloat("spotLightsRoadsRight[" + i + "].quadratic", 0.022f);
            shader.setFloat("spotLightsRoadsRight[" + i + "].cutOff", glm::cos(glm::radians(30.0f)));
            shader.setFloat("spotLightsRoadsRight[" + i + "].outerCutOff", glm::cos(glm::radians(30.0f)));

            shader.setBool("spotlightthatemitOn", RoadspotLightOn);

            if (RoadspotLightOn)
            {

                if (ambientLightOn)
                {
                    shader.setVec3("spotLightsRoads[" + i + "].ambient", 1.0f, 1.0f, 1.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].ambient", 1.0f, 1.0f, 1.0f);
                }
                else
                {
                    shader.setVec3("spotLightsRoads[" + i + "].ambient", 0.0f, 0.0f, 0.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].ambient", 0.0f, 0.0f, 0.0f);
                }

                if (diffuseLightOn)
                {
                    shader.setVec3("spotLightsRoads[" + i + "].diffuse", 1.0f, 1.0f, 1.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].diffuse", 1.0f, 1.0f, 1.0f);
                }
                else
                {
                    shader.setVec3("spotLightsRoads[" + i + "].diffuse", 0.0f, 0.0f, 0.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].diffuse", 0.0f, 0.0f, 0.0f);
                }

                if (specularLightOn)
                {
                    shader.setVec3("spotLightsRoads[" + i + "].specular", 1.0f, 1.0f, 1.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].specular", 1.0f, 1.0f, 1.0f);
                }
                else
                {
                    shader.setVec3("spotLightsRoads[" + i + "].specular", 0.0f, 0.0f, 0.0f);
                    shader.setVec3("spotLightsRoadsRight[" + i + "].specular", 0.0f, 0.0f, 0.0f);
                }
            }
            else
            {
                shader.setVec3("spotLightsRoads[" + i + "].ambient", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoads[" + i + "].diffuse", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoads[" + i + "].specular", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoadsRight[" + i + "].ambient", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoadsRight[" + i + "].diffuse", 0.0f, 0.0f, 0.0f);
                shader.setVec3("spotLightsRoadsRight[" + i + "].specular", 0.0f, 0.0f, 0.0f);
            }

        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
        model = translateMatrix;

        floor.drawTexture(shader, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        //floor.drawTexture(shader,5.0,2.5);
        roof.drawTexture(shader);
        leftWall.drawTexture(shader);
        rightWall.drawTexture(shader);
        //frontWall.drawTexture(shader);
        backWall.drawTexture(shader);
        //Cube_necessary.drawTexture(shader, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

        translateMatrixinit = glm::translate(identityMatrix, glm::vec3(0.25, 0.25, 0.25));


        rotateAngle = -90;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.25, 0.75, -2.25));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 3.0f, 3.0f));

        glm::mat4 rotationz = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        modelCenter = translateMatrix * rotationz * scaleMatrix;
        glm::mat4 rotationy;
        glm::mat4 carRotationMatrix;

        if (moveCar) {

            carRotationMatrix = glm::rotate(identityMatrix, glm::radians(carRotate), glm::vec3(0.0f, 1.0f, 0.0f));
            
            carWheelMatrix = (glm::translate(identityMatrix, glm::vec3(moving, -1.1, 12.0f))) * (glm::scale(identityMatrix, glm::vec3(2.0f,2.0f, 2.0f))) * carRotationMatrix;
            carLightPositions[0].x = carlightleftx + moving;
            carLightPositions[1].x = carlightrightx + moving;
        }
        else
        {
            float stopposition = moving;
            carWheelMatrix = (glm::translate(identityMatrix, glm::vec3(stopposition, -1.1, 12.0f))) * (glm::scale(identityMatrix, glm::vec3(2.0f, 2.0f, 2.0f)));
            carLightPositions[0].x = carlightleftx + stopposition;
            carLightPositions[1].x = carlightrightx + stopposition;
        }

        shader.use();

        shader.setVec3("CarFlashLight[0].position", carLightPositions[0].x, carLightPositions[0].y+5.0, carLightPositions[0].z + 0.3);
        shader.setVec3("CarFlashLight[0].direction", -0.707f, -0.707f, 0.0f);

        shader.setFloat("CarFlashLight[0].constant", 1.0f);
        shader.setFloat("CarFlashLight[0].linear", 0.22f);
        shader.setFloat("CarFlashLight[0].quadratic", 0.020f);
        shader.setFloat("CarFlashLight[0].cutOff", glm::cos(glm::radians(25.0f)));
        shader.setFloat("CarFlashLight[0].outerCutOff", glm::cos(glm::radians(25.0f)));


        shader.setVec3("CarFlashLight[1].position", carLightPositions[1].x, carLightPositions[1].y+5.0, carLightPositions[1].z + 0.2);
        shader.setVec3("CarFlashLight[1].direction", -0.707f, -0.707f, 0.0f);

        shader.setFloat("CarFlashLight[1].constant", 1.0f);
        shader.setFloat("CarFlashLight[1].linear", 0.22f);
        shader.setFloat("CarFlashLight[1].quadratic", 0.020f);
        shader.setFloat("CarFlashLight[1].cutOff", glm::cos(glm::radians(25.0f)));
        shader.setFloat("CarFlashLight[1].outerCutOff", glm::cos(glm::radians(25.0f)));
        shader.setBool("CarHeadLighton", carheadlightOn);


        if (carheadlightOn)
        {
            if (ambientLightOn)
            {
                shader.setVec3("CarFlashLight[0].ambient", 0.5f, 0.5f, 0.5f);
                shader.setVec3("CarFlashLight[1].ambient", 0.5f, 0.5f, 0.5f);

                //shader.setVec3("spotLight.ambient", 1.0f, 1.0f, 1.0f);
            }
            else
            {
                shader.setVec3("CarFlashLight[0].ambient", 0.0f, 0.0f, 0.0f);
                shader.setVec3("CarFlashLight[1].ambient", 0.0f, 0.0f, 0.0f);

            }

            if (diffuseLightOn)
            {
                shader.setVec3("CarFlashLight[0].diffuse", 1.0f, 1.0f, 1.0f);
                shader.setVec3("CarFlashLight[1].diffuse", 1.0f, 1.0f, 1.0f);

            }
            else
            {
                shader.setVec3("CarFlashLight[0].diffuse", 0.0f, 0.0f, 0.0f);
                shader.setVec3("CarFlashLight[1].diffuse", 0.0f, 0.0f, 0.0f);
            }

            if (specularLightOn)
            {
                shader.setVec3("CarFlashLight[0].specular", 1.0f, 1.0f, 1.0f);
                shader.setVec3("CarFlashLight[1].specular", 1.0f, 1.0f, 1.0f);
            }
            else
            {
                shader.setVec3("CarFlashLight[0].specular", 0.0f, 0.0f, 0.0f);
                shader.setVec3("CarFlashLight[1].specular", 0.0f, 0.0f, 0.0f);
            }
        }
        else
        {
            shader.setVec3("CarFlashLight[0].ambient", 0.0f, 0.0f, 0.0f);
            shader.setVec3("CarFlashLight[0].diffuse", 0.0f, 0.0f, 0.0f);
            shader.setVec3("CarFlashLight[0].specular", 0.0f, 0.0f, 0.0f);
            shader.setVec3("CarFlashLight[1].ambient", 0.0f, 0.0f, 0.0f);
            shader.setVec3("CarFlashLight[1].diffuse", 0.0f, 0.0f, 0.0f);
            shader.setVec3("CarFlashLight[1].specular", 0.0f, 0.0f, 0.0f);
        }



        //Helicopter
        //translateMatrix = glm::translate(identityMatrix, vec3((0.4566 * 2), 0.2f, (0.4 * 6)));
        //model = translateMatrix * scale(identityMatrix, glm::vec3(0.2f, 0.2f, 0.2));
        //Helicopter.drawWithTexture(shader, model);


        //// Tree
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 4.0f,0.5f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-9.0f, 0.25, 4.75 ));
        glm::mat4 rootModel =  scaleMatrix  * translateMatrixinit;
        // Draw fractal tree
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 4.0f, 4.0f));

        //glm::mat4 rootModel2 = translate(identityMatrix, glm::vec3(-1.0f, 1.0f,-1.0f))  * scaleMatrix * translateMatrixinit;
        //Leaf_Obj.drawWithTexture(shader, rootModel2);
        //glm::mat4 rootModel2 = translate(identityMatrix, glm::vec3(-0.8f, -0.65f, 0.0f)) * scaleMatrix;
        glm::mat4 rootModel2 = translate(identityMatrix, glm::vec3(0.0f, -0.75f, 0.0f)) * scaleMatrix;
        drawFractalTree(shader, rootModel, 4 , Tree_root, translateMatrix, Leaf_Obj, rootModel2); // Change depth as needed




        //Front Wall
        glm::vec3 door_cord;
        glm::vec3 window_cord;
        bool doorsave = false;
        bool windowsave = false;

        float xcube, ycube = -1.4, zcube;
        // Loop to draw the 15x15 grid
        for (int id = 0; id < 15; ++id) {
            zcube = (0.01) + (id * 0.2);

            for (int jf = 0; jf < 15; ++jf) {
                xcube = (-1.8) + (jf * 0.2283);

                // Create the model matrix for the current cube
                translateMatrix = glm::translate(identityMatrix, glm::vec3(xcube, ycube, zcube));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4566f, 0.2f, 0.4f));
                rotateAngle = 270;
                rotationz = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
                model = rotationz * translateMatrix * scaleMatrix * translateMatrixinit;
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.92f, 1.9925f, 1.0f));
                translateMatrix = glm::translate(identityMatrix, glm::vec3(0.26f,0.0f , 3.65f));
                model = translateMatrix * scaleMatrix * model;

                if (((jf == 3) || (jf == 4) ) && ((id == 0) || (id == 1) || (id == 2) || (id == 3) || (id == 4) || (id == 5)))
                {
                    if (!doorsave)
                    {
                        door_cord = glm::vec3(xcube, ycube, zcube);
                        doorsave = true;
                    }
                }
                else if (( (jf == 10) || (jf == 11) || (jf == 12)) && ((id == 7) || (id == 8) || (id == 9)))
                {
                    if (!windowsave)
                    {
                        window_cord = glm::vec3(xcube, ycube, zcube);
                        windowsave = true;
                    }
                }
                else
                {

                    Cubefront.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
                }
            }
        }



        // door
        translateMatrix = glm::translate(identityMatrix, door_cord);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3((0.4566 * 2), 0.2f, (0.4 * 6)));
        model = rotationz * translateMatrix * scaleMatrix * translateMatrixinit;
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.92f, 1.9925f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.26f, 0.0f, 3.65f));
        model = translateMatrix * scaleMatrix * model;
        CubeDoor.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);




        // window 1st half
        glm::vec3 new_window_cord = window_cord + glm::vec3(0, 0, 0);
        translateMatrix = glm::translate(identityMatrix, new_window_cord);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3((0.4566 * 1.5), 0.1f, (0.4 * 3)));
        if (window_open)
        {
            if (angleofwindow1st == -160)
                angleofwindow1st = -160;
            else
                angleofwindow1st--;
        }
        else
        {
            if (angleofwindow1st == 0)
                angleofwindow1st = 0;
            else
                angleofwindow1st++;
        }
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(angleofwindow1st), glm::vec3(0.0f, 0.0f, 1.0f));
        model = rotationz  * translateMatrix * rotationMatrix  * scaleMatrix * translateMatrixinit;
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.92f, 1.9925f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.26f, 0.0f, 3.65f));
        model = translateMatrix * scaleMatrix * model;
        CubeWindow_L.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);



        // window 2nd half
        new_window_cord = window_cord + glm::vec3(0.4566 * 1.5, 0.05, 0);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3((0.4566 * 1.5), 0.1f, (0.4 * 3)));

        if (window_open)
        {
            if (angleofwindow2nd == 315)
                angleofwindow2nd = 315;
            else
                angleofwindow2nd++;
        }
        else
        {
            if (angleofwindow2nd == 210)
                angleofwindow2nd = 210;
            else
                angleofwindow2nd--;
        }
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(angleofwindow2nd), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, new_window_cord);
        model = rotationz * translateMatrix * rotationMatrix * scaleMatrix * translateMatrixinit;
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.92f, 1.9925f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.20f, 0.0f, 3.7f));
        model = translateMatrix * scaleMatrix * model;

        CubeWindow_R.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //window grill

        for (int i = 0; i < 4; i++)
        {
            float incx = (i * 0.25) + 0.05;
            new_window_cord = window_cord + glm::vec3(incx, 0.03, 0);

            translateMatrix = glm::translate(identityMatrix, new_window_cord);
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.05f, (0.4 * 3)));
            model = rotationz * translateMatrix * scaleMatrix * translateMatrixinit;
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.92f, 1.9925f, 1.0f));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.26f, 0.0f, 3.65f));
            model = translateMatrix * scaleMatrix * model;
            Cubegrill.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        }

        // window silling
        new_window_cord = window_cord + glm::vec3(-0.05, -0.1, 0.6);
        translateMatrix = glm::translate(identityMatrix, new_window_cord);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3((0.4566 * 3.5), 0.2f, 0.2));
        model = rotationz * translateMatrix * scaleMatrix * translateMatrixinit;
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.92f, 1.9925f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.26f, 0.0f, 3.65f));
        model = translateMatrix * scaleMatrix * model;
        Cubegrill.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        ////sphere
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 2.2f, -1.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.3f, 4.3f, 4.3f));
        //model = translateMatrix * scaleMatrix;
        //sphere.drawSphereTexture(shader, model, diffMaps, specMaps);
            


        ////Home Top Curve Left
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.25, 5.21, -5.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 10.0, 20.0));
        glm::mat4 shearMatrix = glm::mat4(1.0);
        shearMatrix[1][0] = 1.25;
        model = translateMatrix * shearMatrix * scaleMatrix * translateMatrixinit;
        Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        ////Home Top Curve Right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.25+6.25*2, 5.21, -5.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 10.0, 20.0));
        shearMatrix = glm::mat4(1.0);
        shearMatrix[1][0] = -1.25;
        model = translateMatrix * shearMatrix * scaleMatrix * translateMatrixinit;
        Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        ////Land
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0, 0, -0.05));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(20, 0.01, 8.0));
        model = translateMatrix * scaleMatrix * translateMatrixinit;
        //Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        floor.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        if (moveCar)
        {
            if (carback)
            {
                moving -= 0.01;
                car_wheel_anglee += 1.5;
            }
            else if (carFront)
            {
                moving += 0.01;
                car_wheel_anglee -= 1.5;
            }

            if (moving < -30.5)
            {
                moving = 35;
            }
        }
        
        //bottom lower
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, 1));
        model = carWheelMatrix * modelCenter * scaleMatrix * translateMatrixinit;
        Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,model);

        
        //a bottom-middle lower
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, .5));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, 0.125));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //a middle lower
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, 1));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.25, 0));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);




        //a top-middle lower 
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, .5));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.375, 0.125));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);



        //a top lower
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .35, 1));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.5, 0));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        
        float hudai = 0.05;
        float hudai2 = 0.23;
        mat4 rotating_in_own;

        //wheel
        

        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.185, 0.025+ hudai));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 25.0f, 0.08f));
        rotateAngle = 90;
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));

        if (moveCar)
        {
            rotating_in_own = rotate(identityMatrix, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
            model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * rotating_in_own * scaleMatrix;
        }
        else
        {
            model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
        }
        Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.185, 0.4 + hudai));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 25.0f, 0.08f));
        rotateAngle = 90;
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        if (moveCar)
        {
            rotating_in_own = rotate(identityMatrix, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
            model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * rotating_in_own * scaleMatrix;
        }
        else
        {
            model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;

        }
        Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.435, 0.025 + hudai));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 25.0f, 0.08f));
        rotateAngle = 90;
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        if (moveCar)
        {
            rotating_in_own = rotate(identityMatrix, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
            model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * rotating_in_own * scaleMatrix;
        }
        else
        {
            model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
        }
        Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.435, 0.4 + hudai));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 25.0f, 0.08f));
        rotateAngle = 90;
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        if (moveCar)
        {
            rotating_in_own = rotate(identityMatrix, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
            model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * rotating_in_own * scaleMatrix;
        }
        else
        {
            model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
        }
        Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        //
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.005, 0.185, 0.025 + hudai2));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 200.75f, 0.01f));
        rotateAngle = 90;
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
        Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.005, 0.435, 0.025 + hudai2));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 200.75f, 0.01f));
        rotateAngle = 90;
        rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
        Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, 1.35, 1));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0, 0));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Top.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);



        //a left back
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0, 0));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Side3.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //a left middle
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.25, 0));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Side2.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //a left top
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .01));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.5, 0));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Side.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //a right back
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0, .5));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Side3.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //a right middle
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.25, .5));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Side2.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //right top 
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .01));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.5, .5));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Side.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //a back lower to upper
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, 0.01, 1));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0, 0));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Front.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //a front lower to upper
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, 0.01, 1));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0.675, 0));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Top_R.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.8, -4.75,-1.0));
        rotateAngle = 90;
        rotationy = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 3.0f, 3.0f)); //changed
        modelCenter = rotationy * scaleMatrix * translateMatrixinit;


        //car curve 1
        rotateAngle = 90;
        glm::mat4 rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateAngle = 180;
        glm::mat4 rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        rotateAngle = -90;
        glm::mat4 rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.245, 0.21,  -0.175));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.348f, 0.998f));  // Scale to flatten
        shearMatrix = glm::mat4(1.0f);
        shearMatrix[1][0] = 0.5f; // Apply shear along x-axis (y changes with x)

        model = carWheelMatrix * modelCenter * translateMatrix * rotateYMatrix *  shearMatrix * scaleMatrix ;
        Cube_Car_Back_window.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);



        ////car curve 2
        // 
        rotateAngle = 270;
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.245, 0.21, -0.375));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.348f, 0.998f));  // Scale to flatten
        shearMatrix = identityMatrix;
        shearMatrix[1][0] = 1.2f; // Apply shear along x-axis (y changes with x)

        model = carWheelMatrix * modelCenter * translateMatrix * rotateYMatrix * shearMatrix * scaleMatrix;
        Cube_Car_Front_side.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        
        //car head
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .01, 0.605));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.3, -0.4));
        rotateAngle = 90;
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
        Cube_Car_Top.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
        //car window front
        
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .01, 0.525));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, -0.6));
        rotateAngle = -40;
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix * translateMatrixinit;
        Cube_Car_Front_window.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


        //car door front 1
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01, 0.35, 0.60));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, -0.4));
        rotateAngle = 90;
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
        Cube_Car_Door.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        //car door front 2
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01, 0.35, 0.60));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.125, -0.4));
        rotateAngle = 90;
        rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
        Cube_Car_Door.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

        // cube.drawCubeWithTexture(shader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bricks051);

        shader.setInt("material.diffuseMap", 0);

        //ball1.drawSphere(shader, -4.6f, 0.4f, -4.6f, -90.0f, 0.0f, 0.0f);


        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, christmasTreeOrnament002);

        //shader.setInt("material.diffuseMap", 0);

        //ball2.drawSphere(shader, -4.6f, 0.4f, -3.8f, -90.0f, 0.0f, 0.0f);

        cone1.drawTexture(shader, 2.3f, 0.25f, -2.3f);
        //cone2.drawTexture(shader, 2.3f, 0.25f, -1.9f);
        //cone3.drawTexture(shader, 2.3f, 0.25f, -1.5f);
        //cone4.drawTexture(shader, 2.3f, 0.25f, -1.1f);
        //cone5.drawTexture(shader, 2.3f, 0.25f, -0.7f);


        // Lighting start

        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setMat4("model", model);

        pointLight1.drawSphere(lightShader, -4.9f, 5.9f, -4.9f);
        pointLight2.drawSphere(lightShader, 4.9f, 5.9f, -4.9f);
        pointLight3.drawSphere(lightShader, (17.5 + 12.5 - 5.0 - 0.1), (0.05 + 20.0 - 0.1), (-13.0 + 1.0 + 10.0 - 0.1));
        pointLight4.drawSphere(lightShader, (-17.5 - 12.5 - 15.0), 15.0, (-13.0 + 6.0));

        //pointLight3.drawSphere(lightShader, -4.9f, 5.9f, 4.9f);
        //pointLight4.drawSphere(lightShader, 4.9f, 5.9f, 4.9f);

        spotLight.drawColor(lightShader, -6.5f, 10.0f, -6.5f);

        //wheel
        //model = identityMatrix;
        //FerrisWheel(lightShader, model, Cube_necessary);

        // Lighting end

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow* window)
{

    float translationSpeed = 1.0f;  // Adjust speed as needed
    float oscillationSpeed = 200.0f; // Adjust for faster/slower oscillation
    float rotationSpeed = 50.0f;     // Speed for rotating the robot
    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        robotZTranslation -= translationSpeed * deltaTime; // Forward movement
        oscillationAngle += oscillationSpeed * deltaTime;  // Oscillate
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        robotZTranslation += translationSpeed * deltaTime; // Backward movement
        oscillationAngle += oscillationSpeed * deltaTime;  // Oscillate
    }

    // Rotate clockwise (NUMPAD_4)
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        robotRotationY -= rotationSpeed * deltaTime; // Clockwise rotation
    }

    // Rotate counterclockwise (NUMPAD_6)
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        robotRotationY += rotationSpeed * deltaTime; // Counterclockwise rotation
    }

    // Keep the oscillation angle and rotation angle bounded
    if (oscillationAngle >= 360.0f) oscillationAngle -= 360.0f;
    if (robotRotationY >= 360.0f) robotRotationY -= 360.0f;
    if (robotRotationY < 0.0f) robotRotationY += 360.0f;


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        dirLightOn = !dirLightOn;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        pointLight1On = !pointLight1On;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        pointLight2On = !pointLight2On;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        pointLight3On = !pointLight3On;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        pointLight4On = !pointLight4On;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        spotLightOn = !spotLightOn;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        ambientLightOn = !ambientLightOn;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        diffuseLightOn = !diffuseLightOn;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        specularLightOn = !specularLightOn;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        _textureNoColorNoLight = false, _textureColorNoLight = false;
        _textureNoColorPhong = false, _textureColorPhong = false;
        _textureNoColorGouraud = false, _textureColorGouraud = true;
        _colorNoTexture = false;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        _textureNoColorNoLight = false, _textureColorNoLight = false;
        _textureNoColorPhong = false, _textureColorPhong = false;
        _textureNoColorGouraud = true, _textureColorGouraud = false;
        _colorNoTexture = false;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        _textureNoColorNoLight = false, _textureColorNoLight = false;
        _textureNoColorPhong = false, _textureColorPhong = true;
        _textureNoColorGouraud = false, _textureColorGouraud = false;
        _colorNoTexture = false;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        _textureNoColorNoLight = false, _textureColorNoLight = false;
        _textureNoColorPhong = true, _textureColorPhong = false;
        _textureNoColorGouraud = false, _textureColorGouraud = false;
        _colorNoTexture = false;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        _textureNoColorNoLight = false, _textureColorNoLight = true;
        _textureNoColorPhong = false, _textureColorPhong = false;
        _textureNoColorGouraud = false, _textureColorGouraud = false;
        _colorNoTexture = false;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        _textureNoColorNoLight = true, _textureColorNoLight = false;
        _textureNoColorPhong = false, _textureColorPhong = false;
        _textureNoColorGouraud = false, _textureColorGouraud = false;
        _colorNoTexture = false;
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        _textureNoColorNoLight = false, _textureColorNoLight = false;
        _textureNoColorPhong = false, _textureColorPhong = false;
        _textureNoColorGouraud = false, _textureColorGouraud = false;
        _colorNoTexture = true;
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        if (moveCar) {
               moveCar = false;
               carheadlightOn = false;
               carback = false;
        }
        else {
            moveCar = true;
            carheadlightOn = true;
            carback = true;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (moveCar) {
            moveCar = false;
            carFront = false;
        }
        else {
            moveCar = true;
            carFront = true;

        }
    }
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
    {
        if (window_open) {
            window_open = false;
            RoadspotLightOn = false;
        }
        else {
            window_open = true;
            RoadspotLightOn = true;

        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void read_file(std::string file_name, std::vector<float>& vec)
{
    ifstream file(file_name);
    float number;

    while (file >> number)
        vec.push_back(number);

    file.close();
}

void FerrisWheel(Shader ourShader, glm::mat4 moveMatrix, Shape Cube_necessary)
{
    ourShader.use();
    //front
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrix, rotateTemp;


    ferrisWheelAngle = fmod(ferrisWheelAngle, 360);
    rotateTemp = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(ferrisWheelAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
    model = translateMatrix * rotateZMatrix * scaleMatrix * rotateTemp;

    glm::vec4 color = glm::vec4(48.0 / 256, 45.0 / 256, 181.0 / 256, 1.0f);
    //glm::vec4 color = glm::vec4(1.0f);
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    /*
    //bars
    glm::mat4 translateToPivot, translateFromPivot;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec4 pivot = glm::vec4(glm::vec3(0.0f), 1.0f);

    pivot = translateMatrix * pivot;

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -0.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.4f, 0.1f));
    for (float i = 0.0f; i >= -360.0f; i -= 45.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
        Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);
        
        //ourShader.setMat4("model", moveMatrix * model);
        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
        
    }
    


    //back
    rotateTemp = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(ferrisWheelAngle), glm::vec3(0.0f, 0.0f, 1.0f));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
    model = translateMatrix * rotateZMatrix * scaleMatrix * rotateTemp;
    ourShader.setMat4("model", moveMatrix * model);
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    //bars

    translateToPivot = glm::translate(identityMatrix, glm::vec3(-pivot));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(pivot));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.0f, -1.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.4f, 0.1f));
    for (float i = 0.0f; i >= -360.0f; i -= 45.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * rotateTemp * scaleMatrix;
        Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);
        //ourShader.setMat4("model", moveMatrix * model);
        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
    }


    //connecting bars
    //middle
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.975f, 1.975f, -1.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 1.7f));
    model = translateFromPivot * rotateZMatrix * translateToPivot * translateMatrix * scaleMatrix;
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);
    //ourShader.setMat4("model", moveMatrix * model);
    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

    //top ones
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 3.5f, -1.005f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 1.7f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    for (float i = 0.0f; i >= -360.0f; i -= 90.0f)
    {
        rotateTemp = glm::rotate(identityMatrix, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateFromPivot * rotateZMatrix * translateToPivot * translateFromPivot * rotateTemp * translateToPivot * translateMatrix * scaleMatrix;
        Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);
        //ourShader.setMat4("model", moveMatrix * model);
        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
    }

    //pillars
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, -4.0f, 0.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-18.3f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);
    //ourShader.setMat4("model", moveMatrix * model);
    //ourShader.setVec4("material.ambient", color * 0.5f);
    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f, -4.0f, 0.1f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(18.3f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);
    //ourShader.setMat4("model", moveMatrix * model);
    //ourShader.setVec4("material.ambient", color * 0.5f);
    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, -4.0f, -1.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-18.3f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);
    //ourShader.setMat4("model", moveMatrix * model);
    //ourShader.setVec4("material.ambient", color * 0.5f);
    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(5.1f, -4.0f, -1.25f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 6.6f, 0.2f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(18.3f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateMatrix * rotateTemp * scaleMatrix;
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);

    //ourShader.setMat4("model", moveMatrix * model);
    //ourShader.setVec4("material.ambient", color * 0.5f);
    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

    //color = glm::vec4(1.0f);
    //ourShader.setVec4("material.ambient", color);
    //ourShader.setVec4("material.diffuse", color);
    //ourShader.setVec4("material.specular", color);
    //sitting places
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    //FerrisWheelSeat(ourShader, moveMatrix*translateMatrix, ferrisWheelAngle);
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 1.5f, 0.0f));
    //FerrisWheelSeat(ourShader, moveMatrix*translateMatrix, ferrisWheelAngle);
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 3.0f, 0.0f));
    //FerrisWheelSeat(ourShader,moveMatrix*translateMatrix, ferrisWheelAngle);
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 1.5f, 0.0f));
    //FerrisWheelSeat(ourShader, moveMatrix*translateMatrix, ferrisWheelAngle);
    
    */
}

void FerrisWheelSeat(Shader ourShader, glm::mat4 moveMatrix, float rotateAngleTest_Z, unsigned int Army_texture, Shape Cube_necessary)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model, rotateZMatrixLocal, rotateZMatrixMain, rotateTemp;
    glm::mat4 translateToPivotMain, translateFromPivotMain;
    glm::vec4 translateToPivotOrgMain, translateFromPivotOrgMain;
    glm::mat4 translateToPivotLocal, translateFromPivotLocal;
    glm::vec4 translateToPivotOrgLocal, translateFromPivotOrgLocal;
    glm::vec4 pivot1, pivot2;
    glm::vec4 mainPivotPoint = glm::vec4(3.0f, 2.0f, 0.0f, 1.0);
    glm::vec4 localPivotPoint = glm::vec4(2.975f, 0.475f, -1.005f, 1.0);

    pivot1 = mainPivotPoint;
    translateToPivotMain = glm::translate(identityMatrix, glm::vec3(-pivot1));
    translateFromPivotMain = glm::translate(identityMatrix, glm::vec3(pivot1));
    rotateZMatrixMain = glm::rotate(identityMatrix, glm::radians(rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));

    pivot2 = translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * moveMatrix * localPivotPoint;
    translateToPivotLocal = glm::translate(identityMatrix, glm::vec3(-pivot2));
    translateFromPivotLocal = glm::translate(identityMatrix, glm::vec3(pivot2));
    rotateZMatrixLocal = glm::rotate(identityMatrix, glm::radians(-rotateAngleTest_Z), glm::vec3(0.0f, 0.0f, 1.0f));

    //glBindTexture(GL_TEXTURE_2D, Army_texture);
    //base
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.1f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);
    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

    //glBindTexture(GL_TEXTURE_2D, Army_texture);
    //left
    //seat
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);


    //seat right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.2f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.45f, -0.2f, -0.9f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 1.5f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

    //top bars
    glBindTexture(GL_TEXTURE_2D, Army_texture);
    //left
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix * model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix* model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.899f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.15f, 0.1f, 0.05f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix* model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.25f, -0.1751f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.15f, 0.1f, 0.05f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix* model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);


    //right
    //1
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.8f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix* model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //2
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.3f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.1f, 0.1f, 0.1f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix* model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //3
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.899f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.15f, 0.1f, 0.05f));
    rotateTemp = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix* model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    //4
    translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.25f, -0.1751f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.15f, 0.1f, 0.05f));
    model = translateFromPivotLocal * rotateZMatrixLocal * translateToPivotLocal * translateFromPivotMain * rotateZMatrixMain * translateToPivotMain * translateMatrix * rotateTemp * scaleMatrix;
    //ourShader.setMat4("model", moveMatrix * model);
    Cube_necessary.drawTexture(ourShader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, moveMatrix* model);

    //glBindVertexArray(cubeVAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

}

long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * 3.1416 / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}





void checkLighting(Shader& shader)
{
    if (dirLightOn)
    {
        if (ambientLightOn)
        {
            shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        }
        else
        {
            shader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
        }

        if (diffuseLightOn)
        {
            shader.setVec3("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
        }
        else
        {
            shader.setVec3("dirLight.diffuse", 0.0f, 0.0f, 0.0f);
        }

        if (specularLightOn)
        {
            shader.setVec3("dirLight.specular", 0.3f, 0.3f, 0.3f);
        }
        else
        {
            shader.setVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        shader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
        shader.setVec3("dirLight.diffuse", 0.0f, 0.0f, 0.0f);
        shader.setVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);
    }

    if (pointLight1On)
    {
        if (ambientLightOn)
        {
            shader.setVec3("pointLights[0].ambient", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
        }

        if (diffuseLightOn)
        {
            shader.setVec3("pointLights[0].diffuse", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[0].diffuse", 0.0f, 0.0f, 0.0f);
        }

        if (specularLightOn)
        {
            shader.setVec3("pointLights[0].specular", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        shader.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
        shader.setVec3("pointLights[0].diffuse", 0.0f, 0.0f, 0.0f);
        shader.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
    }

    if (pointLight2On)
    {
        if (ambientLightOn)
        {
            shader.setVec3("pointLights[1].ambient", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
        }

        if (diffuseLightOn)
        {
            shader.setVec3("pointLights[1].diffuse", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[1].diffuse", 0.0f, 0.0f, 0.0f);
        }

        if (specularLightOn)
        {
            shader.setVec3("pointLights[1].specular", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[1].specular", 0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        shader.setVec3("pointLights[1].ambient", 0.0f, 0.0f, 0.0f);
        shader.setVec3("pointLights[1].diffuse", 0.0f, 0.0f, 0.0f);
        shader.setVec3("pointLights[1].specular", 0.0f, 0.0f, 0.0f);
    }

    if (pointLight3On)
    {
        if (ambientLightOn)
        {
            shader.setVec3("pointLights[2].ambient", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[2].ambient", 0.0f, 0.0f, 0.0f);
        }

        if (diffuseLightOn)
        {
            shader.setVec3("pointLights[2].diffuse", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 0.0f);
        }

        if (specularLightOn)
        {
            shader.setVec3("pointLights[2].specular", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        shader.setVec3("pointLights[2].ambient", 0.0f, 0.0f, 0.0f);
        shader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 0.0f);
        shader.setVec3("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
    }

    if (pointLight4On)
    {
        if (ambientLightOn)
        {
            shader.setVec3("pointLights[3].ambient", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[3].ambient", 0.0f, 0.0f, 0.0f);
        }

        if (diffuseLightOn)
        {
            shader.setVec3("pointLights[3].diffuse", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[3].diffuse", 0.0f, 0.0f, 0.0f);
        }

        if (specularLightOn)
        {
            shader.setVec3("pointLights[3].specular", 0.7f, 0.7f, 0.7f);
        }
        else
        {
            shader.setVec3("pointLights[3].specular", 0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        shader.setVec3("pointLights[3].ambient", 0.0f, 0.0f, 0.0f);
        shader.setVec3("pointLights[3].diffuse", 0.0f, 0.0f, 0.0f);
        shader.setVec3("pointLights[3].specular", 0.0f, 0.0f, 0.0f);
    }

    if (spotLightOn)
    {
        if (ambientLightOn)
        {
            shader.setVec3("spotLight.ambient", 1.0f, 1.0f, 1.0f);
        }
        else
        {
            shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        }

        if (diffuseLightOn)
        {
            shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        }
        else
        {
            shader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        }

        if (specularLightOn)
        {
            shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        }
        else
        {
            shader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        shader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        shader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
    }

}


       
void DrawCar(Shader shader,Shape Cube_necessary, Shape Cube_Car_Door, Shape Car_wheel, Shape Cube_Car_Top, Shape Cube_Car_Side3, Shape Cube_Car_Side2, Shape Cube_Car_Side, Shape Cube_Car_Front, Shape Cube_Car_Top_R, Shape Cube_Car_Back_window, Shape Cube_Car_Front_side, Shape Cube_Car_Front_window , mat4 identityMatrix,mat4 translateMatrixinit, mat4 translateMatrix, mat4 rotationMatrix, mat4 scaleMatrix, mat4 modelCenter, mat4 model, mat4 carWheelMatrix, mat4 rotationy, mat4 shearMatrix)
{

    //bottom lower
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, 1));
    model = carWheelMatrix * modelCenter * scaleMatrix * translateMatrixinit;
    Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,model);


    //a bottom-middle lower
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, .5));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, 0.125));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    //a middle lower
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.25, 0));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);




    //a top-middle lower
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .25, .5));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.375, 0.125));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);



    //a top lower
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .35, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.5, 0));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_necessary.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    float hudai = 0.05;
    float hudai2 = 0.23;
    mat4 rotating_in_own;

    //wheel



    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.185, 0.025+ hudai));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 25.0f, 0.08f));
    float rotateAngle = 90;
    rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));

    if (moveCar)
    {
        rotating_in_own = rotate(identityMatrix, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * rotating_in_own * scaleMatrix;
    }
    else
    {
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
    }
    Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.185, 0.4 + hudai));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 25.0f, 0.08f));
    rotateAngle = 90;
    rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    if (moveCar)
    {
        rotating_in_own = rotate(identityMatrix, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * rotating_in_own * scaleMatrix;
    }
    else
    {
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;

    }
    Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.435, 0.025 + hudai));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 25.0f, 0.08f));
    rotateAngle = 90;
    rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    if (moveCar)
    {
        rotating_in_own = rotate(identityMatrix, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * rotating_in_own * scaleMatrix;
    }
    else
    {
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
    }
    Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.435, 0.4 + hudai));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 25.0f, 0.08f));
    rotateAngle = 90;
    rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    if (moveCar)
    {
        rotating_in_own = rotate(identityMatrix, glm::radians(car_wheel_anglee), glm::vec3(0.0f, 1.0f, 0.0f));
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * rotating_in_own * scaleMatrix;
    }
    else
    {
        model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
    }
    Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    //
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.005, 0.185, 0.025 + hudai2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 200.75f, 0.01f));
    rotateAngle = 90;
    rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
    Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.005, 0.435, 0.025 + hudai2));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 200.75f, 0.01f));
    rotateAngle = 90;
    rotationMatrix = rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix;
    Car_wheel.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, 1.35, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0, 0));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Top.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);



    //a left back
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0, 0));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Side3.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    //a left middle
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.25, 0));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Side2.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    //a left top
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.5, 0));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Side.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    //a right back
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0, .5));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Side3.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    //a right middle
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .25, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.25, .5));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Side2.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    //right top
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .01));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.125, 0.5, .5));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Side.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    //a back lower to upper
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, 0.01, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0, 0));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Front.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    //a front lower to upper
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, 0.01, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.125, 0.675, 0));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Top_R.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.8, -4.75,-1.0));
    rotateAngle = 90;
    rotationy = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 3.0f, 3.0f)); //changed
    modelCenter = rotationy * scaleMatrix * translateMatrixinit;


    //car curve 1
    rotateAngle = 90;
    glm::mat4 rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateAngle = 180;
    glm::mat4 rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    rotateAngle = -90;
    glm::mat4 rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.245, 0.21,  -0.175));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.348f, 0.998f));  // Scale to flatten
    shearMatrix = glm::mat4(1.0f);
    shearMatrix[1][0] = 0.5f; // Apply shear along x-axis (y changes with x)

    model = carWheelMatrix * modelCenter * translateMatrix * rotateYMatrix *  shearMatrix * scaleMatrix ;
    Cube_Car_Back_window.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    

    ////car curve 2
    //
    rotateAngle = 270;
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.245, 0.21, -0.375));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.348f, 0.998f));  // Scale to flatten
    shearMatrix = identityMatrix;
    shearMatrix[1][0] = 1.2f; // Apply shear along x-axis (y changes with x)

    model = carWheelMatrix * modelCenter * translateMatrix * rotateYMatrix * shearMatrix * scaleMatrix;
    Cube_Car_Front_side.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    //car head
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .01, 0.605));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.3, -0.4));
    rotateAngle = 90;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
    Cube_Car_Top.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);
    //car window front

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .01, 0.525));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, -0.6));
    rotateAngle = -40;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    model = carWheelMatrix * modelCenter * translateMatrix * rotationMatrix * scaleMatrix * translateMatrixinit;
    Cube_Car_Front_window.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);


    //car door front 1
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01, 0.35, 0.60));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0.125, -0.4));
    rotateAngle = 90;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
    Cube_Car_Door.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    //car door front 2
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01, 0.35, 0.60));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5, 0.125, -0.4));
    rotateAngle = 90;
    rotationMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = carWheelMatrix * modelCenter * translateMatrix * scaleMatrix * rotationMatrix * translateMatrixinit;
    Cube_Car_Door.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, model);

    // cube.drawCubeWithTexture(shader);

}