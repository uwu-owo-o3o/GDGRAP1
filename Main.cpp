#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "iostream"
#include "string"
#include "windows.h"

#include "cmath"

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

    float polar_theta = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // use polar coordinate system instead of cartesian coordinate system //

        // conversion of polar to cartesian //
        float radian_tetha = (polar_theta / 180) * 3.14;
        float x_mod = 0.55f * cos(radian_tetha);
        float y_mod = 0.55f * sin(radian_tetha);

        // increment angle so it spins //
        polar_theta += 0.05f;

        // if angle hits 360, reset to 0 //
        if (polar_theta == 360.0f) {
            polar_theta = 0.0f;
        }

        // modify transformation matrix coordinates //
        x = x_mod;
        y = y_mod;


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


