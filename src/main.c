#define SDL_MAIN_HANDLED
#include "shared.h"

#include <windows.h>

typedef void (*function_proc)(struct Game *game);

const int window_width = 540;
const int window_height = 960;

int main(int argc, char **argv) {
    struct Game g = {0};

    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    g.window = SDL_CreateWindow("RoadWizard App Demo",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                window_width,
                                window_height,
                                0);
    g.renderer = SDL_CreateRenderer(g.window, -1, 0);
    
    g.camera = (SDL_Rect){
        0, 0,
        window_width, window_height
    };
    
    CopyFileA("library.dll", "library_temp.dll", false);
    HMODULE dll = LoadLibraryA("library_temp.dll");
    if (!dll) return 1;
    function_proc fun = (function_proc)GetProcAddress(dll, "RunGame");
    
    int running = 1;
    while (running) {
        SDL_Event event;
        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        g.timer++;
        if (g.timer > 60) {
            FreeLibrary(dll);
            CopyFileA("library.dll", "library_temp.dll", false);
            dll = LoadLibraryA("library_temp.dll");
            if (!dll) return 1;
            fun = (function_proc)GetProcAddress(dll, "RunGame");
            g.timer = 0;
        }

        fun(&g);

        g.initialized = 1;
    }

    SDL_DestroyWindow(g.window);
    SDL_DestroyRenderer(g.renderer);

    return 0;
}
