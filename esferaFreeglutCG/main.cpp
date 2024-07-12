/*
Programa de esfera - Feita a partir de um octaedro
Usando Phong Shading
Escrito por Pedro Gabriel de Morais Ribeiro
*/

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Sphere.hpp"

#define FPS 60

bool fullscreen = false;

int win_w = 800, win_h = 600;

float aspect = (float)win_w/win_h;

float fov = 90.0f;

float ang = 0.0f;

//Shading
char *readShader(const char *shaderName);
void initShader();
unsigned int shaderProgram;

static void windowSettings();
static void drawWindow();
void specialFunc(int key, int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void reshapeFunc(int width, int height);
void timer(int i);

float luzRaio = 4.0f, luzAng_a = 90.0f, luzAng_b = 0.0f;
glm::vec3 luzPos(0.0f);
glm::vec3 luzColor(1.0f, 1.0f, 1.0f);

//Vetores da câmera
glm::vec3 eyePos(8.0f, 0.0f, 0.0f);
glm::vec3 centerPos(0.0f);
glm::vec3 upVec(0.0f, 0.0f, 1.0f);

//Esfera/Octaedro :)
Sphere S(2.0f);
Sphere luz(0.04f, luzColor);

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(win_w, win_h);
    glutCreateWindow("ESFERA COM SHADER :)");
    windowSettings();
    glutDisplayFunc(drawWindow);
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutReshapeFunc(reshapeFunc);
    glutTimerFunc(1000/FPS, timer, 0);
    glutMainLoop();
    return 0;
}

static void windowSettings() {
    //Inicializando biblioteca GLEW
    unsigned int err = glewInit();
    //se a GLEW não estiver instalada ou não carregada corretamente, mostre uma msg de erro e saia do programa
    if(err != GLEW_OK) {    
        std::cout << "Erro no carregamento da GLEW" << std::endl;
        exit(1);
    }
    //caso contrário, mostre a versão da GLEW sendo usada no momento
    // std::cout << "Usando GLEW " << glewGetString(GLEW_VERSION) << std::endl;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Indica qual cor será usada para limpar o frame buffer (normalmente usa uma cor de background)
    glClearDepth(1);
    glPointSize(6.0f);
    glLineWidth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    //Inicializando shaders
    initShader();
}

char *readShader(const char *shaderName){
    char *text = NULL;
    std::ifstream shaderFile(shaderName);
    std::string fileText;

    if(shaderFile.is_open()) {
        std::ostringstream stream;
        stream << shaderFile.rdbuf();
        fileText = stream.str();
        text = new char[fileText.length() + 1];
        strcpy(text, fileText.c_str());
    }
    else {
        std::cout << "Arquivo "<< shaderName <<" não pôde ser aberto" << std::endl;
    }
    return text;
}

void initShader() {
    const char *vertShaderSource = readShader("shaders/vertShader.glsl");
    const char *fragShaderSource = readShader("shaders/fragShader.glsl");

    int  success;
    char infoLog[512];

    //Criação do vertex shader
    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER); //indicando o OpenGL a criação de um vertex shader
    glShaderSource(vertShader, 1, &vertShaderSource, NULL);         //enviando o código do vertex shader para OpenGL
    glCompileShader(vertShader);                                //pedindo ao OpenGL a compilação desse código
    
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);     //verificando se ocorreu erro na compilação do shader
    if(!success){                                         //se o status possuir algum erro
        //pede ao OpenGL a mensagem de erro gerada na compilação e exibe na tela
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cout << "Erro de compilacao no vertex shader: \n" << infoLog << std::endl;
    }

    //Criação do fragment shader (igual ao processo acima, apenas adaptando de vertex para fragment)
    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "Erro de compilacao no fragment shader: \n" << infoLog << std::endl;
    }

    //Criando o programa de shaders (vertex e fragment shaders precisam ser conectados um ao outro)
    shaderProgram = glCreateProgram();          //criando o programa de shaders
    glAttachShader(shaderProgram, vertShader);  //anexando o vertex shader
    glAttachShader(shaderProgram, fragShader);  //anexando o fragment shader
    glLinkProgram(shaderProgram);               //linkando os dois shaders

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); //verificando se ocorreu erro na ligação dos shaders
    if(!success) {
        //pede ao OpenGL a mensagem de erro gerada na ligação e exibe na tela
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Erro de linkagem dos shaders: \n" << infoLog << std::endl;
    }
}

static void drawWindow() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Transformações de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4 projMatrix;

    //Projeção em perspectiva
    projMatrix = glm::perspective(glm::radians(fov), aspect, 1.0f, 256.0f);

    glMultMatrixf(glm::value_ptr(projMatrix));

    //Transformações de câmera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = glm::cos(glm::radians(ang)) * 8.0f;
    float camY = glm::sin(glm::radians(ang)) * 8.0f;

    eyePos = glm::vec3(camX, camY, 0.0f);

    glm::mat4 camMatrix;
    
    camMatrix = glm::lookAt(eyePos,
                            centerPos - eyePos,
                            upVec);
    glMultMatrixf(glm::value_ptr(camMatrix));

    //Ponto de luz
    luzPos = glm::vec3(glm::cos(glm::radians(luzAng_a)) * glm::cos(glm::radians(luzAng_b)) * luzRaio,
                       glm::sin(glm::radians(luzAng_b)) * glm::cos(glm::radians(luzAng_a)) * luzRaio, 
                       glm::sin(glm::radians(luzAng_a)) * luzRaio);

    glPushMatrix();
    glMultMatrixf(glm::value_ptr(glm::translate(glm::mat4(1.0f), luzPos)));
    luz.setDiv(3);
    luz.draw();
    glPopMatrix();

    /*glBegin(GL_POINTS);
        glColor3fv(glm::value_ptr(luzColor));
        glVertex3fv(glm::value_ptr(luzPos));
    glEnd();*/

    //Ativando shader
    glUseProgram(shaderProgram);
    
    //Iluminação
    unsigned int loc_luzPos = glGetUniformLocation(shaderProgram, "luzPos"); //Procurando a variável da posição da luz
    glUniform3f(loc_luzPos, luzPos.x, luzPos.y, luzPos.z); //Enviando para o programa de shader
    unsigned int loc_luzColor = glGetUniformLocation(shaderProgram, "luzColor"); //Procurando a variável de cor da luz
    glUniform3f(loc_luzColor, luzColor.r, luzColor.g, luzColor.b); //Enviando a cor
    unsigned int loc_eyePos = glGetUniformLocation(shaderProgram, "eyePos"); //Procurando a variável de posição de câmera
    glUniform3f(loc_eyePos, eyePos.x, eyePos.y, eyePos.z); //Enviando a posição da câmera
    
    //Desenhando a esfera
    S.draw();

    //Desativando shader
    glUseProgram(0);

    glutSwapBuffers();
}

void specialFunc(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            if(S.getDiv() < 6) S.setDiv(S.getDiv() + 1);
            break;
        case GLUT_KEY_DOWN:     
            if(S.getDiv() > 0) S.setDiv(S.getDiv() - 1);
            break;
        case GLUT_KEY_LEFT:
            ang += 180.0f/FPS;
            if(ang >= 360.0f) ang -= 360.0f; 
            break;
        case GLUT_KEY_RIGHT: 
            ang -= 180.0f/FPS;
            if(ang < 0.0f) ang += 360.0f;
            break;
        case GLUT_KEY_F11:
            if(fullscreen == false)  {
                glutFullScreen();
                fullscreen = true;
            }
            else {
                glutReshapeWindow(win_w, win_h);
                fullscreen = false;
            }
            break;
    }
}

void keyboardFunc(unsigned char key, int x, int y) {
    switch(key) {
        case 'r':
        case 'R':
            S.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 'g':
        case 'G':
            S.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case 'b':
        case 'B':
            S.setColor(glm::vec3(0.0f, 0.0f, 1.0f));
            break;
        case 'i':
        case 'I':
            S.setColor(glm::vec3(0.0f, 0.0f, 0.0f));
            break;
        case '-':
            if(fov < 90.0f) fov += 1.0f;
            break;
        case '+':
            if(fov > 30.0f) fov -= 1.0f;
            break;
        case 'w':
        case 'W':
            if(luzAng_a < 90.0f) luzAng_a += 90.0f/FPS;
            break;
        case 's':
        case 'S':
            if(luzAng_a > -90.0f) luzAng_a -= 90.0f/FPS;
            break;
        case 'a':
        case 'A':
            luzAng_b -= 180.0f/FPS;
            if(luzAng_b < 0.0f) luzAng_b += 360.0f;
            break;
        case 'd':
        case 'D': 
            luzAng_b += 180.0f/FPS;
            if(luzAng_b >= 360.0f) luzAng_b -= 360.0f; 
            break;
    }
}

void reshapeFunc(int width, int height) {
    win_w = width;
    win_h = height;
    aspect = (float)width/height;
    glViewport(0, 0, width, height);
}

void timer(int i) {
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer, 0);
}
