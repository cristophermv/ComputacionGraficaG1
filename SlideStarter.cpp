#include "SlideStarter.h"
#include <iostream>
#include <cstdio>
#include <cmath>

// Variable estática para el singleton
SlideStarter* SlideStarter::instance = NULL;

// Constructor
SlideStarter::SlideStarter() : currentSlide(0), totalSlides(3), fadeAlpha(1.0f), 
                 fadingOut(false), slideTimer(0), slideDisplayTime(180),
                 texturesLoaded(false), isActive(true) {
    loadTextures();
}

// Destructor
SlideStarter::~SlideStarter() {
    if (texturesLoaded) {
        glDeleteTextures(3, textures);
    }
}

// Función para cargar BMP
SlideStarter::BMPImage SlideStarter::loadBMP(const char* filename) {
    BMPImage img;
    img.data = NULL;
    img.width = 0;
    img.height = 0;
        
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return img;
    }
    
    // Leer header BMP
    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) {
        fclose(file);
        return img;
    }
    
    // Verificar que es un BMP
    if (header[0] != 'B' || header[1] != 'M') {
        fclose(file);
        return img;
    }
    
    // Obtener dimensiones
    img.width = *(int*)&header[18];
    img.height = *(int*)&header[22];
    int bpp = *(int*)&header[28];
    
    if (bpp != 24) {
        fclose(file);
        return img;
    }
    
    // Calcular tamaño de datos
    int imageSize = img.width * img.height * 3;
    img.data = new unsigned char[imageSize];
    
    // Leer datos de imagen
    fread(img.data, 1, imageSize, file);
    fclose(file);
    
    // Convertir BGR a RGB
    for (int i = 0; i < imageSize; i += 3) {
        unsigned char temp = img.data[i];
        img.data[i] = img.data[i + 2];
        img.data[i + 2] = temp;
    }
    
    return img;
}

void SlideStarter::loadTextures() {
    const char* logoFiles[3] = {
        "slide1.bmp",
        "slide2.bmp", 
        "slide3.bmp"
    };
        
    glGenTextures(3, textures);
    
    for (int i = 0; i < 3; i++) {
        BMPImage img = loadBMP(logoFiles[i]);
        
        if (img.data) {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
            
            delete[] img.data;
            texturesLoaded = true;
        }
    }
    
}

void SlideStarter::drawCenteredLogo(int slideIndex) {
    if (texturesLoaded) {
        // Dibujar logo centrado (tamaño reducido)
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[slideIndex]);
        glColor4f(1.0f, 1.0f, 1.0f, fadeAlpha);
        
        // Logo más pequeño y centrado
        float logoSize = 0.4f; // Ajusta este valor para cambiar el tamaño
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-logoSize, -logoSize);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(logoSize, -logoSize);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(logoSize, logoSize);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-logoSize, logoSize);
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
    } else {
        // Fallback: formas simples centradas
        glColor4f(1.0f, 1.0f, 1.0f, fadeAlpha);
        
        if (slideIndex == 0) {
            // Logo 1: Círculo
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(0.0f, 0.0f);
            for (int i = 0; i <= 360; i += 10) {
                float angle = i * 3.14159f / 180.0f;
                glVertex2f(cos(angle) * 0.15f, sin(angle) * 0.15f);
            }
            glEnd();
        } else if (slideIndex == 1) {
            // Logo 2: Cuadrado
            glBegin(GL_QUADS);
            glVertex2f(-0.15f, -0.15f);
            glVertex2f(0.15f, -0.15f);
            glVertex2f(0.15f, 0.15f);
            glVertex2f(-0.15f, 0.15f);
            glEnd();
        } else {
            // Logo 3: Triángulo
            glBegin(GL_TRIANGLES);
            glVertex2f(0.0f, 0.15f);
            glVertex2f(-0.15f, -0.15f);
            glVertex2f(0.15f, -0.15f);
            glEnd();
        }
        
        // Texto del logo
        glRasterPos2f(-0.08f, -0.3f);
        char text[20];
        for (int i = 0; text[i]; i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
        }
    }
}

// Función principal de renderizado
void SlideStarter::render() {
    if (!isActive) return;
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Fondo negro completo
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
    
    // Dibujar logo centrado
    drawCenteredLogo(currentSlide);
    
// Barra de progreso más angosta
glDisable(GL_TEXTURE_2D);
glColor4f(0.2f, 0.2f, 0.2f, 0.6f);

// Reducido el ancho (más cerca del centro)
float barLeft = -0.15f;
float barRight = 0.15f;
float barBottom = -0.88f;
float barTop = -0.875f;

// Fondo de la barra
glBegin(GL_QUADS);
glVertex2f(barLeft, barBottom);
glVertex2f(barRight, barBottom);
glVertex2f(barRight, barTop);
glVertex2f(barLeft, barTop);
glEnd();

// Cálculo de progreso
float progress = (float)(currentSlide * slideDisplayTime + slideTimer) / (float)(totalSlides * slideDisplayTime);

// Parte visible del progreso
glColor4f(0.8f, 0.8f, 0.8f, 0.8f);
glBegin(GL_QUADS);
glVertex2f(barLeft, barBottom);
glVertex2f(barLeft + progress * (barRight - barLeft), barBottom);
glVertex2f(barLeft + progress * (barRight - barLeft), barTop);
glVertex2f(barLeft, barTop);
glEnd();

glutSwapBuffers();

}

// Función de actualización
void SlideStarter::update() {
    if (!isActive) return;
    
    slideTimer++;
    
    // Fade out al final del slide
    if (slideTimer >= slideDisplayTime - 20) {
        fadeAlpha -= 0.05f;
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            fadingOut = true;
        }
    }
    
    // Cambiar slide
    if (fadingOut && slideTimer >= slideDisplayTime) {
        currentSlide++;
        if (currentSlide >= totalSlides) {
            // Terminar slides
            isActive = false;
            return;
        }
        slideTimer = 0;
        fadeAlpha = 1.0f;
        fadingOut = false;
    }
}

// Manejo de teclado
void SlideStarter::handleKeyboard(unsigned char key) {
    switch (key) {
        case ' ': // Saltar slide
            currentSlide++;
            if (currentSlide >= totalSlides) {
                isActive = false;
                return;
            }
            slideTimer = 0;
            fadeAlpha = 1.0f;
            fadingOut = false;
            break;
        case 's': // Saltar todos los slides
            isActive = false;
            break;
    }
}

// Verificar si los slides están activos
bool SlideStarter::isRunning() const {
    return isActive;
}

// Reiniciar slides
void SlideStarter::restart() {
    currentSlide = 0;
    slideTimer = 0;
    fadeAlpha = 1.0f;
    fadingOut = false;
    isActive = true;
}

// Métodos estáticos para manejo de instancia
SlideStarter* SlideStarter::getInstance() {
    return instance;
}

void SlideStarter::setInstance(SlideStarter* inst) {
    instance = inst;
}

void SlideStarter::initializeOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
