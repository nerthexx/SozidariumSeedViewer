#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <SDL/SDL.H>

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "FontGlyph.h"
#include "Font.h"
#include "Rect.h"

struct Color {
    uint8_t r, g, b, a;
};

const Color COLOR_WHITE = {255, 255, 255, 255};
//const Color COLOR_SHADOW = {255, 0, 0, 255};
const Color COLOR_SHADOW = {0, 0, 0, 160};

class FreeTypeWrapper;

const double DEFAULT_DPI = 96.0f;

inline float getDpi(SDL_Window* window) {
    float dpi = DEFAULT_DPI;

    if (int displayIndex = SDL_GetWindowDisplayIndex(window) < 0) {
        printf("SDL_GetWindowDisplayIndex() failed: %s\n", SDL_GetError());
    } else {
        float ddpi, xdpi, ydpi;
        if (SDL_GetDisplayDPI(displayIndex, &ddpi, &xdpi, &ydpi) < 0)
            printf("SDL_GetDisplayDPI() failed: %s\n", SDL_GetError());
        else
            dpi = xdpi;

        printf("Dpis: %f, %f, %f\n", ddpi, xdpi, ydpi);
    }

    return dpi;
}

class FontCache {
public:
    FontCache(float dpi);
    virtual ~FontCache();

    // Returns nullptr on fail
    Font* addFont(const char* fontPath, unsigned int fontSize, bool useAutohint = false, uint16_t errorGlyphKeyCode = 131);

    // Called after all fonts have been added. Re-renders the texture atlas, etc...
    bool compileFonts(SDL_Renderer* renderer, int padding);

    // Renders text and also returns pen position in pixels on screen
    glm::ivec2 renderText(SDL_Renderer* renderer, Font* font, const std::wstring& text, int x, int y, const Color& color = COLOR_WHITE, float scale = 1.0f);

    // This one supports '\n', also has max width to define the bounding box. If width = 0, it doesn't do anything.
    Rect renderTextLines(SDL_Renderer* renderer, Font* font, const std::wstring& text, int x, int y, const Color& color = COLOR_WHITE, float scale = 1.0f, int width = 0, bool drawShadow = false, const Color& shadowColor = COLOR_SHADOW);

    Rect getTextLinesRect(Font* font, const std::wstring& text, int x, int y, int width = 0);


    // TODO: delete this function. I moved it to font class
    glm::ivec2 getTextBB(Font* font, const std::wstring& text);

    void renderGlyph(SDL_Renderer* renderer, FontGlyph* glyph, int x, int y, float scale = 1.0f);

    void testRender(SDL_Renderer* renderer);

private:
    int calculateTextureSize();
    bool packGlyphs(int& textureSize, int padding);
    SDL_Texture* createGlyphAtlasTexture(SDL_Renderer* renderer, int textureSize);

    float m_dpi;

    FreeTypeWrapper* m_freetype;

    SDL_Texture* m_atlas;
    int m_atlasSize;

    std::vector<Font*> m_fonts;
};

#endif // FONTRENDERER_H
