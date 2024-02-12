#include <SDL2/SDL.h>
#include <stdbool.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Rect rect = {.x = 10, .y = 10, .w = 150, .h = 100};

void re_draw() {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);
  SDL_RenderFillRect(renderer, &rect);
  SDL_RenderPresent(renderer);
}

uint32_t ticksForNextKeyDown = 0;

bool handle_events() {
  SDL_Event event;
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    return false;
  }
  if (event.type == SDL_KEYDOWN) {
    uint32_t ticksNow = SDL_GetTicks();
    if (SDL_TICKS_PASSED(ticksNow, ticksForNextKeyDown)) {
      ticksForNextKeyDown = ticksNow + 10;
      switch (event.key.keysym.sym) {
      case SDLK_UP:
        rect.y -= 1;
        break;
      case SDLK_DOWN:
        rect.y += 1;
        break;
      case SDLK_RIGHT:
        rect.x += 1;
        break;
      case SDLK_LEFT:
        rect.x -= 1;
        break;
      }
      re_draw();
    }
  }
  return true;
}

void handle_events_lamd() { handle_events(); }

void main_loop() {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(handle_events_lamd, 0, 1);
#else
  while (handle_events())
    ;
#endif
}

int main() {

  SDL_Init(SDL_INIT_VIDEO);

  SDL_CreateWindowAndRenderer(300, 300, 0, &window, &renderer);
  re_draw();
  main_loop();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
