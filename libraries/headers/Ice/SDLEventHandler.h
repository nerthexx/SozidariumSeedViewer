#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <SDL/SDL.h>
#include "MathCommon.h"

class EventHandler {
public:
    EventHandler(): mouseCoords( { 0, 0 }), m_droppedFile(nullptr), m_mouseMoved(false) {}
    int update() {
        mouseDiff = { 0, 0 };
        mouseScrollDiff = { 0, 0 };
        m_mouseMoved = false;

        // Loop through keyMap and copy it over to _prevoiusKeyMap
        for(auto& it : keyMap)
            previousKeyMap[it.first] = it.second;

        if (m_droppedFile) {
            SDL_free(m_droppedFile);
            m_droppedFile = nullptr;
        }

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    return 0;
                    break;
                case SDL_KEYDOWN:
                    pressKey(e.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    releaseKey(e.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    pressKey(e.button.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    releaseKey(e.button.button);
                    break;
                case SDL_MOUSEMOTION:
                    setMouseCoords(e.motion.x, e.motion.y);
                    setMouseDiff(e.motion.xrel, e.motion.yrel);
                    m_mouseMoved = true;
                    break;
                case SDL_MOUSEWHEEL:
                    setMouseScrollDiff(e.wheel.x, e.wheel.y);
                    break;
                case SDL_DROPFILE:
                    m_droppedFile = e.drop.file;
                    break;
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                        windowSize.x = e.window.data1;
                        windowSize.y = e.window.data2;
                    }
                    break;
            }
        }
        return 1;
    }

    bool mouseMoved() { return m_mouseMoved; }

    bool isKeyDown(unsigned int keyID) {
        auto it = keyMap.find(keyID);
        if(it != keyMap.end())
            return it->second;
        return false;
    }

    bool isKeyPressed(unsigned int keyID) {
        // Check if it is pressed this frame and wasn't pressed last frame
        if(isKeyDown(keyID) == true && wasKeyDown(keyID) == false)
            return true;
        return false;
    }

    bool isKeyReleased(unsigned int keyID) {
        if(isKeyDown(keyID) == false && wasKeyDown(keyID) == true)
            return true;
        return false;
    }

    glm::ivec2 getWindowSize() { return windowSize; }
    glm::ivec2 getMouseCoords() { return mouseCoords; }
    glm::ivec2 getMouseDiff() { return mouseDiff; }
    glm::ivec2 getMouseScrollDiff() { return mouseScrollDiff; }
    char* getDroppedFile() { return m_droppedFile; }

    glm::ivec2 windowSize;

private:
    void pressKey(unsigned int keyID) { keyMap[keyID] = true; }
    void releaseKey(unsigned int keyID) { keyMap[keyID] = false; }
    void setMouseCoords(int x, int y) { mouseCoords = { x, y }; }
    void setMouseDiff(int x, int y) { mouseDiff = { x, y }; }
    void setMouseScrollDiff(int x, int y) { mouseScrollDiff = { x, y }; }

    // Returns true if the key was held down last frame
    bool wasKeyDown(unsigned int keyID) {
        auto it = previousKeyMap.find(keyID);
        if(it != previousKeyMap.end())
            return it->second;
        return false;
    }

    std::unordered_map<unsigned int, bool> keyMap;
    std::unordered_map<unsigned int, bool> previousKeyMap;

    glm::ivec2 mouseCoords;
    glm::ivec2 mouseDiff;
    glm::ivec2 mouseScrollDiff;

    char* m_droppedFile;

    bool m_mouseMoved;
};

#endif // INPUTHANDLER_H
