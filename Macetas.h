#ifndef MACETAS_H
#define MACETAS_H

#include <GL/gl.h>
#include <math.h>

// Función para dibujar cilindro con normales
void drawSimpleCylinder(float radius, float height, int segments) {
    float angle = 2.0f * 3.14159f / segments;
    
    // Dibujar los lados del cilindro con normales
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= segments; i++) {
        float x = radius * cos(i * angle);
        float y = radius * sin(i * angle);
        float nx = cos(i * angle);
        float ny = sin(i * angle);
        
        glNormal3f(nx, ny, 0.0f);
        glVertex3f(x, y, 0.0f);      // Punto inferior
        glVertex3f(x, y, height);    // Punto superior
    }
    glEnd();
    
    // Base inferior
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for(int i = 0; i <= segments; i++) {
        float x = radius * cos(i * angle);
        float y = radius * sin(i * angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    
    // Base superior
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, height);
    for(int i = segments; i >= 0; i--) {
        float x = radius * cos(i * angle);
        float y = radius * sin(i * angle);
        glVertex3f(x, y, height);
    }
    glEnd();
}

// Función para dibujar maceta con normales
void drawPot(float baseRadius, float topRadius, float height, int segments) {
    float angle = 2.0f * 3.14159f / segments;
    
    // Lados de la maceta con normales calculadas
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= segments; i++) {
        float cosA = cos(i * angle);
        float sinA = sin(i * angle);
        
        float x1 = baseRadius * cosA;
        float y1 = baseRadius * sinA;
        float x2 = topRadius * cosA;
        float y2 = topRadius * sinA;
        
        // Calcular normal para superficie cónica
        float dx = x2 - x1;
        float dy = y2 - y1;
        float dz = height;
        float len = sqrt(dx*dx + dy*dy + dz*dz);
        float nx = cosA * dz / len;
        float ny = sinA * dz / len;
        float nz = -(dx*cosA + dy*sinA) / len;
        
        glNormal3f(nx, ny, nz);
        glVertex3f(x1, y1, 0.0f);      // Base
        glVertex3f(x2, y2, height);    // Tope
    }
    glEnd();
    
    // Base de la maceta
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for(int i = 0; i <= segments; i++) {
        float x = baseRadius * cos(i * angle);
        float y = baseRadius * sin(i * angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}

// Función para dibujar tierra
void drawSoil(float radius, float height) {
    // Material para tierra
    GLfloat soil_ambient[] = {0.3f, 0.15f, 0.05f, 1.0f};
    GLfloat soil_diffuse[] = {0.4f, 0.2f, 0.1f, 1.0f};
    GLfloat soil_specular[] = {0.1f, 0.05f, 0.02f, 1.0f};
    GLfloat soil_shininess = 10.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, soil_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, soil_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, soil_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, soil_shininess);
    
    glColor3f(0.4f, 0.2f, 0.1f);
    
    glPushMatrix();
    glTranslatef(0, 0, height * 0.85f);
    
    // Superficie de tierra
    float soilRadius = radius * 0.9f;
    int segments = 16;
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, height * 0.1f);
    
    for(int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        float x = soilRadius * cos(angle);
        float y = soilRadius * sin(angle);
        glVertex3f(x, y, height * 0.08f);
    }
    glEnd();
    
    glPopMatrix();
}

// Función para dibujar hoja
void drawLeaf() {
    // Material para hojas
    GLfloat leaf_ambient[] = {0.0f, 0.3f, 0.0f, 1.0f};
    GLfloat leaf_diffuse[] = {0.2f, 0.7f, 0.2f, 1.0f};
    GLfloat leaf_specular[] = {0.1f, 0.4f, 0.1f, 1.0f};
    GLfloat leaf_shininess = 30.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, leaf_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, leaf_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, leaf_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, leaf_shininess);
    
    glBegin(GL_TRIANGLES);
    // Cara frontal
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.3f, 0.1f, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.2f);
    
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.2f);
    glVertex3f(0.3f, -0.1f, 0.0f);
    
    // Cara trasera
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.2f);
    glVertex3f(0.3f, 0.1f, 0.0f);
    
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.3f, -0.1f, 0.0f);
    glVertex3f(0.2f, 0.0f, 0.2f);
    glEnd();
}

// Función para dibujar planta
void drawPlant(float height) {
    glColor3f(0.0f, 0.6f, 0.0f);
    
    glPushMatrix();
    glTranslatef(0, 0, height * 0.95f);
    
    // Tallo principal con material
    GLfloat stem_ambient[] = {0.0f, 0.2f, 0.0f, 1.0f};
    GLfloat stem_diffuse[] = {0.1f, 0.4f, 0.1f, 1.0f};
    GLfloat stem_specular[] = {0.05f, 0.2f, 0.05f, 1.0f};
    GLfloat stem_shininess = 20.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, stem_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, stem_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, stem_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, stem_shininess);
    
    glColor3f(0.0f, 0.4f, 0.0f);
    glPushMatrix();
    glScalef(0.02f, 0.02f, 0.25f);
    
    // Dibujar tallo como cilindro
    glBegin(GL_QUAD_STRIP);
    for(int i = 0; i <= 8; i++) {
        float angle = 2.0f * 3.14159f * i / 8;
        float x = cos(angle);
        float y = sin(angle);
        glNormal3f(x, y, 0.0f);
        glVertex3f(x, y, -1.0f);
        glVertex3f(x, y, 1.0f);
    }
    glEnd();
    glPopMatrix();
    
    // Hojas
    glColor3f(0.0f, 0.7f, 0.0f);
    for(int i = 0; i < 6; i++) {
        glPushMatrix();
        glRotatef(i * 60, 0, 0, 1);
        glTranslatef(0, 0, i * 0.05f);
        drawLeaf();
        glPopMatrix();
    }
    
    glPopMatrix();
}

// Función para dibujar una maceta completa
void drawSinglePot(float x, float y, float z, float baseR, float topR, 
                   float h, float r, float g, float b) {
    glPushMatrix();
    
    // Posición
    glTranslatef(x, y, z);
    
    // Material para maceta con brillo cerámico
    GLfloat pot_ambient[] = {r * 0.3f, g * 0.3f, b * 0.3f, 1.0f};
    GLfloat pot_diffuse[] = {r, g, b, 1.0f};
    GLfloat pot_specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat pot_shininess = 80.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, pot_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, pot_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pot_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, pot_shininess);
    
    // Maceta
    glColor3f(r, g, b);
    drawPot(baseR, topR, h, 24);
    
    // Tierra
    drawSoil(topR, h);
    
    // Planta
    drawPlant(h);

    glPopMatrix();
}

// Función principal que dibuja las 3 macetas
void DrawMacetas() {
    // Maceta roja (izquierda)
    drawSinglePot(-2.0f, 0.0f, 0.0f, 0.6f, 0.8f, 1.2f, 0.8f, 0.2f, 0.2f);
    
    // Maceta azul (centro)
    drawSinglePot(0.0f, 0.0f, 0.0f, 0.5f, 0.7f, 1.0f, 0.2f, 0.2f, 0.8f);
    
    // Maceta marrón (derecha)
    drawSinglePot(1.8f, 0.0f, 0.0f, 0.4f, 0.6f, 0.8f, 0.4f, 0.2f, 0.1f);
}

#endif // MACETAS_H
