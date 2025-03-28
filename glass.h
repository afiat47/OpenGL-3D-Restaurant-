#pragma once
//
//  bottle.h
//  OpenGL Bottle Object
//

#ifndef GLASS_H
#define GLASS_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class Glass {
public:
    // Material properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Texture properties
    unsigned int diffuseMap;
    unsigned int specularMap;
    float shininess;

    // Constructor with default values
    Glass()
        : ambient(glm::vec3(0.2f)), diffuse(glm::vec3(0.5f)), specular(glm::vec3(1.0f)), shininess(32.0f),
        diffuseMap(0), specularMap(0) {
        setUpVertexData();
    }

    Glass(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
        : ambient(amb), diffuse(diff), specular(spec), shininess(shiny), diffuseMap(0), specularMap(0) {
        setUpVertexData();
    }

    Glass(unsigned int dMap, unsigned int sMap, float shiny)
        : diffuseMap(dMap), specularMap(sMap), shininess(shiny), ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)) {
        setUpVertexData();
    }

    ~Glass() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void drawWithMaterial(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();
        shader.setVec3("material.ambient", ambient);
        shader.setVec3("material.diffuse", diffuse);
        shader.setVec3("material.specular", specular);
        shader.setFloat("material.shininess", shininess);
        shader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }

    void drawWithTexture(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();

        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", shininess);
        shader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void setUpVertexData() {
        // Define the vertex data (positions, normals, texture coordinates)
        vertices = {
         0.080455f, 0.326600f, -0.000387f, 0.0000f, 1.0000f, 0.0000f, -1.4589f, 19.5897f,  // Vertex 1
         0.040562f, 0.326600f, 0.069482f, 0.0000f, -1.0000f, 0.0000f, 1.4589f, 19.5897f,  // Vertex 2
         0.039892f, 0.326600f, -0.069869f, 0.4908f, 0.1425f, -0.8596f, -1.5838f, 12.7649f,  // Vertex 3
         -0.039892f, 0.326600f, 0.069869f, 0.9898f, 0.1425f, -0.0048f, -0.2297f, 0.4821f,  // Vertex 4
         -0.040562f, 0.326600f, -0.069482f, -0.5042f, 0.0000f, -0.8636f, 0.2297f, 0.4821f,  // Vertex 5
         -0.080455f, 0.326600f, 0.000387f, -1.0000f, 0.0000f, 0.0048f, -1.7851f, -2.4982f,  // Vertex 6
         0.011671f, 0.049992f, -0.000056f, -0.4990f, 0.1425f, -0.8548f, 2.2320f, 12.2812f,  // Vertex 7
         0.005787f, 0.049992f, -0.010135f, -0.9898f, 0.1425f, 0.0048f, -2.2320f, 12.2812f,  // Vertex 8
         0.005884f, 0.049992f, 0.010079f, 0.4958f, 0.0000f, -0.8684f, 1.7409f, 19.1553f,  // Vertex 9
         -0.005787f, 0.049992f, 0.010135f, -0.4908f, 0.1425f, 0.8596f, -1.7409f, 19.1553f,  // Vertex 10
         -0.005884f, 0.049992f, -0.010079f, -0.4958f, 0.0000f, 0.8684f, 3.5702f, -0.0172f,  // Vertex 11
         -0.011671f, 0.049992f, 0.000056f, 0.4990f, 0.1425f, 0.8548f, 1.7702f, -3.1004f,  // Vertex 12
         0.056219f, 0.326600f, -0.098465f, 1.0000f, 0.0000f, -0.0048f, 1.7999f, 3.0833f,  // Vertex 13
         0.113383f, 0.326600f, -0.000545f, 0.5042f, 0.0000f, 0.8636f, -1.7702f, 3.1004f,  // Vertex 14
         0.043851f, 0.499862f, -0.076803f, 0.9993f, -0.0366f, -0.0048f, -1.7999f, -3.0833f,  // Vertex 15
         0.088439f, 0.499862f, -0.000425f, 0.4955f, -0.0366f, -0.8678f, -3.5702f, 0.0172f,  // Vertex 16
         0.090682f, 0.000000f, -0.000436f, -0.5038f, -0.0366f, -0.8630f, -1.7851f, 0.0000f,  // Vertex 17
         0.044964f, 0.000000f, -0.078751f, -0.4955f, -0.0366f, 0.8678f, -1.7851f, 0.6935f,  // Vertex 18
         0.045719f, 0.000000f, 0.078315f, 0.5038f, -0.0366f, 0.8630f, 1.7851f, 0.0000f,  // Vertex 19
         -0.044964f, 0.000000f, 0.078751f, -0.9993f, -0.0366f, 0.0048f, 1.7851f, 0.6935f,  // Vertex 20
         -0.045719f, 0.000000f, -0.078315f, 0.3792f, 0.9253f, -0.0018f, 1.5838f, 12.7649f,  // Vertex 21
         -0.090682f, 0.000000f, 0.000436f, 0.1912f, 0.9253f, 0.3274f, 1.7851f, -2.4982f,  // Vertex 22
         -0.045719f, 0.017616f, -0.078315f, -0.1880f, 0.9253f, 0.3293f, 0.9828f, 5.9778f,  // Vertex 23
         -0.090682f, 0.017616f, 0.000436f, 0.1872f, -0.9285f, 0.3207f, 0.2297f, 4.5411f,  // Vertex 24
         -0.057163f, 0.326600f, -0.097920f, -0.1841f, -0.9285f, 0.3225f, -0.9828f, 5.9778f,  // Vertex 25
         -0.044587f, 0.499862f, -0.076377f, -0.8537f, -0.5208f, 0.0041f, -0.2297f, 4.5411f,  // Vertex 26
         -0.113383f, 0.326600f, 0.000545f, -0.4233f, -0.5208f, 0.7414f, -1.8318f, 11.6914f,  // Vertex 27
         -0.088439f, 0.499862f, 0.000425f, -0.1872f, -0.9285f, -0.3207f, -2.2320f, 13.1750f,  // Vertex 28
         0.044964f, 0.017616f, -0.078751f, -0.3713f, -0.9285f, 0.0018f, 1.8318f, 11.6914f,  // Vertex 29
         -0.043851f, 0.499862f, 0.076803f, -0.3792f, 0.9253f, 0.0018f, 2.2320f, 13.1750f,  // Vertex 30
         -0.056219f, 0.326600f, 0.098465f, 0.1880f, 0.9253f, -0.3293f, 0.2297f, 9.9697f,  // Vertex 31
         -0.044964f, 0.017616f, 0.078751f, -0.1912f, 0.9253f, -0.3274f, 0.2297f, 1.9682f,  // Vertex 32
         0.057163f, 0.326600f, 0.097920f, 0.3713f, -0.9285f, -0.0018f, -0.2297f, 9.9697f,  // Vertex 33
         0.044587f, 0.499862f, 0.076377f, 0.1841f, -0.9285f, -0.3225f, -0.2297f, 1.9682f,  // Vertex 34
         0.090682f, 0.017616f, -0.000436f, 0.8537f, -0.5208f, -0.0041f, -1.8318f, 9.0228f,  // Vertex 35
         0.045719f, 0.017616f, 0.078315f, 0.4304f, -0.5208f, 0.7372f, 1.8318f, 9.0228f,  // Vertex 36
         -0.074113f, 0.499862f, 0.000356f, -0.4304f, -0.5208f, -0.7372f, -0.9828f, 7.2586f,  // Vertex 37
         -0.036748f, 0.499862f, 0.064362f, 0.4233f, -0.5208f, -0.7414f, 0.9828f, 7.2586f,  // Vertex 38
         0.037365f, 0.499862f, 0.064006f, 0.4977f, -0.8673f, -0.0024f, -3.4818f, -0.0167f,  // Vertex 39
         0.036748f, 0.499862f, -0.064362f, 0.2468f, -0.8673f, -0.4322f, -2.9178f, -0.0140f,  // Vertex 40
         -0.037365f, 0.499862f, -0.064006f, -0.2509f, -0.8673f, -0.4298f, -1.7264f, -3.0237f,  // Vertex 41
         0.074113f, 0.499862f, -0.000356f, 0.2509f, -0.8673f, 0.4298f, -1.7554f, 3.0070f,  // Vertex 42
         0.025170f, 0.268529f, 0.043115f, -0.2468f, -0.8673f, 0.4322f, -1.4468f, -2.5339f,  // Vertex 43
         0.005884f, 0.253229f, 0.010079f, -0.4977f, -0.8673f, 0.0024f, -1.4711f, 2.5199f,  // Vertex 44
         -0.024754f, 0.268529f, 0.043355f, -0.4908f, -0.1425f, 0.8596f, 1.7554f, -3.0070f,  // Vertex 45
         -0.005787f, 0.253229f, 0.010135f, -0.9898f, -0.1425f, 0.0048f, 1.4711f, -2.5199f,  // Vertex 46
         -0.093056f, 0.293280f, 0.000447f, 0.9898f, -0.1425f, -0.0048f, 1.7264f, 3.0237f,  // Vertex 47
         -0.046141f, 0.293280f, 0.080813f, 0.4990f, -0.1425f, 0.8548f, 1.4468f, 2.5339f,  // Vertex 48
         0.011671f, 0.253229f, -0.000056f, 0.4908f, -0.1425f, -0.8596f, 2.9178f, 0.0140f,  // Vertex 49
         -0.005884f, 0.253229f, -0.010079f, -0.4990f, -0.1425f, -0.8548f, 3.4818f, 0.0167f,  // Vertex 50
         -0.025170f, 0.268529f, -0.043115f, -0.4955f, 0.0366f, 0.8678f, -1.4589f, 19.5897f,  // Vertex 51
         -0.011671f, 0.253229f, 0.000056f, -0.9993f, 0.0366f, 0.0048f, 1.4589f, 19.5897f,  // Vertex 52
         -0.049924f, 0.268529f, 0.000240f, 0.5038f, 0.0366f, 0.8630f, -1.5838f, 12.7649f,  // Vertex 53
         0.005787f, 0.253229f, -0.010135f, 0.4955f, 0.0366f, -0.8678f, -0.2297f, 0.4821f,  // Vertex 54
         0.049924f, 0.268529f, -0.000240f, -0.5038f, 0.0366f, -0.8630f, 0.2297f, 0.4821f,  // Vertex 55
         0.024754f, 0.268529f, -0.043355f, 0.9993f, 0.0366f, -0.0048f, -1.7851f, -2.4982f,  // Vertex 56
         0.093056f, 0.293280f, -0.000447f, -0.3792f, -0.9253f, 0.0018f, 2.2320f, 12.2812f,  // Vertex 57
         0.046915f, 0.293280f, 0.080365f, -0.1912f, -0.9253f, -0.3274f, -2.2320f, 12.2812f,  // Vertex 58
         -0.046915f, 0.293280f, -0.080365f, 0.1880f, -0.9253f, -0.3293f, 1.7409f, 19.1553f,  // Vertex 59
         0.046141f, 0.293280f, -0.080813f, 0.1841f, 0.9285f, -0.3225f, -1.7409f, 19.1553f,  // Vertex 60
        };

        indices = {
            2, 1, 0, 1, 2, 3,  // Face 1
            3, 2, 4, 3, 4, 5,  // Face 2
            8, 7, 6, 7, 8, 9,  // Face 3
            7, 9, 10, 10, 9, 11,  // Face 4
            14, 13, 12, 13, 14, 15,  // Face 5
            18, 17, 16, 17, 18, 19,  // Face 6
            17, 19, 20, 20, 19, 21,  // Face 7
            21, 22, 20, 22, 21, 23,  // Face 8
            26, 25, 24, 25, 26, 27,  // Face 9
            22, 17, 20, 17, 22, 28,  // Face 10
            26, 29, 27, 29, 26, 30,  // Face 11
            19, 23, 21, 23, 19, 31,  // Face 12
            15, 32, 13, 32, 15, 33,  // Face 13
            33, 30, 32, 30, 33, 29,  // Face 14
            35, 16, 34, 16, 35, 18,  // Face 15
            34, 17, 28, 17, 34, 16,  // Face 16
            35, 19, 18, 19, 35, 31,  // Face 17
            25, 12, 24, 12, 25, 14,  // Face 18
            37, 5, 36, 5, 37, 3,  // Face 19
            37, 1, 3, 1, 37, 38,  // Face 20
            39, 4, 2, 4, 39, 40,  // Face 21
            1, 41, 0, 41, 1, 38,  // Face 22
            40, 5, 4, 5, 40, 36,  // Face 23
            0, 39, 2, 39, 0, 41,  // Face 24
            6, 35, 34, 35, 6, 8,  // Face 25
            8, 31, 35, 31, 8, 9,  // Face 26
            44, 43, 42, 43, 44, 45,  // Face 27
            47, 26, 46, 26, 47, 30,  // Face 28
            43, 6, 48, 6, 43, 8,  // Face 29
            43, 9, 8, 9, 43, 45,  // Face 30
            51, 50, 49, 50, 51, 52,  // Face 31
            11, 31, 9, 31, 11, 23,  // Face 32
            48, 7, 53, 7, 48, 6,  // Face 33
            44, 51, 45, 51, 44, 52,  // Face 34
            11, 49, 10, 49, 11, 51,  // Face 35
            9, 51, 11, 51, 9, 45,  // Face 36
            22, 7, 28, 7, 22, 10,  // Face 37
            48, 55, 54, 55, 48, 53,  // Face 38
            53, 50, 55, 50, 53, 49,  // Face 39
            49, 7, 10, 7, 49, 53,  // Face 40
            22, 11, 10, 11, 22, 23,  // Face 41
            42, 48, 54, 48, 42, 43,  // Face 42
            28, 6, 34, 6, 28, 7,  // Face 43
            32, 56, 13, 56, 32, 57,  // Face 44
            24, 59, 58, 59, 24, 12,  // Face 45
            32, 47, 57, 47, 32, 30,  // Face 46
            54, 59, 56, 59, 54, 55,  // Face 47
            13, 59, 12, 59, 13, 56,  // Face 48
            55, 58, 59, 58, 55, 50,  // Face 49
            47, 42, 57, 42, 47, 44,  // Face 50
            58, 26, 24, 26, 58, 46,  // Face 51
            57, 54, 56, 54, 57, 42,  // Face 52
            47, 52, 44, 52, 47, 46,  // Face 53
            52, 58, 50, 58, 52, 46,  // Face 54
            14, 41, 15, 41, 33, 15,  // Face 55
            41, 14, 39, 33, 41, 38,  // Face 56
            39, 14, 25, 39, 25, 40,  // Face 57
            33, 38, 29, 29, 38, 37,  // Face 58
            40, 25, 36, 29, 37, 36,  // Face 59
            36, 25, 27, 29, 36, 27,  // Face 60
            12, 13, 14, 15, 14, 13,  // Face 61
            16, 17, 18, 19, 18, 17,  // Face 62
            20, 19, 17, 21, 19, 20,  // Face 63
            20, 22, 21, 23, 21, 22,  // Face 64
            24, 25, 26, 27, 26, 25,  // Face 65
            20, 17, 22, 28, 22, 17,  // Face 66
            27, 29, 26, 30, 26, 29,  // Face 67
            21, 23, 19, 31, 19, 23,  // Face 68
            13, 32, 15, 33, 15, 32,  // Face 69
            32, 30, 33, 29, 33, 30,  // Face 70
            34, 16, 35, 18, 35, 16,  // Face 71
            28, 17, 34, 16, 34, 17,  // Face 72
            18, 19, 35, 31, 35, 19,  // Face 73
            24, 12, 25, 14, 25, 12,  // Face 74
            36, 5, 37, 3, 37, 5,  // Face 75
            3, 1, 37, 38, 37, 1,  // Face 76
            2, 4, 39, 40, 39, 4,  // Face 77
            0, 41, 1, 38, 1, 41,  // Face 78
            4, 5, 40, 36, 40, 5,  // Face 79
            2, 39, 0, 41, 0, 39,  // Face 80
            34, 35, 6, 8, 6, 35,  // Face 81
            35, 31, 8, 9, 8, 31,  // Face 82
            42, 43, 44, 45, 44, 43,  // Face 83
            46, 26, 47, 30, 47, 26,  // Face 84
            48, 6, 43, 8, 43, 6,  // Face 85
            8, 9, 43, 45, 43, 9,  // Face 86
            49, 50, 51, 52, 51, 50,  // Face 87
            9, 31, 11, 23, 11, 31,  // Face 88
            53, 7, 48, 6, 48, 7,  // Face 89
            45, 51, 44, 52, 44, 51,  // Face 90
            10, 49, 11, 51, 11, 49,  // Face 91
            11, 51, 9, 45, 9, 51,  // Face 92
            28, 7, 22, 10, 22, 7,  // Face 93
            54, 55, 48, 53, 48, 55,  // Face 94
            55, 50, 53, 49, 53, 50,  // Face 95
            10, 7, 49, 53, 49, 7,  // Face 96
            10, 11, 22, 23, 22, 11,  // Face 97
            54, 48, 42, 43, 42, 48,  // Face 98
            34, 6, 28, 7, 28, 6,  // Face 99
            13, 56, 32, 57, 32, 56,  // Face 100
            58, 59, 24, 12, 24, 59,  // Face 101
            57, 47, 32, 30, 32, 47,  // Face 102
            56, 59, 54, 55, 54, 59,  // Face 103
            12, 59, 13, 56, 13, 59,  // Face 104
            59, 58, 55, 50, 55, 58,  // Face 105
            57, 42, 47, 44, 47, 42,  // Face 106
            24, 26, 58, 46, 58, 26,  // Face 107
            56, 54, 57, 42, 57, 54,  // Face 108
            44, 52, 47, 46, 47, 52,  // Face 109
            50, 58, 52, 46, 52, 58,  // Face 110
            15, 41, 14, 15, 33, 41,  // Face 111
            39, 14, 41, 38, 41, 33,  // Face 112
            25, 14, 39, 40, 25, 39,  // Face 113
            29, 38, 33, 37, 38, 29,  // Face 114
            36, 25, 40, 36, 37, 29,  // Face 115
            27, 25, 36, 27, 36, 29,  // Face 116
        };




        // Generate and bind VAO, VBO, EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
};

#endif /* GLASS_H */
