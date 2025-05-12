#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#define MUSIC_FILE                                                 \
  "/mnt/c/projects/rew/C7_BrickGame_v1.0-1/src/brick_game/tetris/" \
  "tetris-8-bit.mp3"

void *play_music() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return NULL;
    }

    // Open audio device with a larger buffer size
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        printf("Error opening audio: %s\n", Mix_GetError());
        SDL_Quit();
        return NULL;
    }

    // Load music
    Mix_Music* bgMusic = Mix_LoadMUS(MUSIC_FILE);
    if (!bgMusic) {
        printf("Error loading music: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return NULL;
    }

    // Play music
    Mix_PlayMusic(bgMusic, -1);

    // Event loop
    SDL_Event event;
    while (1) {
        // Wait for an event
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            break; // Exit loop on quit or escape key
        }
        SDL_Delay(1000);
    }

    // Cleanup resources
    Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
    SDL_Quit();

    return NULL; // Return NULL at the end of the function
}