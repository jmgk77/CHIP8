#include "sdl1_chip8.h"
#include <dirent.h>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "font.cpp"
#include "font2.cpp"

#define WIDTH 640
#define HEIGHT 480
#define MENU_ITENS_PER_PAGE 17
#define ROM_DIRECTORY "."

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
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) == -1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    exit(-1);
  }

  // set a video mode
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 0, 0);
  if (!screen) {
    printf("SDL_SetVideoMode: %s\n", SDL_GetError());
    exit(-2);
  }

  // init joystick
  SDL_JoystickEventState(SDL_ENABLE);
  joystick = SDL_JoystickOpen(0);
  if (joystick == NULL) {
    printf("Couldn't open Joystick 0\n");
    exit(-3);
  }

  // init ttf
  if (TTF_Init() == -1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(-4);
  }

  // load font.ttf for title
  font = TTF_OpenFontRW(SDL_RWFromConstMem(font_array, font_array_length), 1,
                        HEIGHT / 10);
  if (!font) {
    printf("TTF_OpenFontRW: %s\n", TTF_GetError());
    exit(-5);
  }

  // create title
  title = TTF_RenderText_Blended(font, "CHIP8 EMULATOR", {255, 255, 255});
  back_arrow = TTF_RenderText_Blended(font, "<<", {192, 192, 192});
  next_arrow = TTF_RenderText_Blended(font, ">>", {192, 192, 192});
  if (!title || !back_arrow || !next_arrow) {
    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
    exit(-6);
  }
  TTF_CloseFont(font);

  // reload font.ttf for texts
  font = TTF_OpenFontRW(SDL_RWFromConstMem(font2_array, font2_array_length), 1,
                        HEIGHT / 21);
  if (!font) {
    printf("TTF_OpenFontRW: %s\n", TTF_GetError());
    exit(-7);
  }

  // read directory
  scan_directory();
  choose_rom();
}

void sdl1_chip8::end_screen() {
  // free menu itens
  for (auto i : files) {
    SDL_FreeSurface(i.item);
    SDL_FreeSurface(i.item2);
  }
  SDL_FreeSurface(next_arrow);
  SDL_FreeSurface(back_arrow);
  SDL_FreeSurface(title);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_JoystickClose(joystick);
  SDL_Quit();
}

void sdl1_chip8::choose_rom() {
  // set variables
  stage = STAGE_MENU;
  menu_item = 0;
  // clear screen
  SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
}

bool sdl1_chip8::handle_input() {
  bool quit = false;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      // WINDOW CONTROL
      if (stage == STAGE_MENU) {
        // CLOSE on menu, exit
        quit = true;
      } else {
        // CLOSE on game, menu
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
          // ESC on game, goes to menu
          choose_rom();
        }
      }
      if (stage == STAGE_MENU) {
        // check keys
        if (event.key.keysym.sym == SDLK_RETURN) {
          // ENTER choose ROM
          stage = STAGE_RUNNING;
          init();
          load((char *)files.at(menu_item).name.c_str());
        } else if (event.key.keysym.sym == SDLK_UP) {
          // ARROW move up
          menu_item--;
        } else if (event.key.keysym.sym == SDLK_DOWN) {
          // ARROW move down
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
      if (event.jbutton.button == BUTTON_BACK /* BACK */) {
        if (stage == STAGE_MENU) {
          // SELECT on menu, exit
          quit = true;
        } else {
          // SELECT on game, menu
          choose_rom();
        }
      } else if (stage == STAGE_MENU) {
        if (event.jbutton.button == BUTTON_A /* X */) {
          // X load ROM
          stage = STAGE_RUNNING;
          init();
          load((char *)files.at(menu_item).name.c_str());
        }
      } else {
        //!!!ingame buttons
      }
      printf("%d\n", event.jbutton.button);
      break;
    case SDL_JOYHATMOTION:
      // JOYSTICK DPAD
      if (stage == STAGE_MENU) {
        if (event.jhat.value & SDL_HAT_UP) {
          // DPAD UP
          menu_item--;
        }
        if (event.jhat.value & SDL_HAT_DOWN) {
          // DPAD DOWN
          menu_item++;
        }
      } else {
        //!!!ingame buttons
      }
      break;
    default:
      break;
    }
  }
  // keep menu_item on limits
  menu_item = (menu_item < 0)                       ? 0
              : (menu_item > (int)files.size() - 1) ? files.size() - 1
                                                    : menu_item;
  return quit;
}

uint32_t sdl1_chip8::get_ticks() { return SDL_GetTicks(); }

void sdl1_chip8::delay(uint16_t x) { SDL_Delay(x); }

void sdl1_chip8::scan_directory() {
  MENU_ITEM entry;
  // read files
  struct dirent *en;
  if (DIR *dr = opendir(ROM_DIRECTORY)) {
    while ((en = readdir(dr)) != NULL) {
      // skip files that start with dot
      if (en->d_name[0] == '.') {
        continue;
      }
      char tmp[256];
      snprintf(tmp, sizeof(tmp), "%s", en->d_name);
      entry.name = tmp;
      entry.item = TTF_RenderText_Blended(font, tmp, {128, 128, 128});
      entry.item2 = TTF_RenderText_Blended(font, tmp, {255, 128, 128});
      files.push_back(entry);
    }
    closedir(dr);
  }
}

void sdl1_chip8::show_menu() {
  SDL_Rect dst;
  // clear screen
  SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
  // title
  dst.x = (WIDTH - title->w) / 2;
  dst.y = 0;
  dst.w = title->w;
  dst.h = title->h;
  SDL_BlitSurface(title, NULL, screen, &dst);
  dst.y = title->h + 20;
  // calc start and end item on current page of menu
  int menu_pos = (menu_item / MENU_ITENS_PER_PAGE) * MENU_ITENS_PER_PAGE;
  int menu_end = menu_pos + (MENU_ITENS_PER_PAGE);
  // we need arrows?
  bool back = (menu_pos == 0) ? false : true;
  bool next = false;
  if (menu_end < (int)files.size()) {
    next = true;
  } else {
    menu_end = files.size();
  };
  // draw itens
  for (; menu_pos < menu_end; menu_pos++) {
    auto i = files[menu_pos];
    dst.x = (WIDTH - i.item->w) / 2;
    SDL_BlitSurface((menu_item == menu_pos) ? i.item2 : i.item, NULL, screen,
                    &dst);
    dst.y += i.item->h;
  }
  // draw arrows
  if (back) {
    dst.x = 0;
    dst.y = title->h + 20;
    dst.w = back_arrow->w;
    dst.h = back_arrow->h;
    SDL_BlitSurface(back_arrow, NULL, screen, &dst);
  }
  if (next) {
    dst.x = WIDTH - next_arrow->w;
    dst.y = HEIGHT - next_arrow->h;
    dst.w = next_arrow->w;
    dst.h = next_arrow->h;
    SDL_BlitSurface(next_arrow, NULL, screen, &dst);
  }
  // render
  SDL_UpdateRect(screen, 0, 0, 0, 0);
}
