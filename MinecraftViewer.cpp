#include "MinecraftViewer.h"

#include "Cubiomes/finders.h"
#include "Cubiomes/util.h"

#include <time.h>

#include <chrono>

#include <cstdint>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <sstream>

#include <glm/gtx/hash.hpp>


//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::wstring getBiomeName(int biomeId) {
    if (biomeId == -1)
        return L"";

    switch (biomeId) {

        case ocean:
            return L"Океан (minecraft:ocean)";

        case plains:
            return L"Равнины (minecraft:plains)";

        case desert:
            return L"Пустыня (minecraft:desert)";

        case mountains:
            return L"Горы (minecraft:mountains)";

        case forest:
            return L"Лес (minecraft:forest)";

        case taiga:
            return L"Тайга (minecraft:taiga)";

        case swamp:
            return L"Болото (minecraft:swamp)";

        case river:
            return L"Река (minecraft:river)";

        case nether_wastes:
            return L"Пустоши Нижнего мира (minecraft:nether_wastes)";

        case the_end:
            return L"Край (minecraft:the_end)";

        case frozen_ocean:
            return L"Замёрзший океан (minecraft:frozen_ocean)";

        case frozen_river:
            return L"Замёрзшая река (minecraft:frozen_river)";

        case snowy_tundra:
            return L"Заснеженная тундра (minecraft:snowy_tundra)";

        case snowy_mountains:
            return L"Заснеженные горы (minecraft:snowy_mountains)";

        case mushroom_fields:
            return L"Грибные поля (minecraft:mushroom_fields)";

        case mushroom_field_shore:
            return L"Грибной берег (minecraft:mushroom_field_shore)";

        case beach:
            return L"Пляж (minecraft:beach)";

        case desert_hills:
            return L"Холмистая пустыня (minecraft:desert_hills)";

        case wooded_hills:
            return L"Холмистый лес (minecraft:wooded_hills)";

        case taiga_hills:
            return L"Холмистая тайга (minecraft:taiga_hills)";

        case mountain_edge:
            return L"Горный перевал (minecraft:mountain_edge)";

        case jungle:
            return L"Джунгли (minecraft:jungle)";

        case jungle_hills:
            return L"Холмистые джунгли (minecraft:jungle_hills)";

        case jungle_edge:
            return L"Окраина джунглей (minecraft:jungle_edge)";

        case deep_ocean:
            return L"Глубокий океан (minecraft:deep_ocean)";

        case stone_shore:
            return L"Каменистый берег (minecraft:stone_shore)";

        case snowy_beach:
            return L"Заснеженный пляж (minecraft:snowy_beach)";

        case birch_forest:
            return L"Березняк (minecraft:birch_forest)";

        case birch_forest_hills:
            return L"Холмистый березняк (minecraft:birch_forest_hills)";

        case dark_forest:
            return L"Тёмный лес (minecraft:dark_forest)";

        case snowy_taiga:
            return L"Заснеженная тайга (minecraft:snowy_taiga)";

        case snowy_taiga_hills:
            return L"Заснеженная гористая тайга (minecraft:snowy_taiga_hills)";

        case giant_tree_taiga:
            return L"Подзолистая тайга (minecraft:giant_tree_taiga)";

        case giant_tree_taiga_hills:
            return L"Подзолистая холмистая тайга (minecraft:giant_tree_taiga_hills)";

        case wooded_mountains:
            return L"Лесистые горы (minecraft:wooded_mountains)";

        case savanna:
            return L"Саванна (minecraft:savanna)";

        case savanna_plateau:
            return L"Плато саванны (minecraft:savanna_plateau)";

        case badlands:
            return L"Пустошь (minecraft:badlands)";

        case wooded_badlands_plateau:
            return L"Лесистое плато пустоши (minecraft:wooded_badlands_plateau)";

        case badlands_plateau:
            return L"Плато пустоши (minecraft:badlands_plateau)";

        case small_end_islands:
            return L"Островки Края (minecraft:small_end_islands)";

        case end_midlands:
            return L"Равнины Края (minecraft:end_midlands)";

        case end_highlands:
            return L"Возвышенности Края (minecraft:end_highlands)";

        case end_barrens:
            return L"Окраины Края (minecraft:end_barrens)";

        case warm_ocean:
            return L"Тёплый океан (minecraft:warm_ocean)";

        case lukewarm_ocean:
            return L"Умеренный океан (minecraft:lukewarm_ocean)";

        case cold_ocean:
            return L"Холодный океан (minecraft:cold_ocean)";

        case deep_warm_ocean:
            return L"Глубокий тёплый океан (minecraft:deep_warm_ocean)";

        case deep_lukewarm_ocean:
            return L"Глубокий умеренный океан (minecraft:deep_lukewarm_ocean)";

        case deep_cold_ocean:
            return L"Глубокий холодный океан (minecraft:deep_cold_ocean)";

        case deep_frozen_ocean:
            return L"Глубокий замёрзший океан (minecraft:deep_frozen_ocean)";

        case the_void:
            return L"Пустота (minecraft:the_void)";

        // mutated variants
        case sunflower_plains:
            return L"Подсолнечниковые равнины (minecraft:sunflower_plains)";

        case desert_lakes:
            return L"Пустынные озёра (minecraft:desert_lakes)";

        case gravelly_mountains:
            return L"Гравийные горы (minecraft:gravelly_mountains)";

        case flower_forest:
            return L"Цветочный лес (minecraft:flower_forest)";

        case taiga_mountains:
            return L"Гористая тайга (minecraft:taiga_mountains)";

        case swamp_hills:
            return L"Холмистое болото (minecraft:swamp_hills)";

        case ice_spikes:
            return L"Ледяные пики (minecraft:ice_spikes)";

        case modified_jungle:
            return L"Рельефные джунгли (minecraft:modified_jungle)";

        case modified_jungle_edge:
            return L"Рельефная окраина джунглей (minecraft:modified_jungle_edge)";

        case tall_birch_forest:
            return L"Крупномерный березняк (minecraft:tall_birch_forest)";

        case tall_birch_hills:
            return L"Крупномерный холмистый березняк (minecraft:tall_birch_hills)";

        case dark_forest_hills:
            return L"Холмистый тёмный лес (minecraft:dark_forest_hills)";

        case snowy_taiga_mountains:
            return L"Заснеженная гористая тайга (minecraft:snowy_taiga_mountains)";

        case giant_spruce_taiga:
            return L"Крупномерная подзолистая тайга (minecraft:giant_spruce_taiga)";

        case giant_spruce_taiga_hills:
            return L"Крупномерная подзолистая холмистая тайга (minecraft:giant_spruce_taiga_hills)";

        case modified_gravelly_mountains:
            return L"Высокие гравийные горы (minecraft:modified_gravelly_mountains)";

        case shattered_savanna:
            return L"Выветренная саванна (minecraft:shattered_savanna)";

        case shattered_savanna_plateau:
            return L"Плато выветренной саванны (minecraft:shattered_savanna_plateau)";

        case eroded_badlands:
            return L"Выветренная пустошь (minecraft:eroded_badlands)";

        case modified_wooded_badlands_plateau:
            return L"Рельефное лесистое плато пустоши (minecraft:modified_wooded_badlands_plateau)";

        case modified_badlands_plateau:
            return L"Рельефное плато пустоши (minecraft:modified_badlands_plateau)";

        case bamboo_jungle:
            return L"Бамбуковые джунгли (minecraft:bamboo_jungle)";

        case bamboo_jungle_hills:
            return L"Холмистые бамбуковые джунгли (minecraft:bamboo_jungle_hills)";

        case soul_sand_valley:
            return L"Долина песка душ (minecraft:soul_sand_valley)";

        case crimson_forest:
            return L"Багровый лес (minecraft:crimson_forest)";

        case warped_forest:
            return L"Искажённый лес (minecraft:warped_forest)";

        case basalt_deltas:
            return L"Базальтовые дельты (minecraft:basalt_deltas)";

    }

    return L"Undefined";
}



struct Texture {
    Texture() {
        underlyingTexture = nullptr;
        center = glm::ivec2(0);
    }

    virtual ~Texture() {
        if (underlyingTexture) {
            SDL_DestroyTexture(underlyingTexture);
        }
    }

    SDL_Texture* underlyingTexture;

    int width;
    int height;

    glm::ivec2 center; // anchor

    void setAlpha(uint8_t alpha) {
        SDL_SetTextureAlphaMod(underlyingTexture, alpha);
    }

    void render(SDL_Renderer* renderer, const glm::ivec2& position) {
        if (!underlyingTexture)
            return;

        SDL_Rect src = {0, 0, width, height};
        SDL_Rect dst = {position.x - center.x, position.y - center.y, width, height};

        SDL_RenderCopy(renderer, underlyingTexture, &src, &dst);
    }
};

SDL_Texture* createTextureFromPixels(SDL_Renderer* renderer, unsigned char* pixels, int width, int height, int components) {
    // This follows RGBA struct pattern (meaning that uint8_t r, g, b, a in code are stored as little endian, which is what these mask values follow. Big endian would have the order flipped.
    unsigned int rmask = 0x000000ff;
    unsigned int gmask = 0x0000ff00;
    unsigned int bmask = 0x00ff0000;
    unsigned int amask = 0xff000000;

    int pitch = width * components; // how many bytes in a row.

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, components * 8, pitch, rmask, gmask, bmask, amask);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    return texture;
}

struct STB_Image {
    STB_Image(const std::string& filename) {
        data = stbi_load(filename.c_str(), &width, &height, &components, 0);
        if (!data) {
            printf("Failed to load image: %s\n", filename.c_str());
        }
    }

    STB_Image() {
        data = nullptr;
    }

    virtual ~STB_Image() {
        if (data) {
            stbi_image_free(data);
        }
    }

    Color getColorAt(int x, int y) {
        int i = (y * width + x) * components;

        if (components == 1) {
            return {data[i], data[i], data[i], data[i]};
        } else if (components == 3) {
            return {data[i], data[i + 1], data[i + 2], 255};
        } else if (components == 4) {
            return {data[i], data[i + 1], data[i + 2], data[i + 3]};
        }

        return {255, 255, 255, 255};
    }

    int width, height, components;
    uint8_t* data;
};

SDL_Texture* loadImageFromDisk(SDL_Renderer* renderer, const std::string& filename) {
    int width, height, components;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &components, 0);
    if (data) {
        SDL_Texture* texture = createTextureFromPixels(renderer, data, width, height, components);

        stbi_image_free(data);
        printf("Loaded image: %s\n", filename.c_str());

        return texture;
    }

    return nullptr;
}


Texture* loadTextureFromDisk(SDL_Renderer* renderer, const std::string& filename) {
    Texture* t = new Texture();


    int width, height, components;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &components, 0);
    if (data) {
        SDL_Texture* texture = createTextureFromPixels(renderer, data, width, height, components);

        stbi_image_free(data);
        printf("Loaded image: %s\n", filename.c_str());

        t->width = width;
        t->height = height;

        t->underlyingTexture = texture;
    }



    return t;
}


SDL_Texture* mapMarkerImage;
Texture* mapMarkerTexture;

STB_Image* forestPixels;
STB_Image* sprucePixels;
STB_Image* acaciaPixels;
STB_Image* mountainPixels;


int lod = 0;

int getLodMultiplicator(int lod) {
    switch (lod) {
        case 0:
            return 1;
        case 1:
            return 4;
        case 2:
            return 16;
        case 3:
            return 64;
    }

    return 1;
}

// https://www.youtube.com/watch?v=ZQ8qtAizis4
glm::vec2 worldToScreen(const glm::vec2& worldPos, const View& view) {
    return (worldPos - view.worldPos) * view.scale;
}

glm::vec2 screenToWorld(const glm::vec2& screenPos, const View& view) {
    return screenPos / view.scale + view.worldPos;
}


int TILE_SIZE = 512; // size of tile in pixels

namespace {
Layer* layer;
LayerStack g;
int* biomeIds;

unsigned char biomeColours[256][3];
}

// X and Y are tile center.
unsigned char* getTilePixels(int x, int y) {
    int w = TILE_SIZE;
    int h = TILE_SIZE;

    // don't use this. This breaks with multilod system. We either need to accout for that, or just don't do anything at all.
    //x -= w / 2;
    //y -= h / 2;

    unsigned char* pixels = new unsigned char[w * h * 3];

    // TODO: very good optimization would be to generate area of 1x1 pixels relative to screenspace density. I.e, we would generate lods based on 1x1 textures.
    genArea(layer, biomeIds, x, y, w, h);

    biomesToImage(pixels, biomeColours, biomeIds, w, h, 1, 2);

    // don't. Looks like shit. Need to have different approach. Maybe spawn trees/mountains separately and lay them as icons on the grid, grid is our tile texture, with some probability....
    /*
    // apply custom shit to pixels based on biome
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int i = y * w + x;
            unsigned char& r = pixels[i * 3];
            unsigned char& g = pixels[i * 3 + 1];
            unsigned char& b = pixels[i * 3 + 2];

            int biomeId = biomeIds[i];

            if (biomeId == mountains) {

                Color c = mountainPixels->getColorAt(x % 256, y % 256);
                r = r * c.r / 255.0f;
                g = g * c.g / 255.0f;
                b = b * c.b / 255.0f;
            } else if (biomeId == forest) {
                Color c = forestPixels->getColorAt(x % 256, y % 256);
                r = r * c.r / 255.0f;
                g = g * c.g / 255.0f;
                b = b * c.b / 255.0f;
            }
        }
    }
    */


    return pixels;
}

void initCubiomes(int64_t seed) {
    initBiomeColoursMyPalette(biomeColours);  // Initialize a color map for biomes.
    initBiomes(); // Initialize global biome table.
    setupGenerator(&g, MC_1_16); // Allocate and initialize a stack of biome layers.
    applySeed(&g, seed);
}

int lodToLayer(int lod) {
    switch (lod) {
        case 0:
            return L_VORONOI_1;
        case 1:
            return L_RIVER_MIX_4;
        case 2:
            return L_ZOOM_16;
        case 3:
            return L_ZOOM_64;
    }

    return L_VORONOI_1;
}

void initLayer(int lod) {
    int desiredLayer = lodToLayer(lod);
    // Extract the desired layer.
    layer = &g.layers[desiredLayer]; // 4 means that there is 4 blocks per pixel.
    biomeIds = allocCache(layer, TILE_SIZE, TILE_SIZE);
}

void destoryLayer() {
    free(biomeIds);
}


struct TerrainTile {
    bool blah;

    // TODO: minecraft has 170 biomes. No need to store them as an int. Could just use 1 byte.
    int* biomes;

    SDL_Texture* texture;
    glm::ivec2 worldPos;
    glm::ivec2 tilePos;

    int creationTime;

    TerrainTile(const glm::ivec2& worldPos, const glm::ivec2& tilePos, SDL_Texture* texture):
        worldPos(worldPos), tilePos(tilePos), texture(texture) {
        creationTime = SDL_GetTicks();
        biomes = nullptr;
        blah = false;
    }

    virtual ~TerrainTile() {
        if (texture)
            SDL_DestroyTexture(texture);
        if (biomes)
            delete biomes;
    }

    void render(SDL_Renderer* renderer, const View& view) {
        if (!texture)
            return;



        int currentTick = SDL_GetTicks();

        float secElapsed = (currentTick - creationTime) / 1000.0f;
        int alpha = glm::clamp(secElapsed * 4.0f, 0.0f, 1.0f) * 255;

        SDL_SetTextureColorMod(texture, alpha, alpha, alpha);

        if (blah) {
            //SDL_SetTextureColorMod(texture, 192, 192, 192);
            blah = false;
        }

        const SDL_Rect src = {0, 0, 512, 512};

        glm::ivec2 screenPos = worldToScreen(worldPos, view);

        int tileSize = TILE_SIZE * getLodMultiplicator(lod);

        // multiply width and height by scale factor to convert from screen to world space.
        SDL_Rect dst = {screenPos.x, screenPos.y, glm::ceil(tileSize * view.scale.x), glm::ceil(tileSize * view.scale.y)};

        // Blit texture on the screen.
        SDL_RenderCopy(renderer, texture, &src, &dst);
    }
};



// Takes in world x and y. Basically, 0 0 is first tile, 512 0 is the second tile, and so on.
SDL_Texture* getTileTexture(SDL_Renderer* renderer, int x, int y) {
    unsigned char* pixels = getTilePixels(x, y);

    SDL_Texture* texture = createTextureFromPixels(renderer, pixels, TILE_SIZE, TILE_SIZE, 3);

    delete pixels;

    return texture;
}

// Takes in tile coords. 0 0 is the first tile, 1 0 is the second, and so on.
TerrainTile* createTile(SDL_Renderer* renderer, int x, int y) {
    int lodMult = getLodMultiplicator(lod);

    // Convert from 0 1 to 0 512 (from tile coords to world coords)
    x *= TILE_SIZE;
    y *= TILE_SIZE;
    SDL_Texture* texture = getTileTexture(renderer, x, y);
    TerrainTile* tile = new TerrainTile(glm::ivec2(x, y) * lodMult, glm::ivec2(x, y), texture);

    tile->biomes = new int[TILE_SIZE * TILE_SIZE];

    memcpy(tile->biomes, biomeIds, TILE_SIZE * TILE_SIZE * sizeof(int));

    return tile;
}

// Takes in tile coords. 0 0 is the first tile, 1 0 is the second, and so on.
SDL_Texture* createTileTexture(SDL_Renderer* renderer, int x, int y) {
    int lodMult = getLodMultiplicator(lod);

    // Convert from 0 1 to 0 512 (from tile coords to world coords)
    x *= TILE_SIZE;
    y *= TILE_SIZE;
    return getTileTexture(renderer, x, y);
}


//std::vector<TerrainTile*> unreadyTiles;
//std::vector<TerrainTile*> readyTiles;
std::vector<TerrainTile*> tiles;
std::unordered_map<glm::ivec2, TerrainTile*> tileMap;

//const char* FONT_PATH = "Resources/Fonts/Montserrat/Montserrat-Light.ttf";
//const char* FONT_PATH = "Resources/Fonts/Montserrat/Montserrat-Regular.ttf";
const char* FONT_PATH = "Resources/Fonts/Montserrat/Montserrat-Regular.ttf";


struct Widget {
    std::wstring description;
    std::wstring name;
};

struct PlayerAreaInfo : public Widget {
    enum Type {
        Spawn, Community, Player
    };

    int type;

    PlayerAreaInfo(Type type, const glm::ivec2& position, float radius, const std::wstring& description, const std::wstring& name) {
        this->description = description;
        worldCoord = position;
        this->radius = radius;
        this->type = type;
        this->name = name;
        selected = false;
    }


    glm::ivec2 worldCoord;
    float radius;

    bool selected;
};

std::vector<PlayerAreaInfo> zones;



void MinecraftViewer::onInit() {
    // calculate initial offset to be centered at 0, 0
    //view.worldPos = glm::vec2((TILE_SIZE - width) / 2, (TILE_SIZE - height) / 2);
    // Since we're not offsetting our tiles anymore, they start at exact 0. Hence we could just ignore tile_size stuff in here, just divide out screen by half and that's it.
    view.worldPos = glm::vec2(-width / 2, -height / 2);

    initCubiomes(-8752152121975834768);

    initLayer(0);

    m_threadRunning = true;
    m_thread = std::thread(anotherThread, this);


    m_fonts = new FontCache(getDpi(window));
    m_mainFont = m_fonts->addFont(FONT_PATH, 16, true);
    m_fonts->compileFonts(renderer, 2);


    mapMarkerImage = loadImageFromDisk(renderer, "Resources/Images/map_marker.png");
    mapMarkerTexture = loadTextureFromDisk(renderer, "Resources/Images/map_marker.png");
    mapMarkerTexture->center = glm::ivec2(16, 32);


    biomeUnderCursor = -1;


    forestPixels = new STB_Image("Resources/Images/oak_forest.png");
    sprucePixels = new STB_Image("Resources/Images/spruce_forest.png");
    acaciaPixels = new STB_Image("Resources/Images/acacia_forest.png");
    mountainPixels = new STB_Image("Resources/Images/mountains.png");

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Spawn, glm::ivec2(0), 100.0, L"Общественная территория:\nСпавн-зона", L"Спавн"));
    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Player, glm::ivec2(-715, -150), 150.0, L"Территория игрока Venaxsys:\nОсновная база", L"Venaxsys"));
    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(300, -50), 150.0, L"Общественная территория:\nДеревня новичков:\n - Торговля с жителями;\n - Ферма железа;\n - Склад;\n - Бар зельеварения;\n - Ферма слизи;\n - Ферма скелетов;\n - И др.", L"Деревня новичков"));
    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-500, 570), 10.0, L"Общественная территория:\nФерма пауков", L"Ферма пауков"));
    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-232, -232), 50.0, L"Общественная территория:\nФерма стражей", L"Ферма стражей"));
    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-3000, -2000), 200.0, L"Общественная территория:\nРаскопки месы", L"Раскопки месы"));
    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-2000, 0), 200.0, L"Общественная территория:\nРаскопки пустыни (ферма песка)", L"Раскопки пустыни"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(3615, 2082), 5.0, L"Территория игрока Venaxsys:\nПортал на старую ферму визеров", L"V"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-150, 960), 100.0, L"Территория игроков MarySnowRose и ArtoSnowRise:\nБаза", L"MarySnowRose\nArtoSnowRise"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-255, -505), 150.0, L"Территория игрока MadMaxNice:\nБаза", L"MadMaxNice"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(645, 45), 50.0, L"Территория игрока Poigral:\nБаза", L"Poigral"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(260, 170), 120.0, L"Территория игрока Judauliss:\nБаза", L"Judauliss"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-570, 200), 100.0, L"Территория игрока GDD:\nБаза", L"GDD"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(240, -300), 50.0, L"Территория игрока Ctrltrigger:\nБаза", L"Ctrltrigger"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(406, -785), 75.0, L"Территория игрока Vydumka:\nБаза", L"Vydumka"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(750, -450), 75.0, L"Территория игрока D1OGEN:\nПобочная база (необжита)", L"D"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(300, 600), 100.0, L"Территория игроков Yffrik и Ssivlay:\nБаза", L"Yffrik\nSsivlay"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(940, -500), 50.0, L"Территория игрока Parker_dok:\nБаза", L"Parker_dok"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-770, 880), 50.0, L"Территория игрока MarySnowRose:\nЛичная ферма жителей", L"M"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(481, -226), 10.0, L"Территория игрока Luyker:\nБаза", L"Luyker"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-900, 300), 100.0, L"Территория игрока COVIDenis19:\nБаза", L"COVIDenis19"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(400, 800), 50.0, L"Территория игрока Tarta:\nМаяк", L"T"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-500, 700), 100.0, L"Территория игрока Zubrilka:\nБаза", L"Zubrilka"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-540, -345), 150.0, L"Территория игрока LegalwyndDuke:\nБаза", L"LegalwyndDuke"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(890, -34), 150.0, L"Территория игрока Ec_lips:\nБаза", L"Ec_lips"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(680, 320), 60.0, L"Территория игрока Gwell:\nБаза", L"Gwell"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(1760, -4060), 100.0, L"Территория игрока D1OGEN:\nОсновная база", L"D1OGEN"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(2000, -4900), 100.0, L"Территория игрока Willi Comca:\nОсновная база (Ледяной замок)", L"Willi Comca"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(430, 100), 75.0, L"Территория игрока Zaharov:\nБаза", L"Zaharov"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(500, 300), 150.0, L"Территория игрока Artrusya:\nБаза (Адская деревня)", L"Artrusya"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(-150, 415), 100.0, L"Территория игрока zzSEREGAss:\nОсновная База", L"zzSEREGAss"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(1000, 960), 100.0, L"Территория игроков Materdaly, Tarta и Sir_Evgesha:\nБаза", L"Materdaly\nTarta\nSir_Evgesha"));

    zones.push_back(PlayerAreaInfo(PlayerAreaInfo::Community, glm::ivec2(0, -250), 50.0, L"Территория игрока OutSider:\nБаза (Шахтёрские угодья)", L"OutSider"));




}


void MinecraftViewer::onExit() {
    destoryLayer();

    m_threadRunning = false;
    m_thread.join();

    delete m_fonts;

    delete forestPixels;
    delete sprucePixels;
    delete acaciaPixels;
    delete mountainPixels;
}

void MinecraftViewer::anotherThread() {
    /*
    while (m_threadRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Thread loop\n");


        std::lock_guard<std::mutex> lock(m_mutex);

        for (auto& tile : unreadyTiles) {
            printf("Processing tile: %i %i\n", tile->tilePos.x, tile->tilePos.y);
            tile->texture = createTileTexture(renderer, tile->tilePos.x, tile->tilePos.y);
            readyTiles.push_back(tile);
        }

        unreadyTiles.clear();
    }
    */
}

void MinecraftViewer::deleteTiles(const glm::ivec2& topLeftTile, const glm::ivec2& botRightTile) {
    for (auto iter = tileMap.begin(); iter != tileMap.end(); ) {
        TerrainTile* tile = iter->second;
        glm::ivec2 coord = iter->first;

        bool shouldErase = coord.x < topLeftTile.x - 1 || coord.y < topLeftTile.y - 1 || coord.x > botRightTile.x || coord.y > botRightTile.y;

        if (shouldErase) {
            tiles.erase(std::remove(tiles.begin(), tiles.end(), tile), tiles.end());

            delete tile;

            iter = tileMap.erase(iter);
        } else ++iter;
    }
}

void MinecraftViewer::createTiles(const glm::ivec2& topLeftTile, const glm::ivec2& botRightTile) {
    int start = SDL_GetTicks();

    for (int y = topLeftTile.y - 1; y <= botRightTile.y; y++) {
        for (int x = topLeftTile.x - 1; x <= botRightTile.x; x++) {
            if (SDL_GetTicks() - start > 5)
                return;

            auto iter = tileMap.find(glm::ivec2(x, y));
            if (iter == tileMap.end()) {
                tiles.push_back(createTile(renderer, x, y));
                tileMap.insert({glm::ivec2(x, y), tiles.back()});
            }
        }
    }
}

Widget* selectedWidget = nullptr;

Widget* MinecraftViewer::selectWidget() {

    glm::vec2 mouseWorld = screenToWorld(events->getMouseCoords(), view);

    // Select widget under cursor

    for (PlayerAreaInfo& zone : zones) {
        float distance = glm::distance(glm::vec2(zone.worldCoord), mouseWorld);

        if (distance < zone.radius) {
            zone.selected = true;
            return &zone;
        }
    }

    return nullptr;


}

void MinecraftViewer::onUpdate() {
    /*
    static int time = SDL_GetTicks();
    if (SDL_GetTicks() > time + 1000) {
        time = SDL_GetTicks();
        printf("Main loop\n");

    }
    */


    if (events->isKeyPressed(SDLK_ESCAPE))
        stopRunning();

    updateCamera();

    // Convert screen top left and bottom right coords to worldspace, then we will see how much tiles we can fit in this rectangle, and iterate through each of them to actually load or unload them.

    /*
    glm::vec2 topLeftWorld = screenToWorld(glm::ivec2(0, 0), view);
    glm::vec2 botRightWorld = screenToWorld(glm::ivec2(width, height), view);
    */

    /* // for testing
    glm::vec2 topLeftWorld = screenToWorld(glm::vec2(width, height) * 0.25f, view);
    glm::vec2 botRightWorld = screenToWorld(glm::vec2(width, height) * 0.75f, view);
    */

    // make it 1 pixel smaller.
    glm::vec2 topLeftWorld = screenToWorld(glm::vec2(1, 1), view);
    glm::vec2 botRightWorld = screenToWorld(glm::vec2(width - 1, height - 1), view);

    // Convert to tile coordinate system. Also, enlarge by 1 tile since it doesn't capture that by default. Without it it would only contain those tiles that are fully inside of our screen

    int lodMult = getLodMultiplicator(lod);

    glm::ivec2 topLeftTile = glm::ivec2(topLeftWorld / float(TILE_SIZE));
    glm::ivec2 botRightTile = glm::ivec2(botRightWorld / float(TILE_SIZE));




    topLeftTile /= lodMult;
    botRightTile /= lodMult;


    //if (m_mutex.try_lock()) {





    deleteTiles(topLeftTile, botRightTile);
    createTiles(topLeftTile, botRightTile);

    /*
    for (auto& tile : readyTiles) {
        tiles.push_back(tile);
    }

    readyTiles.clear();

    */

    //m_mutex.unlock();
    // }

    mouseWorldPos = screenToWorld(events->getMouseCoords(), view);

    glm::ivec2 mouseTilePos = glm::floor((mouseWorldPos / float(TILE_SIZE))/ float(lodMult));

    auto iter = tileMap.find(mouseTilePos);
    if (iter != tileMap.end()) {
        TerrainTile* tile = iter->second;

        glm::vec2 tileWorldStart = mouseTilePos * TILE_SIZE * lodMult;
        glm::vec2 relativeOffset = mouseWorldPos - tileWorldStart;
        glm::ivec2 pixelCoord = relativeOffset / float(lodMult);

        pixelCoord.x = glm::clamp(pixelCoord.x, 0, TILE_SIZE - 1);
        pixelCoord.y = glm::clamp(pixelCoord.y, 0, TILE_SIZE - 1);

        biomeUnderCursor = tile->biomes[pixelCoord.y * TILE_SIZE + pixelCoord.x];
        tile->blah = true;
    } else {
        biomeUnderCursor = -1;
    }


    selectedWidget = selectWidget();




}




void drawPixels(SDL_Renderer* renderer, unsigned char* pixels) {
    int w = TILE_SIZE;
    int h = TILE_SIZE;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            unsigned char* col = &pixels[(y * w + x)*3];
            unsigned char r = col[0];
            unsigned char g = col[1];
            unsigned char b = col[2];

            SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void clearTiles() {
    for (auto& tile : tiles) {
        delete tile;
    }

    tiles.clear();
    tileMap.clear();
}

void setLod(int lod) {
    destoryLayer();
    initLayer(lod);
}

void MinecraftViewer::updateCamera() {
    bool supportPan = true;
    bool supportZoom = true;

    if (supportPan) {
        // Move camera on mouse press + drag.
        // Also, could just bake this into events, like "MouseOffsetOnPress"
        static glm::vec2 viewAtPress(0);
        static glm::ivec2 mouseAtPress(0);

        // At start of the click, remember our current mouse and view position.
        if (events->isKeyPressed(SDL_BUTTON_LEFT)) {
            mouseAtPress = events->getMouseCoords();
            viewAtPress = view.worldPos;
        }

        if (events->isKeyDown(SDL_BUTTON_LEFT)) {
            // mouse pos in screenspace
            glm::ivec2 currentMouse = events->getMouseCoords();

            // difference (mouse offset from the click) in screenspace
            // converting to vec2 instead of ivec2 since we're going to divive by float later.
            glm::vec2 mouseDiffScreen = mouseAtPress - currentMouse;

            // divide by scale to convert to worldspace
            glm::vec2 mouseDiffWorldPos = mouseDiffScreen / view.scale;

            // then translate our captured view worldposition with this offset.
            view.worldPos = viewAtPress + mouseDiffWorldPos;
        }
    }



    if (supportZoom) {
        static float realZoom = 1.0f; // real zoom value
        static float interpolatedZoom = 1.0f; // sluggish lowpassed version of real zoom



        // capture mouse world position before zoom
        // capture mouse world position after zoom
        // calculate the difference
        // and offset our camera world pos by this difference.
        // So, basically, our screespace mousePos doesn't change. But in worldspace, at different scales, it does change. So, we record this change and see the difference between previous zoom-level and current zoom-level, and this will be our worldspace offset.
        glm::vec2 mouseWorldBeforeZoom = screenToWorld(events->getMouseCoords(), view);

        // Apply zoom based on mouse wheel movement
        realZoom += realZoom * events->getMouseScrollDiff().y * 0.2f; // multiply by itself to normalize it (make zooming linear)

        realZoom = glm::clamp(realZoom, 1.0f / 128.0f, 64.0f);

        // interpolatedZoom is basically lagging behind realZoom.
        interpolatedZoom = glm::mix(interpolatedZoom, realZoom, 0.4f);


        int oldLod = lod;

        if (realZoom <= 1.0f / 64.0f) {
            lod = 3;
        } else if (realZoom <= 1.0f / 16.0f) {
            lod = 2;
        } else if (realZoom <= 1.0 / 4.0f) {
            lod = 1;
        } else if (realZoom <= 1.0f) {
            lod = 0;
        }

        if (oldLod != lod) {
            printf("Current LOD: %i\n", lod);
            clearTiles();
            setLod(lod);
        }




        // store zoom in view scale variable
        view.scale = glm::vec2(interpolatedZoom);



        // get mouse world pos after the zoom
        glm::vec2 mouseWorldAfterZoom = screenToWorld(events->getMouseCoords(), view);




        // translate camera according to mouse difference between zoom levels
        view.worldPos += mouseWorldBeforeZoom - mouseWorldAfterZoom;



    }


}

void set_pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawPoint(renderer, x, y);
}



glm::ivec2 clamp(const glm::ivec2& value, const glm::ivec2& min, const glm::ivec2& max) {
    return glm::ivec2(glm::clamp(value.x, min.x, max.x), glm::clamp(value.y, min.y, max.y));
}

//Distance to ceil:
double DC(int r, int y) {
    double x = std::sqrt(r*r-y*y);
    return std::ceil(x) - x;
}

// https://gist.github.com/bert/1085538
// The '(x != 0 && y != 0)' test in the last line of this function
// may be omitted for a performance benefit if the radius of the
// circle is known to be non-zero.
void plot4points (SDL_Renderer* renderer, int cx, int cy, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  set_pixel (renderer, cx + x, cy + y, r, g, b, a);
  if (x != 0) set_pixel (renderer, cx - x, cy + y, r, g, b, a);
  if (y != 0) set_pixel (renderer, cx + x, cy - y, r, g, b, a);
  if (x != 0 && y != 0) set_pixel (renderer, cx - x, cy - y, r, g, b, a);
}

void plot8points (SDL_Renderer* renderer, int cx, int cy, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  plot4points (renderer, cx, cy, x, y, r, g, b, a);
  if (x != y) plot4points (renderer, cx, cy, y, x, r, g, b, a);
}




void MinecraftViewer::drawAntialiasedCircle(float centerX, float centerY, float r, const Color& col) {
    //printf(" RAD: %i\n", r);
    if (centerX - r > width || centerY - r > height || centerX < -r || centerY < -r) return;


    centerX--;
    centerY--;

    float A = col.a;

    float y=-1; // takes care of holes. Previously was = 0, but we had to fill in missing pixels.
    float x=r;
    float d=0;

    // TODO: ideally, we would want to do this 8 times.
    //set_pixel(renderer, x + centerX,y + centerY, 255, 255, 255, A);

    while (x - 2 > y - 0.001f) {
        y++;

        float dc = DC(r, y);
        if (dc < d) x--;
        d = dc;

        plot8points(renderer, centerX, centerY, x, y, col.r, col.g, col.b, A*(1-dc ));
        plot8points(renderer, centerX, centerY, x - 1, y, col.r, col.g, col.b, A*   dc );
    }
}












void BSP_LCD_DrawThickCircle(SDL_Renderer* renderer, uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint16_t lineThickness) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 127);

    Radius += (lineThickness/2);
    uint16_t Radius2 = Radius - lineThickness;

    // Good, may not be the fastest
    // Does not draw overlapping pixels
    //BSP_LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);	// Line Draw Color
    //BSP_LCD_DrawHLine(Xpos - Radius + 1, Ypos, lineThickness);	// Left Middle
    //BSP_LCD_DrawHLine(Xpos + Radius2, Ypos, lineThickness);	// Right Middle
    //BSP_LCD_DrawVLine(Xpos, Ypos - Radius + 1, lineThickness);	// Top Middle
    //BSP_LCD_DrawVLine(Xpos, Ypos + Radius2, lineThickness);	// Bottom middle
    //uint32_t TextColor = BSP_LCD_GetTextColor();			// Pixel Draw Color
    uint32_t RR = Radius * Radius;
    uint32_t R2R2 = Radius2 * Radius2;
    for(int16_t y = -Radius; y < 0; y++) {
        for(int16_t x= -Radius; x < 0; x++) {
            uint32_t r2 = x * x + y * y;
            if ((r2 <= RR) && (r2 >= R2R2)) {
                SDL_RenderDrawPoint(renderer, Xpos+x, Ypos+y);
                SDL_RenderDrawPoint(renderer, Xpos+x, Ypos-y);
                SDL_RenderDrawPoint(renderer, Xpos-x, Ypos+y);
                SDL_RenderDrawPoint(renderer, Xpos-x, Ypos-y);
            }
        }
    }
}






void MinecraftViewer::onRender() {
    for (auto& tile : tiles) {

        tile->render(renderer, view);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    glm::vec2 centerWorld = worldToScreen(glm::ivec2(0), view);

    /*
    SDL_Rect rect = {centerWorld.x-8, centerWorld.y-8, 8, 8};



    SDL_RenderFillRect(renderer, &rect);
    */


    SDL_Rect zone = { width * 0.25f, height * 0.25, width * 0.5f, height * 0.5f };

    //SDL_RenderDrawRect(renderer, &zone);

    //m_fonts->testRender(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 96);

    static glm::ivec2 prevSize;

    SDL_Rect r = {8, 8, prevSize.x, prevSize.y};
    SDL_RenderFillRect(renderer,  &r);

    std::wstringstream ss;
    ss << L"[" << int(mouseWorldPos.x) << L", " << int(mouseWorldPos.y) << L"]";
    ss << L"\n" << getBiomeName(biomeUnderCursor);
    ss << L"\n" << L"Ур. приближения: " << lod;

    Color c1 = {255, 255, 255, 255};
    Color c2 = {0, 0, 0, 32};

    // Render black font and white on top of it.
    /*

    glm::ivec2 p1 = m_fonts->renderText(renderer, m_mainFont, s1.str(), 17, 17, c2);
    m_fonts->renderText(renderer, m_mainFont, s1.str(), 16, 16, c1);
    glm::ivec2 p2 = m_fonts->renderText(renderer, m_mainFont, s2.str(), 17, 17 + p1.y, c2);
    m_fonts->renderText(renderer, m_mainFont, s2.str(), 16, 16 + p1.y, c1);
    glm::ivec2 p3 = m_fonts->renderText(renderer, m_mainFont, s3.str(), 17, 17 + p1.y + p2.y, c2);
    m_fonts->renderText(renderer, m_mainFont, s3.str(), 16, 16 + p1.y + p2.y, c1);

    prevSize.x = glm::max(glm::max(p1.x, p2.x), p3.x);
    prevSize.y = p1.y + p2.y + p3.y + 16;

    */

    Rect r1 = m_fonts->renderTextLines(renderer, m_mainFont, ss.str(), 16, 16, c1, 1.0f, 0, true);



    // Draw widget description
    if (selectedWidget) {
        //SDL_Rect textRect = {r1.x, r1.y, r1.w, r1.h};
        //SDL_RenderFillRect(renderer, &textRect);

        static Rect pRect;

        //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 64);
        //SDL_Rect textRect = {pRect.x - 8, pRect.y - 8, pRect.w + 8 * 2, pRect.h + 8 * 2};
        //SDL_RenderFillRect(renderer, &textRect);

        int widgetWidth = 0;

        std::wstringstream s1;
        s1 << selectedWidget->description;

        Rect r1 = m_fonts->getTextLinesRect(m_mainFont, s1.str(), 0, 0, widgetWidth);
        Rect r2 = m_fonts->renderTextLines(renderer, m_mainFont, s1.str(), width - r1.w - 16, 16, c1, 1.0f, widgetWidth, true);
        pRect = r2;
    }


    /*
    SDL_Rect src = {0, 0, 32, 32};
    // Offset by 16 in X to center it, and by 32 to make it at the bottom. I.e - we're moving anchor to the bottom center.



    SDL_Rect dst = {centerWorld.x - 16, centerWorld.y - 32, 32, 32};
    dst.x = glm::clamp(dst.x, -16, width - 16);
    dst.y = glm::clamp(dst.y, -16, height - 16);

    SDL_RenderCopy(renderer, mapMarkerImage, &src, &dst);
    */





    /*
    SDL_SetRenderDrawColor(renderer, 186, 91, 48, 255);
    SDL_Rect r22 = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &r22);
    */



    Color col1 = {255, 255, 255, 255};
    Color col2 = {255, 255, 255, 192};

    // Draw player zones
    for (PlayerAreaInfo& zone : zones) {
        Color col = col2;
        Color shadowCol = COLOR_SHADOW;

        if (zone.selected) {
            col = col1;
            zone.selected = false;
        }

        // make shadow less visible
        shadowCol.a *= col.a / 255.0f;

        glm::ivec2 pos = worldToScreen(zone.worldCoord, view);

        int worldRadius = zone.radius * view.scale.x;
        drawAntialiasedCircle(pos.x, pos.y, worldRadius, col);

        if (zone.type == PlayerAreaInfo::Spawn) {
            glm::ivec2 offset = glm::ivec2(mapMarkerTexture->width, mapMarkerTexture->height) / 2;
            glm::ivec2 markerPos = clamp(centerWorld, {0, offset.y}, {width, height + offset.y});


            mapMarkerTexture->setAlpha(col.a);
            mapMarkerTexture->render(renderer, markerPos);


            // Draw text under spawn marker
            Rect r1 = m_fonts->getTextLinesRect(m_mainFont, zone.name, pos.x, pos.y);
            m_fonts->renderTextLines(renderer, m_mainFont, zone.name, pos.x - r1.w / 2, pos.y, col, 1.0f, 0, true, shadowCol);
        } else {
            // Draw same text, but higher since there is no icon here.
            Rect r1 = m_fonts->getTextLinesRect(m_mainFont, zone.name, pos.x, pos.y);
            m_fonts->renderTextLines(renderer, m_mainFont, zone.name, pos.x - r1.w / 2, pos.y - r1.h / 2, col, 1.0f, 0, true, shadowCol);
        }




    }




    // Draw playzone

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 192);

    glm::vec2 livingStart = worldToScreen({-1024, -1024}, view);
    glm::vec2 livingEnd = worldToScreen({1024, 1024}, view);

    bool select = !selectedWidget && (mouseWorldPos.x >= -1024 && mouseWorldPos.x <= 1024 && mouseWorldPos.y >= -1024 && mouseWorldPos.y <= 1024);

    if (select) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    glm::vec2 livingSize = livingEnd - livingStart;
    SDL_Rect livingAreaRect = {livingStart.x, livingStart.y, livingSize.x, livingSize.y };

    SDL_RenderDrawRect(renderer, &livingAreaRect);


    // Draw playzone description
    if (select) {
        int widgetWidth = 300;

        std::wstringstream s1;
        s1 << L"Жилая зона";

        Rect r1 = m_fonts->getTextLinesRect(m_mainFont, s1.str(), 0, 0, widgetWidth);
        m_fonts->renderTextLines(renderer, m_mainFont, s1.str(), width - r1.w - 16, 16, c1, 1.0f, widgetWidth, true);

        //SDL_Rect textRect = {r1.x, r1.y, r1.w, r1.h};
        //SDL_RenderFillRect(renderer, &textRect);

    }

    //BSP_LCD_DrawThickCircle(renderer, 300, 300, 300, 5);
}
