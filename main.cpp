#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "Shader.hpp"
#include "Model.hpp"
#include "FrameCounter.hpp"

void processInput(GLFWwindow *window);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 1024;
const std::string title("OpenGL playground");

int main() {
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);
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

    // Create FrameCounter
    FrameCounter frame_counter;

    // Set clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Load model
    Model dragon_model("/Users/simon/Documents/Workspace/models/dragon.ply");

    // Load shaders
    Shader diffuse_shader_v("shaders/diffuse.vert", ShaderType::Vertex);
    Shader diffuse_shader_f("shaders/diffuse.frag", ShaderType::Fragment);
    // Create program
    Program diffuse_program({diffuse_shader_v, diffuse_shader_f});

    // Prefetch attributes and uniforms locations
    diffuse_program.prefetchAttributes({"vertex_position", "vertex_normal"});
    diffuse_program.prefetchUniform("model");
    diffuse_program.prefetchUniformBlock("Matrices");
    // Print informations
#ifndef NDEBUG
    diffuse_program.printInformations();
#endif

    // Load shaders
    Shader normal_shader_v("shaders/normal.vert", ShaderType::Vertex);
    Shader normal_shader_f("shaders/normal.frag", ShaderType::Fragment);
    // Create program
    Program normal_program({normal_shader_v, normal_shader_f});

    // Prefetch attributes and uniforms locations
    normal_program.prefetchAttributes({"vertex_position", "vertex_normal"});
    normal_program.prefetchUniform("model");
    normal_program.prefetchUniformBlock("Matrices");
    // Print informations
#ifndef NDEBUG
    normal_program.printInformations();
#endif

    // Create buffer with view and projection matrix
    const std::vector<glm::mat4> matrices = {
            glm::lookAt(glm::vec3(0.f, 2.f, 7.f), glm::vec3(0.f, 0.2f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
            glm::perspective(glm::radians(45.f), static_cast<float>(WIDTH) / HEIGHT, 0.1f, 20.f)};

//    // Create a buffer to store
//    GLuint matrices_buffer;
//    glGenBuffers(1, &matrices_buffer);
//    // Bind buffer to uniform buffer
//    glBindBuffer(GL_UNIFORM_BUFFER, matrices_buffer);

    // Create uniform buffer
    Buffer matrices_buffer(GL_UNIFORM_BUFFER, GL_STATIC_DRAW);
    matrices_buffer.bind();

    // Upload data
    const auto& un_block = diffuse_program.getUniformBlock("Matrices");
    constexpr bool whole_buffer = true;

    if (whole_buffer) {
        // Send data to buffer
        matrices_buffer.submitData(matrices);
    } else {
        // Allocate space
        matrices_buffer.allocateSpace(un_block.getBlockSize());
        // Send view matrix
        auto view_desc = un_block.getUniformDescription("view");
        matrices_buffer.submitSubData<glm::mat4>(
                std::vector<glm::mat4>(matrices.begin(), matrices.begin() + 1), view_desc.offset);
        // Send projection matrix
        auto proj_desc = un_block.getUniformDescription("proj");
        matrices_buffer.submitSubData<glm::mat4>(
                std::vector<glm::mat4>(matrices.begin() + 1, matrices.end()), proj_desc.offset);
    }

    matrices_buffer.unbind();

    // Bind buffer object to shader binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, diffuse_program.getUniformBlock("Matrices").getBindingPoint(),
                     matrices_buffer.getID());
    glBindBufferBase(GL_UNIFORM_BUFFER, normal_program.getUniformBlock("Matrices").getBindingPoint(),
                     matrices_buffer.getID());
    GL_CHECK();

    double last_frame_update = 0.0;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Update FrameCounter
        frame_counter.update();

        // Update window title
        if (last_frame_update > 0.1) {
            last_frame_update = 0.0;
            glfwSetWindowTitle(window, (title + " @ " + std::to_string(frame_counter.getFrameRate()) + " fps").c_str());
        } else {
            last_frame_update += frame_counter.getElapsedTime();
        }

        // Process input
        processInput(window);

        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute rotation matrix
        auto translate = glm::translate(glm::mat4(1.f), glm::vec3(-2.f, 0.f, 0.f));
        const auto rotation = glm::rotate(glm::mat4(1.f),
                                          glm::radians(45.f) * static_cast<float>(glfwGetTime()),
                                          glm::vec3(0.f, 1.f, 0.f));

        // Add translation
        auto model_diffuse = translate * rotation; //glm::translate(scale_rot, glm::vec3(-0.1f, 0.f, 0.f));

        diffuse_program.use();
        diffuse_program.setMat4("model", model_diffuse);
        // Draw mesh
        dragon_model.draw();

        // Draw right dragon
        translate = glm::translate(glm::mat4(1.f), glm::vec3(2.f, 0.f, 0.f));
        auto model_normal = translate * rotation;
        normal_program.use();
        normal_program.setMat4("model", model_normal);
        // Draw mesh
        dragon_model.draw();

        // Swap buffer
        glfwSwapBuffers(window);

        // Poll events
        glfwPollEvents();
    }

    // Cleanup

    // Destroy matrices buffer
    matrices_buffer.destroy();

    // Destroy model
    dragon_model.destroy();

    // destroy shaders
    diffuse_shader_v.destroy();
    diffuse_shader_f.destroy();
    diffuse_program.destroy();

    normal_shader_v.destroy();
    normal_shader_f.destroy();
    normal_program.destroy();

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
