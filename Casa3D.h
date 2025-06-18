#ifndef CASA_3D_H
#define CASA_3D_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <cmath>

class Casa3D {
private:
    void drawCube(float x, float y, float z, float width, float height, float depth, float r, float g, float b) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(r, g, b);
        
        glBegin(GL_QUADS);
        // Frente
        glVertex3f(0, 0, depth);
        glVertex3f(width, 0, depth);
        glVertex3f(width, height, depth);
        glVertex3f(0, height, depth);
        
        // Atrás
        glVertex3f(width, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, height, 0);
        glVertex3f(width, height, 0);
        
        // Izquierda
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, depth);
        glVertex3f(0, height, depth);
        glVertex3f(0, height, 0);
        
        // Derecha
        glVertex3f(width, 0, depth);
        glVertex3f(width, 0, 0);
        glVertex3f(width, height, 0);
        glVertex3f(width, height, depth);
        
        // Arriba
        glVertex3f(0, height, depth);
        glVertex3f(width, height, depth);
        glVertex3f(width, height, 0);
        glVertex3f(0, height, 0);
        
        // Abajo
        glVertex3f(0, 0, 0);
        glVertex3f(width, 0, 0);
        glVertex3f(width, 0, depth);
        glVertex3f(0, 0, depth);
        glEnd();
        
        glPopMatrix();
    }

    void drawRoom() {
        // Paredes celestes
        glColor3f(0.6f, 0.8f, 0.9f);
        
        // Pared trasera
        glBegin(GL_QUADS);
        glVertex3f(-6, 0, -6);
        glVertex3f(6, 0, -6);
        glVertex3f(6, 5.5f, -6);
        glVertex3f(-6, 5.5f, -6);
        glEnd();
        
        // Pared izquierda
        glBegin(GL_QUADS);
        glVertex3f(-6, 0, -6);
        glVertex3f(-6, 5.5f, -6);
        glVertex3f(-6, 5.5f, 6);
        glVertex3f(-6, 0, 6);
        glEnd();
        
        // Pared derecha
        glBegin(GL_QUADS);
        glVertex3f(6, 0, 6);
        glVertex3f(6, 5.5f, 6);
        glVertex3f(6, 5.5f, -6);
        glVertex3f(6, 0, -6);
        glEnd();
        
        // Pared frontal con ventana y puerta
        glColor3f(0.6f, 0.8f, 0.9f);
        
        // Parte izquierda de la pared frontal (antes de la ventana)
        glBegin(GL_QUADS);
        glVertex3f(-6, 0, 6);
        glVertex3f(-4.0f, 0, 6);
        glVertex3f(-4.0f, 5.5f, 6);
        glVertex3f(-6, 5.5f, 6);
        glEnd();
        
        // Parte entre ventana y puerta
        glBegin(GL_QUADS);
        glVertex3f(-2.5f, 0, 6);
        glVertex3f(-1.0f, 0, 6);
        glVertex3f(-1.0f, 5.5f, 6);
        glVertex3f(-2.5f, 5.5f, 6);
        glEnd();
        
        // Parte superior de la ventana
        glBegin(GL_QUADS);
        glVertex3f(-4.0f, 3.0f, 6);
        glVertex3f(-2.5f, 3.0f, 6);
        glVertex3f(-2.5f, 5.5f, 6);
        glVertex3f(-4.0f, 5.5f, 6);
        glEnd();
        
        // Parte inferior de la ventana
        glBegin(GL_QUADS);
        glVertex3f(-4.0f, 0, 6);
        glVertex3f(-2.5f, 0, 6);
        glVertex3f(-2.5f, 1.0f, 6);
        glVertex3f(-4.0f, 1.0f, 6);
        glEnd();
        
        // Parte superior de la pared frontal (encima de la puerta)
        glBegin(GL_QUADS);
        glVertex3f(-1.0f, 3.0f, 6);
        glVertex3f(1.0f, 3.0f, 6);
        glVertex3f(1.0f, 5.5f, 6);
        glVertex3f(-1.0f, 5.5f, 6);
        glEnd();
        
        // Parte derecha de la pared frontal
        glBegin(GL_QUADS);
        glVertex3f(1.0f, 0, 6);
        glVertex3f(6, 0, 6);
        glVertex3f(6, 5.5f, 6);
        glVertex3f(1.0f, 5.5f, 6);
        glEnd();
        
        // Ventana (cristal celeste claro)
        glColor3f(0.8f, 0.9f, 1.0f);
        glBegin(GL_QUADS);
        glVertex3f(-4.0f, 1.0f, 6.01f);
        glVertex3f(-2.5f, 1.0f, 6.01f);
        glVertex3f(-2.5f, 3.0f, 6.01f);
        glVertex3f(-4.0f, 3.0f, 6.01f);
        glEnd();
        
        // Marco de la ventana (blanco)
        glColor3f(0.9f, 0.9f, 0.9f);
        // Marco superior
        glBegin(GL_QUADS);
        glVertex3f(-4.1f, 2.9f, 6.02f);
        glVertex3f(-2.4f, 2.9f, 6.02f);
        glVertex3f(-2.4f, 3.1f, 6.02f);
        glVertex3f(-4.1f, 3.1f, 6.02f);
        glEnd();
        // Marco inferior
        glBegin(GL_QUADS);
        glVertex3f(-4.1f, 0.9f, 6.02f);
        glVertex3f(-2.4f, 0.9f, 6.02f);
        glVertex3f(-2.4f, 1.1f, 6.02f);
        glVertex3f(-4.1f, 1.1f, 6.02f);
        glEnd();
        // Marco izquierdo
        glBegin(GL_QUADS);
        glVertex3f(-4.1f, 1.0f, 6.02f);
        glVertex3f(-3.9f, 1.0f, 6.02f);
        glVertex3f(-3.9f, 3.0f, 6.02f);
        glVertex3f(-4.1f, 3.0f, 6.02f);
        glEnd();
        // Marco derecho
        glBegin(GL_QUADS);
        glVertex3f(-2.6f, 1.0f, 6.02f);
        glVertex3f(-2.4f, 1.0f, 6.02f);
        glVertex3f(-2.4f, 3.0f, 6.02f);
        glVertex3f(-2.6f, 3.0f, 6.02f);
        glEnd();
        // División central vertical
        glBegin(GL_QUADS);
        glVertex3f(-3.35f, 1.0f, 6.02f);
        glVertex3f(-3.15f, 1.0f, 6.02f);
        glVertex3f(-3.15f, 3.0f, 6.02f);
        glVertex3f(-3.35f, 3.0f, 6.02f);
        glEnd();
        // División central horizontal
        glBegin(GL_QUADS);
        glVertex3f(-4.0f, 1.9f, 6.02f);
        glVertex3f(-2.5f, 1.9f, 6.02f);
        glVertex3f(-2.5f, 2.1f, 6.02f);
        glVertex3f(-4.0f, 2.1f, 6.02f);
        glEnd();
        
        // Puerta (café/marrón)
        glColor3f(0.5f, 0.3f, 0.2f);
        glBegin(GL_QUADS);
        glVertex3f(-1.0f, 0, 6);
        glVertex3f(1.0f, 0, 6);
        glVertex3f(1.0f, 3.0f, 6);
        glVertex3f(-1.0f, 3.0f, 6);
        glEnd();
        
        // Número de la puerta "72"
        glColor3f(0.9f, 0.9f, 0.9f);
        glBegin(GL_QUADS);
        glVertex3f(-0.3f, 2.2f, 6.01f);
        glVertex3f(0.3f, 2.2f, 6.01f);
        glVertex3f(0.3f, 2.6f, 6.01f);
        glVertex3f(-0.3f, 2.6f, 6.01f);
        glEnd();
        
        // Manija de la puerta
        glColor3f(0.8f, 0.7f, 0.3f);
        glBegin(GL_QUADS);
        glVertex3f(0.6f, 1.4f, 6.01f);
        glVertex3f(0.8f, 1.4f, 6.01f);
        glVertex3f(0.8f, 1.6f, 6.01f);
        glVertex3f(0.6f, 1.6f, 6.01f);
        glEnd();
        
        // Suelo gris
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_QUADS);
        glVertex3f(-6, 0, -6);
        glVertex3f(6, 0, -6);
        glVertex3f(6, 0, 6);
        glVertex3f(-6, 0, 6);
        glEnd();
        
        // Techo blanco
        glColor3f(0.9f, 0.9f, 0.9f);
        glBegin(GL_QUADS);
        glVertex3f(-6, 5.5f, 6);
        glVertex3f(6, 5.5f, 6);
        glVertex3f(6, 5.5f, -6);
        glVertex3f(-6, 5.5f, -6);
        glEnd();
    }

    void drawSofa() {
        // Base del sofá (marrón)
        drawCube(-2.5f, 0.0f, -2.0f, 3.0f, 0.8f, 1.5f, 0.6f, 0.4f, 0.2f);
        
        // Respaldo del sofá
        drawCube(-2.5f, 0.8f, -2.0f, 3.0f, 1.2f, 0.3f, 0.6f, 0.4f, 0.2f);
        
        // Brazos del sofá
        drawCube(-2.5f, 0.8f, -2.0f, 0.3f, 0.8f, 1.5f, 0.6f, 0.4f, 0.2f);
        drawCube(2.2f, 0.8f, -2.0f, 0.3f, 0.8f, 1.5f, 0.6f, 0.4f, 0.2f);
    }

    void drawTable() {
        // Superficie de la mesa
        drawCube(-0.8f, 1.5f, 1.0f, 1.6f, 0.1f, 1.2f, 0.8f, 0.6f, 0.4f);
        
        // Patas de la mesa
        drawCube(-0.7f, 0.0f, 1.1f, 0.1f, 1.5f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(0.6f, 0.0f, 1.1f, 0.1f, 1.5f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(-0.7f, 0.0f, 2.1f, 0.1f, 1.5f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(0.6f, 0.0f, 2.1f, 0.1f, 1.5f, 0.1f, 0.8f, 0.6f, 0.4f);
    }

    void drawChair(float x, float z) {
        // Asiento
        drawCube(x, 0.8f, z, 0.8f, 0.1f, 0.8f, 0.8f, 0.6f, 0.4f);
        
        // Respaldo
        drawCube(x, 0.9f, z, 0.8f, 1.0f, 0.1f, 0.8f, 0.6f, 0.4f);
        
        // Patas
        drawCube(x, 0.0f, z, 0.1f, 0.8f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(x + 0.7f, 0.0f, z, 0.1f, 0.8f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(x, 0.0f, z + 0.7f, 0.1f, 0.8f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(x + 0.7f, 0.0f, z + 0.7f, 0.1f, 0.8f, 0.1f, 0.8f, 0.6f, 0.4f);
    }

    void drawSideboard() {
        // Mueble bajo (marrón oscuro)
        drawCube(2.5f, 0.0f, -4.5f, 2.5f, 1.5f, 1.0f, 0.5f, 0.3f, 0.2f);
        
        // Objetos decorativos en el mueble
        drawCube(2.7f, 1.5f, -4.3f, 0.3f, 0.5f, 0.3f, 0.9f, 0.1f, 0.1f); // Objeto rojo
        drawCube(3.2f, 1.5f, -4.3f, 0.2f, 0.3f, 0.2f, 0.8f, 0.8f, 0.2f); // Objeto amarillo
        drawCube(4.2f, 1.5f, -4.3f, 0.4f, 0.4f, 0.4f, 0.6f, 0.4f, 0.2f); // Objeto marrón
    }

    void drawBookshelf() {
        // Estantería de libros
        drawCube(-5.5f, 0.0f, -3.0f, 0.5f, 4.5f, 2.0f, 0.7f, 0.5f, 0.3f);
        
        // Estantes
        for(int i = 0; i < 6; i++) {
            drawCube(-5.5f, 0.7f * (i + 1), -3.0f, 0.5f, 0.05f, 2.0f, 0.8f, 0.6f, 0.4f);
        }
        
        // Libros simulados
        for(int shelf = 0; shelf < 5; shelf++) {
            for(int book = 0; book < 6; book++) {
                float bookY = 0.7f * (shelf + 1) + 0.05f;
                float bookZ = -2.8f + book * 0.3f;
                float r = 0.3f + (book % 3) * 0.2f;
                float g = 0.2f + (shelf % 2) * 0.3f;
                float b = 0.4f + ((book + shelf) % 2) * 0.3f;
                drawCube(-5.4f, bookY, bookZ, 0.3f, 0.6f, 0.25f, r, g, b);
            }
        }
    }

    void drawArmchair() {
        // Sillón individual
        drawCube(-4.5f, 0.0f, 2.0f, 1.2f, 0.8f, 1.2f, 0.4f, 0.6f, 0.3f);
        drawCube(-4.5f, 0.8f, 2.0f, 1.2f, 1.0f, 0.3f, 0.4f, 0.6f, 0.3f);
        drawCube(-4.5f, 0.8f, 2.0f, 0.3f, 0.6f, 1.2f, 0.4f, 0.6f, 0.3f);
        drawCube(-3.6f, 0.8f, 2.0f, 0.3f, 0.6f, 1.2f, 0.4f, 0.6f, 0.3f);
    }

    void drawWallDecorations() {
        // Cuadros en la pared
        drawCube(-2.0f, 3.0f, -5.9f, 0.8f, 0.6f, 0.1f, 0.9f, 0.9f, 0.8f);
        drawCube(1.0f, 3.0f, -5.9f, 0.6f, 0.8f, 0.1f, 0.9f, 0.9f, 0.8f);
        drawCube(3.5f, 3.5f, -5.9f, 0.5f, 0.4f, 0.1f, 0.2f, 0.3f, 0.8f);
        
        // Banderines decorativos
        glColor3f(0.8f, 0.2f, 0.3f);
        glBegin(GL_TRIANGLES);
        for(int i = 0; i < 5; i++) {
            float x = -3.0f + i * 1.2f;
            glVertex3f(x, 4.8f, -5.8f);
            glVertex3f(x + 0.4f, 4.8f, -5.8f);
            glVertex3f(x + 0.2f, 4.5f, -5.8f);
        }
        glEnd();
    }

    void drawRug() {
        // Alfombra debajo de la mesa
        glColor3f(0.8f, 0.7f, 0.6f);
        glBegin(GL_QUADS);
        glVertex3f(-2.0f, 0.01f, 0.5f);
        glVertex3f(2.0f, 0.01f, 0.5f);
        glVertex3f(2.0f, 0.01f, 3.0f);
        glVertex3f(-2.0f, 0.01f, 3.0f);
        glEnd();
        
        // Patrón de la alfombra
        glColor3f(0.6f, 0.5f, 0.4f);
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 2; j++) {
                float x1 = -1.5f + i * 1.0f;
                float z1 = 0.8f + j * 1.0f;
                glBegin(GL_QUADS);
                glVertex3f(x1, 0.02f, z1);
                glVertex3f(x1 + 0.6f, 0.02f, z1);
                glVertex3f(x1 + 0.6f, 0.02f, z1 + 0.6f);
                glVertex3f(x1, 0.02f, z1 + 0.6f);
                glEnd();
            }
        }
    }

public:
    // Método público para dibujar toda la casa
    void draw() {
        // Guardar el estado actual de la matriz
        glPushMatrix();
        
        // Configurar iluminación
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        // Configurar luz
        GLfloat lightPos[] = {2.0f, 4.5f, 2.0f, 1.0f};
        GLfloat lightColor[] = {1.0f, 1.0f, 0.9f, 1.0f};
        GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        
        // Dibujar todos los elementos de la habitación
        drawRoom();
        drawRug();
        drawSofa();
        drawTable();
        drawChair(-1.5f, 0.2f);
        drawChair(0.7f, 0.2f);
        drawChair(-1.5f, 2.8f);
        drawChair(0.7f, 2.8f);
        drawSideboard();
        drawBookshelf();
        drawArmchair();
        drawWallDecorations();
        
        // Restaurar el estado de la matriz
        glPopMatrix();
    }
    
    // Método para dibujar la casa en una posición específica
    void draw(float x, float y, float z) {
        glPushMatrix();
        glTranslatef(x, y, z);
        draw();
        glPopMatrix();
    }
    
    // Método para dibujar la casa con transformaciones
    void draw(float x, float y, float z, float scaleX, float scaleY, float scaleZ, float rotateY = 0.0f) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
        glScalef(scaleX, scaleY, scaleZ);
        draw();
        glPopMatrix();
    }
    
    // Método para inicializar configuraciones de OpenGL si es necesario
    void init() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        
        // Configurar material
        GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat mat_shininess[] = {10.0f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    }
};

#endif // CASA_3D_H
