#ifndef CONFIG_H
#define CONFIG_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

struct Resolution {
    int width;
    int height;
    std::string name;
    
    Resolution(int w, int h, const std::string& n) : width(w), height(h), name(n) {}
};

class GameConfig {
private:
    int currentResolution;
    bool fullscreen;
    float masterVolume;
    float musicVolume;
    float sfxVolume;
    
    std::vector<Resolution> availableResolutions;
    std::string configFile;
    bool configLoaded; // Flag para saber si ya se cargó la configuración
    bool audioInitialized; // Flag para saber si el audio fue inicializado
    
    std::string floatToString(float value) const {
        std::ostringstream oss;
        oss << (int)(value * 100);
        return oss.str();
    }
    
    // Función mejorada para controlar el volumen del sistema
    void setSystemVolume(float volume) {
        // Método 1: Control del mixer del sistema (más confiable)
        HMIXER hMixer;
        if (mixerOpen(&hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR) {
            MIXERLINE mixerLine;
            mixerLine.cbStruct = sizeof(MIXERLINE);
            mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
            
            if (mixerGetLineInfo((HMIXEROBJ)hMixer, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE) == MMSYSERR_NOERROR) {
                MIXERLINECONTROLS mixerLineControls;
                MIXERCONTROL mixerControl;
                
                mixerLineControls.cbStruct = sizeof(MIXERLINECONTROLS);
                mixerLineControls.dwLineID = mixerLine.dwLineID;
                mixerLineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
                mixerLineControls.cControls = 1;
                mixerLineControls.pamxctrl = &mixerControl;
                mixerLineControls.cbmxctrl = sizeof(MIXERCONTROL);
                
                if (mixerGetLineControls((HMIXEROBJ)hMixer, &mixerLineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE) == MMSYSERR_NOERROR) {
                    MIXERCONTROLDETAILS mixerControlDetails;
                    MIXERCONTROLDETAILS_UNSIGNED volumeValue;
                    
                    volumeValue.dwValue = (DWORD)(volume * mixerControl.Bounds.dwMaximum);
                    
                    mixerControlDetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
                    mixerControlDetails.dwControlID = mixerControl.dwControlID;
                    mixerControlDetails.cChannels = 1;
                    mixerControlDetails.cMultipleItems = 0;
                    mixerControlDetails.paDetails = &volumeValue;
                    mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
                    
                    mixerSetControlDetails((HMIXEROBJ)hMixer, &mixerControlDetails, MIXER_SETCONTROLDETAILSF_VALUE);
                }
            }
            mixerClose(hMixer);
        }
        
        // Método 2: Wave out (mantener como respaldo)
        DWORD vol = (DWORD)(volume * 0xFFFF);
        DWORD stereoVol = (vol << 16) | vol;
        waveOutSetVolume(0, stereoVol);
        
        std::cout << "Volumen del sistema aplicado: " << (volume * 100) << "%" << std::endl;
    }
    
public:
    GameConfig() : currentResolution(0), fullscreen(false), 
                   masterVolume(0.7f), musicVolume(0.5f), sfxVolume(0.8f),
                   configFile("config.ini"), configLoaded(false), audioInitialized(false) {
        
        // Inicializar resoluciones disponibles
        availableResolutions.push_back(Resolution(800, 600, "800x600"));
        availableResolutions.push_back(Resolution(1024, 768, "1024x768"));
        availableResolutions.push_back(Resolution(1280, 720, "1280x720 (HD)"));
        availableResolutions.push_back(Resolution(1366, 768, "1366x768"));
        availableResolutions.push_back(Resolution(1920, 1080, "1920x1080 (Full HD)"));
        availableResolutions.push_back(Resolution(2560, 1440, "2560x1440 (2K)"));
        
        // Cargar configuración al inicializar
        loadConfig();
    }
    
    // Función para cargar configuración desde archivo
    void loadConfig() {
        std::ifstream file(configFile.c_str());
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.find("resolution=") == 0) {
                    int res = atoi(line.substr(11).c_str());
                    if (res >= 0 && res < (int)availableResolutions.size()) {
                        currentResolution = res;
                    }
                } else if (line.find("fullscreen=") == 0) {
                    fullscreen = (line.substr(11) == "1");
                } else if (line.find("masterVolume=") == 0) {
                    masterVolume = atof(line.substr(13).c_str());
                } else if (line.find("musicVolume=") == 0) {
                    musicVolume = atof(line.substr(12).c_str());
                } else if (line.find("sfxVolume=") == 0) {
                    sfxVolume = atof(line.substr(10).c_str());
                }
            }
            file.close();
            configLoaded = true;
            std::cout << "Configuracion cargada desde " << configFile << std::endl;
            std::cout << "Valores cargados - Master: " << masterVolume << ", Musica: " << musicVolume << ", SFX: " << sfxVolume << std::endl;
        } else {
            std::cout << "No se pudo cargar la configuracion, usando valores por defecto" << std::endl;
            configLoaded = false;
        }
    }
    
    // Función mejorada para aplicar configuración automáticamente al inicio
    void initializeSettings() {
        if (configLoaded) {
            std::cout << "Aplicando configuracion cargada..." << std::endl;
            
            // Aplicar configuración de video
            applyVideoSettings();
            
            // Aplicar configuración de audio (con más tiempo para inicializar)
            std::cout <<"Inicializando sistema de audio..." << std::endl;
            Sleep(100); // Pequeña pausa para asegurar que el sistema de audio esté listo
            
            applyAudioSettings();
            audioInitialized = true;
            
            std::cout << "Configuracion inicial aplicada completamente." << std::endl;
        }
    }
    
    // Función para guardar configuración en archivo
    void saveConfig() {
        std::ofstream file(configFile.c_str());
        if (file.is_open()) {
            file << "resolution=" << currentResolution << std::endl;
            file << "fullscreen=" << (fullscreen ? 1 : 0) << std::endl;
            file << "masterVolume=" << masterVolume << std::endl;
            file << "musicVolume=" << musicVolume << std::endl;
            file << "sfxVolume=" << sfxVolume << std::endl;
            file.close();
            std::cout << "Configuracion guardada en " << configFile << std::endl;
        } else {
            std::cout << "Error: No se pudo guardar la configuracion" << std::endl;
        }
    }

    // Getters
    Resolution getCurrentResolution() const {
        return availableResolutions[currentResolution];
    }
    
    bool isFullscreen() const { return fullscreen; }
    float getMasterVolume() const { return masterVolume; }
    float getMusicVolume() const { return musicVolume; }
    float getSfxVolume() const { return sfxVolume; }
    
    int getCurrentResolutionIndex() const { return currentResolution; }
    int getResolutionCount() const { return (int)availableResolutions.size(); }
    
    std::string getResolutionName(int index) const {
        if (index >= 0 && index < (int)availableResolutions.size()) {
            return availableResolutions[index].name;
        }
        return "Unknown";
    }
    
    // Setters con validación
    void setResolution(int index) {
        if (index >= 0 && index < (int)availableResolutions.size()) {
            currentResolution = index;
        }
    }
    
    void nextResolution() {
        currentResolution = (currentResolution + 1) % availableResolutions.size();
    }
    
    void previousResolution() {
        currentResolution--;
        if (currentResolution < 0) {
            currentResolution = availableResolutions.size() - 1;
        }
    }
    
    void setFullscreen(bool fs) {
        fullscreen = fs;
    }
    
    void toggleFullscreen() {
        fullscreen = !fullscreen;
    }
    
    void setMasterVolume(float volume) {
        if (volume >= 0.0f && volume <= 1.0f) {
            masterVolume = volume;
        }
    }
    
    void setMusicVolume(float volume) {
        if (volume >= 0.0f && volume <= 1.0f) {
            musicVolume = volume;
        }
    }
    
    void setSfxVolume(float volume) {
        if (volume >= 0.0f && volume <= 1.0f) {
            sfxVolume = volume;
        }
    }
    
    // Aplicar configuración
    void applyVideoSettings() {
        Resolution res = getCurrentResolution();
        
        if (fullscreen) {
            std::cout << "Aplicando pantalla completa: " << res.name << std::endl;
            glutFullScreen();
        } else {
            std::cout << "Aplicando ventana: " << res.name << std::endl;
            glutLeaveFullScreen();
            glutReshapeWindow(res.width, res.height);
            
            // Centrar ventana
            int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
            int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
            int x = (screenWidth - res.width) / 2;
            int y = (screenHeight - res.height) / 2;
            glutPositionWindow(x, y);
        }
    }
    
    // Función mejorada para aplicar configuración de audio
    void applyAudioSettings() {
        std::cout << "=== APLICANDO CONFIGURACION DE AUDIO ===" << std::endl;
        std::cout << "Master Volume: " << masterVolume << " (" << (masterVolume * 100) << "%)" << std::endl;
        std::cout << "Music Volume: " << musicVolume << " (" << (musicVolume * 100) << "%)" << std::endl;
        std::cout << "SFX Volume: " << sfxVolume << " (" << (sfxVolume * 100) << "%)" << std::endl;
        
        // Calcular volumen final
        float finalMasterVolume = masterVolume;
        float finalMusicVolume = masterVolume * musicVolume;
        
        std::cout << "Volumen final calculado - Master: " << (finalMasterVolume * 100) << "%, "
                  << "Musica efectiva: " << (finalMusicVolume * 100) << "%" << std::endl;
        
        // Aplicar al sistema usando múltiples métodos
        setSystemVolume(finalMasterVolume);
        
        // Si tienes un sistema de audio específico para tu juego, aplícalo aquí
        // Por ejemplo, si usas OpenAL, DirectSound, etc.
        applyGameAudioSettings(finalMasterVolume, finalMusicVolume, sfxVolume);
        
        std::cout << "=== CONFIGURACION DE AUDIO APLICADA ===" << std::endl;
    }
    
    // Función virtual para que puedas sobrescribir en tu sistema de audio específico
    virtual void applyGameAudioSettings(float masterVol, float musicVol, float sfxVol) {
        // Aquí deberías llamar a tu sistema de audio específico
        // Por ejemplo, si tienes una clase AudioManager:
        // AudioManager::getInstance()->setMasterVolume(masterVol);
        // AudioManager::getInstance()->setMusicVolume(musicVol);
        // AudioManager::getInstance()->setSFXVolume(sfxVol);
        
        std::cout << "INFO: applyGameAudioSettings() - Implementa aquí tu sistema de audio específico" << std::endl;
        std::cout << "      Master: " << masterVol << ", Music: " << musicVol << ", SFX: " << sfxVol << std::endl;
    }
    
    // Función para aplicar solo el volumen de música (útil para cambios en tiempo real)
    void applyMusicVolumeOnly() {
        float finalMusicVolume = masterVolume * musicVolume;
        std::cout << "Aplicando solo volumen de musica: " << (finalMusicVolume * 100) << "%" << std::endl;
        
        // Aquí deberías llamar específicamente a tu reproductor de música
        applyGameAudioSettings(masterVolume, finalMusicVolume, sfxVolume);
    }
    
    // Obtener información para mostrar en UI
    std::string getVolumePercentage(float volume) const {
        return floatToString(volume) + "%";
    }
    
    std::string getMasterVolumeText() const {
        return "Master: " + getVolumePercentage(masterVolume);
    }
    
    std::string getMusicVolumeText() const {
        return "Musica: " + getVolumePercentage(musicVolume);
    }
    
    std::string getSfxVolumeText() const {
        return "SFX: " + getVolumePercentage(sfxVolume);
    }
    
    std::string getFullscreenText() const {
        return fullscreen ? "Pantalla completa: SI" : "Pantalla completa: NO";
    }
    
    std::string getCurrentResolutionText() const {
        return "Resolucion: " + availableResolutions[currentResolution].name;
    }
    
    // Función de debug para verificar que los valores se están cargando correctamente
    void printCurrentSettings() const {
        std::cout << "=== CONFIGURACION ACTUAL ===" << std::endl;
        std::cout << "Resolucion: " << getCurrentResolutionText() << std::endl;
        std::cout << "Pantalla completa: " << (fullscreen ? "SI" : "NO") << std::endl;
        std::cout << "Master Volume: " << masterVolume << " (" << (masterVolume * 100) << "%)" << std::endl;
        std::cout << "Music Volume: " << musicVolume << " (" << (musicVolume * 100) << "%)" << std::endl;
        std::cout << "SFX Volume: " << sfxVolume << " (" << (sfxVolume * 100) << "%)" << std::endl;
        std::cout << "Audio inicializado: " << (audioInitialized ? "SI" : "NO") << std::endl;
        std::cout << "=============================" << std::endl;
    }
};

// Clase para el menú de opciones (sin cambios significativos)
class OptionsMenu {
private:
    struct OptionItem {
        std::string text;
        float x, y;
        int type;
        
        OptionItem(const std::string& t, float posX, float posY, int tp = 0) 
            : text(t), x(posX), y(posY), type(tp) {}
    };
    
    std::vector<OptionItem> optionItems;
    int selectedOption;
    int windowWidth, windowHeight;
    GameConfig* config;
    bool showingOptions;
    
    struct Color {
        float r, g, b, a;
        Color(float red, float green, float blue, float alpha = 1.0f) 
            : r(red), g(green), b(blue), a(alpha) {}
    };
    
    Color normalColor;
    Color selectedColor;
    Color valueColor;
    
public:
    OptionsMenu(GameConfig* cfg) : selectedOption(0), windowWidth(800), windowHeight(600),
                                   config(cfg), showingOptions(false),
                                   normalColor(0.8f, 0.8f, 0.8f, 0.9f),
                                   selectedColor(1.0f, 0.9f, 0.7f, 1.0f),
                                   valueColor(0.7f, 0.9f, 1.0f, 1.0f) {
        
        optionItems.push_back(OptionItem("Resolucion", 0.0f, 0.3f, 3));
        optionItems.push_back(OptionItem("Pantalla completa", 0.0f, 0.2f, 2));
        optionItems.push_back(OptionItem("Volumen Master", 0.0f, 0.1f, 1));
        optionItems.push_back(OptionItem("Volumen Musica", 0.0f, 0.0f, 1));
        optionItems.push_back(OptionItem("Volumen SFX", 0.0f, -0.1f, 1));
        optionItems.push_back(OptionItem("Aplicar cambios", 0.0f, -0.3f, 0));
        optionItems.push_back(OptionItem("Volver al menu", 0.0f, -0.4f, 0));
    }
    
    void setWindowSize(int width, int height) {
        windowWidth = width;
        windowHeight = height;
    }
    
    void show() {
        showingOptions = true;
        selectedOption = 0;
    }
    
    void hide() {
        showingOptions = false;
    }
    
    bool isShowing() const {
        return showingOptions;
    }
    
    void render() {
        if (!showingOptions) return;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glViewport(0, 0, windowWidth, windowHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glColor4f(0.1f, 0.1f, 0.15f, 0.9f);
        glBegin(GL_QUADS);
            glVertex2f(-1.0f, -1.0f);
            glVertex2f(1.0f, -1.0f);
            glVertex2f(1.0f, 1.0f);
            glVertex2f(-1.0f, 1.0f);
        glEnd();
        
        renderTitle();
        renderOptionItems();
        
        glutSwapBuffers();
    }
    
    void renderTitle() {
        glColor4f(0.9f, 0.8f, 0.6f, 1.0f);
        glRasterPos2f(-0.1f, 0.7f);
        
        std::string title = "OPCIONES";
        for (size_t i = 0; i < title.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title[i]);
        }
    }
    
    void renderOptionItems() {
        for (size_t i = 0; i < optionItems.size(); i++) {
            Color color = normalColor;
            
            if ((int)i == selectedOption) {
                color = selectedColor;
                renderSelectionGlow(optionItems[i].x, optionItems[i].y);
            }
            
            glColor4f(color.r, color.g, color.b, color.a);
            glRasterPos2f(optionItems[i].x - 0.4f, optionItems[i].y);
            
            for (size_t j = 0; j < optionItems[i].text.length(); j++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, optionItems[i].text[j]);
            }
            
            renderOptionValue(i);
        }
    }
    
    void renderOptionValue(int index) {
        glColor4f(valueColor.r, valueColor.g, valueColor.b, valueColor.a);
        glRasterPos2f(optionItems[index].x + 0.1f, optionItems[index].y);
        
        std::string value;
        
        switch (index) {
            case 0:
                value = config->getCurrentResolutionText();
                break;
            case 1:
                value = config->getFullscreenText();
                break;
            case 2:
                value = config->getMasterVolumeText();
                renderVolumeBar(optionItems[index].x + 0.3f, optionItems[index].y, config->getMasterVolume());
                break;
            case 3:
                value = config->getMusicVolumeText();
                renderVolumeBar(optionItems[index].x + 0.3f, optionItems[index].y, config->getMusicVolume());
                break;
            case 4:
                value = config->getSfxVolumeText();
                renderVolumeBar(optionItems[index].x + 0.3f, optionItems[index].y, config->getSfxVolume());
                break;
            default:
                return;
        }
        
        for (size_t i = 0; i < value.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, value[i]);
        }
    }
    
    void renderVolumeBar(float x, float y, float volume) {
        float barWidth = 0.2f;
        float barHeight = 0.02f;
        
        glColor4f(0.3f, 0.3f, 0.3f, 0.8f);
        glBegin(GL_QUADS);
            glVertex2f(x, y - barHeight/2);
            glVertex2f(x + barWidth, y - barHeight/2);
            glVertex2f(x + barWidth, y + barHeight/2);
            glVertex2f(x, y + barHeight/2);
        glEnd();
        
        float fillWidth = barWidth * volume;
        glColor4f(0.2f, 0.8f, 0.2f, 1.0f);
        glBegin(GL_QUADS);
            glVertex2f(x, y - barHeight/2);
            glVertex2f(x + fillWidth, y - barHeight/2);
            glVertex2f(x + fillWidth, y + barHeight/2);
            glVertex2f(x, y + barHeight/2);
        glEnd();
    }
    
    void renderSelectionGlow(float x, float y) {
        glColor4f(1.0f, 0.9f, 0.7f, 0.2f);
        glBegin(GL_QUADS);
            glVertex2f(x - 0.45f, y - 0.025f);
            glVertex2f(x + 0.55f, y - 0.025f);
            glVertex2f(x + 0.55f, y + 0.035f);
            glVertex2f(x - 0.45f, y + 0.035f);
        glEnd();
    }
    
    void handleKeyboard(unsigned char key) {
        if (!showingOptions) return;
        
        std::cout << "Debug - OptionsMenu::handleKeyboard, key: " << (int)key << std::endl;
        
        switch (key) {
            case 27: // ESC
                std::cout << "Debug - ESC en OptionsMenu::handleKeyboard - NO debería llegar aquí" << std::endl;
                break;
            case 13: // ENTER
                std::cout << "Debug - ENTER en OptionsMenu, opcion seleccionada: " << selectedOption << std::endl;
                selectCurrentOption();
                break;
            case 'w':
            case 'W':
                navigateUp();
                break;
            case 's':
            case 'S':
                navigateDown();
                break;
            case 'a':
            case 'A':
                adjustOptionLeft();
                break;
            case 'd':
            case 'D':
                adjustOptionRight();
                break;
        }
    }
    
    void handleSpecialKeys(int key) {
        if (!showingOptions) return;
        
        switch (key) {
            case GLUT_KEY_UP:
                navigateUp();
                break;
            case GLUT_KEY_DOWN:
                navigateDown();
                break;
            case GLUT_KEY_LEFT:
                adjustOptionLeft();
                break;
            case GLUT_KEY_RIGHT:
                adjustOptionRight();
                break;
        }
    }
    
    void navigateUp() {
        selectedOption--;
        if (selectedOption < 0) {
            selectedOption = optionItems.size() - 1;
        }
    }
    
    void navigateDown() {
        selectedOption++;
        if (selectedOption >= (int)optionItems.size()) {
            selectedOption = 0;
        }
    }
    
    void adjustOptionLeft() {
        switch (selectedOption) {
            case 0:
                config->previousResolution();
                break;
            case 1:
                config->toggleFullscreen();
                break;
            case 2:
                config->setMasterVolume(config->getMasterVolume() - 0.1f);
                config->applyAudioSettings();
                break;
            case 3:
                config->setMusicVolume(config->getMusicVolume() - 0.1f);
                config->applyMusicVolumeOnly(); // Usar la nueva función específica
                break;
            case 4:
                config->setSfxVolume(config->getSfxVolume() - 0.1f);
                break;
        }
    }
    
    void adjustOptionRight() {
        switch (selectedOption) {
            case 0:
                config->nextResolution();
                break;
            case 1:
                config->toggleFullscreen();
                break;
            case 2:
                config->setMasterVolume(config->getMasterVolume() + 0.1f);
                config->applyAudioSettings();
                break;
            case 3:
                config->setMusicVolume(config->getMusicVolume() + 0.1f);
                config->applyMusicVolumeOnly(); // Usar la nueva función específica
                break;
            case 4:
                config->setSfxVolume(config->getSfxVolume() + 0.1f);
                break;
        }
    }
    
    void selectCurrentOption() {
        std::cout << "Debug - selectCurrentOption llamada, opcion: " << selectedOption << std::endl;
        
        switch (selectedOption) {
            case 0:
            case 1: 
            case 2:
            case 3:
            case 4:
                std::cout << "Debug - Opcion de ajuste, no hace nada con ENTER" << std::endl;
                break;
            case 5:
                std::cout << "Debug - Aplicando cambios..." << std::endl;
                applyAllSettings();
                break;
            case 6:
                std::cout << "Debug - Volver al menu seleccionado - el cambio de estado se maneja en main.cpp" << std::endl;
                hide();
                break;
        }
    }
    
    void applyAllSettings() {
        config->applyVideoSettings();
        config->applyAudioSettings();
        config->saveConfig();
        
        // Imprimir configuración actual para debug
        config->printCurrentSettings();
        
        std::cout << "Configuracion aplicada y guardada." << std::endl;
    }
    
    int getSelectedOption() const {
        return selectedOption;
    }
};

#endif // CONFIG_H
