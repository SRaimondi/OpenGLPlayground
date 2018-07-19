//
// Created by Simon on 19.07.18.
//

#ifndef OPENGLPLAYGROUND_FRAMECOUNTER_HPP
#define OPENGLPLAYGROUND_FRAMECOUNTER_HPP

#include <cstddef>

// Small frame counter utility based on the glfw timer
class FrameCounter {
private:
    // Current frame rate
    double m_frame_rate;
    // Previous time
    double m_prev_time;
    // Elapsed time since last frame
    double m_elapsed_time;

public:
    // Create new FrameCounter
    FrameCounter();

    // Update frame rate counter
    void update();

    // Get current frame rate
    inline double getFrameRate() const {
        return m_frame_rate;
    }

    // Get elapsed time since last frame
    inline double getElapsedTime() const {
        return m_elapsed_time;
    }
};

#endif //OPENGLPLAYGROUND_FRAMECOUNTER_HPP
