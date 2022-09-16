#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_ICONS 2048

// Shows details of an icon.
struct Popup {
    int x, y;
    char name[256];
    char desc[256];
};

struct Texture {
    SDL_Texture *tex;
    int w, h;
};

struct Textures {
    struct Texture *map_background,
        *icon_you_are_here,
        *top_bar,
        *new_issue_popup,
        *icon_pothole,
        *icon_bad_road,
        *icon_flood,
        *pothole,
        *bad_road,
        *flood;
};

struct Icon {
    struct Texture *texture;
    int x, y;
};

struct Game {
    int initialized;
    long timer;

    int car_x, car_y;

    bool issue_popup;
    int issue_popup_x, issue_popup_y;

    // Icons in the menu
    struct Icon icons[3];
    unsigned icon_count;
    int current_menu_icon;

    // The actual map icons
    struct Icon map_icons[MAX_ICONS];
    unsigned map_icon_count;

    int issue_location_x, issue_location_y;

    bool was_right_pressed;

    struct Textures textures;

    SDL_Rect camera;

    int map_w, map_h;
    int mx, my, pmx, pmy;

    SDL_Window *window;
    SDL_Renderer *renderer;
};

void string_copy(char *dst, const char *src) {
    while (*dst) {
        *dst++ = *src++;
    }
}

#define Assert(window, condition) (_assert(condition, window, __func__, __FILE__, __LINE__))

inline void _assert(bool condition, SDL_Window *window, const char *func, const char *file, const int line) {
    if (condition) return;

    char message[64] = {0};
    char line_of_code[2048] = {0};

    FILE *f = fopen(file, "r");
    if (f) {
        int iterator = 0;
        char temp_buffer[2048] = {0};
        while (fgets(temp_buffer, 2048, f)) {
            iterator++;
            if (iterator == line) {
                string_copy(line_of_code, temp_buffer);
                break;
            }
        }

        sprintf(message, "%s :: at %s:%d\nLine:%s", func, file, line, line_of_code);
        if (window) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Assertion Failed!", message, window);
        }
    }

    fprintf(stderr, "\n:::: ASSERTION FAILED ::::\n%s", message);
    fflush(stdout);
}

bool is_point_in_rect(int x, int y, int x1, int y1, int w, int h) {
    return x >= x1 && x <= x1+w && y >= y1 && y <= y1+h;
}
