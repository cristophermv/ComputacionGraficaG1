#ifndef GAME_H
#define GAME_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>

// Estructura para representar objetos con colisi�n
struct CollisionBox {
    float x, y, z;      // Posici�n
    float width, height, depth; // Dimensiones
    
    CollisionBox(float px, float py, float pz, float w, float h, float d);
};

class Game {
private:
    // Variables del jugador
    float playerX, playerY, playerZ;
    float playerYaw;   // Rotaci�n horizontal
    float playerPitch; // Rotaci�n vertical
    float velocityY;   // Velocidad vertical para salto
    bool onGround;
    bool isRunning;
    bool isShifting;
    bool firstPerson;
    
    // Variables de movimiento
    bool keys[256];
    bool specialKeys[256];
    
    // Variables del mouse
    int mouseX, mouseY;
    bool mouseInitialized;
    
    // Vector de objetos con colisi�n
    std::vector<CollisionBox> collisionBoxes;
    
    // Constantes del juego
    static const float GRAVITY;
    static const float JUMP_FORCE;
    static const float WALK_SPEED;
    static const float RUN_SPEED;
    static const float SHIFT_SPEED;
    static const float GROUND_LEVEL;
    static const float PLAYER_HEIGHT;
    static const float PLAYER_WIDTH;
    static const float MOUSE_SENSITIVITY;
    static const float PI;
    
    // M�todos privados
    void initArrays();
    void initCollisionBoxes();
    bool checkCollision(float newX, float newY, float newZ);
    void drawCube(float x, float y, float z, float size, float r, float g, float b);
    void drawGround();
    void drawPlayer();
    void drawScene();
    void setCamera();
    void renderBitmapString(float x, float y, void* font, const char* string);

public:
    // Constructor y destructor
    Game();
    ~Game();
    
    // M�todos p�blicos para inicializaci�n
    void initGL();
    void init(int argc, char** argv);
    
    // M�todos de callback (deben ser p�blicos para los callbacks de GLUT)
    void display();
    void keyboard(unsigned char key, int x, int y);
    void keyboardUp(unsigned char key, int x, int y);
    void specialKeysFunc(int key, int x, int y);
    void specialKeysUpFunc(int key, int x, int y);
    void mouseMotion(int x, int y);
    void updateTimer();
    void reshape(int w, int h);
    
    // MOVED TO PUBLIC: M�todos de actualizaci�n que necesita main.cpp
    void updatePhysics();
    void updateMovement();
    
    // M�todo para ejecutar el juego
    void run();
    void printControls();
    
    // Instancia est�tica para callbacks
    static Game* instance;
};

// Funciones callback globales para GLUT
void displayCallback();
void keyboardCallback(unsigned char key, int x, int y);
void keyboardUpCallback(unsigned char key, int x, int y);
void specialKeysCallback(int key, int x, int y);
void specialKeysUpCallback(int key, int x, int y);
void mouseMotionCallback(int x, int y);
void updateTimerCallback(int value);
void reshapeCallback(int w, int h);

#endif // GAME_H
