#ifndef MINECRAFTVIEWERAPP_H
#define MINECRAFTVIEWERAPP_H

#include <Ice/SDLApp.h>

#include <thread>
#include <mutex>
#include <atomic>

#include "Font/FontCache.h"

struct View {
    View(): worldPos(0), scale(1) {}

    glm::vec2 worldPos; // worldpos
    glm::vec2 scale; // zoom
};

class Widget;

class MinecraftViewer : public SDLApp {
public:
    MinecraftViewer(const char* title, int w, int h): SDLApp(title, w, h) {}
private:
    void onInit() override;
    void onExit() override;
    void onUpdate() override;
    void onRender() override;

    /* Variables */
    void updateCamera();
    void deleteTiles(const glm::ivec2& topLeftTile, const glm::ivec2& botRightTile);
    void createTiles(const glm::ivec2& topLeftTile, const glm::ivec2& botRightTile);
    void anotherThread();
    void drawAntialiasedCircle(float centerX, float centerY, float r, const Color& col);
    Widget* selectWidget();

    std::thread m_thread;
    std::atomic_bool m_threadRunning;
    std::mutex m_mutex;
    std::mutex m_mutex2;

    View view;
    View threadView;

    FontCache* m_fonts;
    Font* m_mainFont;

    glm::vec2 mouseWorldPos;

    int biomeUnderCursor;
};

#endif // MINECRAFTVIEWERAPP_H
