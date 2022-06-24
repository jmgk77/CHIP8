#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <vector>

#include "chip8.h"

class sdl1_chip8 : public chip8 {
private:
  // keyboard
  uint8_t sdl_keys[16] = {SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w,
                          SDLK_e, SDLK_a, SDLK_s, SDLK_d, SDLK_z, SDLK_c,
                          SDLK_4, SDLK_r, SDLK_f, SDLK_v};
  void check_keypress(SDL_Event *event);
  void check_keyrelease(SDL_Event *event);

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
