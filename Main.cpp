#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "iostream"
#include "string"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// when submitting .exe on release, put 3D folder in the Release folder //
float x_mod = 0;


void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        x_mod += 0.1f;
    }
}

int main(){
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
    glfwSetKeyCallback(window, Key_Callback);

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
    float x = 1.0f; 
    float y = 2.0f;
    float z = 0.0f;

    float scale_x = 1.0f;
    float scale_y = 1.0f;
    float scale_z = 0.0f;

    float theta = 45.0f;
    float axis_x = 1.0f;
    float axis_y = 1.0f;
    float axis_z = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
         
        // start with translation matrix //
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));

       
        // multiply matrix with scale matrix //
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));

        // multiply with rotate matrix //
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));


        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");


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