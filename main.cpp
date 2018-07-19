#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "Shader.hpp"
#include "Model.hpp"
#include "FrameCounter.hpp"
#include "UniformBlock.hpp"

void processInput(GLFWwindow *window);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
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
    Model dragon_model("/Users/simon/Documents/Workspace/models/dragon_recon/dragon_vrip.ply");

    // Load shaders
    Shader diffuse_shader_v("shaders/diffuse.vert", ShaderType::Vertex);
    Shader diffuse_shader_f("shaders/diffuse.frag", ShaderType::Fragment);
    // Create program
    Program diffuse_program({diffuse_shader_v, diffuse_shader_f});


    // FIXME
    UniformBlock block(diffuse_program.getID(), "Matrices");

    // Prefetch attributes and uniforms locations
    diffuse_program.prefetchAttributes({"vertex_position", "vertex_normal"});
    diffuse_program.prefetchUniforms({"model", "view", "proj"});
    // Print informations
    diffuse_program.printInformations();

    // Set shader active
    diffuse_program.use();

    // Set uniforms
    diffuse_program.setMat4("view",
                            glm::lookAt(glm::vec3(0.f, 1.f, 1.f), glm::vec3(0.f, 0.2f, 0.f), glm::vec3(0.f, 1.f, 0.f)));
    diffuse_program.setMat4("proj", glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 20.f));

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

        // Update model matrix
        auto model = glm::rotate(glm::scale(glm::mat4(1.f), glm::vec3(3.f)),
                                 static_cast<float>(glfwGetTime()) * glm::radians(50.f), glm::vec3(0.f, 1.f, 0.f));
        diffuse_program.setMat4("model", model);

        // Draw mesh
        dragon_model.draw();

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
