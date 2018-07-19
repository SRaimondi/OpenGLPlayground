//
// Created by Simon on 19.07.18.
//

#include <GLFW/glfw3.h>
#include "FrameCounter.hpp"

FrameCounter::FrameCounter()
        : m_frame_rate(0.0), m_prev_time(0.0), m_elapsed_time(0.0) {}

void FrameCounter::update() {
    // Get current time
    const double current_time = glfwGetTime();
    // Compute elapsed time
    m_elapsed_time = current_time - m_prev_time;
    // Update previous frame time
    m_prev_time = current_time;
    // Update frame rate
    m_frame_rate = 1.0 / m_elapsed_time;
}
