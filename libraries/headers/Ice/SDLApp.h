#ifndef MYSDLAPP_H
#define MYSDLAPP_H

#include "SDL/SDL.h"

#include "SDLEventHandler.h"
#include "Debug.h"

inline SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, const char* filepath) {
    SDL_Surface* bmp = SDL_LoadBMP(filepath);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    return texture;
}

inline Uint32 createRGBA32(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return a << 24 | r << 16 | g << 8 | b; // why is it reversed? ARGB ordering for RGBA32?
}

class SDLApp {
public:
    SDLApp(const char* title, int w, int h):
        window(nullptr), renderer(nullptr), events(nullptr) {
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO) == 0) {
            window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_RESIZABLE);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

            events = new EventHandler();

            events->windowSize = glm::ivec2(w, h);

			//SDL_Cursor* cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
			//SDL_SetCursor(cursor);
        }

        width = w;
        height = h;
    }
    ~SDLApp() {
        if (renderer)
            SDL_DestroyRenderer(renderer);
        if (window)
            SDL_DestroyWindow(window);
        if (events)
            delete events;
        SDL_Quit();
    }

    int run() {
        if (window && renderer) {
            onInit();

            while (events->update() && running) {
                width = events->windowSize.x;
                height = events->windowSize.y;

                Debug::quickAndDirtyFpsCounter(window);

                onUpdate();

                SDL_SetRenderDrawColor(renderer, 32, 32, 32, SDL_ALPHA_OPAQUE);
                SDL_SetRenderTarget(renderer, 0);
                SDL_RenderClear(renderer);

                onRender();

                SDL_RenderPresent(renderer);
            }
            onExit();
            return 1;
        }
        return -1;
    }
protected:
	void stopRunning() { running = false; }

    virtual void onInit() = 0;
    virtual void onExit() = 0;
    virtual void onUpdate() = 0;
    virtual void onRender() = 0;

    SDL_Window* window;
    SDL_Renderer* renderer;
    EventHandler* events;

    int width;
    int height;

private:
	bool running;
};

#endif // MYSDLAPP_H
