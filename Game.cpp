#include "Game.h"
#include "Casa3D.h"  // Asegúrate de guardar el código como Casa3D.h
#include "FlorindaCasa.h"
#include "CasaClotilde.h"
#include "Barril.h"
#include "Pelota.h"
#include "Macetas.h"
#include "Escalera.h"
#include "Lavadero.h"
#include "Pileta.h"  // ¡Aquí importas tu pileta!
#include "Casac.h"
#include "ramon.h"
#include "Carro.h"
// Definir constantes estáticas
const float Game::GRAVITY = -0.008f;
const float Game::JUMP_FORCE = 0.25f;
const float Game::WALK_SPEED = 0.1f;
const float Game::RUN_SPEED = 0.2f;
const float Game::SHIFT_SPEED = 0.05f;
const float Game::GROUND_LEVEL = 0.0f;
const float Game::PLAYER_HEIGHT = 1.8f;
const float Game::PLAYER_WIDTH = 0.6f;
const float Game::MOUSE_SENSITIVITY = 0.2f;
const float Game::PI = 3.14159265f;
Casa3D casa;  // En tu clase Game o como variable global
FlorindaCasa casa1;
CasaClotilde casa2;
Escalera escalera1;
Lavadero lavadero;
Pileta miPileta;  // Instancia de la pileta
Ramon personaje;

// Instancia estática para callbacks
Game* Game::instance = NULL;

// Implementación de CollisionBox
CollisionBox::CollisionBox(float px, float py, float pz, float w, float h, float d) 
    : x(px), y(py), z(pz), width(w), height(h), depth(d) {}

// Constructor
Game::Game() : playerX(40.0f), playerY(0.0f), playerZ(60.0f), 
               playerYaw(-90.0f), playerPitch(0.0f), velocityY(0.0f),
               onGround(true), isRunning(false), isShifting(false), 
               firstPerson(true), mouseX(0), mouseY(0), mouseInitialized(false) {
    initArrays();
    Game::instance = this;
}

// Destructor
Game::~Game() {
    Game::instance = NULL;
}

// Inicializar arrays
void Game::initArrays() {
    for(int i = 0; i < 256; i++) {
        keys[i] = false;
        specialKeys[i] = false;
    }
}

// Inicializar cajas de colisión
void Game::initCollisionBoxes() {
    collisionBoxes.clear();
    
    // Agregar cajas de colisión para todos los objetos del mundo
    collisionBoxes.push_back(CollisionBox(5, 0.5f, 5, 1.0f, 1.0f, 1.0f));
    collisionBoxes.push_back(CollisionBox(-5, 0.5f, 5, 1.0f, 1.0f, 1.0f));
    collisionBoxes.push_back(CollisionBox(5, 0.5f, -5, 1.0f, 1.0f, 1.0f));
    collisionBoxes.push_back(CollisionBox(-5, 0.5f, -5, 1.0f, 1.0f, 1.0f));
    
    // Edificios
    collisionBoxes.push_back(CollisionBox(10, 2.5f, 0, 5.0f, 5.0f, 5.0f));
    collisionBoxes.push_back(CollisionBox(-10, 1.5f, -10, 3.0f, 3.0f, 3.0f));
}

// Verificar colisiones
bool Game::checkCollision(float newX, float newY, float newZ) {
    float playerLeft = newX - PLAYER_WIDTH/2;
    float playerRight = newX + PLAYER_WIDTH/2;
    float playerBottom = newY;
    float playerTop = newY + PLAYER_HEIGHT;
    float playerFront = newZ + PLAYER_WIDTH/2;
    float playerBack = newZ - PLAYER_WIDTH/2;
    
    for(size_t i = 0; i < collisionBoxes.size(); i++) {
        const CollisionBox& box = collisionBoxes[i];
        
        float boxLeft = box.x - box.width/2;
        float boxRight = box.x + box.width/2;
        float boxBottom = box.y - box.height/2;
        float boxTop = box.y + box.height/2;
        float boxFront = box.z + box.depth/2;
        float boxBack = box.z - box.depth/2;
        
        bool collisionX = (playerRight > boxLeft) && (playerLeft < boxRight);
        bool collisionY = (playerTop > boxBottom) && (playerBottom < boxTop);
        bool collisionZ = (playerFront > boxBack) && (playerBack < boxFront);
        
        if(collisionX && collisionY && collisionZ) {
            return true;
        }
    }
    
    return false;
}

// Inicializar OpenGL
void Game::initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    
    glutSetCursor(GLUT_CURSOR_NONE);
    
    initCollisionBoxes();
}

// Dibujar cubo
void Game::drawCube(float x, float y, float z, float size, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(r, g, b);
    glutSolidCube(size);
    glPopMatrix();
}

// Dibujar suelo
void Game::drawGround() {
    glColor3f(0.2f, 0.7f, 0.2f);
    glBegin(GL_QUADS);
    for(int x = -200; x < 200; x++) {
        for(int z = -200; z < 200; z++) {
            glNormal3f(0, 1, 0);
            glVertex3f((float)x, GROUND_LEVEL, (float)z);
            glVertex3f((float)(x+1), GROUND_LEVEL, (float)z);
            glVertex3f((float)(x+1), GROUND_LEVEL, (float)(z+1));
            glVertex3f((float)x, GROUND_LEVEL, (float)(z+1));
        }
    }
    glEnd();
}

// Dibujar jugador
void Game::drawPlayer() {
    if(firstPerson) return;
    
    glPushMatrix();
    glTranslatef(playerX, playerY + 0.9f, playerZ);
    glColor3f(1.0f, 0.5f, 0.0f);
    glutSolidCube(0.6f);
    
    glTranslatef(0, 0.6f, 0);
    glColor3f(1.0f, 0.8f, 0.6f);
    glutSolidSphere(0.3f, 12, 12);
    
    glPopMatrix();
}

// Dibujar escena
void Game::drawScene() {
	
	// Habilita luz y material
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_DEPTH_TEST);

// Define una luz (por ejemplo, una blanca suave desde arriba)
GLfloat light_pos[] = { 0.0f, 5.0f, 5.0f, 1.0f };
GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    drawGround();
    
    
    
    
    
    drawPlayer();
    
    drawCube(5, 0.5f, 5, 1.0f, 1.0f, 0.0f, 0.0f);
    drawCube(-5, 0.5f, 5, 1.0f, 0.0f, 1.0f, 0.0f);
    drawCube(5, 0.5f, -5, 1.0f, 0.0f, 0.0f, 1.0f);
    drawCube(-5, 0.5f, -5, 1.0f, 1.0f, 1.0f, 0.0f);
    
    drawCube(10, 2.5f, 0, 5.0f, 0.6f, 0.6f, 0.6f);
    drawCube(-10, 1.5f, -10, 3.0f, 0.8f, 0.4f, 0.2f);




    glPushMatrix();                    // Guarda la matriz actual
    glTranslatef(40.0f, 0.1f, 0.0f);   // Mueve la casa 10 unidades a la derecha (x)
    glScalef(1.3f, 1.3f, 1.0f);        // Escala la casa en 1.5x (puedes usar 2.0f si prefieres el doble)
    casa.draw();                      // Dibuja la casa escalada y trasladada
    glPopMatrix();                     // Restaura la matriz original

    
glPushMatrix();                              
glTranslatef(15.0f, 3.7f, 15.0f);             // Mueve la casa a donde la quieras ubicar
glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rota 90° en Y ? de "mirar al frente" pasa a "mirar a la izquierda"

glScalef(-1.2f, 1.2f, 1.0f);                 // Voltea horizontalmente y escala
casa1.dibujar();                             
glPopMatrix();


    glPushMatrix();                    // Guarda la matriz actual
    glTranslatef(20.0f, 0.1f, 0.0f);   // Mueve la casa 10 unidades a la derecha (x)
    glScalef(2.4f, 2.4f, 2.4f);        // Escala la casa en 1.5x (puedes usar 2.0f si prefieres el doble)
    casa2.render();                      // Dibuja la casa escalada y trasladada
    glPopMatrix();                     // Restaura la matriz original



    glPushMatrix();                    // Guarda la matriz actual
    glTranslatef(32.0f, 1.1f, 21.0f);   // Mueve la casa 10 unidades a la derecha (x)
    glScalef(1.3f, 1.3f, 1.0f);        // Escala la casa en 1.5x (puedes usar 2.0f si prefieres el doble)
    DrawBarril();                      // Dibuja la casa escalada y trasladada
    glPopMatrix();                     // Restaura la matriz original

    glPushMatrix();                    // Guarda la matriz actual
    glTranslatef(32.0f, 1.1f, 16.0f);   // Mueve la casa 10 unidades a la derecha (x)
    glScalef(1.3f, 1.3f, 1.0f);        // Escala la casa en 1.5x (puedes usar 2.0f si prefieres el doble)
DrawPelotaColor(1.0f, 0.0f, 0.0f); // Rojo
    glPopMatrix();                     // Restaura la matriz original
    
    
    
        glPushMatrix();                    // Guarda la matriz actual
glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rota 90° sobre X para levantarla

    glTranslatef(46.0f, -8.1f, 0.0f);   // Mueve la casa 10 unidades a la derecha (x)

    glScalef(1.3f, 1.3f, 1.0f);        // Escala la casa en 1.5x (puedes usar 2.0f si prefieres el doble)
drawSinglePot(-2.0f, 0.0f, 0.0f, 0.6f, 0.8f, 1.2f, 0.8f, 0.2f, 0.2f);
    glPopMatrix();                     // Restaura la matriz original
    
    
    
glPushMatrix();
glTranslatef(25.0f, 0.0f, 28.0f);    // Mover la escalera
glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Rotar 90° sobre el eje Y
glScalef(1.1f, 1.1f, 1.1f);         // Escalar 10% más en todos los ejes
escalera1.draw(0, 0, 0);
glPopMatrix();
    
    
    
    glPushMatrix();
glTranslatef(21.0f, 0.0f, 20.5f);    // Mover la escalera
glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Rotar 90° sobre el eje Y

lavadero.draw();
glPopMatrix();
    
glPushMatrix();
glTranslatef(33.0f, 0.0f, -25.0f);   // Cambia los valores según dónde quieras mover la pileta
glScalef(2.5f, 2.5f, 2.5f);         // Escala un 30% más en todos los ejes
miPileta.dibujar();
glPopMatrix();





glPushMatrix();
glTranslatef(18.5f, 7.0f, 28.0f);    // Mover la escalera
glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Rotar 90° sobre el eje Y
glScalef(2.8f, 2.8f, 2.8f);         // Escalar 10% más en todos los ejes
DrawCasac();
glPopMatrix();

    glPushMatrix();
glTranslatef(35.5f, 2.0f, 13.0f);    // Mover la escalera
glRotatef(0.0f, 0.0f, 1.0f, 0.0f);  // Rotar 90° sobre el eje Y
glScalef(1.4f, 1.4f, 1.4f);         // Escalar 10% más en todos los ejes
    personaje.dibujar();
glPopMatrix();

    
    
    
    glPushMatrix();
glTranslatef(20.5f, 0.0f, 18.0f);    // Mover la escalera
glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // Rotar 90° sobre el eje Y
      // Escalar 10% más en todos los ejes
DrawCarro();
glPopMatrix();

}

// Configurar cámara
void Game::setCamera() {
    glLoadIdentity();
    
    if(firstPerson) {
        float eyeX = playerX;
        float eyeY = playerY + PLAYER_HEIGHT - 0.2f;
        float eyeZ = playerZ;
        
        float lookX = eyeX + cos(playerYaw * PI / 180.0f) * cos(playerPitch * PI / 180.0f);
        float lookY = eyeY + sin(playerPitch * PI / 180.0f);
        float lookZ = eyeZ + sin(playerYaw * PI / 180.0f) * cos(playerPitch * PI / 180.0f);
        
        gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 1, 0);
    } else {
        float distance = 5.0f;
        float eyeX = playerX - distance * cos(playerYaw * PI / 180.0f);
        float eyeY = playerY + 3.0f;
        float eyeZ = playerZ - distance * sin(playerYaw * PI / 180.0f);
        
        gluLookAt(eyeX, eyeY, eyeZ, playerX, playerY + 1.0f, playerZ, 0, 1, 0);
    }
}

// Actualizar física
void Game::updatePhysics() {
    if(!onGround) {
        velocityY += GRAVITY;
    }
    
    float newY = playerY + velocityY;
    
    if(newY <= GROUND_LEVEL) {
        playerY = GROUND_LEVEL;
        velocityY = 0.0f;
        onGround = true;
    } else {
        if(!checkCollision(playerX, newY, playerZ)) {
            playerY = newY;
            onGround = false;
        } else {
            velocityY = 0.0f;
            if(playerY <= GROUND_LEVEL + 0.1f) {
                onGround = true;
            }
        }
    }
}

// Actualizar movimiento
void Game::updateMovement() {
    float speed = WALK_SPEED;
    
    if(isShifting) {
        speed = SHIFT_SPEED;
    } else if(isRunning) {
        speed = RUN_SPEED;
    }
    
    float moveX = 0.0f, moveZ = 0.0f;
    
    if(keys['w'] || keys['W']) {
        moveX += cos(playerYaw * PI / 180.0f) * speed;
        moveZ += sin(playerYaw * PI / 180.0f) * speed;
    }
    if(keys['s'] || keys['S']) {
        moveX -= cos(playerYaw * PI / 180.0f) * speed;
        moveZ -= sin(playerYaw * PI / 180.0f) * speed;
    }
    if(keys['a'] || keys['A']) {
        moveX += cos((playerYaw - 90) * PI / 180.0f) * speed;
        moveZ += sin((playerYaw - 90) * PI / 180.0f) * speed;
    }
    if(keys['d'] || keys['D']) {
        moveX += cos((playerYaw + 90) * PI / 180.0f) * speed;
        moveZ += sin((playerYaw + 90) * PI / 180.0f) * speed;
    }
    
    float newX = playerX + moveX;
    float newZ = playerZ + moveZ;
    
    if(!checkCollision(newX, playerY, playerZ)) {
        playerX = newX;
    }
    
    if(!checkCollision(playerX, playerY, newZ)) {
        playerZ = newZ;
    }
    
    if(keys[' '] && onGround) {
        velocityY = JUMP_FORCE;
        onGround = false;
    }
}

// Renderizar texto
void Game::renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Función display
void Game::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float aspectRatio = (float)viewport[2] / (float)viewport[3];
    gluPerspective(75.0, aspectRatio, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    setCamera();
    
    drawScene();
    
    // Información en pantalla
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1280, 0, 720, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    if(!firstPerson) {
        renderBitmapString(10, 700, GLUT_BITMAP_HELVETICA_12, "Tercera Persona - C para cambiar");
    } else {
        renderBitmapString(10, 700, GLUT_BITMAP_HELVETICA_12, "Primera Persona - C para cambiar");
        
        // Crosshair
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(635, 360); glVertex2f(645, 360);
        glVertex2f(640, 355); glVertex2f(640, 365);
        glEnd();
    }
    
    // Información de controles
    glColor3f(1.0f, 1.0f, 0.0f);
    renderBitmapString(10, 670, GLUT_BITMAP_HELVETICA_10, "WASD: Mover, Espacio: Saltar");
    renderBitmapString(10, 655, GLUT_BITMAP_HELVETICA_10, "R=Toggle Correr, X=Toggle Lento");
    renderBitmapString(10, 640, GLUT_BITMAP_HELVETICA_10, "C=Cambiar camara, Mouse: Mirar");
    
    // Estado actual
    glColor3f(0.0f, 1.0f, 1.0f);
    if(isRunning && !isShifting) {
        renderBitmapString(10, 625, GLUT_BITMAP_HELVETICA_10, "Estado: CORRIENDO");
    } else if(isShifting) {
        renderBitmapString(10, 625, GLUT_BITMAP_HELVETICA_10, "Estado: CAMINANDO DESPACIO");
    } else {
        renderBitmapString(10, 625, GLUT_BITMAP_HELVETICA_10, "Estado: CAMINANDO NORMAL");
    }
    
    // Información de colisiones
    glColor3f(1.0f, 0.5f, 0.5f);
    renderBitmapString(10, 610, GLUT_BITMAP_HELVETICA_10, "Colisiones activadas - No puedes atravesar objetos");
    
    glEnable(GL_LIGHTING);
    
    glutSwapBuffers();
}

// Funciones de teclado
void Game::keyboard(unsigned char key, int x, int y) {
    keys[key] = true;
    
    switch(key) {
        case 'c':
        case 'C':
            firstPerson = !firstPerson;
            break;
        case 'r':
        case 'R':
            isRunning = !isRunning;
            break;
        case 'x':
        case 'X':
            isShifting = !isShifting;
            break;
        case 27: // ESC
            exit(0);
            break;
    }
}

void Game::keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void Game::specialKeysFunc(int key, int x, int y) {
    specialKeys[key] = true;
    
    switch(key) {
        case GLUT_KEY_LEFT:
            if(!isRunning) isRunning = true;
            break;
        case GLUT_KEY_RIGHT:
            if(!isShifting) isShifting = true;
            break;
        case GLUT_KEY_UP:
            if(!isRunning) isRunning = true;
            break;
        case GLUT_KEY_DOWN:
            if(!isShifting) isShifting = true;
            break;
    }
}

void Game::specialKeysUpFunc(int key, int x, int y) {
    specialKeys[key] = false;
    
    switch(key) {
        case GLUT_KEY_LEFT:
            isRunning = false;
            break;
        case GLUT_KEY_RIGHT:
            isShifting = false;
            break;
        case GLUT_KEY_UP:
            isRunning = false;
            break;
        case GLUT_KEY_DOWN:
            isShifting = false;
            break;
    }
}

// Movimiento del mouse
void Game::mouseMotion(int x, int y) {
    if(!mouseInitialized) {
        mouseX = x;
        mouseY = y;
        mouseInitialized = true;
        return;
    }
    
    int deltaX = x - mouseX;
    int deltaY = y - mouseY;
    
    playerYaw += deltaX * MOUSE_SENSITIVITY;
    playerPitch -= deltaY * MOUSE_SENSITIVITY;
    
    if(playerPitch > 89.0f) playerPitch = 89.0f;
    if(playerPitch < -89.0f) playerPitch = -89.0f;
    
    mouseX = x;
    mouseY = y;
    
    glutWarpPointer(640, 360);
    mouseX = 640;
    mouseY = 360;
}

// Timer de actualización
void Game::updateTimer() {
    updateMovement();
    updatePhysics();
    glutPostRedisplay();

}

// Redimensionar ventana
void Game::reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

// Inicialización del juego
void Game::init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Juego 3D - WASD, Salto, Correr, Primera/Tercera Persona");
    
    initGL();
    
    // Configurar callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);
    glutKeyboardUpFunc(keyboardUpCallback);
    glutSpecialFunc(specialKeysCallback);
    glutSpecialUpFunc(specialKeysUpCallback);
    glutPassiveMotionFunc(mouseMotionCallback);
    glutTimerFunc(16, updateTimerCallback, 0);
}

// Ejecutar el juego
void Game::run() {
    printControls();
    glutMainLoop();
}

// Imprimir controles
void Game::printControls() {
    std::cout << "=== CONTROLES DEL JUEGO ===" << std::endl;
    std::cout << "WASD: Mover" << std::endl;
    std::cout << "Espacio: Saltar" << std::endl;
    std::cout << "OPCION 2 - Toggle (presionar una vez):" << std::endl;
    std::cout << "  R: Activar/Desactivar correr" << std::endl;
    std::cout << "  X: Activar/Desactivar caminar lento" << std::endl;
    std::cout << "=== CAMARA ===" << std::endl;
    std::cout << "C: Cambiar entre 1ra y 3ra persona" << std::endl;
    std::cout << "Mouse: Mirar alrededor" << std::endl;
    std::cout << "ESC: Salir" << std::endl;
    std::cout << "=== COLISIONES ===" << std::endl;
    std::cout << "El jugador ahora colisiona con todos los objetos" << std::endl;
    std::cout << "=========================" << std::endl;
}

// Funciones callback globales
void displayCallback() {
    if(Game::instance) Game::instance->display();
}

void keyboardCallback(unsigned char key, int x, int y) {
    if(Game::instance) Game::instance->keyboard(key, x, y);
}

void keyboardUpCallback(unsigned char key, int x, int y) {
    if(Game::instance) Game::instance->keyboardUp(key, x, y);
}

void specialKeysCallback(int key, int x, int y) {
    if(Game::instance) Game::instance->specialKeysFunc(key, x, y);
}

void specialKeysUpCallback(int key, int x, int y) {
    if(Game::instance) Game::instance->specialKeysUpFunc(key, x, y);
}

void mouseMotionCallback(int x, int y) {
    if(Game::instance) Game::instance->mouseMotion(x, y);
}

void updateTimerCallback(int value) {
    if(Game::instance) {
        Game::instance->updateTimer();
        glutTimerFunc(16, updateTimerCallback, 0);
    }
}

void reshapeCallback(int w, int h) {
    if(Game::instance) Game::instance->reshape(w, h);
}
