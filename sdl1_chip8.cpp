#include "sdl1_chip8.h"
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "font.cpp"

#define WIDTH 640
#define HEIGHT 480

void sdl1_chip8::check_keypress(SDL_Event *event) {
  for (uint16_t i = 0; i < sizeof(sdl_keys); i++) {
    if (event->key.keysym.sym == sdl_keys[i]) {
      key_press(i);
    }
  }
}

void sdl1_chip8::check_keyrelease(SDL_Event *event) {
  for (uint16_t i = 0; i < sizeof(sdl_keys); i++) {
    if (event->key.keysym.sym == sdl_keys[i]) {
      key_release(i);
    }
  }
}

void sdl1_chip8::show_display() {
  // show display
  SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
  SDL_Rect dstrect;
  dstrect.h = 9;
  dstrect.w = 9;
  for (int yy = 0; yy < 32; yy++) {
    for (int xx = 0; xx < 64; xx++) {
      if (display[xx + (yy * 64)]) {
        dstrect.x = xx * 10;
        dstrect.y = yy * 10;
        SDL_FillRect(screen, &dstrect, SDL_MapRGB(screen->format, 255, 128, 0));
      }
    }
  }
  SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void sdl1_chip8::init_screen() {
  // init SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("SDL_Init: %s\n", SDL_GetError());
    exit(-1);
  }

  // set a video mode
  if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, 0, 0))) {
    printf("SDL_SetVideoMode: %s\n", SDL_GetError());
    SDL_Quit();
    exit(-2);
  }

  // init joystick
  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);

  // init ttf
  if (TTF_Init() == -1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    SDL_Quit();
    exit(-3);
  }

  // load font.ttf for title
  font = TTF_OpenFontRW(SDL_RWFromConstMem(font_array, font_array_length), 1,
                        HEIGHT / 10);
  if (!font) {
    printf("TTF_OpenFontRW: %s\n", TTF_GetError());
    TTF_Quit();
    SDL_Quit();
    exit(-4);
  }

  // create title
  title = TTF_RenderText_Blended(font, "CHIP8 EMULATOR", {255, 255, 255});
  TTF_CloseFont(font);
  if (!title) {
    TTF_Quit();
    SDL_Quit();
    exit(-5);
  }

  // load font.ttf for texts
  font = TTF_OpenFontRW(SDL_RWFromConstMem(font_array, font_array_length), 1,
                        HEIGHT / 21);
  if (!font) {
    printf("TTF_OpenFontRW: %s\n", TTF_GetError());
    TTF_Quit();
    SDL_Quit();
    exit(-6);
  }

  // read directory
  scan_directory();
  //
  choose_rom();
}

void sdl1_chip8::choose_rom() {
  // set variables
  stage = STAGE_MENU;
  menu_item = 0;
  // clear screen
  SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
}

void sdl1_chip8::end_screen() {
  // free menu itens
  for (auto i : files) {
    SDL_FreeSurface(i.item);
    SDL_FreeSurface(i.item2);
  }
  SDL_FreeSurface(title);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_JoystickClose(joystick);
  SDL_Quit();
}

bool sdl1_chip8::handle_input() {
  bool quit = false;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      // WINDOW CONTROL
      if (stage == STAGE_MENU) {
        // ESC on menu, exit
        quit = true;
      } else {
        // ESC on game, menu
        choose_rom();
      }
      break;
    case SDL_KEYDOWN:
      // KEYBOARD
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        if (stage == STAGE_MENU) {
          // ESC on menu, exit
          quit = true;
        } else {
          // ESC on game, menu
          choose_rom();
        }
      }
      if (stage == STAGE_MENU) {
        // check keys
        if (event.key.keysym.sym == SDLK_RETURN) {
          // choose ROM
          stage = STAGE_RUNNING;
          load((char *)files.at(menu_item).name.c_str());
        } else if (event.key.keysym.sym == SDLK_UP) {
          // move up
          menu_item--;
        } else if (event.key.keysym.sym == SDLK_DOWN) {
          // move down
          menu_item++;
        }
      } else {
        // chip8 keyboard handler
        check_keypress(&event);
      }
      break;
    case SDL_KEYUP:
      if (stage == STAGE_RUNNING) {
        // chip8 keyboard handler
        check_keyrelease(&event);
      }
      break;
    case SDL_JOYBUTTONDOWN:
      // JOYSTICK BUTTON
      if (event.jbutton.button == 6 /* BACK */) {
        if (stage == STAGE_MENU) {
          // ESC on menu, exit
          quit = true;
        } else {
          // ESC on game, menu
          choose_rom();
        }
      } else if (stage == STAGE_MENU) {
        if (event.jbutton.button == 0 /* X */) {
          // choose ROM
          stage = STAGE_RUNNING;
          load((char *)files.at(menu_item).name.c_str());
        }
      } else {
        //!!!ingame
      }
      //!!!printf("%d\n", event.jbutton.button);
      break;
    case SDL_JOYHATMOTION:
      // JOYSTICK DPAD
      if (stage == STAGE_MENU) {
        if (event.jhat.value & SDL_HAT_UP) {
          menu_item--;
        }
        if (event.jhat.value & SDL_HAT_DOWN) {
          menu_item++;
        }
      } else {
        //!!!ingame
      }
      break;
    default:
      break;
    }
  }
  menu_item = (menu_item < 0)                       ? 0
              : (menu_item > (int)files.size() - 1) ? files.size() - 1
                                                    : menu_item;
  return quit;
}

uint32_t sdl1_chip8::get_ticks() { return SDL_GetTicks(); }

void sdl1_chip8::delay(uint16_t x) { SDL_Delay(x); }

void sdl1_chip8::scan_directory() {
  //
  MENU_ITEM entry;
  for (int i = 0; i < 28; i++) {
    char tmp[128];
    //!!!read files
    snprintf(tmp, sizeof(tmp), "test_%d.ch8", i);
    entry.name = tmp;
    entry.item = TTF_RenderText_Blended(font, tmp, {128, 128, 128});
    entry.item2 = TTF_RenderText_Blended(font, tmp, {255, 128, 128});
    files.push_back(entry);
  }
}

void sdl1_chip8::show_menu() {
  SDL_Rect dst;
  // title
  dst.x = (WIDTH - title->w) / 2;
  dst.y = 0;
  dst.w = title->w;
  dst.h = title->h;
  SDL_BlitSurface(title, NULL, screen, &dst);
  //!!!pages on menu
  dst.y = title->h;
  int yy = dst.y;
  int count = 0;
  for (auto i : files) {
    dst.x = ((WIDTH / 2) - i.item->w) / 2;
    // do 2nd column
    if (count > 17) {
      dst.x += (WIDTH / 2);
      if (yy != 0) {
        dst.y = yy;
        yy = 0;
      }
    }
    SDL_BlitSurface((menu_item == count) ? i.item2 : i.item, NULL, screen,
                    &dst);
    dst.y += i.item->h;
    count++;
  }
  SDL_UpdateRect(screen, 0, 0, 0, 0);
}
