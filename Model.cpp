//
// Created by Simon on 18.07.18.
//

#include "Model.hpp"

// Assimp includes
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

// STL includes
#include <iostream>

void Model::processNode(aiNode *node, const aiScene *scene) {
    // Process all meshes at current node, if any
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        // Get pointer to mesh
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        // Process mesh
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // After processing the meshes, keep looking for other nodes
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *) {
    // Vector to fill with data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    // Loop over all the vertices and store them
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        // Store position
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        // Store normal
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        // Add vertex
        vertices.push_back(vertex);
    }

    // Loop over all faces and store the indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        // Get reference to face
        const aiFace& face = mesh->mFaces[i];
        // Add indices
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Print mesh statistics
    std::cout << "Loaded mesh with " << vertices.size() << " vertices and " << indices.size() / 3 << " triangles\n";

    return {vertices, indices};
}

Model::Model(const std::string& file_name) {
    // Read file with assimp
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file_name.c_str(),
                                             aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                             aiProcess_CalcTangentSpace);

    // Check if loading was successful
    if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr) {
        std::cerr << "Error when loading mesh with assimp\n";
        exit(EXIT_FAILURE);
    }

    // Start recursive node processing
    processNode(scene->mRootNode, scene);
}

void Model::destroy() {
    for (auto& mesh : m_meshes) {
        mesh.destroy();
    }
}

void Model::draw() const {
    for (const auto& mesh : m_meshes) {
        mesh.draw();
    }
}
