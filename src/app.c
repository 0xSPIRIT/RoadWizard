#include "shared.h"
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "assets.c"

__declspec(dllexport) void RunGame(struct Game *g) {
    if (!g->initialized) {
        TexturesInit(g->window, g->renderer, &g->textures);
        g->map_w = g->textures.map_background->w;
        g->map_h = g->textures.map_background->h;

        g->camera.x = 200;
        g->camera.y = 200;

        g->icon_count = 3;

        g->icons[0].texture = g->textures.icon_pothole;
        g->icons[1].texture = g->textures.icon_bad_road;
        g->icons[2].texture = g->textures.icon_flood;

        g->current_menu_icon = -1;

        g->map_icon_count = 0;
    }

    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    const Uint32 mouse = SDL_GetMouseState(&g->mx, &g->my);

    if (mouse & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (g->issue_popup && g->current_menu_icon != -1 && is_point_in_rect(g->mx, g->my, g->issue_popup_x, g->issue_popup_y, g->textures.new_issue_popup->w, g->textures.new_issue_popup->h)) {
            struct Texture *texture = NULL;

            switch (g->current_menu_icon) {
            case 0:
                texture = g->textures.pothole;
                break;
            case 1:
                texture = g->textures.bad_road;
                break;
            case 2:
                texture = g->textures.flood;
                break;
            }

            g->map_icons[g->map_icon_count++] = (struct Icon){
                texture,
                g->issue_location_x, g->issue_location_y,
            };

            g->issue_popup = false;
        } else {
            g->camera.x -= g->mx - g->pmx;
            g->camera.y -= g->my - g->pmy;
        }
    }

    if (mouse & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        if (!g->was_right_pressed) {
            if (keys[SDL_SCANCODE_LCTRL]) {
                g->car_x = g->camera.x + g->mx;
                g->car_y = g->camera.y + g->my;
            } else {
                g->issue_popup = !g->issue_popup;
                g->issue_popup_x = g->camera.w/2 - g->textures.new_issue_popup->w/2;
                g->issue_popup_y = g->camera.h/2 - g->textures.new_issue_popup->h/2;

                g->issue_location_x = g->camera.x + g->mx;
                g->issue_location_y = g->camera.y + g->my;
            }
        }
        g->was_right_pressed = true;
    } else {
        g->was_right_pressed = false;
    }

    if (g->camera.x < 0) g->camera.x = 0;
    if (g->camera.y < 0) g->camera.y = 0;
    if (g->camera.x+g->camera.w > g->map_w) g->camera.x = g->map_w-g->camera.w;
    if (g->camera.y+g->camera.h > g->map_h) g->camera.y = g->map_h-g->camera.h;

    SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
    SDL_RenderClear(g->renderer);

    SDL_RenderCopy(g->renderer, g->textures.map_background->tex, &g->camera, NULL);

    // You are here icon
    {
        float scale = 0.1;

        int w = g->textures.icon_you_are_here->w;
        int h = g->textures.icon_you_are_here->h;

        SDL_Rect dst = (SDL_Rect){
            g->car_x - g->camera.x - 0.5*(w*scale),
            g->car_y - g->camera.y - h*scale,
            w * scale, h * scale
        };
        SDL_RenderCopy(g->renderer, g->textures.icon_you_are_here->tex, NULL, &dst);
    }

    // Render the map icons
    for (int i = 0; i < g->map_icon_count; i++) {
        struct Icon icon = g->map_icons[i];

        SDL_Rect dst = (SDL_Rect){
            icon.x - g->camera.x - icon.texture->w/2,
            icon.y - g->camera.y - icon.texture->h,
            icon.texture->w,
            icon.texture->h
        };
        SDL_RenderCopy(g->renderer, icon.texture->tex, NULL, &dst);
    }

    // Render the top bar
    {
        SDL_Rect dst = (SDL_Rect){
            0, 0,
            g->textures.top_bar->w, g->textures.top_bar->h
        };
        SDL_RenderCopy(g->renderer, g->textures.top_bar->tex, NULL, &dst);
    }

    // Issue popup
    if (g->issue_popup) {
        SDL_Rect dst = (SDL_Rect){
            g->issue_popup_x,
            g->issue_popup_y,
            g->textures.new_issue_popup->w, g->textures.new_issue_popup->h
        };
        SDL_RenderCopy(g->renderer, g->textures.new_issue_popup->tex, NULL, &dst);

        g->current_menu_icon = -1;

        // Icons
        for (int i = 0; i < g->icon_count; i++) {
            // Icon pothole
            SDL_Rect rect = (SDL_Rect){
                g->issue_popup_x + 40,
                g->issue_popup_y + 80 + 80*i,
                g->icons[i].texture->w, g->icons[i].texture->h
            };

            bool is_point_in_icon = is_point_in_rect(g->mx, g->my, rect.x, rect.y, rect.w, rect.h);
            if (is_point_in_icon) {
                g->current_menu_icon = i;
                SDL_SetTextureColorMod(g->icons[i].texture->tex, 200, 200, 200);
            } else {
                SDL_SetTextureColorMod(g->icons[i].texture->tex, 255, 255, 255);
            }
        
            SDL_RenderCopy(g->renderer, g->icons[i].texture->tex, NULL, &rect);
        }
    }

    SDL_RenderPresent(g->renderer);

    g->pmx = g->mx;
    g->pmy = g->my;
}
