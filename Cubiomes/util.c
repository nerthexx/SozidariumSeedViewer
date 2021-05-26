#include "util.h"
#include "layers.h"

#include <stdio.h>
#include <string.h>


const char *biome2str(int id)
{
    switch (id)
    {
    case ocean: return "ocean";
    case plains: return "plains";
    case desert: return "desert";
    case mountains: return "mountains";
    case forest: return "forest";
    case taiga: return "taiga";
    case swamp: return "swamp";
    case river: return "river";
    case nether_wastes: return "nether_wastes";
    case the_end: return "the_end";
    // 10
    case frozen_ocean: return "frozen_ocean";
    case frozen_river: return "frozen_river";
    case snowy_tundra: return "snowy_tundra";
    case snowy_mountains: return "snowy_mountains";
    case mushroom_fields: return "mushroom_fields";
    case mushroom_field_shore: return "mushroom_field_shore";
    case beach: return "beach";
    case desert_hills: return "desert_hills";
    case wooded_hills: return "wooded_hills";
    case taiga_hills: return "taiga_hills";
    // 20
    case mountain_edge: return "mountain_edge";
    case jungle: return "jungle";
    case jungle_hills: return "jungle_hills";
    case jungle_edge: return "jungle_edge";
    case deep_ocean: return "deep_ocean";
    case stone_shore: return "stone_shore";
    case snowy_beach: return "snowy_beach";
    case birch_forest: return "birch_forest";
    case birch_forest_hills: return "birch_forest_hills";
    case dark_forest: return "dark_forest";
    // 30
    case snowy_taiga: return "snowy_taiga";
    case snowy_taiga_hills: return "snowy_taiga_hills";
    case giant_tree_taiga: return "giant_tree_taiga";
    case giant_tree_taiga_hills: return "giant_tree_taiga_hills";
    case wooded_mountains: return "wooded_mountains";
    case savanna: return "savanna";
    case savanna_plateau: return "savanna_plateau";
    case badlands: return "badlands";
    case wooded_badlands_plateau: return "wooded_badlands_plateau";
    case badlands_plateau: return "badlands_plateau";
    // 40  --  1.13
    case small_end_islands: return "small_end_islands";
    case end_midlands: return "end_midlands";
    case end_highlands: return "end_highlands";
    case end_barrens: return "end_barrens";
    case warm_ocean: return "warm_ocean";
    case lukewarm_ocean: return "lukewarm_ocean";
    case cold_ocean: return "cold_ocean";
    case deep_warm_ocean: return "deep_warm_ocean";
    case deep_lukewarm_ocean: return "deep_lukewarm_ocean";
    case deep_cold_ocean: return "deep_cold_ocean";
    // 50
    case deep_frozen_ocean: return "deep_frozen_ocean";

    case the_void: return "the_void";

    // mutated variants
    case sunflower_plains: return "sunflower_plains";
    case desert_lakes: return "desert_lakes";
    case gravelly_mountains: return "gravelly_mountains";
    case flower_forest: return "flower_forest";
    case taiga_mountains: return "taiga_mountains";
    case swamp_hills: return "swamp_hills";
    case ice_spikes: return "ice_spikes";
    case modified_jungle: return "modified_jungle";
    case modified_jungle_edge: return "modified_jungle_edge";
    case tall_birch_forest: return "tall_birch_forest";
    case tall_birch_hills: return "tall_birch_hills";
    case dark_forest_hills: return "dark_forest_hills";
    case snowy_taiga_mountains: return "snowy_taiga_mountains";
    case giant_spruce_taiga: return "giant_spruce_taiga";
    case giant_spruce_taiga_hills: return "giant_spruce_taiga_hills";
    case modified_gravelly_mountains: return "modified_gravelly_mountains";
    case shattered_savanna: return "shattered_savanna";
    case shattered_savanna_plateau: return "shattered_savanna_plateau";
    case eroded_badlands: return "eroded_badlands";
    case modified_wooded_badlands_plateau: return "modified_wooded_badlands_plateau";
    case modified_badlands_plateau: return "modified_badlands_plateau";
    // 1.14
    case bamboo_jungle: return "bamboo_jungle";
    case bamboo_jungle_hills: return "bamboo_jungle_hills";
    // 1.16
    case soul_sand_valley: return "soul_sand_valley";
    case crimson_forest: return "crimson_forest";
    case warped_forest: return "warped_forest";
    case basalt_deltas: return "basalt_deltas";
    }
    return NULL;
}

void setBiomeColour(unsigned char biomeColour[256][3], int id,
        unsigned char r, unsigned char g, unsigned char b)
{
    biomeColour[id][0] = r;
    biomeColour[id][1] = g;
    biomeColour[id][2] = b;
}

int min(int v0, int v1) {
    return v0 < v1 ? v0 : v1;
}

int max(int v0, int v1) {
    return v0 > v1 ? v0 : v1;
}

int clamp(int v, int vMin, int vMax) {
    return max(min(v, vMin), vMax);
}

void setBiomeColourB(unsigned char biomeColour[256][3], int id,
        unsigned char r, unsigned char g, unsigned char b)
{
    // Brightness
    r = min(r + 10, 255);
    g = min(g + 10, 255);
    b = min(b + 10, 255);

    // Contrast
    r = min((int)(r + r * 0.05f), 255);
    g = min((int)(g + g * 0.05f), 255);
    b = min((int)(b + b * 0.05f), 255);

    biomeColour[id][0] = r;
    biomeColour[id][1] = g;
    biomeColour[id][2] = b;
}

void setBiomeColourHill(unsigned char biomeColour[256][3], int id, unsigned char r, unsigned char g, unsigned char b) {
    setBiomeColourB(biomeColour, id, min(r+5, 255), min(g+5, 255), min(b+5, 255));
}

void setBiomeColourDeep(unsigned char biomeColour[256][3], int id, unsigned char r, unsigned char g, unsigned char b) {
    setBiomeColourB(biomeColour, id,  max(r-5, 0), max(g-5, 0), max(b-5, 0));
}

void setMutationColour(unsigned char biomeColour[256][3], int mutated, int parent)
{
    unsigned int c;
    biomeColour[mutated][0] = (c = biomeColour[parent][0] + 40) > 255 ? 255 : c;
    biomeColour[mutated][1] = (c = biomeColour[parent][1] + 40) > 255 ? 255 : c;
    biomeColour[mutated][2] = (c = biomeColour[parent][2] + 40) > 255 ? 255 : c;
}

void initBiomeColours(unsigned char biomeColours[256][3])
{
    // This colouring scheme is taken from the AMIDST program:
    // https://github.com/toolbox4minecraft/amidst
    // https://sourceforge.net/projects/amidst.mirror/

    memset(biomeColours, 0, 256*3);

    setBiomeColour(biomeColours, ocean, 0, 0, 112);
    setBiomeColour(biomeColours, plains, 141, 179, 96);
    setBiomeColour(biomeColours, desert, 250, 148, 24);
    setBiomeColour(biomeColours, mountains, 96, 96, 96);
    setBiomeColour(biomeColours, forest, 5, 102, 33);
    setBiomeColour(biomeColours, taiga, 11, 102, 89);
    setBiomeColour(biomeColours, swamp, 7, 249, 178);
    setBiomeColour(biomeColours, river, 0, 0, 255);
    setBiomeColour(biomeColours, hell, 255, 0, 0);
    setBiomeColour(biomeColours, sky, 128, 128, 255);
    setBiomeColour(biomeColours, frozen_ocean, 112, 112, 214);
    setBiomeColour(biomeColours, frozen_river, 160, 160, 255);
    setBiomeColour(biomeColours, snowy_tundra, 255, 255, 255);
    setBiomeColour(biomeColours, snowy_mountains, 160, 160, 160);
    setBiomeColour(biomeColours, mushroom_fields, 255, 0, 255);
    setBiomeColour(biomeColours, mushroom_field_shore, 160, 0, 255);
    setBiomeColour(biomeColours, beach, 250, 222, 85);
    setBiomeColour(biomeColours, desert_hills, 210, 95, 18);
    setBiomeColour(biomeColours, wooded_hills, 34, 85, 28);
    setBiomeColour(biomeColours, taiga_hills, 22, 57, 51);
    setBiomeColour(biomeColours, mountain_edge, 114, 120, 154);
    setBiomeColour(biomeColours, jungle, 83, 123, 9);
    setBiomeColour(biomeColours, jungle_hills, 44, 66, 5);
    setBiomeColour(biomeColours, jungleEdge, 98, 139, 23);
    setBiomeColour(biomeColours, deep_ocean, 0, 0, 48);
    setBiomeColour(biomeColours, stone_shore, 162, 162, 132);
    setBiomeColour(biomeColours, snowy_beach, 250, 240, 192);
    setBiomeColour(biomeColours, birch_forest, 48, 116, 68);
    setBiomeColour(biomeColours, birch_forest_hills, 31, 95, 50);
    setBiomeColour(biomeColours, dark_forest, 64, 81, 26);
    setBiomeColour(biomeColours, snowy_taiga, 49, 85, 74);
    setBiomeColour(biomeColours, snowy_taiga_hills, 36, 63, 54);
    setBiomeColour(biomeColours, giant_tree_taiga, 89, 102, 81);
    setBiomeColour(biomeColours, giant_tree_taiga_hills, 69, 79, 62);
    setBiomeColour(biomeColours, wooded_mountains, 80, 112, 80);
    setBiomeColour(biomeColours, savanna, 189, 178, 95);
    setBiomeColour(biomeColours, savanna_plateau, 167, 157, 100);
    setBiomeColour(biomeColours, badlands, 217, 69, 21);
    setBiomeColour(biomeColours, wooded_badlands_plateau, 176, 151, 101);
    setBiomeColour(biomeColours, badlands_plateau, 202, 140, 101);

    setBiomeColour(biomeColours, warm_ocean, 0, 0, 172);
    setBiomeColour(biomeColours, lukewarm_ocean, 0, 0, 144);
    setBiomeColour(biomeColours, cold_ocean, 32, 32, 112);
    setBiomeColour(biomeColours, deep_warm_ocean, 0, 0, 80);
    setBiomeColour(biomeColours, deep_lukewarm_ocean, 0, 0, 64);
    setBiomeColour(biomeColours, deep_cold_ocean, 32, 32, 56);
    setBiomeColour(biomeColours, deep_frozen_ocean, 64, 64, 144);

    setBiomeColour(biomeColours, the_void, 0, 0, 0);

    setMutationColour(biomeColours, sunflower_plains, plains);
    setMutationColour(biomeColours, desert_lakes, desert);
    setMutationColour(biomeColours, gravelly_mountains, mountains);
    setMutationColour(biomeColours, flower_forest, forest);
    setMutationColour(biomeColours, taiga_mountains, taiga);
    setMutationColour(biomeColours, swamp_hills, swamp);
    setBiomeColour(biomeColours, ice_spikes, 180, 220, 220);
    setMutationColour(biomeColours, modified_jungle, jungle);
    setMutationColour(biomeColours, modified_jungle_edge, jungle_edge);
    setMutationColour(biomeColours, tall_birch_forest, birch_forest);
    setMutationColour(biomeColours, tall_birch_hills, birch_forest_hills);
    setMutationColour(biomeColours, dark_forest_hills, dark_forest);
    setMutationColour(biomeColours, snowy_taiga_mountains, snowy_taiga);
    setMutationColour(biomeColours, giant_spruce_taiga, giant_tree_taiga);
    setMutationColour(biomeColours, giant_spruce_taiga_hills, giant_tree_taiga_hills);
    setMutationColour(biomeColours, modified_gravelly_mountains, wooded_mountains);
    setMutationColour(biomeColours, shattered_savanna, savanna);
    setMutationColour(biomeColours, shattered_savanna_plateau, savanna_plateau);
    setMutationColour(biomeColours, eroded_badlands, badlands);
    setMutationColour(biomeColours, modified_wooded_badlands_plateau, wooded_badlands_plateau);
    setMutationColour(biomeColours, modified_badlands_plateau, badlands_plateau);

    setBiomeColour(biomeColours, bamboo_jungle, 118, 142, 20);
    setBiomeColour(biomeColours, bamboo_jungle_hills, 59, 71, 10);

    setBiomeColour(biomeColours, soul_sand_valley, 82, 41, 33);
    setBiomeColour(biomeColours, crimson_forest, 221, 8, 8);
    setBiomeColour(biomeColours, warped_forest, 73, 144, 123);
    setBiomeColour(biomeColours, basalt_deltas, 104, 95, 112); // TBD
}

void setBiomeColourChild(unsigned char biomeColour[256][3], int mutated, int parent) {
    biomeColour[mutated][0] = biomeColour[parent][0];
    biomeColour[mutated][1] = biomeColour[parent][1];
    biomeColour[mutated][2] = biomeColour[parent][2];
}

// Tries to have true-to-vanilla colors.
void initBiomeColoursMyPalette(unsigned char biomeColours[256][3]) {
    memset(biomeColours, 0, 256*3);

    setBiomeColourB(biomeColours, ocean, 35, 62, 116);
    setBiomeColourB(biomeColours, plains, 83, 108, 51);
    setBiomeColourB(biomeColours, desert, 216, 205, 161);
    setBiomeColourB(biomeColours, mountains, 79, 104, 78);
    setBiomeColourB(biomeColours, forest, 69, 110, 52);
    setBiomeColourB(biomeColours, taiga, 77, 105, 75);
    setBiomeColourB(biomeColours, swamp, 50, 66 + 15, 33 + 15);
    setBiomeColourB(biomeColours, river, 64, 81, 127);
    setBiomeColourB(biomeColours, hell, 255, 0, 0);
    setBiomeColourB(biomeColours, sky, 128, 128, 255);
    setBiomeColourB(biomeColours, frozen_ocean, 111, 139, 196);
    setBiomeColourB(biomeColours, frozen_river, 111, 139, 196);
    setBiomeColourB(biomeColours, snowy_tundra, 246, 251, 251);
    setBiomeColourB(biomeColours, snowy_mountains, 246, 251, 251);
    setBiomeColourB(biomeColours, mushroom_fields, 110, 97, 100);
    setBiomeColourB(biomeColours, mushroom_field_shore, 110, 97, 100);
    setBiomeColourB(biomeColours, beach, 216, 205, 161);
    setBiomeColourHill(biomeColours, desert_hills, 216, 205, 161);
    setBiomeColourHill(biomeColours, wooded_hills, 69, 110, 52);
    setBiomeColourHill(biomeColours, taiga_hills, 77, 105, 75);
    setBiomeColourB(biomeColours, mountain_edge, 79, 104, 78);
    setBiomeColourB(biomeColours, jungle, 27, 97, 7);
    setBiomeColourHill(biomeColours, jungle_hills, 27, 97, 7);
    setBiomeColourB(biomeColours, jungleEdge, 27, 97, 7);
    setBiomeColourDeep(biomeColours, deep_ocean, 35, 62, 116);
    setBiomeColourB(biomeColours, stone_shore, 124, 124, 124);
    setBiomeColourB(biomeColours, snowy_beach, 246, 251, 251);
    setBiomeColourB(biomeColours, birch_forest, 78, 107, 58);
    setBiomeColourHill(biomeColours, birch_forest_hills, 78, 107, 58);
    setBiomeColourB(biomeColours, dark_forest, 46, 70, 29);
    setBiomeColourB(biomeColours, snowy_taiga, 246, 251, 251);
    setBiomeColourHill(biomeColours, snowy_taiga_hills, 246, 251, 251);
    setBiomeColourB(biomeColours, giant_tree_taiga, 91, 63, 24);
    setBiomeColourHill(biomeColours, giant_tree_taiga_hills, 91, 63, 24);
    setBiomeColourB(biomeColours, wooded_mountains, 79, 104, 78);
    setBiomeColourB(biomeColours, savanna, 109, 105, 49);
    setBiomeColourB(biomeColours, savanna_plateau, 109, 105, 49);
    setBiomeColourB(biomeColours, badlands, 188, 101, 33);
    setBiomeColourB(biomeColours, wooded_badlands_plateau, 120, 86, 60);
    setBiomeColourB(biomeColours, badlands_plateau, 150, 93, 67);

    setBiomeColourB(biomeColours, warm_ocean, 41, 111, 121);
    setBiomeColourB(biomeColours, lukewarm_ocean, 42, 92, 123);
    setBiomeColourB(biomeColours, cold_ocean, 34, 47, 108);

    setBiomeColourDeep(biomeColours, deep_warm_ocean, 41, 111, 121);
    setBiomeColourDeep(biomeColours, deep_lukewarm_ocean, 42, 92, 123);
    setBiomeColourDeep(biomeColours, deep_cold_ocean, 34, 47, 108);
    setBiomeColourDeep(biomeColours, deep_frozen_ocean, 33, 32, 102);

    setBiomeColourB(biomeColours, the_void, 0, 0, 0);
    setBiomeColourB(biomeColours, ice_spikes, 246, 251, 251);
    setBiomeColourB(biomeColours, bamboo_jungle, 27, 97, 7);
    setBiomeColourHill(biomeColours, bamboo_jungle_hills, 27, 97, 7);

    setBiomeColourB(biomeColours, soul_sand_valley, 82, 41, 33); // not changed
    setBiomeColourB(biomeColours, crimson_forest, 221, 8, 8); // not changed
    setBiomeColourB(biomeColours, warped_forest, 73, 144, 123); // not changed
    setBiomeColourB(biomeColours, basalt_deltas, 104, 95, 112);  // not changed

    setBiomeColourChild(biomeColours, sunflower_plains, plains);
    setBiomeColourChild(biomeColours, desert_lakes, desert);
    setBiomeColourChild(biomeColours, gravelly_mountains, mountains);
    setBiomeColourChild(biomeColours, flower_forest, forest);
    setBiomeColourChild(biomeColours, taiga_mountains, taiga);
    setBiomeColourChild(biomeColours, swamp_hills, swamp);
    setBiomeColourChild(biomeColours, modified_jungle, jungle);
    setBiomeColourChild(biomeColours, modified_jungle_edge, jungle_edge);
    setBiomeColourChild(biomeColours, tall_birch_forest, birch_forest);
    setBiomeColourChild(biomeColours, tall_birch_hills, birch_forest_hills);
    setBiomeColourChild(biomeColours, dark_forest_hills, dark_forest);
    setBiomeColourChild(biomeColours, snowy_taiga_mountains, snowy_taiga);
    setBiomeColourChild(biomeColours, giant_spruce_taiga, giant_tree_taiga);
    setBiomeColourChild(biomeColours, giant_spruce_taiga_hills, giant_tree_taiga_hills);
    setBiomeColourChild(biomeColours, modified_gravelly_mountains, wooded_mountains);
    setBiomeColourChild(biomeColours, shattered_savanna, savanna);
    setBiomeColourChild(biomeColours, shattered_savanna_plateau, savanna_plateau);
    setBiomeColourChild(biomeColours, eroded_badlands, badlands);
    setBiomeColourChild(biomeColours, modified_wooded_badlands_plateau, wooded_badlands_plateau);
    setBiomeColourChild(biomeColours, modified_badlands_plateau, badlands_plateau);




}

void initBiomeTypeColours(unsigned char biomeColours[256][3])
{
    memset(biomeColours, 0, 256*3);

    setBiomeColour(biomeColours, Oceanic,  0x00, 0x00, 0xa0);
    setBiomeColour(biomeColours, Warm,     0xff, 0xc0, 0x00);
    setBiomeColour(biomeColours, Lush,     0x00, 0xa0, 0x00);
    setBiomeColour(biomeColours, Cold,     0x60, 0x60, 0x60);
    setBiomeColour(biomeColours, Freezing, 0xff, 0xff, 0xff);
}


int biomesToImage(unsigned char *pixels,
        const unsigned char biomeColours[256][3], const int *biomes,
        const unsigned int sx, const unsigned int sy,
        const unsigned int pixscale, const int flip)
{
    unsigned int i, j;
    int containsInvalidBiomes = 0;

    for (j = 0; j < sy; j++)
    {
        for (i = 0; i < sx; i++)
        {
            int id = biomes[j*sx+i];
            unsigned int r, g, b;

            if (id < 0 || id >= 256)
            {
                // This may happen for some intermediate layers
                containsInvalidBiomes = 1;
                r = biomeColours[id&0x7f][0]-40; r = (r>0xff) ? 0x00 : r&0xff;
                g = biomeColours[id&0x7f][1]-40; g = (g>0xff) ? 0x00 : g&0xff;
                b = biomeColours[id&0x7f][2]-40; b = (b>0xff) ? 0x00 : b&0xff;
            }
            else
            {
                r = biomeColours[id][0];
                g = biomeColours[id][1];
                b = biomeColours[id][2];
            }

            unsigned int m, n;
            for (m = 0; m < pixscale; m++) {
                for (n = 0; n < pixscale; n++) {
                    int idx = pixscale * i + n;
                    if (flip)
                        idx += (sx * pixscale) * ((pixscale * j) + m);
                    else
                        idx += (sx * pixscale) * ((pixscale * (sy-1-j)) + m);

                    unsigned char *pix = pixels + 3*idx;
                    pix[0] = (unsigned char)r;
                    pix[1] = (unsigned char)g;
                    pix[2] = (unsigned char)b;
                }
            }
        }
    }

    return containsInvalidBiomes;
}

int savePPM(const char *path, const unsigned char *pixels, const unsigned int sx, const unsigned int sy)
{
    FILE *fp = fopen(path, "wb");
    if (!fp)
        return -1;
    fprintf(fp, "P6\n%d %d\n255\n", sx, sy);
    int written = fwrite(pixels, sx*sy, 3, fp);
    fclose(fp);
    return (unsigned int)written != 3*sx*sy;
}

