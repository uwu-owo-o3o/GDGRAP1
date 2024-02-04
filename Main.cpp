#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/type_ptr.hpp"

#include "iostream"
#include "string"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Model/Bunny.hpp"

using namespace models;

CalledKey keyVar = CalledKey::NONE;

void processCalledKey(int key) {
    switch (key) {
        case GLFW_KEY_W:
            keyVar = CalledKey::MOVE_W;
            break;
        case GLFW_KEY_A:
            keyVar = CalledKey::MOVE_A;
            break;
        case GLFW_KEY_S:
            keyVar = CalledKey::MOVE_S;
            break;
        case GLFW_KEY_D:
            keyVar = CalledKey::MOVE_D;
            break;
        case GLFW_KEY_LEFT:
            keyVar = CalledKey::ROTATE_LEFT;
            break;
        case GLFW_KEY_RIGHT:
            keyVar = CalledKey::ROTATE_RIGHT;
            break;
        case GLFW_KEY_UP:
            keyVar = CalledKey::ROTATE_UP;
            break;
        case GLFW_KEY_DOWN:
            keyVar = CalledKey::ROTATE_DOWN;
            break;
        case GLFW_KEY_E:
            keyVar = CalledKey::SCALEUP_E;
            break;
        case GLFW_KEY_Q:
            keyVar = CalledKey::SCALEDOWN_Q;
            break;
        case GLFW_KEY_Z:
            keyVar = CalledKey::ZOOMIN_Z;
            break;
        case GLFW_KEY_X:
            keyVar = CalledKey::ZOOMOUT_X;
            break;
    }
}


void Key_Callback (GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        processCalledKey(key);
    }

    if (action == GLFW_RELEASE) {
        keyVar = CalledKey::NONE;
    }
  
}

int main(){
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    float window_width = 600.f;
    float window_height = 600.f;

    window = glfwCreateWindow(window_width, window_height, "Ong, Lance", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetKeyCallback(window, Key_Callback);

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<GLuint> mesh_indices; // EBO of 3d object
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) { // get EBO indices array
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }
    GLuint VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO); // line responsible for VAO
    glGenBuffers(1, &VBO); // line responsible for VBO
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // assigns VAO currently being edited
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // assigns VBO currently being edited and attaches VBO to VAO

    // the VBO's contents:
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size(), &attributes.vertices[0], GL_STATIC_DRAW); // sizeof(vertices) -> size of array in bytes, GL_STATIC_DRAW as the model doesn't move

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // the EBO;s contents:
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indices.size(), mesh_indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);
    
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f), // fov
        // zoom in - fov
        // zoom out + fov
        window_height / window_width, // aspect ratio
        0.1f, //zNear [NOTE]: zNear cannot be >= 0 !
        100.f //zFar
    );

    Bunny bunnyModel = Bunny();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //// start with translation matrix //
        //glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(bunnyModel.getTranslateVar('X'), bunnyModel.getTranslateVar('Y'), bunnyModel.getTranslateVar('Z')));

        //// multiply matrix with scale matrix //
        //transformation_matrix = glm::scale(transformation_matrix, glm::vec3(bunnyModel.getScaleVar('X'), bunnyModel.getScaleVar('Y'), bunnyModel.getScaleVar('Z')));

        //// multiply with rotate matrix //
        //transformation_matrix = glm::rotate(transformation_matrix, glm::radians(bunnyModel.getRotateVar('T')), glm::normalize(glm::vec3(bunnyModel.getRotateVar('X'), bunnyModel.getRotateVar('Y'), bunnyModel.getRotateVar('Z'))));
        
        bunnyModel.calculateTransformMatrix();

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(*bunnyModel.getMatrix()));

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
       
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);


        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();

        int state = glfwGetKey(window, GLFW_KEY_D);

        if (state == GLFW_PRESS) {
            std::cout << "listened and D key has been pressed!" << std::endl;
        }

    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}