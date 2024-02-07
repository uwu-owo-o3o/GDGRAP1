#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "iostream"
#include "string"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// when submitting .exe on release, put 3D folder in the Release folder //
float x_mod = 0;
float z_mod = -2.f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        x_mod += 20.0f;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        x_mod -= 20.0f;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        std::cout << "pressed W" << std::endl;
        z_mod -= 0.3f;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        z_mod += 0.3f;
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

    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* tex_bytes = stbi_load("3d/ayaya.png", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

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

    std::string path = "3D/myCube.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };


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
    
    GLuint VAO, VBO, EBO, VBO_UV;

    glGenVertexArrays(1, &VAO); // line responsible for VAO
    glGenBuffers(1, &VBO); // line responsible for VBO
    glGenBuffers(1, &VBO_UV);
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
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), &UV[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    glm::mat4 identity_matrix = glm::mat4(1.0f);

    float x = 0.f; 
    float y = 0.f;
    float z = -2.f;

    float scale_x = 3.f;
    float scale_y = 3.f;
    float scale_z = 1.f;

    float theta = 60.f;
    float axis_x = 0.f;
    float axis_y = 1.f;
    float axis_z = 0.f;
    
    /*glm::mat4 projectionMatrix = glm::ortho(
        -2.0f, // Left
        2.0f, // Right
       -2.0f, // Bot
        2.0f, // Top
        -1.0f, // zNear
        1.0f // zFar
    );*/

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f), // fov
        // zoom in - fov
        // zoom out + fov
        window_height / window_width, // aspect ratio
        0.1f, //zNear [NOTE]: zNear cannot be >= 0 !
        100.f //zFar
    );

    // shortcut is through glm::lookat() for the camera, 1st param is eye, 2nd param is center, and 3rd is WorldUp variable //
    glm::vec3 camera(0, 0, 10.f);
    glm::mat4 cameraPositionMatrix = glm::translate(glm::mat4(1.0f), camera * -1.0f);

    glm::vec3 WorldUp = glm::vec3(0, 1.0f, 0);
    glm::vec3 Center = glm::vec3(0, 0.f, 0);
    glm::vec3 Front = glm::vec3(Center - camera);
    Front = glm::normalize(Front);

    glm::vec3 Right = glm::normalize(glm::cross(Front, WorldUp));
    glm::vec3 Up = glm::normalize(glm::cross(Right, Front));

    //setup camera matrix //
    glm::mat4 cameraOrientation = glm::mat4(1.f);
    cameraOrientation[0][0] = Right.x;
    cameraOrientation[1][0] = Right.y;
    cameraOrientation[2][0] = Right.z;

    cameraOrientation[0][1] = Up.x;
    cameraOrientation[1][1] = Up.y;
    cameraOrientation[2][1] = Up.z;

    cameraOrientation[0][2] = -Front.x;
    cameraOrientation[1][2] = -Front.y;
    cameraOrientation[2][2] = -Front.z;

    glm::mat4 viewMatrix = cameraOrientation * cameraPositionMatrix;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        z = z_mod;
        //theta = x_mod;
        theta += 0.001f;

        // start with translation matrix //
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));

       
        // multiply matrix with scale matrix //
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));

        // multiply with rotate matrix //
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));
        
       unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
       glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
        
        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

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