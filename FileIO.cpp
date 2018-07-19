//
// Created by Simon on 18.07.18.
//

#include "FileIO.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::string loadFile(const std::string& file_name) {
    // Source file stream
    std::ifstream file;
    // Enable exceptions
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // Try to read source file
    try {
        // Open file
        file.open(file_name);
        // Read file into stream
        std::stringstream file_stream;
        file_stream << file.rdbuf();
        // Close file handler
        file.close();

        // Return file as string
        return file_stream.str();
    } catch (const std::ifstream::failure& ex) {
        std::cerr << "Error during file reading: " << ex.what() << "\n";
        return std::string();
    }
}
