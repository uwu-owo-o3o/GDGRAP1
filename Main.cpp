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
float x_mod = 0.f;
float y_mod = 0.f;
float z_mod = -2.f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_D) {
        x_mod += 0.5f;
    }

    if (key == GLFW_KEY_A) {
        x_mod -= 0.5f;
    }

    if (key == GLFW_KEY_W) {
        y_mod += 0.5f;
    }

    if (key == GLFW_KEY_S) {
        y_mod -= 0.5f;
    }

    if (key == GLFW_KEY_Z) {
        z_mod += 0.3f;
    }

    if (key == GLFW_KEY_Q) {
        z_mod -= 0.3f;
    }

}

int main(){
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    float window_width = 600.f;
    float window_height = 600.f;

    window = glfwCreateWindow((int)window_width, (int)window_height, "Ong, Lance", NULL, NULL);
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
    unsigned char* tex_bytes = stbi_load("3d/partenza.jpg", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);
    glEnable(GL_DEPTH_TEST);

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

    std::string path = "3D/djSword.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    std::vector<GLfloat> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        mesh_indices.push_back(attributes.vertices[(vData.vertex_index * 3)]);
        mesh_indices.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
        mesh_indices.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);

        mesh_indices.push_back(attributes.normals[(vData.normal_index * 3)]);
        mesh_indices.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
        mesh_indices.push_back(attributes.normals[(vData.normal_index * 3) + 2]);

        mesh_indices.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
        mesh_indices.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);
    }

    GLuint VAO, VBO;

    GLintptr normPtr = 3 * sizeof(float);
    GLintptr uvPtr = 6 * sizeof(float); // UV starts at index 3 or the 4th index of our vertex data

    glGenVertexArrays(1, &VAO); // line responsible for VAO
    glGenBuffers(1, &VBO); // line responsible for VBO

    glBindVertexArray(VAO); // assigns VAO currently being edited
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // assigns VBO currently being edited and attaches VBO to VAO

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh_indices.size(), mesh_indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 5 for 5 vertices (X,Y,Z,U,V)
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)normPtr); // 5 for 5 vertices (X,Y,Z,U,V)
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)uvPtr);
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    float x = 0.f; 
    float y = 0.f;
    float z = -2.0f;

    float scale_x = 0.095f;
    float scale_y = 0.095f;
    float scale_z = 0.095f;

    float theta = 90.0f;
    float axis_x = 0.f;
    float axis_y = 1.f;
    float axis_z = 0.f;
    
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f), 
        window_height / window_width,
        0.1f, 
        100.f 
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

    // lighting things
    glm::vec3 lightPos = glm::vec3(-10.f, 3.f, 0.f); // -10, 3, 0 //
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    float ambientStr = 0.00025f; // keep strength low, preferably less than 1
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16.f; // usual range is 16 to 25

    float baseConstant = 1.0f;
    float baseLinear = 0.027f;
    float baseQuadratic = 0.0028f;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        x = x_mod;
        y = y_mod;
        z = z_mod;

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

        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint baseConstantAddress = glGetUniformLocation(shaderProg, "constant");
        glUniform1f(baseConstantAddress, baseConstant);

        GLuint baseLinearAddress = glGetUniformLocation(shaderProg, "linear");
        glUniform1f(baseLinearAddress, baseLinear);

        GLuint baseQuadraticAddress = glGetUniformLocation(shaderProg, "quadratic");
        glUniform1f(baseQuadraticAddress, baseQuadratic);

        GLuint ambientStrAddress = glGetUniformLocation(shaderProg, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint camerPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(camerPosAddress, 1, glm::value_ptr(cameraPositionMatrix));

        GLuint specStrAddress = glGetUniformLocation(shaderProg, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProg, "specPhong");
        glUniform1f(specPhongAddress, specPhong);

   

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
       
        glDrawArrays(GL_TRIANGLES, 0, mesh_indices.size() / 8);

        glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}