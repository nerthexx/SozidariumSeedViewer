#include "FreeTypeWrapper.h"

#include <freetype/ftoutln.h>

FreeTypeWrapper::FreeTypeWrapper() {
    if (FT_Init_FreeType(&ft))
        printf("ERROR::FREETYPE: Could not init FreeType Library.\n");
}

FreeTypeWrapper::~FreeTypeWrapper() {
    FT_Done_FreeType(ft);
}

bool FreeTypeWrapper::loadFont(const char* fontPath, float dpi, int pixelSize, bool useAutohint) {
    // Load font as face
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        printf("ERROR::FREETYPE: Failed to load font.\n");
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, pixelSize);

    flags = FT_LOAD_DEFAULT; // the same as = 0
    flags |= useAutohint ? FT_LOAD_FORCE_AUTOHINT : 0;

    this->pixelSize = pixelSize;

    return true;
}

bool FreeTypeWrapper::loadGlyph(FontGlyph& glyph, uint16_t c) {
    unsigned int gindex = FT_Get_Char_Index(face, c);
    if (FT_Load_Glyph(face, gindex, flags)) {
        printf("loadGlyph() failed to load glyph: %i.\n", c);
        return false;
    }

    // So, this x >> 6 this is just x / 64 - conversion between points and pixels
    // Freetype works with points (of some sort), and we extract pixels from them.
    glyph.w = face->glyph->metrics.width >> 6;
    glyph.h = face->glyph->metrics.height >> 6;

    // For why we use hori bearing see screenshot 2019.07.20
    glyph.bearing.x = face->glyph->metrics.horiBearingX >> 6;
    glyph.bearing.y = pixelSize - (face->glyph->metrics.horiBearingY >> 6);

    glyph.advance = face->glyph->advance.x >> 6;

    glyph.x = 0;
    glyph.y = 0;

    //glyph.keyCode = c;

    return true;
}

void FreeTypeWrapper::unloadFont() {
    FT_Done_Face(face);
}

int FreeTypeWrapper::getSpaceWidth() {
    if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ' '), flags)) { // failed to load
        printf("getSpaceWidth() failed to load glyph.\n");
        return pixelSize; // assume half of font size as our space width
    }

    return face->glyph->metrics.horiAdvance >> 6;
}

int FreeTypeWrapper::getLineGap() {
    return face->size->metrics.height >> 6;
}

uint8_t* FreeTypeWrapper::renderGlyph(FontGlyph& glyph, uint16_t keyCode) {
    unsigned int gindex = FT_Get_Char_Index(face, keyCode);
    if (FT_Load_Glyph(face, gindex, flags)) {
        printf("renderGlyph() failed to load glyph: %i.\n", keyCode);
        return nullptr;
    }

    if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) {
        printf("renderGlyph() failed to render glyph: %i.\n", keyCode);
        return nullptr;
    }

    return face->glyph->bitmap.buffer;
}
