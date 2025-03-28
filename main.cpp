// Naimur Rahman
// Roll: 1907031
// 3D Classroom Assignment

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "basic_camera.h"
#include "camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "cube.h"
#include "stb_image.h"
#include "bottle.h"
#include "glass.h"
#include "Stool.h"
#include "Sofa.h"
#include "sink.h"
#include "fan.h"
#include "Bush.h"
#include "Mug.h"
#include "Burger.h"
#include "Air.h"
#include "curve.h"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

using namespace std;
static bool qKeyPressed = false;
bool toggleTexture = true;
//function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawTableChair(unsigned int VAO, Shader lightingShaderWithTexture, glm::mat4 matrix, Cube c, Cube d);
void drawChair(unsigned int VAO, Shader lightingShaderWithTexture, glm::mat4 matrix, Cube c);
void drawFan(unsigned int VAO, Shader lightingShader, glm::mat4 matrix);
void drawCube(unsigned int& VAO, Shader& lightingShader, glm::mat4 model, glm::vec3 color);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

bool fan_turn = false;
float fanrot = 0.0f;

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//screen
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float aspectRatio = 4.0f / 3.0f;

//camera
float eyeX = 2.0f, eyeY = 1.0f, eyeZ = 13.5f;
float lookAtX = 2.0f, lookAtY = 1.5f, lookAtZ = 4.75f;
Camera camera(glm::vec3(eyeX, eyeY, eyeZ));

//timing
float deltaTime = 0.0f;     // time between current frame and last frame
float lastFrame = 0.0f;

float ts = 0.0f;


float robotZTranslation = 0.0f;
float oscillationAngle = 0.0f;  // Tracks the oscillation angle (in degrees)
float robotRotationY = 0.0f;       // Tracks robot's Y-axis rotation angle (in degrees)
float robotFixingArmLegRotation = 0.0f;
bool serve = false;
bool tex = true;
bool toggletex = false;
//bool fan_turn = false;
//float fanrot = 0.0f;
float scaleZ = 3.0f;
float sp = 0.0f;
float sp2 = 0.0f; // Starting rotation angle
bool rotateDirection = true;  // true for increasing, false for decreasing
int scaleDown = 2; // To track whether scaling down or up
float scaleStep = 0.01f; // Adjust step size as needed


bool rotateDirection2 = true;
//fan
float r = 0.0f;
bool fanOn = false;
bool doorOn = false;
bool TVOn = false;

//bird's eye view
bool birdEyeView = false;
glm::vec3 birdEyePosition(2.0f, 3.5f, 13.5f);
glm::vec3 birdEyeTarget(2.0f, 0.0f, 7.5f);
float birdEyeSpeed = 1.0f;

//rotation around a point
float theta = 0.0f; // Angle around the Y-axis
float radius = 2.0f;

//directional light
bool directionLightOn = true;
bool directionalAmbient = true;
bool directionalDiffuse = true;
bool directionalSpecular = true;

//spot light
bool spotLightOn = true;

//point light
bool point1 = true;
bool point2 = true;
bool point3 = true;

//custom projection matrix
float fov = glm::radians(camera.Zoom);
float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
float near = 0.1f;
float far = 100.0f;
float tanHalfFOV = tan(fov / 2.0f);

//doors and windows
bool openDoor = true;
float doorAngle = 90.0f;

//positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-2.9f,  2.0f,  5.0f),
    glm::vec3(6.7f,  2.0f,  5.0f),
    glm::vec3(8.9f, 3.2f, 11.0f),
   
};

PointLight pointlight1(
    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,       // position
    0.2f, 0.2f, 0.2f,       //ambient
    0.8f, 0.8f, 0.8f,       //diffuse
    0.2f, 0.2f, 0.2f,       //specular
    1.0f,       //k_c
    0.09f,      //k_l
    0.032f,     //k_q
    1       //light number
);

PointLight pointlight2(
    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,
    0.2f, 0.2f, 0.2f,
    0.8f, 0.8f, 0.8f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    2
);

PointLight pointlight3(
    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,
    0.2f, 0.2f, 0.2f,
    1.0f, 0.0f, 0.0f,
    0.2f, 0.2f, 0.2f,
    1.0f,
    0.09f,
    0.032f,
    2
);





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
    Sphere& sphere,
    Shader& lightingShaderWithTexture,
    unsigned int VAOdec,
    unsigned int diffMaps,
    unsigned int specMaps) {

    // Base z-position
    baseZ = 4.0f + robotZTranslation;

    // Oscillation parameters
    float armLegOscillation = sin(glm::radians(oscillationAngle)) * 30.0f; // Oscillation in degrees

    // Base model rotation about the Y-axis
    glm::mat4 baseModel = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Robot serving
    // Head
    glm::mat4 model = transformation(1.5f, 0.5f + 0.3f, baseZ, 0.0f, robotRotationY, 0.0f, 0.3f, 0.3f, 0.3f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);

    // Body
    model = transformation(1.5f, -0.8f + 0.3f, baseZ, 0.0f, robotRotationY, 0.0f, 0.1f, 1.0f, 0.1f);
    cylinder_custom(lightingShaderWithTexture, VAOdec, model);

    // Left leg
    model = baseModel * transformation(1.3f, -0.7f, baseZ, armLegOscillation, 0.0f, 0.0f, 0.05f, 0.3f, 0.05f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);

    // Right leg
    model = baseModel * transformation(1.7f, -0.7f, baseZ, -armLegOscillation, 0.0f, 0.0f, 0.05f, 0.3f, 0.05f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);

    // Left arm
    model = baseModel * transformation(1.08f, 0.1f, baseZ, armLegOscillation, 0.0f, -30.0f, 0.05f, 0.3f, 0.05f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);

    // Right arm
    model = baseModel * transformation(1.92f, 0.1f, baseZ, -armLegOscillation, 0.0f, 30.0f, 0.05f, 0.3f, 0.05f);
    sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps, specMaps);
}

void cylinder(Shader& ourShader, unsigned int& VAOdec, float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz)
{
    glm::mat4 model;
    model = transformation(tx, ty, tz, rx, ry, rz, sx, sy, sz);
    //glm::mat4 identityMatrix = glm::mat4(1.0f);
    //glm::mat4 rot = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    //model = model * rot;
    ourShader.setMat4("model", model);
    glBindVertexArray(VAOdec);
    glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
}

void drawFractalTree(Shader& shader, const glm::mat4& model, int depth, Cube& CubeGreen, const glm::mat4& a_translate_mat, Bush& Leaf_Obj, const glm::mat4& model2) {
    if (depth == 0) return;

    // Draw the current cube
    CubeGreen.drawCubeWithTexture(shader, a_translate_mat * model);
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
    glm::mat4 rightBranchforLeaf = translateRight * rotateY * rotateRight * model2;
    drawFractalTree(shader, rightBranchModel, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, rightBranchforLeaf);

    deg = 90;
    rotateY = rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    // Left branch 2
    translateLeft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.15f));
    glm::mat4 leftBranchModel2 = translateLeft * rotateY * rotateLeft * scaleMatrix * model;
    glm::mat4 leftBranchforLeaf2 = translateLeft * rotateY * rotateLeft * model2;
    drawFractalTree(shader, leftBranchModel2, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, leftBranchforLeaf2);
    translateRight = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.15f));
    glm::mat4 rightBranchModel2 = translateRight * rotateY * rotateRight * scaleMatrix * model;
    glm::mat4 rightBranchforLeaf2 = translateRight * rotateY * rotateRight * model2;
    drawFractalTree(shader, rightBranchModel2, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, rightBranchforLeaf2);

}

glm::mat4 Transformation(float tx, float ty, float tz,
    float rx, float ry, float rz,
    float sx, float sy, float sz) {
    // Identity Matrix
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    glm::mat4 rotateXMatrix = glm::rotate(translateMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateYMatrix = glm::rotate(rotateXMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotateZMatrix = glm::rotate(rotateYMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaleMatrix = glm::scale(translateMatrix, glm::vec3(sx, sy, sz));
    glm::mat4 sm = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    return sm;
}

int main()
{
    //glfw initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Restaurant", NULL, NULL);
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

    //tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //glad load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //configure global opengl state
    glEnable(GL_DEPTH_TEST);

    //build and compile our shader program
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    //Shader constantShader("vertexShader.vs", "fragmentShaderV2.fs");


    float fan_holder[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,

        0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f
    };
    //.44f, .22f, .05f,
    float fan_pivot[] = {
        0.0f, 0.0f, 0.0f,    .1569f, .2f, .2902f,
        0.5f, 0.0f, 0.0f,    .1569f, .2f, .2902f,
        0.5f, 0.5f, 0.0f,     .1569f, .2f, .2902f,
        0.0f, 0.5f, 0.0f,    .1569f, .2f, .2902f,

        0.5f, 0.0f, 0.0f,    .1569f, .2f, .2902f,
        0.5f, 0.5f, 0.0f,    .1569f, .2f, .2902f,
        0.5f, 0.0f, 0.5f,    .1569f, .2f, .2902f,
        0.5f, 0.5f, 0.5f,    .1569f, .2f, .2902f,

        0.0f, 0.0f, 0.5f,     .1569f, .2f, .2902f,
        0.5f, 0.0f, 0.5f,    .1569f, .2f, .2902f,
        0.5f, 0.5f, 0.5f,    .1569f, .2f, .2902f,
        0.0f, 0.5f, 0.5f,    .1569f, .2f, .2902f,

        0.0f, 0.0f, 0.5f,     .1569f, .2f, .2902f,
        0.0f, 0.5f, 0.5f,     .1569f, .2f, .2902f,
        0.0f, 0.5f, 0.0f,    .1569f, .2f, .2902f,
        0.0f, 0.0f, 0.0f,     .1569f, .2f, .2902f,

        0.5f, 0.5f, 0.5f,     .1569f, .2f, .2902f,
        0.5f, 0.5f, 0.0f,     .1569f, .2f, .2902f,
        0.0f, 0.5f, 0.0f,    .1569f, .2f, .2902f,
        0.0f, 0.5f, 0.5f,     .1569f, .2f, .2902f,

        0.0f, 0.0f, 0.0f,     .1569f, .2f, .2902f,
        0.5f, 0.0f, 0.0f,     .1569f, .2f, .2902f,
        0.5f, 0.0f, 0.5f,    .1569f, .2f, .2902f,
        0.0f, 0.0f, 0.5f,    .1569f, .2f, .2902f,
    };
    //.0f, .0f, .42f,
    float fan_blade[] = {
        0.0f, 0.0f, 0.0f,   .49f, .7058f, .424f,
        0.5f, 0.0f, 0.0f,    .49f, .7058f, .424f,
        0.5f, 0.5f, 0.0f,    .49f, .7058f, .424f,
        0.0f, 0.5f, 0.0f,    .49f, .7058f, .424f,

        0.5f, 0.0f, 0.0f,    .49f, .7058f, .424f,
        0.5f, 0.5f, 0.0f,    .49f, .7058f, .424f,
        0.5f, 0.0f, 0.5f,    .49f, .7058f, .424f,
        0.5f, 0.5f, 0.5f,    .49f, .7058f, .424f,

        0.0f, 0.0f, 0.5f,    .49f, .7058f, .424f,
        0.5f, 0.0f, 0.5f,   .49f, .7058f, .424f,
        0.5f, 0.5f, 0.5f,    .49f, .7058f, .424f,
        0.0f, 0.5f, 0.5f,    .49f, .7058f, .424f,

        0.0f, 0.0f, 0.5f,    .49f, .7058f, .424f,
        0.0f, 0.5f, 0.5f,    .49f, .7058f, .424f,
        0.0f, 0.5f, 0.0f,    .49f, .7058f, .424f,
        0.0f, 0.0f, 0.0f,    .49f, .7058f, .424f,

        0.5f, 0.5f, 0.5f,    .49f, .7058f, .424f,
        0.5f, 0.5f, 0.0f,    .49f, .7058f, .424f,
        0.0f, 0.5f, 0.0f,    .49f, .7058f, .424f,
        0.0f, 0.5f, 0.5f,    .49f, .7058f, .424f,

        0.0f, 0.0f, 0.0f,    .49f, .7058f, .424f,
        0.5f, 0.0f, 0.0f,    .49f, .7058f, .424f,
        0.5f, 0.0f, 0.5f,    .49f, .7058f, .424f,
        0.0f, 0.0f, 0.5f,    .49f, .7058f, .424f,
    };

    vector<float> controlPoints =
    {
        0.1300, 2.0200, 5.1000,
        -0.3600, 2.0250, 5.1000,
        -0.7350, 1.8650, 5.1000,
        -0.8300, 1.5400, 5.1000,
        -0.6350, 1.2200, 5.1000,
        -0.5250, 0.9850, 5.1000,
        -0.4950, 0.6400, 5.1000,
        -0.6550, 0.4600, 5.1000,
        -0.9600, 0.3300, 5.1000
    };

    vector<float> ctrlBowl1 =
    {
        0.2350, 1.8650, 5.1000,
        0.0800, 1.8850, 5.1000,
        -0.1300, 1.9000, 5.1000,
        -0.3100, 1.8450, 5.1000,
        -0.4000, 1.7000, 5.1000
    };

    vector<float> ctrlVase = {
    0.0550, 2.0150, 5.1000,
    -0.0700, 2.0250, 5.1000,
    -0.2750, 2.0300, 5.1000,
    -0.4150, 1.9400, 5.1000,
    -0.4250, 1.7900, 5.1000,
    -0.3300, 1.5950, 5.1000,
    -0.2450, 1.4450, 5.1000,
    -0.2150, 1.3100, 5.1000,
    -0.2150, 1.1900, 5.1000,
    -0.2050, 1.0800, 5.1000,
    -0.2100, 0.9350, 5.1000,
    -0.2300, 0.7800, 5.1000,
    -0.2850, 0.6750, 5.1000
    };

    vector<float> ctrlBowl2 = {
        0.2450, 2.0050, 5.1000,
        0.1200, 2.0050, 5.1000,
        0.0200, 2.0100, 5.1000,
        -0.1300, 2.0300, 5.1000,
        -0.2800, 2.0150, 5.1000,
        -0.3850, 2.0150, 5.1000,
        -0.3900, 1.9200, 5.1000,
        -0.3900, 1.7700, 5.1000,
        -0.3800, 1.6050, 5.1000
    };



    //set up vertex data (and buffer(s)) and configure vertex attributes

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



    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
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


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    //second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int VBOF1, VAOF1, EBOF1;
    glGenVertexArrays(1, &VAOF1);
    glGenBuffers(1, &VBOF1);
    glGenBuffers(1, &EBOF1);
    glBindVertexArray(VAOF1);
    glBindBuffer(GL_ARRAY_BUFFER, VBOF1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fan_holder), fan_holder, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    unsigned int VBOF2, VAOF2, EBOF2;
    glGenVertexArrays(1, &VAOF2);
    glGenBuffers(1, &VBOF2);
    glGenBuffers(1, &EBOF2);
    glBindVertexArray(VAOF2);
    glBindBuffer(GL_ARRAY_BUFFER, VBOF2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fan_pivot), fan_pivot, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    unsigned int VBOF3, VAOF3, EBOF3;
    glGenVertexArrays(1, &VAOF3);
    glGenBuffers(1, &VBOF3);
    glGenBuffers(1, &EBOF3);
    glBindVertexArray(VAOF3);
    glBindBuffer(GL_ARRAY_BUFFER, VBOF3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fan_blade), fan_blade, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //ourShader.use();
    //constantShader.use();
    /**/
    //floor texture
    string diffuseMapPaths = "yellow.jpeg";
    string specularMapPaths = "yellow.jpeg";

    // Load textures
    unsigned int diffMaps = loadTexture(diffuseMapPaths.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMaps = loadTexture(specularMapPaths.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Create a sphere object
    float radius = 1.0f;
    int sectorCount = 36;
    int stackCount = 18;
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 32.0f;

    Sphere sphere(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess);

    //sphere2
    string diffuseMapPaths2 = "dis.jpg";
    string specularMapPaths2 = "dis.jpg";
    unsigned int diffMaps2 = loadTexture(diffuseMapPaths2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMaps2 = loadTexture(specularMapPaths2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere sphere2(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess);
    


    //plate
    string diffuseMapPathsplate = "white.jpeg";
    string specularMapPathsplate = "white.jpeg";
    unsigned int diffMapsplate = loadTexture(diffuseMapPathsplate.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapsplate = loadTexture(specularMapPathsplate.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere sphere3(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess);


    string diffuseMapPathB = "redglass.png";
    string specularMapPathB = "redglass.png";
    unsigned int diffMapB = loadTexture(diffuseMapPathB.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapB = loadTexture(specularMapPathB.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Bottle bottle = Bottle(diffMapB, specMapB, 32.0f);

    //sofa
    string diffuseMapPathSofa = "blue.jpeg";
    string specularMapPathSofa = "blue.jpeg";
    unsigned int diffMapSofa = loadTexture(diffuseMapPathSofa.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapSofa = loadTexture(specularMapPathSofa.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sofa sofa = Sofa(diffMapSofa, specMapSofa, 32.0f);


    //sofa
    string diffuseMapPathSink = "white.jpeg";
    string specularMapPathSink = "white.jpeg";
    unsigned int diffMapSink = loadTexture(diffuseMapPathSink.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapSink = loadTexture(specularMapPathSink.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sink sink = Sink(diffMapSink, specMapSofa, 32.0f);

    //glass
    string diffuseMapPathG = "glass.jpg";
    string specularMapPathG = "glass.jpg";
    unsigned int diffMapG = loadTexture(diffuseMapPathG.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapG = loadTexture(specularMapPathG.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Glass glass = Glass(diffMapG, specMapG, 32.0f);


    //Stool
    string diffuseMapPathStool = "stool_texture.png";
    string specularMapPathStool = "stool_texture.png";
    unsigned int diffMapStool= loadTexture(diffuseMapPathStool.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapStool = loadTexture(specularMapPathStool.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Stool stool = Stool(diffMapStool, specMapStool, 32.0f);


    //Mug
    string diffuseMapPathMug = "white.jpeg";
    string specularMapPathMug = "white.jpeg";
    unsigned int diffMapMug = loadTexture(diffuseMapPathMug.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapMug = loadTexture(specularMapPathMug.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Mug mug = Mug(diffMapMug, specMapMug, 32.0f);
    //fries
    string diffuseMapPathF = "burger.png";
    string specularMapPathF = "burger.png";
    unsigned int diffMapF= loadTexture(diffuseMapPathF.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapF = loadTexture(specularMapPathF.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Burger burger = Burger(diffMapF, specMapF, 32.0f);


    string diffuseMapPathB2 = "glass.jpg";
    string specularMapPathB2 = "glass.jpg";
    unsigned int diffMapB2 = loadTexture(diffuseMapPathB2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapB2 = loadTexture(specularMapPathB2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Bottle bottle2 = Bottle(diffMapB2, specMapB2, 32.0f);


    string diffuseMapPath = "floortiles.jpeg";
    string specularMapPath = "floortiles.jpeg";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.25f, 2.25f);

    //roof texture
    string diffuseMapPath2 = "classroom_roof.jpg";
    string specularMapPath2 = "classroom_roof.jpg";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 2.5f, 4.5f);

    Bush Leaf_Obj = Bush(diffMap2, specMap2, 32.0f);

    //wall texture
    string diffuseMapPath3 = "classroom_wall.jpg";
    string specularMapPath3 = "classroom_wall.jpg";
    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube3 = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //Air
    string diffuseMapPathair = "air.png";
    string specularMapPathair = "air.png";
    unsigned int diffMapair = loadTexture(diffuseMapPathair.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapair = loadTexture(specularMapPathair.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Air air = Air(diffMapair, diffMapair, 32.0f);



    string diffuseMapPathwas = "whitetiles.jpg";
    string specularMapPathwas = "whitetiles.jpg";
    unsigned int diffMapwas = loadTexture(diffuseMapPathwas.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapwas = loadTexture(specularMapPathwas.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cubewas = Cube(diffMapwas, specMapwas, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //backwall
    string diffuseMapPathwall = "back.jpg";
    string specularMapPathwall = "back.jpg";
    unsigned int diffMapwall = loadTexture(diffuseMapPathwall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapwall =loadTexture(specularMapPathwall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube wall = Cube(diffMapwall, specMapwall, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //clock
    string diffuseMapPathclock = "clock.jpg";
    string specularMapPathclock = "clock.jpg";
    unsigned int diffMapclock = loadTexture(diffuseMapPathclock.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapclock = loadTexture(specularMapPathclock.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube clock = Cube(diffMapclock, specMapclock, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //TV
    string diffuseMapPathTV1 = "tiger.jpg";
    string specularMapPathTV1 = "tiger.jpg";
    unsigned int diffMapTV1 = loadTexture(diffuseMapPathTV1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV1 = loadTexture(specularMapPathTV1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV1 = Cube(diffMapTV1, specMapTV1, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV2 = "frame_001.png";
    string specularMapPathTV2 = "frame_001.png";
    unsigned int diffMapTV2 = loadTexture(diffuseMapPathTV2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV2 = loadTexture(specularMapPathTV2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV2 = Cube(diffMapTV2, specMapTV2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV3 = "frame_002.png";
    string specularMapPathTV3 = "frame_002.png";
    unsigned int diffMapTV3 = loadTexture(diffuseMapPathTV3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV3 = loadTexture(specularMapPathTV3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV3 = Cube(diffMapTV3, specMapTV3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV4 = "frame_003.png";
    string specularMapPathTV4 = "frame_003.png";
    unsigned int diffMapTV4 = loadTexture(diffuseMapPathTV4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV4 = loadTexture(specularMapPathTV4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV4 = Cube(diffMapTV4, specMapTV4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV5 = "frame_004.png";
    string specularMapPathTV5 = "frame_004.png";
    unsigned int diffMapTV5 = loadTexture(diffuseMapPathTV5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV5 = loadTexture(specularMapPathTV5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV5 = Cube(diffMapTV5, specMapTV5, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV6 = "frame_005.png";
    string specularMapPathTV6 = "frame_005.png";
    unsigned int diffMapTV6 = loadTexture(diffuseMapPathTV6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV6 = loadTexture(specularMapPathTV6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV6 = Cube(diffMapTV6, specMapTV6, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV7 = "frame_006.png";
    string specularMapPathTV7 = "frame_006.png";
    unsigned int diffMapTV7 = loadTexture(diffuseMapPathTV7.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV7 = loadTexture(specularMapPathTV7.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV7 = Cube(diffMapTV7, specMapTV7, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV8 = "frame_007.png";
    string specularMapPathTV8 = "frame_007.png";
    unsigned int diffMapTV8 = loadTexture(diffuseMapPathTV8.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV8 = loadTexture(specularMapPathTV8.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV8 = Cube(diffMapTV8, specMapTV8, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV9 = "frame_008.png";
    string specularMapPathTV9 = "frame_008.png";
    unsigned int diffMapTV9 = loadTexture(diffuseMapPathTV9.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV9 = loadTexture(specularMapPathTV9.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV9 = Cube(diffMapTV9, specMapTV9, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV10 = "frame_009.png";
    string specularMapPathTV10 = "frame_009.png";
    unsigned int diffMapTV10 = loadTexture(diffuseMapPathTV10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV10 = loadTexture(specularMapPathTV10.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV10 = Cube(diffMapTV10, specMapTV10, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV11 = "frame_010.png";
    string specularMapPathTV11 = "frame_010.png";
    unsigned int diffMapTV11 = loadTexture(diffuseMapPathTV11.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV11 = loadTexture(specularMapPathTV11.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV11 = Cube(diffMapTV11, specMapTV11, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV12 = "frame_011.png";
    string specularMapPathTV12 = "frame_011.png";
    unsigned int diffMapTV12 = loadTexture(diffuseMapPathTV12.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV12 = loadTexture(specularMapPathTV12.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV12 = Cube(diffMapTV12, specMapTV12, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV13 = "frame_012.png";
    string specularMapPathTV13 = "frame_012.png";
    unsigned int diffMapTV13 = loadTexture(diffuseMapPathTV13.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV13 = loadTexture(specularMapPathTV13.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV13 = Cube(diffMapTV13, specMapTV13, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV14 = "frame_013.png";
    string specularMapPathTV14 = "frame_013.png";
    unsigned int diffMapTV14 = loadTexture(diffuseMapPathTV14.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV14 = loadTexture(specularMapPathTV14.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV14 = Cube(diffMapTV14, specMapTV14, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV15 = "frame_014.png";
    string specularMapPathTV15 = "frame_014.png";
    unsigned int diffMapTV15 = loadTexture(diffuseMapPathTV15.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV15 = loadTexture(specularMapPathTV15.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV15 = Cube(diffMapTV15, specMapTV15, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV16 = "frame_015.png";
    string specularMapPathTV16 = "frame_015.png";
    unsigned int diffMapTV16 = loadTexture(diffuseMapPathTV16.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV16 = loadTexture(specularMapPathTV16.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV16 = Cube(diffMapTV16, specMapTV16, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV17 = "frame_016.png";
    string specularMapPathTV17 = "frame_016.png";
    unsigned int diffMapTV17 = loadTexture(diffuseMapPathTV17.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV17 = loadTexture(specularMapPathTV17.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV17 = Cube(diffMapTV17, specMapTV17, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV18 = "frame_017.png";
    string specularMapPathTV18 = "frame_017.png";
    unsigned int diffMapTV18 = loadTexture(diffuseMapPathTV18.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV18 = loadTexture(specularMapPathTV18.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV18 = Cube(diffMapTV18, specMapTV18, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV19 = "frame_018.png";
    string specularMapPathTV19 = "frame_018.png";
    unsigned int diffMapTV19 = loadTexture(diffuseMapPathTV19.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV19 = loadTexture(specularMapPathTV19.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV19 = Cube(diffMapTV19, specMapTV19, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV20 = "frame_019.png";
    string specularMapPathTV20 = "frame_019.png";
    unsigned int diffMapTV20 = loadTexture(diffuseMapPathTV20.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV20 = loadTexture(specularMapPathTV20.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV20 = Cube(diffMapTV20, specMapTV20, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV21 = "frame_020.png";
    string specularMapPathTV21 = "frame_020.png";
    unsigned int diffMapTV21 = loadTexture(diffuseMapPathTV21.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV21 = loadTexture(specularMapPathTV21.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV21 = Cube(diffMapTV21, specMapTV21, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV22 = "frame_021.png";
    string specularMapPathTV22 = "frame_021.png";
    unsigned int diffMapTV22 = loadTexture(diffuseMapPathTV22.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV22 = loadTexture(specularMapPathTV22.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV22 = Cube(diffMapTV22, specMapTV22, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV23 = "frame_022.png";
    string specularMapPathTV23 = "frame_022.png";
    unsigned int diffMapTV23 = loadTexture(diffuseMapPathTV23.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV23 = loadTexture(specularMapPathTV23.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV23 = Cube(diffMapTV23, specMapTV23, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV24 = "frame_023.png";
    string specularMapPathTV24 = "frame_023.png";
    unsigned int diffMapTV24 = loadTexture(diffuseMapPathTV24.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV24 = loadTexture(specularMapPathTV24.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV24 = Cube(diffMapTV24, specMapTV24, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV25 = "frame_024.png";
    string specularMapPathTV25 = "frame_024.png";
    unsigned int diffMapTV25 = loadTexture(diffuseMapPathTV25.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV25 = loadTexture(specularMapPathTV25.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV25 = Cube(diffMapTV25, specMapTV25, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV26 = "frame_025.png";
    string specularMapPathTV26 = "frame_025.png";
    unsigned int diffMapTV26 = loadTexture(diffuseMapPathTV26.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV26 = loadTexture(specularMapPathTV26.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV26 = Cube(diffMapTV26, specMapTV26, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV27 = "frame_026.png";
    string specularMapPathTV27 = "frame_026.png";
    unsigned int diffMapTV27 = loadTexture(diffuseMapPathTV27.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV27 = loadTexture(specularMapPathTV27.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV27 = Cube(diffMapTV27, specMapTV27, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV28 = "frame_027.png";
    string specularMapPathTV28 = "frame_027.png";
    unsigned int diffMapTV28 = loadTexture(diffuseMapPathTV28.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV28 = loadTexture(specularMapPathTV28.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV28 = Cube(diffMapTV28, specMapTV28, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV29 = "frame_028.png";
    string specularMapPathTV29 = "frame_028.png";
    unsigned int diffMapTV29 = loadTexture(diffuseMapPathTV29.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV29 = loadTexture(specularMapPathTV29.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV29 = Cube(diffMapTV29, specMapTV29, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV30 = "frame_029.png";
    string specularMapPathTV30 = "frame_029.png";
    unsigned int diffMapTV30 = loadTexture(diffuseMapPathTV30.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV30 = loadTexture(specularMapPathTV30.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV30 = Cube(diffMapTV30, specMapTV30, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV31 = "frame_030.png";
    string specularMapPathTV31 = "frame_030.png";
    unsigned int diffMapTV31 = loadTexture(diffuseMapPathTV31.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV31 = loadTexture(specularMapPathTV31.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV31 = Cube(diffMapTV31, specMapTV31, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV32 = "frame_031.png";
    string specularMapPathTV32 = "frame_031.png";
    unsigned int diffMapTV32 = loadTexture(diffuseMapPathTV32.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV32 = loadTexture(specularMapPathTV32.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV32 = Cube(diffMapTV32, specMapTV32, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV33 = "frame_032.png";
    string specularMapPathTV33 = "frame_032.png";
    unsigned int diffMapTV33 = loadTexture(diffuseMapPathTV33.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV33 = loadTexture(specularMapPathTV33.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV33 = Cube(diffMapTV33, specMapTV33, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV34 = "frame_033.png";
    string specularMapPathTV34 = "frame_033.png";
    unsigned int diffMapTV34 = loadTexture(diffuseMapPathTV34.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV34 = loadTexture(specularMapPathTV34.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV34 = Cube(diffMapTV34, specMapTV34, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV35 = "frame_034.png";
    string specularMapPathTV35 = "frame_034.png";
    unsigned int diffMapTV35 = loadTexture(diffuseMapPathTV35.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV35 = loadTexture(specularMapPathTV35.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV35 = Cube(diffMapTV35, specMapTV35, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV36 = "frame_035.png";
    string specularMapPathTV36 = "frame_035.png";
    unsigned int diffMapTV36 = loadTexture(diffuseMapPathTV36.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV36 = loadTexture(specularMapPathTV36.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV36 = Cube(diffMapTV36, specMapTV36, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV37 = "frame_036.png";
    string specularMapPathTV37 = "frame_036.png";
    unsigned int diffMapTV37 = loadTexture(diffuseMapPathTV37.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV37 = loadTexture(specularMapPathTV37.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV37 = Cube(diffMapTV37, specMapTV37, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV38 = "frame_037.png";
    string specularMapPathTV38 = "frame_037.png";
    unsigned int diffMapTV38 = loadTexture(diffuseMapPathTV38.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV38 = loadTexture(specularMapPathTV38.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV38 = Cube(diffMapTV38, specMapTV38, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV39 = "frame_038.png";
    string specularMapPathTV39 = "frame_038.png";
    unsigned int diffMapTV39 = loadTexture(diffuseMapPathTV39.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV39 = loadTexture(specularMapPathTV39.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV39 = Cube(diffMapTV39, specMapTV39, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV40 = "frame_039.png";
    string specularMapPathTV40 = "frame_039.png";
    unsigned int diffMapTV40 = loadTexture(diffuseMapPathTV40.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV40 = loadTexture(specularMapPathTV40.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV40 = Cube(diffMapTV40, specMapTV40, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV41 = "frame_040.png";
    string specularMapPathTV41 = "frame_040.png";
    unsigned int diffMapTV41 = loadTexture(diffuseMapPathTV41.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV41 = loadTexture(specularMapPathTV41.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV41 = Cube(diffMapTV41, specMapTV41, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV42 = "frame_041.png";
    string specularMapPathTV42 = "frame_041.png";
    unsigned int diffMapTV42 = loadTexture(diffuseMapPathTV42.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV42 = loadTexture(specularMapPathTV42.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV42 = Cube(diffMapTV42, specMapTV42, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV43 = "frame_042.png";
    string specularMapPathTV43 = "frame_042.png";
    unsigned int diffMapTV43 = loadTexture(diffuseMapPathTV43.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV43 = loadTexture(specularMapPathTV43.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV43 = Cube(diffMapTV43, specMapTV43, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV44 = "frame_043.png";
    string specularMapPathTV44 = "frame_043.png";
    unsigned int diffMapTV44 = loadTexture(diffuseMapPathTV44.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV44 = loadTexture(specularMapPathTV44.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV44 = Cube(diffMapTV44, specMapTV44, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV45 = "frame_044.png";
    string specularMapPathTV45 = "frame_044.png";
    unsigned int diffMapTV45 = loadTexture(diffuseMapPathTV45.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV45 = loadTexture(specularMapPathTV45.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV45 = Cube(diffMapTV45, specMapTV45, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV46 = "frame_045.png";
    string specularMapPathTV46 = "frame_045.png";
    unsigned int diffMapTV46 = loadTexture(diffuseMapPathTV46.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV46 = loadTexture(specularMapPathTV46.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV46 = Cube(diffMapTV46, specMapTV46, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV47 = "frame_046.png";
    string specularMapPathTV47 = "frame_046.png";
    unsigned int diffMapTV47 = loadTexture(diffuseMapPathTV47.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV47 = loadTexture(specularMapPathTV47.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV47 = Cube(diffMapTV47, specMapTV47, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV48 = "frame_047.png";
    string specularMapPathTV48 = "frame_047.png";
    unsigned int diffMapTV48 = loadTexture(diffuseMapPathTV48.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV48 = loadTexture(specularMapPathTV48.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV48 = Cube(diffMapTV48, specMapTV48, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV49 = "frame_048.png";
    string specularMapPathTV49 = "frame_048.png";
    unsigned int diffMapTV49 = loadTexture(diffuseMapPathTV49.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV49 = loadTexture(specularMapPathTV49.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV49 = Cube(diffMapTV49, specMapTV49, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV50 = "frame_049.png";
    string specularMapPathTV50 = "frame_049.png";
    unsigned int diffMapTV50 = loadTexture(diffuseMapPathTV50.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV50 = loadTexture(specularMapPathTV50.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV50 = Cube(diffMapTV50, specMapTV50, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPathTV51 = "frame_050.png";
    string specularMapPathTV51 = "frame_050.png";
    unsigned int diffMapTV51 = loadTexture(diffuseMapPathTV51.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapTV51 = loadTexture(specularMapPathTV51.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube TV51 = Cube(diffMapTV51, specMapTV51, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);







    std::vector<Cube> tvs = { TV1, TV2, TV3, TV4, TV5, TV6, TV7, TV8, TV9, TV10,
                              TV11, TV12, TV13, TV14, TV15, TV16, TV17, TV18, TV19, TV20,
                              TV21, TV22, TV23, TV24, TV25, TV26, TV27, TV28, TV29, TV30,
                              TV31, TV32, TV33, TV34, TV35, TV36, TV37, TV38, TV39, TV40,
                              TV41, TV42, TV43, TV44, TV45, TV46, TV47, TV48, TV49, TV50 };



    //wood texture
    string diffuseMapPath4 = "wood.jpg";
    string specularMapPath4 = "wood.jpg";
    unsigned int diffMap4 = loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap4 = loadTexture(specularMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube4 = Cube(diffMap4, specMap4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //wood texture 2
    string diffuseMapPath5 = "wood2.jpg";
    string specularMapPath5 = "wood2.jpg";
    unsigned int diffMap5 = loadTexture(diffuseMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap5 = loadTexture(specularMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube5 = Cube(diffMap5, specMap5, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //wood texture 2
    string diffuseMapPathchair = "chair.jpeg";
    string specularMapPathchair = "chair.jpeg";
    unsigned int diffMapchair = loadTexture(diffuseMapPathchair.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapchair = loadTexture(specularMapPathchair.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube chair = Cube(diffMapchair, specMapchair, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //door and window texture
    string diffuseMapPath6 = "window.png";
    string specularMapPath6 = "window.png";
    unsigned int diffMap6 = loadTexture(diffuseMapPath6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap6 = loadTexture(specularMapPath6.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube6 = Cube(diffMap6, specMap6, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapwin = "door.png";
    string specularMapwin = "door.png";
    unsigned int diffwin = loadTexture(diffuseMapwin.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specwin = loadTexture(specularMapwin.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube win = Cube(diffwin, specwin, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "popcornbowl.png";
    specularMapPath = "popcornbowl.png";
    unsigned int popcornbowldiff = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int popcornbowlspec = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    r = 0.0f;
  // Starts with textures enabled

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input
        processInput(window);

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //ourShader.use();
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        //point lights set up
        pointlight1.setUpPointLight(lightingShader);
        pointlight2.setUpPointLight(lightingShader);

        //directional light set up
        lightingShader.setVec3("directionalLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShader.setVec3("directionalLight.ambient", 0.1f, 0.1f, 0.1f);
        lightingShader.setVec3("directionalLight.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("directionalLight.specular", 0.0f, 0.0f, 0.0f);
        lightingShader.setBool("directionLightOn", directionLightOn);

        //spot light set up
        lightingShader.setVec3("spotLight.position", 2.0f, 2.0f, 2.0f);
        lightingShader.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShader.setVec3("spotLight.ambient", 1.0f, 1.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f,1.0f, 0.0f);
        lightingShader.setVec3("spotLight.specular", 0.2f, 0.2f, 0.2f);
        lightingShader.setFloat("spotLight.k_c", 1.0f);
        lightingShader.setFloat("spotLight.k_l", 0.09);
        lightingShader.setFloat("spotLight.k_q", 0.032);
        //lightingShader.setFloat("spotLight.cos_theta", glm::cos(glm::radians(60.0f)));
        lightingShader.setFloat("spotLight.inner_circle", glm::cos(glm::radians(7.5f)));
        lightingShader.setFloat("spotLight.outer_circle", glm::cos(glm::radians(15.0f)));
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

        glm::mat4 projection(0.0f);
        projection[0][0] = 1.0f / (aspect * tanHalfFOV);
        projection[1][1] = 1.0f / tanHalfFOV;
        projection[2][2] = -(far + near) / (far - near);
        projection[2][3] = -1.0f;
        projection[3][2] = -(2.0f * far * near) / (far - near);
        //pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        //ourShader.setMat4("projection", projection);
        //constantShader.setMat4("projection", projection);
        lightingShader.setMat4("projection", projection);

        //camera view transformation
        //constantShader.setMat4("view", view);
        //ourShader.setMat4("view", view);

        glm::mat4 view;

        //check for bird's eye view and normal view
        if (birdEyeView) {
            glm::vec3 up(0.0f, 1.0f, 0.0f);
            view = glm::lookAt(birdEyePosition, birdEyeTarget, up);
        }
        else {
            view = camera.GetViewMatrix();
        }

        lightingShader.setMat4("view", view);

        //define matrices and vectors needed
        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, RotateTranslateMatrix, InvRotateTranslateMatrix;
        glm::vec3 color;

        //initialize all elements as non-emissive
        lightingShader.setVec3("material.emissive", glm::vec3(0.0f, 0.0f, 0.0f));


        float z = 0.0f;

        //fan
        model = transformation(2.25f, 2.5f, -0.8f, 0.0f, 0.0f, 0.0f, .01f, 1.1f, .01f);
        cylinder_custom(lightingShaderWithTexture, VAOdec, model);

        model = transformation(2.25f, 2.45f, -0.8f, 0.0f, fanrot, 0.0f, .05f, .05f, .05f);
        cylinder_custom(lightingShaderWithTexture, VAOdec, model);

        model = transformation(2.25f, 2.45f, -0.8f, 0.0f, fanrot, 0.0f, .05f, .05f, .05f);
        cylinder_custom(lightingShaderWithTexture, VAOdec, model);

        Fan fan;
        fan.toz += 5.0;
        ourShader = fan.local_rotation(ourShader, VAOF3, fanrot);
        if (fan_turn)
            fanrot += 10.0f;
      

        Curve bowl(ctrlBowl1, popcornbowldiff, popcornbowlspec, 1.0f);
        glm:: mat4 sm = Transformation(0.9, 3.22, 2.1, 180, 0, 0, 0.66, 1.66, 1.33);
        bowl.draw(lightingShaderWithTexture, sm, glm::vec3(1.0, 0.0, 1.0));


        robot_key_move(sphere, lightingShaderWithTexture, VAOdec, diffMaps, specMaps);
    
        //// Tree
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 2.0f, 0.25f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, -1.0, -2.0));
        glm::mat4 rootModel = scaleMatrix ;
        // Draw fractal tree
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 2.0f, 4.0f));

        //glm::mat4 rootModel2 = translate(identityMatrix, glm::vec3(-1.0f, 1.0f,-1.0f))  * scaleMatrix * translateMatrixinit;
        //Leaf_Obj.drawWithTexture(shader, rootModel2);
        //glm::mat4 rootModel2 = translate(identityMatrix, glm::vec3(-0.8f, -0.65f, 0.0f)) * scaleMatrix;
        glm::mat4 rootModel2 = translate(identityMatrix, glm::vec3(0.0f, -0.75f, 0.0f)) * scaleMatrix;
        drawFractalTree(lightingShaderWithTexture, rootModel, 4, cube3, translateMatrix, Leaf_Obj, rootModel2); // Change depth as needed

        //11.1f, -1.0f, 8.8f
        // 0.1f, 4.5f, 5.1f
        //whiteboard
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.9f, 1.0f, 9.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.0f, 4.1));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        drawCube(VAO, lightingShader, model, color);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.8f, 1.0f, 9.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 4.1));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        drawCube(VAO, lightingShader, model, color);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.8f, 2.0f, 9.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 4.1));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        drawCube(VAO, lightingShader, model, color);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.8f, 3.0f, 9.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1f, 0.1f, 4.1));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        drawCube(VAO, lightingShader, model, color);

        //bottle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 1.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.035, .4, .035));
        model = translateMatrix * scaleMatrix;
        bottle.drawWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 1.0f, 11.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.035, .4, .035));
        model = translateMatrix * scaleMatrix;
        bottle.drawWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 1.0f, 12.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.035, .4, .035));
        model = translateMatrix * scaleMatrix;
        bottle.drawWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 1.0f, 13.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.035, .4, .035));
        model = translateMatrix * scaleMatrix;
        bottle.drawWithTexture(lightingShaderWithTexture, model);

        //wineglass
         //glass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 2.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.75, .99, .75));
        model = translateMatrix * scaleMatrix;
        glass.drawWithTexture(lightingShaderWithTexture, model);





        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 2.0f, 11.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.75, .99, .75));
        model = translateMatrix * scaleMatrix;
        glass.drawWithTexture(lightingShaderWithTexture, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 2.0f, 12.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.75, .99, .75));
        model = translateMatrix * scaleMatrix;
        glass.drawWithTexture(lightingShaderWithTexture, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 2.0f, 13.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.75, .99, .75));
        model = translateMatrix * scaleMatrix;
        glass.drawWithTexture(lightingShaderWithTexture, model);

       



        //draw fans with rotations
  /*      z = 0.0f;
        for (int i = 0; i < 2; i++) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.7f, 0.0f, z));
            drawFan(VAO, lightingShader, translateMatrix);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.33f, 0.0f, z));
            drawFan(VAO, lightingShader, translateMatrix);

            z += 4.5;
        }*/

        //draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);

        for (unsigned int i = 0; i < 2; i++)
        {
            translateMatrix = glm::translate(identityMatrix, pointLightPositions[i]);
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.2f, 0.2f));
            model = translateMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

            translateMatrix = glm::translate(identityMatrix, pointLightPositions[2]);
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.2f, 1.2f));
            model = translateMatrix * scaleMatrix;
            ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }



        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        //point lights set up
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        pointlight3.setUpPointLight(lightingShaderWithTexture);
       

        //directional light set up
        lightingShaderWithTexture.setVec3("directionalLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShaderWithTexture.setVec3("directionalLight.ambient", 0.1f, 0.1f, 0.1f);
        lightingShaderWithTexture.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShaderWithTexture.setVec3("directionalLight.specular", 0.0f, 0.0f, 0.0f);
        lightingShaderWithTexture.setBool("directionLightOn", directionLightOn);

        //BarSpotLight
        lightingShaderWithTexture.setVec3("spotLight.position", 8.5f, 2.2f, 11.0);
        lightingShaderWithTexture.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShaderWithTexture.setVec3("spotLight.ambient", 1.0f, 1.0f, 0.0f);
        lightingShaderWithTexture.setVec3("spotLight.diffuse", 1.0f, 1.0f, 0.0f);
        lightingShaderWithTexture.setVec3("spotLight.specular", 0.2f, 0.2f, 0.2f);
        lightingShaderWithTexture.setFloat("spotLight.k_c", 1.0f);
        lightingShaderWithTexture.setFloat("spotLight.k_l", 0.09);
        lightingShaderWithTexture.setFloat("spotLight.k_q", 0.032);



        //lightingShaderWithTexture.setFloat("spotLight.cos_theta", glm::cos(glm::radians(30.0f)));
        lightingShaderWithTexture.setFloat("spotLight.inner_circle", glm::cos(glm::radians(7.5f)));
        lightingShaderWithTexture.setFloat("spotLight.outer_circle", glm::cos(glm::radians(15.0f)));
        lightingShaderWithTexture.setBool("spotLightOn", spotLightOn);

        //floor
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, -4.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, -0.1f, 18.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.494f, 0.514f, 0.541f);
        //drawCube(VAO, lightingShader, model, color);
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //roof
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 3.5f, -4.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.1f, 18.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.494f, 0.514f, 0.541f);
        //drawCube(VAO, lightingShader, model, color);
        cube2.drawCubeWithTexture(lightingShaderWithTexture, model);

        //front wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, -4.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.5f, 4.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);






        //washroom
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, -9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 4.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cubewas.drawCubeWithTexture(lightingShaderWithTexture, model);

        //doorup
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 2.1f, -4.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 1.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);


        if (doorOn) {
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, -1.0f, -4.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 3.2f, 0.1f));
            model = translateMatrix * rotateYMatrix* scaleMatrix;
            win.drawCubeWithTexture(lightingShaderWithTexture, model);

        }
        else {
           
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, -1.0f, -4.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5f, 3.2f, 0.1f));
            model = translateMatrix * scaleMatrix;
            win.drawCubeWithTexture(lightingShaderWithTexture, model);
        }

        //door
       
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
    


        //leftwashroomwall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, -9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 4.5f, 5.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cubewas.drawCubeWithTexture(lightingShaderWithTexture, model);
        //right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -1.0f, -9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 4.5f, 5.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cubewas.drawCubeWithTexture(lightingShaderWithTexture, model);

        //floorwashroom
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.1f, -9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.1f, 5.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cubewas.drawCubeWithTexture(lightingShaderWithTexture, model);


        //sink

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -1.1f, -9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 3.1f, 2.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        sink.drawWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, -1.1f, -9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 3.1f, 2.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        sink.drawWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, -1.1f, -9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f, 3.1f, 2.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        sink.drawWithTexture(lightingShaderWithTexture, model);

        //roofwashroom
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 3.45f, -9.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 0.1f, 5.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cubewas.drawCubeWithTexture(lightingShaderWithTexture, model);
        

        ////left wall section 1
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, -4.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 4.5f, 4.5f));
        //model = translateMatrix * scaleMatrix;
        ////color = glm::vec3(0.659f, 0.820f, 0.843f);
        ////drawCube(VAO, lightingShader, model, color);
        //cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //sofa
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.6f, -1.0f,0.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 3.0f, 2.0f));
        model = translateMatrix * rotateYMatrix*scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        sofa.drawWithTexture(lightingShaderWithTexture, model);

        //left wall window 1 down section
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.1f, -1.1f, -3.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.1f, 0.1f, 7.5f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);


        ////window
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 0.5f, 0.5f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.5f, 2.5f));
        //model = translateMatrix * scaleMatrix;
        ////color = glm::vec3(0.659f, 0.820f, 0.843f);
        ////drawCube(VAO, lightingShader, model, color);
        //win.drawCubeWithTexture(lightingShaderWithTexture, model);

        ////left wall window 1 up section
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 2.5f, 0.5f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 2.5f));
        //model = translateMatrix * scaleMatrix;
        ////color = glm::vec3(0.659f, 0.820f, 0.843f);
        ////drawCube(VAO, lightingShader, model, color);
        //cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left wall section 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, 3.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 4.5f, 4.5f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //window
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 0.5f, 7.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.5f, 2.5f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        win.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left wall window 2 down section
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, 7.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.5f, 2.5f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left wall window 2 up section
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 2.5f, 7.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 2.5f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left wall section 3
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 4.5f, 4.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right wall section 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -1.0f, -4.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.1f, 4.5f, 6.0f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);


        //AC
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(6.5f, 3.0f, -1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.0018, .0040, .0040));
        model = translateMatrix * rotateZMatrix* scaleMatrix;
        air.drawWithTexture(lightingShaderWithTexture, model);

        //right wall window down section
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -1.0f, 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.1f, 1.5f, 2.5f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right wall window up section
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, 2.5f, 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.1f, 1.0f, 2.5f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right wall section 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -1.0f, 4.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.1f, 4.5f, 4.5f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);

        //BAR

        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -1.0f, 8.8f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.1f, 4.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
        
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(11.1f, -1.0f, 8.8f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 4.5f, 5.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);
     
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -1.0f, 13.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.1f, 4.5f, 0.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        //floor2

        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, -1.1f, 8.8f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.1f, 0.1f, 5.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);


        //roof2
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, 3.47f, 8.8f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.1f, 0.1f, 5.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
       

        //back wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -1.0f, 14.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 4.5f, -0.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);

        

   

        //right wall window 1
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, 0.5f, 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.1f, 2.0f, 1.25f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.404f, 0.353f, 0.325f);
        //drawCube(VAO, lightingShader, model, color);
        win.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right wall window 2
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.0f, 0.5f, 3.25f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.1f, 2.0f, 1.25f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.404f, 0.353f, 0.325f);
        //drawCube(VAO, lightingShader, model, color);
        win.drawCubeWithTexture(lightingShaderWithTexture, model);

    

        z = 0.0f;


        //facultable
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.8f, 0.0f, 2.75f));
        drawTableChair(VAO, lightingShaderWithTexture, translateMatrix* rotateYMatrix, cube4, chair);

        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.8f, 0.0f, 5.75f));
        drawTableChair(VAO, lightingShaderWithTexture, translateMatrix* rotateYMatrix, cube4, chair);

  

        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.8f, 0.0f, 8.75f));
        drawTableChair(VAO, lightingShaderWithTexture, translateMatrix* rotateYMatrix, cube4,chair);

        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.8f, 0.0f, 11.75f));
        drawTableChair(VAO, lightingShaderWithTexture, translateMatrix* rotateYMatrix, cube4,chair);


        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.8f, 0.0f, 13.25f));
        drawChair(VAO, lightingShaderWithTexture, translateMatrix* rotateYMatrix, chair);

        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.8f, 0.0f, 10.25f));
        drawChair(VAO, lightingShaderWithTexture, translateMatrix* rotateYMatrix, chair);

        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.8f, 0.0f, 7.25f));
        drawChair(VAO, lightingShaderWithTexture, translateMatrix* rotateYMatrix, chair);


        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.8f, 0.0f, 4.25f));
        drawChair(VAO, lightingShaderWithTexture, translateMatrix* rotateYMatrix, chair);
       



        //sphere

        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(sp), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 rotateZMatrix2 = glm::rotate(identityMatrix, glm::radians(-sp2), glm::vec3(0.0f, 0.0f, 1.0f));

        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-sp2), glm::vec3(0.0f, 1.0f, 0.0f));

        // clock

        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 2.2f, -3.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 1.0f, 0.1f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.659f, 0.820f, 0.843f);
        //drawCube(VAO, lightingShader, model, color);
        clock.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.35f, 2.2f, -3.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.04f, -0.8f, 0.04f));
        model = translateMatrix * rotateZMatrix* scaleMatrix;
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        drawCube(VAO, lightingShader, model, color);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.33f, 2.62f, -3.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.04f, -0.2f, 0.04f));
        model = translateMatrix * rotateZMatrix2 * scaleMatrix;
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        drawCube(VAO, lightingShader, model, color);

        //TV
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.00f, 1.62f, -3.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 1.5f, 0.1f));
        model = translateMatrix  * scaleMatrix;
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        drawCube(VAO, lightingShader, model, color);


        if (TVOn) {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-.9f, 1.73f, -3.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.8f, 1.2f, 0.15f));
            model = translateMatrix * scaleMatrix;
            //color = glm::vec3(0.659f, 0.820f, 0.843f);
            //drawCube(VAO, lightingShader, model, color);



            tvs[1.0].drawCubeWithTexture(lightingShaderWithTexture, model);

            ts = ts + 1.0f;
            if (ts > 49) {
                ts = 0.0;
            }
        }
        // Second object


        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 2.2f, -1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
        model = translateMatrix * rotateYMatrix* scaleMatrix;
        sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMaps2, specMaps2);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 2.2f, -1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.04f, 1.1f, 0.04f));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        drawCube(VAO, lightingShader, model, color);


        sp2 += 0.4f;
        // Update rotation angle based on direction
        if (rotateDirection) {
            sp += 1.0f;
            if (sp >= 30.0f) {
                rotateDirection = false;  // Reverse direction
            }
        }
        else {
            sp -= 1.0f;
            if (sp <= -30.0f) {
                rotateDirection = true;  // Reverse direction
            }
        }

     

        //Table Accessories


        //glass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.9f, 0.20f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.075, .080, .075));
        model = translateMatrix * scaleMatrix;
        mug.drawWithTexture(lightingShaderWithTexture, model);
        //burger
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.15f, 5.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.0125, .015, .0125));
        model = translateMatrix * scaleMatrix;
        burger.drawWithTexture(lightingShaderWithTexture, model);
        //plate
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.15f, 5.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.02f, 0.3f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMapsplate, specMapsplate);
        //bottle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.1f, 0.10f, 5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.017, .2, .017));
        model = translateMatrix * scaleMatrix;
        bottle2.drawWithTexture(lightingShaderWithTexture, model);


        //second
          //glass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.9f, 0.20f, 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.075, .080, .075));
        model = translateMatrix * scaleMatrix;
        mug.drawWithTexture(lightingShaderWithTexture, model);
        //burger
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.15f, 2.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.0125, .015, .0125));
        model = translateMatrix * scaleMatrix;
        burger.drawWithTexture(lightingShaderWithTexture, model);
        //plate
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.15f, 2.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.02f, 0.3f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMapsplate, specMapsplate);
        //bottle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.1f, 0.10f, 2.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.017, .2, .017));
        model = translateMatrix * scaleMatrix;
        bottle2.drawWithTexture(lightingShaderWithTexture, model);

        //third
              //second
          //glass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.9f, 0.20f, 8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.075, .080, .075));
        model = translateMatrix * scaleMatrix;
        mug.drawWithTexture(lightingShaderWithTexture, model);
        //burger
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.15f, 8.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.0125, .015, .0125));
        model = translateMatrix * scaleMatrix;
        burger.drawWithTexture(lightingShaderWithTexture, model);
        //plate
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.15f, 8.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.02f, 0.3f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMapsplate, specMapsplate);
        //bottle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.1f, 0.10f, 8.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.017, .2, .017));
        model = translateMatrix * scaleMatrix;
        bottle2.drawWithTexture(lightingShaderWithTexture, model);

        //third
            //second
        //glass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.9f, 0.20f, 11.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.075, .080, .075));
        model = translateMatrix * scaleMatrix;
        mug.drawWithTexture(lightingShaderWithTexture, model);
        //burger
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.15f, 11.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.0125, .015, .0125));
        model = translateMatrix * scaleMatrix;
        burger.drawWithTexture(lightingShaderWithTexture, model);
        //plate
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 0.15f, 11.3f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.02f, 0.3f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        sphere.drawSphereTexture(lightingShaderWithTexture, model, diffMapsplate, specMapsplate);
        //bottle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.1f, 0.10f, 11.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.017, .2, .017));
        model = translateMatrix * scaleMatrix;
        bottle2.drawWithTexture(lightingShaderWithTexture, model);



        //BAR ACCessories
        
        //7.0f, -1.0f, 8.8f
        // 0.1f, 4.5f, 5.1f
        //standing structure top
        translateMatrix = glm::translate(identityMatrix, glm::vec3(9.0f, -1.0f, 8.8));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.3f, 4.2f));
        model = translateMatrix * scaleMatrix;
        //color = glm::vec3(0.624f, 0.416f, 0.310f);
        //drawCube(VAO, lightingShader, model, color);
        cube5.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0f, -1.0f, 9.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 2.0, 2.5));
        model = translateMatrix * scaleMatrix;
        stool.drawWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0f, -1.0f, 10.75));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 2.0, 2.5));
        model = translateMatrix * scaleMatrix;
        stool.drawWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0f, -1.0f, 12.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 2.0, 2.5));
        model = translateMatrix * scaleMatrix;
        stool.drawWithTexture(lightingShaderWithTexture, model);

        //spot
        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.5f, 2.2f, 11.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.098, .20, .098));
        model = translateMatrix * scaleMatrix;
        bottle2.drawWithTexture(lightingShaderWithTexture, model);

        //rod
        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.5f, 2.2f, 10.945));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.5f, 0.1));
        model = translateMatrix * scaleMatrix;
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        drawCube(VAO, lightingShader, model, color);





        //glfw swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //optional: de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //glfw terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}



//void drawFan(unsigned int VAO, Shader lightingShader, glm::mat4 matrix) {
//    //define matrices and vectors needed
//    glm::mat4 identityMatrix = glm::mat4(1.0f);
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, RotateTranslateMatrix, InvRotateTranslateMatrix;
//    glm::vec3 color;
//
//    //when fan is on
//    if (fanOn) {
//        //fan rod
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.28f, 3.5f, 1.61f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.75f, 0.1f));
//        model = matrix * translateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 0.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan middle
//        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.13f, 2.75f, 1.46f));
//        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.0f, -0.2f));
//        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.2f, 0.0f, 0.2f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, -0.1f, 0.4f));
//        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 0.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan propelars left
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.13f, 2.75f, 1.56f));
//        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.0f, -0.1f));
//        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.2f, 0.0f, 0.1f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.75f, -0.1f, 0.2f));
//        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 1.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan propelars right
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.53f, 2.75f, 1.56f));
//        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.2f, 0.0f, -0.1f));
//        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.2f, 0.0f, 0.1f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.75f, -0.1f, 0.2f));
//        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 1.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan propelars up
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.23f, 2.75f, 1.46f));
//        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, -0.2f));
//        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f, 0.0f, 0.2f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -0.1f, -0.75f));
//        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 1.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan propelars down
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.23f, 2.75f, 1.86f));
//        RotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(-0.1f, 0.0f, 0.2f));
//        InvRotateTranslateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f, 0.0f, -0.2f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -0.1f, 0.75f));
//        model = matrix * translateMatrix * InvRotateTranslateMatrix * rotateYMatrix * RotateTranslateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 1.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        r += 1.0f;
//    }
//
//    //when fan is off
//    else {
//        //fan rod
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.28f, 3.5f, 1.61f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.75f, 0.1f));
//        model = matrix * translateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 0.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan middle
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.13f, 2.75f, 1.46f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4f, -0.1f, 0.4f));
//        model = matrix * translateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 0.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan propelars left
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.13f, 2.75f, 1.56f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.75f, -0.1f, 0.2f));
//        model = matrix * translateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 1.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan propelars right
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.53f, 2.75f, 1.56f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.75f, -0.1f, 0.2f));
//        model = matrix * translateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 1.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan propelars up
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.23f, 2.75f, 1.46f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -0.1f, -0.75f));
//        model = matrix * translateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 1.0f);
//        drawCube(VAO, lightingShader, model, color);
//
//        //fan propelars down
//        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.23f, 2.75f, 1.86f));
//        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, -0.1f, 0.75f));
//        model = matrix * translateMatrix * scaleMatrix;
//        color = glm::vec3(0.0f, 0.0f, 1.0f);
//        drawCube(VAO, lightingShader, model, color);
//    }
//}

void drawTableChair(unsigned int VAO, Shader lightingShaderWithTexture, glm::mat4 matrix, Cube c, Cube d) {
    //define matrices and vectors needed
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    glm::vec3 color;

    //table top
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.1f, 1.0f));
    model = matrix * scaleMatrix;
    color = glm::vec3(0.882f, 0.710f, 0.604f);
    //drawCube(VAO, lightingShader, model, color);
    c.drawCubeWithTexture(lightingShaderWithTexture, model);



    //table right leg back
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.4f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -1.0f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.647f, 0.408f, 0.294f);
    //drawCube(VAO, lightingShader, model, color);
    c.drawCubeWithTexture(lightingShaderWithTexture, model);


    //chair mid section
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.5f, 1.15f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.1f, 0.5f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.455f, 0.235f, 0.102f);
    //drawCube(VAO, lightingShader, model, color);
    d.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair leg back left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.5f, 1.15f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.5f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    d.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair leg front left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.5f, 1.55f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.5f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    d.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair leg front right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.65f, -0.5f, 1.55f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.5f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    d.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair leg back right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.65f, -0.5f, 1.15f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.5f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    d.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair upper piller left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.4f, 1.55f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.65f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    d.drawCubeWithTexture(lightingShaderWithTexture, model);


}

void drawChair(unsigned int VAO, Shader lightingShaderWithTexture, glm::mat4 matrix, Cube c) {

    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    glm::vec3 color;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.5f, 1.15f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.1f, 0.5f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.455f, 0.235f, 0.102f);
    //drawCube(VAO, lightingShader, model, color);
    c.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair leg back left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.5f, 1.15f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.5f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    c.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair leg front left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.5f, 1.55f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.5f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    c.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair leg front right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.65f, -0.5f, 1.55f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.5f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    c.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair leg back right
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.65f, -0.5f, 1.15f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, -0.5f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    c.drawCubeWithTexture(lightingShaderWithTexture, model);

    //chair upper piller left
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25f, -0.4f, 1.15f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.65f, 0.1f));
    model = matrix * translateMatrix * scaleMatrix;
    //color = glm::vec3(0.329f, 0.173f, 0.110f);
    //drawCube(VAO, lightingShader, model, color);
    c.drawCubeWithTexture(lightingShaderWithTexture, model);

}

void drawCube(unsigned int& VAO, Shader& lightingShader, glm::mat4 model, glm::vec3 color)
{
    //use the shadder
    lightingShader.use();

    //define lighting properties
    lightingShader.setVec3("material.ambient", color);
    lightingShader.setVec3("material.diffuse", color);
    lightingShader.setVec3("material.specular", color);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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



    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(P_UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(P_DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(R_LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        if (!birdEyeView)
            camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        fanOn = !fanOn;
        fan_turn = !fan_turn;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        doorOn = !doorOn;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        TVOn = !TVOn;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        birdEyeView = !birdEyeView;
    }

    if (birdEyeView) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            birdEyePosition.z -= birdEyeSpeed * deltaTime; // Move forward along Z
            birdEyeTarget.z -= birdEyeSpeed * deltaTime;
            if (birdEyePosition.z <= 2.0) {
                birdEyePosition.z = 2.0;
            }
            if (birdEyeTarget.z <= -4.0) {
                birdEyeTarget.z = -4.0;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            birdEyePosition.z += birdEyeSpeed * deltaTime; // Move backward along Z
            birdEyeTarget.z += birdEyeSpeed * deltaTime;
            if (birdEyePosition.z >= 13.5) {
                birdEyePosition.z = 13.5;
            }
            if (birdEyeTarget.z >= 7.5) {
                birdEyeTarget.z = 7.5;
            }
        }
    }

    /*
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!birdEyeView) {
            theta += 0.01f;
            camera.Position.x = lookAtX + radius * sin(theta);
            camera.Position.y = lookAtY;
            camera.Position.z = lookAtZ + radius * cos(theta);
        }
    }
    */

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

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        if (pointlight3.ambientOn > 0 && pointlight3.diffuseOn > 0 && pointlight3.specularOn > 0) {
            pointlight3.turnOff();
            point3 = false;
        }
        else {
            pointlight3.turnOn();
            point3 = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        directionLightOn = !directionLightOn;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        spotLightOn = !spotLightOn;
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        if (pointlight1.ambientOn > 0 || pointlight2.ambientOn > 0) {
            if (point1)
                pointlight1.turnAmbientOff();
            if (point2)
                pointlight2.turnAmbientOff();
        }
        else {
            if (point1)
                pointlight1.turnAmbientOn();
            if (point2)
                pointlight2.turnAmbientOn();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        if (pointlight1.diffuseOn > 0 || pointlight2.diffuseOn > 0) {
            if (point1)
                pointlight1.turnDiffuseOff();
            if (point2)
                pointlight2.turnDiffuseOff();
        }
        else {
            if (point1)
                pointlight1.turnDiffuseOn();
            if (point2)
                pointlight2.turnDiffuseOn();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        if (pointlight1.specularOn > 0 || pointlight2.specularOn > 0) {
            if (point1)
                pointlight1.turnSpecularOff();
            if (point2)
                pointlight2.turnSpecularOff();
        }
        else {
            if (point1)
                pointlight1.turnSpecularOn();
            if (point2)
                pointlight2.turnSpecularOn();
        }
    }
}

//glfw whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //make sure the viewport matches the new window dimensions; note that width and height will be significantly larger than specified on retina displays.
    float aspectRatio = 4.0f / 3.0f;
    int viewWidth, viewHeight;

    if (width / (float)height > aspectRatio) {
        //Window is too wide, fit height and adjust width
        viewHeight = height;
        viewWidth = (int)(height * aspectRatio);
    }
    else {
        //Window is too tall, fit width and adjust height
        viewWidth = width;
        viewHeight = (int)(width / aspectRatio);
    }

    //Center the viewport
    int xOffset = (width - viewWidth) / 2;
    int yOffset = (height - viewHeight) / 2;

    glViewport(xOffset, yOffset, viewWidth, viewHeight);
    //glViewport(0, 0, width, height);
}

//glfw whenever the mouse moves, this callback is called
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
    float yoffset = lastY - ypos;       //reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

//glfw whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
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