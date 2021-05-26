#ifndef PROFILING_H
#define PROFILING_H

#include <iostream>
#include <sstream>

namespace Debug {
	inline void quickAndDirtyFpsCounter() {
		static int frames = 0;
		static unsigned int prevTime = 0;
		frames++;
		if(SDL_GetTicks() > prevTime + 999) {
			printf("FPS: %i\n", frames);
			frames = 0;
			prevTime = SDL_GetTicks();
		}
	}

	inline void quickAndDirtyFpsCounter(SDL_Window* window) {
        static std::string originalTitle = SDL_GetWindowTitle(window);
		static int frames = 0;
		static unsigned int prevTime = 0;
		frames++;
		if(SDL_GetTicks() > prevTime + 999) {
		    std::stringstream s;
            s << originalTitle << " / FPS: " << frames;
            SDL_SetWindowTitle(window, s.str().c_str());
			frames = 0;
			prevTime = SDL_GetTicks();
		}
	}
}

#endif // PROFILING_H
