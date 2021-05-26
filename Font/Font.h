#ifndef FONT_H
#define FONT_H

#include "FontGlyph.h"

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

typedef std::unordered_map<uint16_t, FontGlyph> GlyphDictionary;

struct Font {
    std::string path;
    uint8_t fontSize;
    bool usesAutohint;

    int spaceWidth; // pixels between characters
    int lineGap; // pixels between lines

    GlyphDictionary glyphs;
    FontGlyph* errorGlyph;

    FontGlyph* findGlyph(uint16_t keyCode);

    glm::ivec2 getSize(const std::wstring& text);
};


#endif // FONT_H
