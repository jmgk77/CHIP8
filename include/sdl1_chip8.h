#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <vector>

#include "chip8.h"
#include "defs.h"

class sdl1_chip8 : public chip8 {
#ifdef PS3
public:
#else
private:
#endif
  // keyboard
  uint8_t sdl_keys[16] = {SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w,
                          SDLK_e, SDLK_a, SDLK_s, SDLK_d, SDLK_z, SDLK_c,
                          SDLK_4, SDLK_r, SDLK_f, SDLK_v};
  // joystick
#ifndef PS3
  uint8_t sdl_buttons[8] = {BUTTON_A,  BUTTON_B,  BUTTON_X,  BUTTON_Y,
                            BUTTON_L1, BUTTON_R1, BUTTON_L3, BUTTON_R3};
  uint8_t buttons2chip8[8] = {5, 0xB, 0xA, 0, 0xC, 0xD, 0xE, 0xF};
  uint8_t sdl_hats[8] = {SDL_HAT_UP,        SDL_HAT_RIGHTUP, SDL_HAT_RIGHT,
                         SDL_HAT_RIGHTDOWN, SDL_HAT_DOWN,    SDL_HAT_LEFTDOWN,
                         SDL_HAT_LEFT,      SDL_HAT_LEFTUP};
  uint8_t hat2chip8[8] = {2, 3, 6, 9, 8, 7, 4, 1};
#else
  // PS3 map dpad as buttons
  uint8_t sdl_buttons[15] = {
      BUTTON_A,     BUTTON_B,  BUTTON_X,  BUTTON_Y,    BUTTON_L1,
      BUTTON_R1,    BUTTON_L3, BUTTON_R3, BUTTON_LEFT, BUTTON_DOWN,
      BUTTON_RIGHT, BUTTON_UP, BUTTON_L2, BUTTON_R2,   BUTTON_START, /* NO F */
  };
  uint8_t buttons2chip8[15] = {
      5, 0xB, 0xA, 0, 1, 3, 0xC, 0xD, 4, 8, 6, 2, 7, 9, 0xE, /* NO F */
  };
#endif
private:
  void check_keypress(SDL_Event *event);
  void check_joystick(SDL_Event *event);

  SDL_Surface *screen;
  SDL_Event event;
  SDL_Joystick *joystick;
  TTF_Font *font;

  // menu
  struct MENU_ITEM {
    std::string name;
    SDL_Surface *item;
    SDL_Surface *item2;
  };
  SDL_Surface *title;
  SDL_Surface *back_arrow;
  SDL_Surface *next_arrow;
  std::vector<struct MENU_ITEM> files;
  int menu_item;
  void choose_rom();
  void scan_directory();

public:
  void show_display();
  void init_screen();
  void end_screen();
  bool handle_input();
  uint32_t get_ticks();
  void delay(uint16_t x);

  // menu
  enum { STAGE_MENU, STAGE_RUNNING };
  int stage;
  void show_menu();
};
