//
// Created by Simon on 18.07.18.
//

#ifndef OPENGLPLAYGROUND_MODEL_HPP
#define OPENGLPLAYGROUND_MODEL_HPP

#include "Mesh.hpp"
#include <assimp/scene.h>

// Wraps a whole set of meshes into a model
class Model {
private:
    // Meshes
    std::vector<Mesh> m_meshes;

    // Process assimp node
    void processNode(aiNode *node, const aiScene *scene);

    // Process assimp mesh
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

public:
    // Construct model from file
    explicit Model(const std::string& file_name);

    // Destroy model
    void destroy();

    // Draw model
    void draw() const;
};

#endif //OPENGLPLAYGROUND_MODEL_HPP
