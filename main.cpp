#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "Shader.hpp"
#include "Model.hpp"

void processInput(GLFWwindow *window);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

double previous_seconds;
int frame_count;

/* we will use this function to update the window title with a frame rate */
void _update_fps_counter(GLFWwindow *window) {
    double current_seconds;
    double elapsed_seconds;
    current_seconds = glfwGetTime();
    elapsed_seconds = current_seconds - previous_seconds;
    /* limit text updates to 4 per second */
    if (elapsed_seconds > 0.1) {
        previous_seconds = current_seconds;
        char tmp[128];
        double fps = (double) frame_count / elapsed_seconds;
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

int main() {
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL playground", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    // Register callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Initialise GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialise GLEW\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Load model
    Model dragon_model("/Users/simon/Documents/Workspace/models/dragon_recon/dragon_vrip.ply");

//    // Load shaders
//    Shader normal_shader_v("shaders/normal.vert", ShaderType::Vertex);
//    Shader normal_shader_f("shaders/normal.frag", ShaderType::Fragment);
//    // Create program
//    Program normal_program({normal_shader_v, normal_shader_f});
//    // Prefetch attributes and uniforms locations
//    normal_program.prefetchAttributes({"vertex_position", "vertex_normal"});
//    normal_program.prefetchUniforms({"model", "view", "proj"});
//    // Print informations
//    normal_program.printInformations();
//    GL_CHECK();
//
//    // Set shader active
//    normal_program.use();


    // Load shaders
    Shader diffuse_shader_v("shaders/diffuse.vert", ShaderType::Vertex);
    Shader diffuse_shader_f("shaders/diffuse.frag", ShaderType::Fragment);
    // Create program
    Program diffuse_program({diffuse_shader_v, diffuse_shader_f});
    // Prefetch attributes and uniforms locations
    diffuse_program.prefetchAttributes({"vertex_position", "vertex_normal"});
    diffuse_program.prefetchUniforms({"model", "view", "proj"});
    // Print informations
    diffuse_program.printInformations();

    // Set shader active
    diffuse_program.use();

//    // Load shader
//    Shader base_shader_vert("shaders/base.vert", ShaderType::Vertex);
//    Shader base_shader_frag("shaders/base.frag", ShaderType::Fragment);
//    // Create program
//    Program base_program({base_shader_vert, base_shader_frag});
//
//    // Prefetch locations
//    base_program.prefetchAttributes({"vertex_position", "vertex_colour"});
//    base_program.prefetchUniforms({"model", "view", "projection"});
//
//    // Print infos about shader
//    base_program.printInformations();

//    // Generate VAO
//    GLuint vao;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//    // Vertex and colour data
//    const std::vector<glm::vec3> triangle_data = {
//            glm::vec3(-1.f, -1.f, 0.f), glm::vec3(1.f, 0.f, 0.f),     // First vertex
//            glm::vec3(1.f, -1.f, 0.f), glm::vec3(0.f, 1.f, 0.f),      // Second vertex
//            glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)        // Last vertex
//    };
//
//    // Create VBO to store data
//    GLuint vbo;
//    glGenBuffers(1, &vbo);
//    // Bind array buffer
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    // Upload data to GPU
//    glBufferData(GL_ARRAY_BUFFER,
//                 triangle_data.size() * sizeof(glm::vec3),
//                 reinterpret_cast<const GLvoid*>(triangle_data.data()), GL_STATIC_DRAW);
//    GL_CHECK();
//
//    // Set attributes pointer
//    glVertexAttribPointer(base_program.getAttributeLocation("vertex_position"),
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          2 * sizeof(glm::vec3),
//                          reinterpret_cast<const GLvoid*>(0));
//    glVertexAttribPointer(base_program.getAttributeLocation("vertex_colour"),
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          2 * sizeof(glm::vec3),
//                          reinterpret_cast<const GLvoid*>(sizeof(glm::vec3)));
//
//    // Enable vertex attribute pointers
//    glEnableVertexAttribArray(base_program.getAttributeLocation("vertex_position"));
//    glEnableVertexAttribArray(base_program.getAttributeLocation("vertex_colour"));

    // Validate shader
//#ifndef NDEBUG
//    base_program.validate();
//#endif
//
//    // Set shader active
//    base_program.use();
//
//    // Set uniforms
//    base_program.setMat4("view", glm::lookAt(glm::vec3(0.f, 0.f, 5.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f)));
//    base_program.setMat4("projection", glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f));

    // Set uniforms
    const auto model = glm::scale(glm::mat4(1.f), glm::vec3(3.f));
    diffuse_program.setMat4("model", model);
    diffuse_program.setMat4("view",
                            glm::lookAt(glm::vec3(0.f, 1.f, 1.f), glm::vec3(0.f, 0.2f, 0.f), glm::vec3(0.f, 1.f, 0.f)));
    diffuse_program.setMat4("proj", glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 20.f));

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        _update_fps_counter(window);

        // Process input
        processInput(window);

        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw mesh
        dragon_model.draw();

//        base_program.setMat4("model", glm::rotate(glm::mat4(1.f),
//                                                  glm::radians(45.f) * static_cast<float>(glfwGetTime()),
//                                                  glm::vec3(0.f, 1.f, 0.f)));
//
//        // Draw
//        glBindVertexArray(vao);
//        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(triangle_data.size() / 2));

        // Swap buffer
        glfwSwapBuffers(window);

        // Poll events
        glfwPollEvents();
    }

    // Cleanup
    dragon_model.destroy();
    diffuse_shader_v.destroy();
    diffuse_shader_f.destroy();
    diffuse_program.destroy();

//    glDeleteBuffers(1, &vbo);
//    glDeleteVertexArrays(1, &vao);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebufferSizeCallback(GLFWwindow *, int width, int height) {
    // Reset viewport
    glViewport(0, 0, width, height);
}
