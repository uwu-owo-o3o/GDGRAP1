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
float y_mod = 0;
float z_mod = -2.f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        y_mod += 10.0f;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        x_mod -= 10.0f;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        y_mod -= 10.f;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        x_mod += 10.0f;
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
    unsigned char* tex_bytes = stbi_load("3d/yae.png", &img_width, &img_height, &colorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);
    glEnable(GL_DEPTH_TEST); // allows the 3d model's rendering for the "back" part of the model to disappear

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

    std::string path = "3D/plane.obj";
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

    // skybox things
    std::fstream skyboxVertSrc("Shaders/skybox.vert");
    std::stringstream skyBoxVertBuff;
    skyBoxVertBuff << skyboxVertSrc.rdbuf();

    std::string skyBoxVertS = skyBoxVertBuff.str();
    const char* sky_v = skyBoxVertS.c_str();

    std::fstream skyboxFragSrc("Shaders/skybox.frag");
    std::stringstream skyboxFragBuff;
    skyboxFragBuff << skyboxFragSrc.rdbuf();

    std::string sky_fragS = skyboxFragBuff.str();
    const char* sky_f = sky_fragS.c_str();

    GLuint skyboxVertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skyboxVertShader, 1, &sky_v, NULL);
    glCompileShader(skyboxVertShader);

    GLuint skyboxFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skyboxFragShader, 1, &sky_f, NULL);
    glCompileShader(skyboxFragShader);

    GLuint skyboxShaderProg = glCreateProgram();
    glAttachShader(skyboxShaderProg, skyboxVertShader);
    glAttachShader(skyboxShaderProg, skyboxFragShader);

    glLinkProgram(skyboxShaderProg);

    glDeleteShader(skyboxVertShader);
    glDeleteShader(skyboxFragShader);

        /*
      7--------6
     /|       /|
    4--------5 |
    | |      | |
    | 3------|-2
    |/       |/
    0--------1
    */
//Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    std::string faceSkybox[]{
         "Skybox/rainbow_rt.png",
         "Skybox/rainbow_lf.png",
         "Skybox/rainbow_up.png",
         "Skybox/rainbow_dn.png",
         "Skybox/rainbow_ft.png",
         "Skybox/rainbow_bk.png"
    };


    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyChannel;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data2 = stbi_load(faceSkybox[i].c_str(), &w, &h, &skyChannel, 0);

        if (data2) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
            stbi_image_free(data2);
        }

    }

    stbi_set_flip_vertically_on_load(true);

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    float x = 0.f; 
    float y = 0.f;
    float z = -2.f;

    float scale_x = 5.5f;
    float scale_y = 5.5f;
    float scale_z = 5.5f;

    float theta = 90.0f;
    float axis_x = 0.f;
    float axis_y = 0.f;
    float axis_z = -1.f;
    
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
  
    // lighting things
    glm::vec3 lightPos = glm::vec3(-10, 3, 0.0);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);

    float ambientStr = 0.25f; // keep strength low, preferably less than 1
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16; // usual range is 16 to 25

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        //theta += 0.005f;

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));


        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShaderProg);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMatrix));

        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShaderProg, "view");
        glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShaderProg, "projection");
        glUniformMatrix4fv(skyboxProjLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        
        glUseProgram(shaderProg);
        
        // start with translation matrix //
        glm::mat4 transformation_matrix = glm::translate(identity_matrix, glm::vec3(x, y, z));

        // multiply matrix with scale matrix //
        transformation_matrix = glm::scale(transformation_matrix, glm::vec3(scale_x, scale_y, scale_z));

        // multiply with rotate matrix //
        transformation_matrix = glm::rotate(transformation_matrix, glm::radians(theta), glm::normalize(glm::vec3(axis_x, axis_y, axis_z)));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformation_matrix));

        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));

        GLuint lightColorAddress = glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

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