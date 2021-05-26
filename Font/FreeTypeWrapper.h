#ifndef FREETYPEWRAPPER_H
#define FREETYPEWRAPPER_H

#include <string>

#include <ft2build.h>
#include <freetype/ftlcdfil.h>
#include <freetype/ftglyph.h>

#include "FontGlyph.h"

class FreeTypeWrapper {
public:
    FreeTypeWrapper();
    virtual ~FreeTypeWrapper();

    // Load font from file
    bool loadFont(const char* fontPath, float dpi, int pixelSize, bool useAutohint);

    // Always call that after all work with current font is done
    void unloadFont();

    bool loadGlyph(FontGlyph& glyph, uint16_t keyCode);

    // Returs horizontal and veritcal advances based on space character
    glm::ivec2 getFontMetrics();

    // Return the size of space character in pixels
    int getSpaceWidth();

    // Return the amount of pixels from one baseline to another between lines
    int getLineGap();

    // Temporarily loads glyph to slot and renders pixels to buffer
    uint8_t* renderGlyph(FontGlyph& glyph, uint16_t keyCode);

private:
    FT_Int32 flags;

    FT_Library ft; // FreeType context
    FT_Face face;

    int pixelSize;
};

#endif // FREETYPEWRAPPER_H
