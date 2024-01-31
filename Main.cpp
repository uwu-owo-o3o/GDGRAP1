#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "iostream"
#include "string"
#include "windows.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// when submitting .exe on release, put 3D folder in the Release folder //

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(900, 900, "Ong, Lance", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

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

    GLfloat vertices[]{
        // x,    y,    z 
          0.f, 0.5f, 0.f,
          -0.5f, -0.5f, 0.f,
          0.5f,   -0.5f, 0.f
    };

    GLuint indices[]{
        0, 1, 2
    };

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

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    float scale_x = 0.5f;
    float scale_y = 0.5f;
    float scale_z = 1.f;

    float theta = 90.f;
    float axis_x = 0.f;
    float axis_y = 1.f;
    float axis_z = 0.f;

    float x_check1 = 0.0f;
    float y_check1 = 0.5f;
  
    float x_check2 = 0.5f;
    float y_check2 = 0.0f;

    float x_check3 = 0.0f;
    float y_check3 = -0.5f;

    float x_check4 = -0.5f;
    float y_check4 =  0.0f;

    bool move = false;
    bool hasPassedCheck1 = false;
    bool hasPassedCheck2 = false;
    bool hasPassedCheck3 = false;
    bool hasPassedCheck4 = false;

    float x_mod = 0.0f;
    float y_mod = 0.5f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

      

        if (move == false) {
            x = 0.0f;
            y = y_mod;
            move = true;
        }

        // updates flags
        if (x == x_check1 && y == y_check1) {
            std::cout << "passed check point 1" << std::endl;
            hasPassedCheck1 = true;
            hasPassedCheck2 = false;
            hasPassedCheck3 = false;
            hasPassedCheck4 = false;
        }

        if (x == x_check2 && y == y_check2) {
            std::cout << "passed check point 2" << std::endl;
            hasPassedCheck1 = false;
            hasPassedCheck2 = true;
            hasPassedCheck3 = false;
            hasPassedCheck4 = false;
        }

        if (x == x_check3 && y == y_check3) {
            std::cout << "passed check point 3" << std::endl;
            hasPassedCheck1 = false;
            hasPassedCheck2 = false;
            hasPassedCheck3 = true;
            hasPassedCheck4 = false;
        }
        
        if (x == x_check4 && y == y_check4) {
            std::cout << "passed check point 4" << std::endl;
            hasPassedCheck1 = false;
            hasPassedCheck2 = false;
            hasPassedCheck3 = false;
            hasPassedCheck4 = true;
        }

        Sleep(300);

        // updates coordinates for movement
        if (hasPassedCheck1 == true) {
            std::cout << "successfully passed check 1 "<< std::endl;
            x_mod += 0.1f;
            if (x_mod == 0.5f) {
                std::cout << "entered correction" << std::endl;
                y_mod = 0.0f;
            }
            else {
                y_mod -= 0.1f;
            }  
        }

        else if (hasPassedCheck2 == true) {
            std::cout << "successfully passed check 2 " << std::endl;
            y_mod -= 0.1f;
            if (y_mod == -0.5f) {
                std::cout << "entered correction" << std::endl;
                x_mod = 0.0f;
            }
            else {
                x_mod -= 0.1f;
            }
        }

        else if (hasPassedCheck3 == true) {
            std::cout << "successfully passed check 3 " << std::endl;
            x_mod -= 0.1f;
            if (x_mod == -0.5f) {
                std::cout << "entered correction" << std::endl;
                y_mod = 0.0f;
            }
            else {
                y_mod += 0.1f;
            }
        }
       
        else if (hasPassedCheck4 == true) {
            std::cout << "successfully passed check 4 " << std::endl;
            y_mod += 0.1f;
            if (y_mod == 0.5f) {
                std::cout << "entered correction" << std::endl;
                x_mod = 0.0f;
            }
            else {
                x_mod += 0.1f;
            }
        }

        x = x_mod;
        std::cout << "check x value: " << x << std::endl;
        y = y_mod;
        std::cout << "check y value: " << y << std::endl;

        // start with translation matrix //
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));

        // multiply matrix with scale matrix //
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


