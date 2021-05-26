#include "Font.h"

FontGlyph* Font::findGlyph(uint16_t keyCode) {
    auto iter = glyphs.find(keyCode);
    if (iter != glyphs.end()) // found
        return &iter->second;
    return errorGlyph; // Default character
}


glm::ivec2 Font::getSize(const std::wstring& text) {
    int spaceAdvance = spaceWidth;
    int penX = 0;

    for (wchar_t keyCode : text) {
        int advance = 0;
        if (keyCode == ' ') {
            advance = spaceAdvance;
        } else {
            FontGlyph* glyph = findGlyph(keyCode);
            if (glyph) {
                advance = glyph->advance;
            }
        }

        penX += advance;
    }

    return glm::ivec2(penX, lineGap);
}
