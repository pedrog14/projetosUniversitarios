#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <glm/vec3.hpp>
#include <vector>

class Sphere {
private:
    //Vetores de transformação
    glm::vec3 vecI, vecJ, vecK, spherePos;
    //Número de divisões feitas em faces do octaedro
    int div;
    //Raio da esfera
    float radius;
    glm::vec3 color;
    //Função para criar os vértices da esfera
    std::vector<glm::vec3> recursiveTriangle(std::vector<glm::vec3> ABC, int i);
    //Função para calcular a normal das faces dos triângulos
    glm::vec3 faceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
public:
    Sphere();
    Sphere(float radius);
    Sphere(float radius, glm::vec3 color);
    void draw();
    int getDiv();
    void setDiv(int div);
    void setColor(glm::vec3 color);
};

#endif // SPHERE_HPP
