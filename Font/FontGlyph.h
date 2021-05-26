#ifndef FONTGLYPH_H
#define FONTGLYPH_H

#include <glm/glm.hpp>
#include <vector>

struct FontGlyph {
    unsigned short w, h;
    glm::ivec2 bearing;
    int advance;
    int x, y; // UVs
    //unsigned char keyCode;
};

typedef std::vector<FontGlyph> FontGlyphs;

#endif // FONTGLYPH_H
