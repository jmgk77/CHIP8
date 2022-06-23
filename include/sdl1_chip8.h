#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <vector>

#include "chip8.h"

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_X 2
#define BUTTON_Y 3
#define BUTTON_L1 4
#define BUTTON_R1 5
#define BUTTON_BACK 6
#define BUTTON_START 7
#define BUTTON_MAIN 8
#define BUTTON_L3 9
#define BUTTON_R3 10

class sdl1_chip8 : public chip8 {
private:
  uint8_t sdl_keys[16] = {SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w,
                          SDLK_e, SDLK_a, SDLK_s, SDLK_d, SDLK_z, SDLK_c,
                          SDLK_4, SDLK_r, SDLK_f, SDLK_v};
  SDL_Surface *screen;
  SDL_Event event;

  void check_keypress(SDL_Event *event);
  void check_keyrelease(SDL_Event *event);

  struct MENU_ITEM {
    std::string name;
    SDL_Surface *item;
    SDL_Surface *item2;
  };

  void choose_rom();
  int menu_item;
  std::vector<struct MENU_ITEM> files;

  TTF_Font *font;

  SDL_Surface *title;

  SDL_Joystick *joystick;

public:
  void show_display();
  void init_screen();
  void end_screen();
  bool handle_input();
  uint32_t get_ticks();
  void delay(uint16_t x);

  enum { STAGE_MENU, STAGE_RUNNING };
  int stage;
  void scan_directory();
  void show_menu();
};
