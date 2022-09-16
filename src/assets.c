#include "shared.h"

#define LoadTexture(fp) (_LoadTexture(window, renderer, fp))

static inline struct Texture *_LoadTexture(SDL_Window *window, SDL_Renderer *renderer, const char *fp) {
    struct Texture *texture = calloc(1, sizeof(struct Texture));
    
    SDL_Surface *surf = IMG_Load(fp);
    Assert(window, surf);
    texture->tex = SDL_CreateTextureFromSurface(renderer, surf);
    Assert(window, texture->tex);

    texture->w = surf->w;
    texture->h = surf->h;

    SDL_FreeSurface(surf);

    SDL_Log("Loaded %s!\n", fp);

    return texture;
}

static void TexturesInit(SDL_Window *window, SDL_Renderer *renderer, struct Textures *textures) {
    // Yes I know I should be using a tilemap for this but it doesn't matter.
    // Computers are fast these days and this is a demo project!

    textures->map_background = LoadTexture("../res/map_background.png");
    textures->icon_you_are_here = LoadTexture("../res/you_are_here.png");
    textures->top_bar = LoadTexture("../res/top_bar.png");
    textures->new_issue_popup = LoadTexture("../res/popup.png");

    textures->icon_pothole = LoadTexture("../res/icon_pothole.png");
    textures->icon_bad_road = LoadTexture("../res/icon_bad_road.png");
    textures->icon_flood = LoadTexture("../res/icon_flood.png");

    textures->pothole = LoadTexture("../res/pothole.png");
    textures->bad_road = LoadTexture("../res/bad_road.png");
    textures->flood = LoadTexture("../res/flood.png");
}

static void TexturesDeInit(struct Textures *textures) {
    struct Texture **texs = (struct Texture**) textures;
    size_t tex_count = sizeof(struct Textures)/sizeof(SDL_Texture*);

    for (size_t i = 0; i < tex_count; i++) {
        SDL_DestroyTexture(texs[i]->tex);
        free(texs[i]);
    }
}
