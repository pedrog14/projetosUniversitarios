#include "Sphere.hpp"

#include <glm/ext.hpp>
#include <GL/glew.h>

//SOBRECARGAS DA CLASSE VECTOR PARA CONCATENAR VETORES UTILIZANDO "+"
template <typename T>
std::vector<T> operator+(std::vector<T> const &x, std::vector<T> const &y) {
    std::vector<T> vec;
    vec.reserve(x.size() + y.size());
    vec.insert(vec.end(), x.begin(), x.end());
    vec.insert(vec.end(), y.begin(), y.end());
    return vec;
}

template <typename T>
std::vector<T> &operator+=(std::vector<T> &x, const std::vector<T> &y) {
    x.reserve(x.size() + y.size());
    x.insert(x.end(), y.begin(), y.end());
    return x;
}

Sphere::Sphere() {
    radius = 1.0f;
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    div = 0;
}

Sphere::Sphere(float radius) {
    this->radius = radius;
    color = glm::vec3(0.0f, 0.0f, 0.0f);
    div = 0;
}

Sphere::Sphere(float radius, glm::vec3 color) {
    this->radius = radius;
    this->color = color;
    div = 0;
}

std::vector<glm::vec3> Sphere::recursiveTriangle(std::vector<glm::vec3> ABC, int i) {
/*
        A
       / \
      D - F
     / \ / \
    B - E - C
*/

    if(i > 0) {
        std::vector<glm::vec3> DEF = {                      
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f)
        };

        DEF[0] = 0.5f * (ABC[0] + ABC[1]);
        DEF[1] = 0.5f * (ABC[1] + ABC[2]);
        DEF[2] = 0.5f * (ABC[0] + ABC[2]);

        return recursiveTriangle(std::vector<glm::vec3>{ABC[0], DEF[0], DEF[2]}, i - 1) +
               recursiveTriangle(std::vector<glm::vec3>{DEF[0], ABC[1], DEF[1]}, i - 1) +
               recursiveTriangle(std::vector<glm::vec3>{DEF[2], DEF[1], ABC[2]}, i - 1) +
               recursiveTriangle(std::vector<glm::vec3>{DEF[0], DEF[1], DEF[2]}, i - 1);
    }
    else {
        return ABC;
    }
}

glm::vec3 Sphere::faceNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
    return glm::normalize(glm::cross(v2 - v1, v3 - v1));
}

void Sphere::draw() {
    //Vetor de vec3 - Armazena os vértices da esfera/octraedro
    std::vector<glm::vec3> vertices;

    //Populando o vetor de vértices
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 4; j++) {

            vertices += recursiveTriangle(std::vector<glm::vec3> {
                glm::vec3(0.0f, 0.0f, glm::pow(-1.0f, i)),
                glm::vec3(glm::cos(glm::radians(45.0f + (90.0f * i) + (90.0f * j))), glm::sin(glm::radians(45.0f + (90.0f * i) + (90.0f * j))), 0.0f),
                glm::vec3(glm::cos(glm::radians(135.0f - (90.0f * i) + (90.0f * j))), glm::sin(glm::radians(135.0f - (90.0f * i) + (90.0f * j))), 0.0f)
            }, div);

        }
    }

    //Normalizando vértices
    for(int i = 0; i < vertices.size(); i++) vertices[i] = glm::normalize(vertices[i]) * radius;

    float *f_vertices = &vertices[0].x;

    //Desenhando esfera/octaedro
    if(!(color.r == 0.0f && color.g == 0.0f && color.b == 0.0f)) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glColor3f(color.r, color.g, color.b);

        for(int i = 0; i < vertices.size()/3; i++) {
            glBegin(GL_TRIANGLES);
                glNormal3fv(glm::value_ptr(faceNormal(vertices[0 + (3 * i)], 
                                                      vertices[1 + (3 * i)], 
                                                      vertices[2 + (3 * i)])));
                glVertex3fv(glm::value_ptr(vertices[0 + (3 * i)]));
                glVertex3fv(glm::value_ptr(vertices[1 + (3 * i)]));
                glVertex3fv(glm::value_ptr(vertices[2 + (3 * i)]));
            glEnd();
        }
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(1.0f, 1.0f, 1.0f);
    
        for(int i = 0; i < vertices.size()/3; i++) {
            glBegin(GL_LINE_LOOP);
                glNormal3fv(glm::value_ptr(faceNormal(vertices[0 + (3 * i)], 
                                                      vertices[1 + (3 * i)], 
                                                      vertices[2 + (3 * i)])));
                glVertex3fv(glm::value_ptr(vertices[0 + (3 * i)]));
                glVertex3fv(glm::value_ptr(vertices[1 + (3 * i)]));
                glVertex3fv(glm::value_ptr(vertices[2 + (3 * i)]));
            glEnd();
        }
    }
}

int Sphere::getDiv() {
    return div;
}

void Sphere::setDiv(int div) {
    this->div = div;
}

void Sphere::setColor(glm::vec3 color) {
    this->color = color;
}
