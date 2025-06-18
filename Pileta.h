#ifndef PILETA_H
#define PILETA_H

#include <cstdio>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Pileta {
private:
    float tiempoAgua;
    
    void dibujarCilindro(float radio, float altura, int segmentos) {
        float angulo = 2.0f * M_PI / segmentos;
        
        // Tapa superior
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, altura/2, 0.0f);
        for(int i = 0; i <= segmentos; i++) {
            float x = radio * cos(i * angulo);
            float z = radio * sin(i * angulo);
            glVertex3f(x, altura/2, z);
        }
        glEnd();
        
        // Tapa inferior
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, -altura/2, 0.0f);
        for(int i = segmentos; i >= 0; i--) {
            float x = radio * cos(i * angulo);
            float z = radio * sin(i * angulo);
            glVertex3f(x, -altura/2, z);
        }
        glEnd();
        
        // Lados
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i <= segmentos; i++) {
            float x = radio * cos(i * angulo);
            float z = radio * sin(i * angulo);
            glNormal3f(x/radio, 0.0f, z/radio);
            glVertex3f(x, altura/2, z);
            glVertex3f(x, -altura/2, z);
        }
        glEnd();
    }

public:
    Pileta() : tiempoAgua(0.0f) {}
    
    void actualizar(float deltaTime) {
        tiempoAgua += deltaTime;
    }
    
    void dibujar() {
        glPushMatrix();
        
        // Base exterior de la pileta (concreto)
        glColor3f(0.7f, 0.7f, 0.7f);
        dibujarCilindro(2.8f, 0.4f, 32);
        
        // Crear el "hoyo" de la pileta
        glTranslatef(0.0f, 0.2f, 0.0f);
        
        // Pared interior de la pileta (azulejos celestes)
        glColor3f(0.4f, 0.7f, 0.9f);
        float radioExterior = 2.2f;
        float radioInterior = 2.0f;
        float alturaParedes = 1.2f;
        
        // Dibujar paredes interiores
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i <= 32; i++) {
            float angulo = 2.0f * M_PI * i / 32;
            float x = radioInterior * cos(angulo);
            float z = radioInterior * sin(angulo);
            glNormal3f(-cos(angulo), 0.0f, -sin(angulo));
            glVertex3f(x, alturaParedes/2, z);
            glVertex3f(x, -alturaParedes/2, z);
        }
        glEnd();
        
        // Fondo de la pileta (azulejos azules más oscuros)
        glColor3f(0.2f, 0.4f, 0.7f);
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -alturaParedes/2, 0.0f);
        for(int i = 0; i <= 32; i++) {
            float angulo = 2.0f * M_PI * i / 32;
            float x = radioInterior * cos(angulo);
            float z = radioInterior * sin(angulo);
            glVertex3f(x, -alturaParedes/2, z);
        }
        glEnd();
        
        // Borde superior de la pileta
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i <= 32; i++) {
            float angulo = 2.0f * M_PI * i / 32;
            float x1 = radioInterior * cos(angulo);
            float z1 = radioInterior * sin(angulo);
            float x2 = radioExterior * cos(angulo);
            float z2 = radioExterior * sin(angulo);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x1, alturaParedes/2, z1);
            glVertex3f(x2, alturaParedes/2, z2);
        }
        glEnd();
        
        // Agua de la pileta con ondas animadas
        glTranslatef(0.0f, 0.4f, 0.0f);
        
        // Habilitar transparencia
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Dibujar agua con ondas en cuadrícula
        int resolucion = 30;
        float radioAgua = 1.9f;
        
        // Agua principal con ondas
        for(int i = 0; i < resolucion-1; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for(int j = 0; j <= resolucion; j++) {
                for(int k = 0; k < 2; k++) {
                    float angulo = 2.0f * M_PI * j / resolucion;
                    float radio = radioAgua * (i + k) / (resolucion-1);
                    
                    float x = radio * cos(angulo);
                    float z = radio * sin(angulo);
                    
                    // Calcular ondas múltiples
                    float onda1 = 0.04f * sin(tiempoAgua * 2.5f + radio * 6.0f);
                    float onda2 = 0.025f * sin(tiempoAgua * 1.8f + angulo * 3.0f);
                    float onda3 = 0.02f * sin(tiempoAgua * 3.2f + x * 4.0f + z * 4.0f);
                    float y = onda1 + onda2 + onda3;
                    
                    // Color del agua más realista
                    float profundidad = 1.0f - radio / radioAgua;
                    glColor4f(0.1f + 0.1f * profundidad, 
                             0.3f + 0.3f * profundidad, 
                             0.8f + 0.1f * profundidad, 
                             0.7f + 0.2f * profundidad);
                    
                    // Normal para iluminación
                    float dx = 0.3f * cos(tiempoAgua * 2.5f + radio * 6.0f) * cos(angulo);
                    float dz = 0.3f * cos(tiempoAgua * 2.5f + radio * 6.0f) * sin(angulo);
                    glNormal3f(-dx, 1.0f, -dz);
                    
                    glVertex3f(x, y, z);
                }
            }
            glEnd();
        }
        
        // Reflejos brillantes en el agua
        glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
        for(int i = 0; i < 6; i++) {
            float anguloReflejo = tiempoAgua * 1.5f + i * M_PI * 0.33f;
            float radioReflejo = 0.3f + 0.4f * sin(tiempoAgua * 1.2f + i);
            float x = radioReflejo * cos(anguloReflejo);
            float z = radioReflejo * sin(anguloReflejo);
            float y = 0.03f + 0.02f * sin(tiempoAgua * 3.0f + i);
            
            glPushMatrix();
            glTranslatef(x, y, z);
            
            // Reflejos circulares
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0f, 0.005f, 0.0f);
            for(int j = 0; j <= 10; j++) {
                float a = 2.0f * M_PI * j / 10;
                float tamano = 0.08f + 0.02f * sin(tiempoAgua * 4.0f + i);
                glVertex3f(tamano * cos(a), 0.005f, tamano * sin(a));
            }
            glEnd();
            glPopMatrix();
        }
        
        glDisable(GL_BLEND);
        
        // Borde decorativo de ladrillos
        glTranslatef(0.0f, -0.8f, 0.0f);
        glColor3f(0.8f, 0.4f, 0.3f);
        
        int numLadrillos = 20;
        float radioLadrillo = 2.6f;
        
        for(int i = 0; i < numLadrillos; i++) {
            glPushMatrix();
            float angulo = 2.0f * M_PI * i / numLadrillos;
            
            glRotatef(angulo * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
            glTranslatef(radioLadrillo, 0.0f, 0.0f);
            
            // Ladrillo más grande y visible
            glBegin(GL_QUADS);
            // Cara frontal
            glNormal3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.15f, -0.15f, -0.2f);
            glVertex3f(0.15f, 0.15f, -0.2f);
            glVertex3f(0.15f, 0.15f, 0.2f);
            glVertex3f(0.15f, -0.15f, 0.2f);
            
            // Cara superior
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-0.1f, 0.15f, -0.2f);
            glVertex3f(0.15f, 0.15f, -0.2f);
            glVertex3f(0.15f, 0.15f, 0.2f);
            glVertex3f(-0.1f, 0.15f, 0.2f);
            
            // Cara lateral
            glNormal3f(0.0f, 0.0f, 1.0f);
            glVertex3f(-0.1f, -0.15f, 0.2f);
            glVertex3f(0.15f, -0.15f, 0.2f);
            glVertex3f(0.15f, 0.15f, 0.2f);
            glVertex3f(-0.1f, 0.15f, 0.2f);
            glEnd();
            
            glPopMatrix();
        }
        
        glPopMatrix();
    }
    
    void dibujarPiso() {
        glColor3f(0.9f, 0.9f, 0.8f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-10.0f, -0.65f, -10.0f);
        glVertex3f(10.0f, -0.65f, -10.0f);
        glVertex3f(10.0f, -0.65f, 10.0f);
        glVertex3f(-10.0f, -0.65f, 10.0f);
        glEnd();
    }
    
    // Métodos para obtener información de la pileta
    float getRadio() const { return 2.0f; }
    float getAltura() const { return 0.6f; }
    float getPosicionY() const { return -0.05f; }
    
    // Verificar si un punto está dentro de la pileta
    bool estaDentro(float x, float z) const {
        float distancia = sqrt(x*x + z*z);
        return distancia <= getRadio();
    }
};

#endif // PILETA_H
