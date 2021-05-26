#ifndef DRAWING_H
#define DRAWING_H

#include <SDL/SDL.h>

#include <math.h>
#include <iostream>

// Warning: this header is very raw

namespace Geometry {
// fast, but low quality
void DrawCircle( SDL_Renderer* r, int center_x, int center_y, int radius) {
    int new_x = 0;
    int new_y = 0;
    int old_x =  center_x + 200;
    int old_y = center_y;
    float step = (M_PI * 2) / 50;

    SDL_SetRenderDrawColor ( r, 255, 255, 255, 255 );

    for ( float theta = 0; theta <= (M_PI * 2); theta += step ) {
        new_x = center_x + (radius * cos ( theta ));
        new_y = center_y - (radius * sin ( theta ));

        SDL_RenderDrawLine ( r, old_x, old_y, new_x, new_y );

        old_x = new_x;
        old_y = new_y;
    }

    new_x =  center_x + (radius * cos ( 0 ));
    new_y =  center_y - (radius * sin ( 0 ));
    SDL_RenderDrawLine( r, old_x, old_y, new_x, new_y );

}

// filled (laggy)
void draw_circle(SDL_Renderer* renderer, SDL_Point center, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }
}

// Pretty fast (midpoint)
void drawcircle(SDL_Renderer* renderer, int x0, int y0, int radius) {
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_Point points[] = {
            {x0 - y, y0 + x},
            {x0 + y, y0 + x},
            {x0 - x, y0 + y},
            {x0 + x, y0 + y},
            {x0 - x, y0 - y},
            {x0 + x, y0 - y},
            {x0 - y, y0 - x},
            {x0 + y, y0 - x},
        };

        SDL_RenderDrawPoints(renderer, points, 8); // hollow
        //SDL_RenderDrawLines(renderer, points, 8); // filled

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}
}

#endif // DRAWING_H
