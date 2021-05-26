#include "FontCache.h"

#include <SDL/SDL.h>

#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#include "FreeTypeWrapper.h"
#include "Conversions.h"

void loadCharacters(FreeTypeWrapper* ft, Font* font, uint16_t start, uint16_t end) {
    for (uint16_t c = start; c <= end; c++) {
        FontGlyph glyph;
        if (ft->loadGlyph(glyph, c))
            font->glyphs.insert({c, glyph});
    }
}

// -------------------------------------------------------------------------------

FontCache::FontCache(float dpi): m_dpi(dpi) {
    m_freetype = new FreeTypeWrapper();
    m_atlas = nullptr;
}

FontCache::~FontCache() {
    for (unsigned int i = 0; i < m_fonts.size(); i++)
        delete m_fonts[i];
    delete m_freetype;
    if (m_atlas) {
        SDL_DestroyTexture(m_atlas);
    }
}

Font* FontCache::addFont(const char* fontPath, unsigned int fontSize, bool useAutohint, uint16_t errorGlyphKeyCode) {
    if (!m_freetype->loadFont(fontPath, m_dpi, fontSize, useAutohint)) {
        printf("FontCache::addFont() failed: %s\n", fontPath);
        return nullptr;
    }

    Font* font = new Font;

    font->path = fontPath;
    font->fontSize = fontSize;
    font->usesAutohint = useAutohint;
    font->spaceWidth = m_freetype->getSpaceWidth();
    font->lineGap = m_freetype->getLineGap();
    font->errorGlyph = nullptr; // default state is null

    loadCharacters(m_freetype, font, 32, 126);
    loadCharacters(m_freetype, font, 148, 150);
    loadCharacters(m_freetype, font, L'А', L'я');
    loadCharacters(m_freetype, font, L'ё', L'ё');
    loadCharacters(m_freetype, font, L'Ё', L'Ё');

    loadCharacters(m_freetype, font, errorGlyphKeyCode, errorGlyphKeyCode);

    auto iter = font->glyphs.find(errorGlyphKeyCode);
    if (iter != font->glyphs.end()) // found
        font->errorGlyph = &iter->second;

    m_freetype->unloadFont();

    m_fonts.push_back(font);

    return font;
}

bool FontCache::compileFonts(SDL_Renderer* renderer, int padding) {
    int textureSize = calculateTextureSize();

    // Packing rects for glyph texture atlas
    if (!packGlyphs(textureSize, padding))
        return false;

    printf("Texture size is %i.\n", textureSize);



    if (m_atlas) {
        SDL_DestroyTexture(m_atlas);
    }

    m_atlas = createGlyphAtlasTexture(renderer, textureSize);

    return true;
}




// TODO: we really should just return Rect or something.
// x and y are penX and penY of text
// w and h are width and height of text
glm::ivec2 FontCache::renderText(SDL_Renderer* renderer, Font* font, const std::wstring& text, int x, int y, const Color& color, float scale) {
    if (!font) {
        wprintf(L"Unable to render text, font is nullptr: %s\n", text.c_str());
        return glm::ivec2(0);
    }

    SDL_SetTextureColorMod(m_atlas, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(m_atlas, color.a);

    int spaceAdvance = font->spaceWidth;

    float penX = x;

    for (wchar_t keyCode : text) {
        int advance = 0;
        if (keyCode == ' ') {
            advance = spaceAdvance;
        } else {
            FontGlyph* glyph = font->findGlyph(keyCode);
            if (glyph) {
                renderGlyph(renderer, glyph, penX, y, scale);
                advance = glyph->advance;
            }
        }

        penX += advance * scale;
    }

    return glm::ivec2(penX, font->lineGap);
}

Rect FontCache::renderTextLines(SDL_Renderer* renderer, Font* font, const std::wstring& text, int x, int y, const Color& color, float scale, int width, bool drawShadow, const Color& shadowColor) {
    if (!font) {
        wprintf(L"Unable to render text, font is nullptr: %s\n", text.c_str());
        return Rect();
    }

    if (drawShadow) {
        SDL_SetTextureColorMod(m_atlas, shadowColor.r, shadowColor.g, shadowColor.b);
        SDL_SetTextureAlphaMod(m_atlas, shadowColor.a);

        int spaceAdvance = font->spaceWidth;

        float penX = x;
        float penY = y;

        int xMax = x + width;
        if (width == 0)
            xMax = 10000;

        for (wchar_t keyCode : text) {
            int advance = 0;
            if (keyCode == ' ') {
                advance = spaceAdvance;
            } else if (keyCode == '\n') {
                penX = x;
                penY += font->lineGap;
            } else {
                FontGlyph* glyph = font->findGlyph(keyCode);
                if (glyph) {
                    // offset shadow by 1 px
                    renderGlyph(renderer, glyph, penX + 1, penY + 1, scale);
                    advance = glyph->advance;
                }
            }

            penX += advance * scale;

            if (penX > xMax) {
                penX = x;
                penY += font->lineGap;
            }
        }
    }

    SDL_SetTextureColorMod(m_atlas, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(m_atlas, color.a);

    int spaceAdvance = font->spaceWidth;

    float penX = x;
    float penY = y;

    int xMax = x + width;
    if (width == 0)
        xMax = 10000;

    int maxPenX = penX;

    for (wchar_t keyCode : text) {
        int advance = 0;
        if (keyCode == ' ') {
            advance = spaceAdvance;
        } else if (keyCode == '\n') {
            penX = x;
            penY += font->lineGap;
        } else {
            FontGlyph* glyph = font->findGlyph(keyCode);
            if (glyph) {
                renderGlyph(renderer, glyph, penX, penY, scale);
                advance = glyph->advance;
            }
        }

        penX += advance * scale;

        if (penX > xMax) {
            penX = x;
            penY += font->lineGap;
        }

        maxPenX = std::max(maxPenX, int(penX));
    }

    // Add a line at the end, specifying that we're done. This way, even if we have one line in the text, and penY never changed, we would still return valid height for this box.
    penY += font->lineGap;

    // Rect that starts at x and y. Why we return it? Well, user could use some wild math inside renderText(a + b + c + d + l + o + p), etc.. So, we give the user the ability to retrieve this wild stuff as rect.x. Then, we specify width and height. Since we don't know width and height, and only know end point, i.e Point(maxPenX, penY), we have to subtract x and y from it to get relative offset, which is our width and height.
    return Rect(x, y, maxPenX - x, penY - y);
}

// Same thing, but without rendering
Rect FontCache::getTextLinesRect(Font* font, const std::wstring& text, int x, int y, int width) {
    if (!font) {
        wprintf(L"Unable to render text, font is nullptr: %s\n", text.c_str());
        return Rect();
    }

    float scale = 1.0f;

    int spaceAdvance = font->spaceWidth;

    float penX = x;
    float penY = y;

    int xMax = x + width;
    if (width == 0)
        xMax = 10000;

    int maxPenX = penX;

    for (wchar_t keyCode : text) {
        int advance = 0;
        if (keyCode == ' ') {
            advance = spaceAdvance;
        } else if (keyCode == '\n') {
            penX = x;
            penY += font->lineGap;
        } else {
            FontGlyph* glyph = font->findGlyph(keyCode);
            if (glyph) {
                advance = glyph->advance;
            }
        }

        penX += advance * scale;

        if (penX > xMax) {
            penX = x;
            penY += font->lineGap;
        }

        maxPenX = std::max(maxPenX, int(penX));
    }

    penY += font->lineGap;

    return Rect(x, y, maxPenX - x, penY - y);
}



glm::ivec2 FontCache::getTextBB(Font* font, const std::wstring& text) {
    int spaceAdvance = font->spaceWidth;
    int penX = 0;

    for (wchar_t keyCode : text) {
        int advance = 0;
        if (keyCode == ' ') {
            advance = spaceAdvance;
        } else {
            FontGlyph* glyph = font->findGlyph(keyCode);
            if (glyph) {
                advance = glyph->advance;
            }
        }

        penX += advance;
    }

    return glm::ivec2(penX, font->lineGap);
}

void FontCache::renderGlyph(SDL_Renderer* renderer, FontGlyph* glyph, int x, int y, float scale) {
    if (glyph) {
        SDL_Rect clip = {glyph->x, glyph->y, glyph->w, glyph->h};
        SDL_Rect dest = {x + glyph->bearing.x * scale, y + glyph->bearing.y * scale,
                         glyph->w * scale, glyph->h * scale
                        };

        SDL_RenderCopy(renderer, m_atlas, &clip, &dest);
    }
}

void FontCache::testRender(SDL_Renderer* renderer) {
    // renders everythng.

    SDL_Rect clip = {0, 0, m_atlasSize, m_atlasSize};

    SDL_RenderCopy(renderer, m_atlas, &clip, &clip);
}

// --------------------------------------------------------------------------------

// Packs all glyphs in said area and gives out x and y coords
// Also, this function will change textureSize if needed
// https://stackoverflow.com/a/39645264
bool FontCache::packGlyphs(int& textureSize, int padding) {
    int glyphCount = 0;

    for (auto* font : m_fonts)
        glyphCount += font->glyphs.size();

    stbrp_rect rects[glyphCount];

    std::vector<std::pair<FontGlyph*, stbrp_rect*>> pairs;

    int i = 0;
    for (auto* font : m_fonts) {
        for (auto& iter : font->glyphs) {
            FontGlyph& glyph = iter.second;
            rects[i] = { 0, glyph.w + padding, glyph.h + padding, 0, 0, 0 };
            pairs.push_back({&glyph, &rects[i]});
            i++;
        }
    }

    stbrp_context context;

    // Try to put all rects in a texture until success or fail
    while (true) {
        int nodeCount = textureSize * 2;
        stbrp_node nodes[nodeCount];

        // Re-Init stuff from librarity
        stbrp_init_target(&context, textureSize, textureSize, nodes, nodeCount);

        if (stbrp_pack_rects(&context, rects, glyphCount)) // if successful, break
            break;

        printf("Texturesize of %i is no good, going up.\n", textureSize);

        textureSize *= 2;

        if (textureSize > 4096) {
            printf("The font texture atlas is too big, try smaller font size.\n\n");
            return false;
        }
    }

    for (auto& pair : pairs) {
        FontGlyph* glyph = pair.first;
        stbrp_rect* rect = pair.second;

        glyph->x = rect->x;
        glyph->y = rect->y;
    }

    return true;
}

int FontCache::calculateTextureSize() {
    int totalPixelAmount = 0;

    for (auto* font : m_fonts)
        for (auto& iter : font->glyphs)
            totalPixelAmount += iter.second.w * iter.second.h;

    int pseudoTextureSize = glm::sqrt(totalPixelAmount);
    int textureSize = nextPow2(pseudoTextureSize);

    return textureSize;
}

SDL_Texture* FontCache::createGlyphAtlasTexture(SDL_Renderer* renderer, int textureSize) {
    m_atlasSize = textureSize;

    SDL_Surface* out = SDL_CreateRGBSurface(0, textureSize, textureSize, 8, 0, 0, 0, 0);

    for (auto* font : m_fonts) {
        m_freetype->loadFont(font->path.c_str(), m_dpi, font->fontSize, font->usesAutohint);
        for (auto& iter : font->glyphs) {
            uint16_t keyCode = iter.first;
            FontGlyph& glyph = iter.second;

            uint8_t* pixels = m_freetype->renderGlyph(glyph, keyCode);
            if (!pixels)
                continue;

            int pitch = sizeof(uint8_t) * glyph.w;

            SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, glyph.w, glyph.h, 8, pitch, 0, 0, 0, 0);

            SDL_Rect destRect = {glyph.x, glyph.y, glyph.w, glyph.h};
            SDL_BlitSurface(surface, nullptr, out, &destRect);
            SDL_FreeSurface(surface);
        }
        m_freetype->unloadFont();
    }

    // Create a better texture since sdl is fucking trash
    SDL_LockSurface(out);

    struct RGBA { uint8_t r, g, b, a; };

    RGBA* newPixels = new RGBA[textureSize * textureSize];

    for (int y = 0; y < textureSize; y++) {
        for (int x = 0; x < textureSize; x++) {
            int index = y * textureSize + x;


            uint8_t alpha = ((uint8_t*)out->pixels)[index];


            RGBA& pixel = newPixels[index];
            pixel.r = 255;
            pixel.g = 255;
            pixel.b = 255;
            pixel.a = alpha;
        }
    }

    SDL_FreeSurface(out);

    // Actually create a texture now.
    // these should match our RGBA struct. I think these do....
    unsigned int rmask = 0x000000ff;
    unsigned int gmask = 0x0000ff00;
    unsigned int bmask = 0x00ff0000;
    unsigned int amask = 0xff000000;

    int pitch = textureSize * sizeof(RGBA); // how many bytes in a row.

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(newPixels, textureSize, textureSize, 32, pitch, rmask, gmask, bmask, amask);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);





    return texture;
}

