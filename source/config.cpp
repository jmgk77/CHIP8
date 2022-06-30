#include "defs.h"
#include "sdl1_chip8.h"
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern sdl1_chip8 c8;

//
struct entry {
  const char *str;
  int n;
};

/* sorted according to str */
struct entry dict_button[] = {
    "BUTTON_A",   BUTTON_A,      "BUTTON_B",     BUTTON_B,     "BUTTON_BACK",
    BUTTON_BACK,  "BUTTON_DOWN", BUTTON_DOWN,    "BUTTON_L1",  BUTTON_L1,
    "BUTTON_L2",  BUTTON_L2,     "BUTTON_L3",    BUTTON_L3,    "BUTTON_LEFT",
    BUTTON_LEFT,  "BUTTON_R1",   BUTTON_R1,      "BUTTON_R2",  BUTTON_R2,
    "BUTTON_R3",  BUTTON_R3,     "BUTTON_RIGHT", BUTTON_RIGHT, "BUTTON_START",
    BUTTON_START, "BUTTON_UP",   BUTTON_UP,      "BUTTON_X",   BUTTON_X,
    "BUTTON_Y",   BUTTON_Y,
};
#ifndef PS3
//
struct entry dict_hat[] = {
    "BUTTON_DOWN",     SDL_HAT_DOWN,     "BUTTON_LEFT",      SDL_HAT_LEFT,
    "BUTTON_LEFTDOWN", SDL_HAT_LEFTDOWN, "BUTTON_LEFTUP",    SDL_HAT_LEFTUP,
    "BUTTON_RIGHT",    SDL_HAT_RIGHT,    "BUTTON_RIGHTDOWN", SDL_HAT_RIGHTDOWN,
    "BUTTON_RIGHTUP",  SDL_HAT_RIGHTUP,  "BUTTON_UP",        SDL_HAT_UP,
};
#endif

int compare(const void *s1, const void *s2) {
  const struct entry *e1 = (const entry *)s1;
  const struct entry *e2 = (const entry *)s2;
  return strcmp(e1->str, e2->str);
}

void read_config() {
  FILE *fp;

  fp = fopen(ROM_DIRECTORY "/chip8.ini", "r");
  if (fp != NULL) {
    int i = 0;
    int j = 0;

    char line[128];
    while (fgets(line, sizeof(line), fp) != NULL) {
      char command[256];
      int code;
      sscanf(line, "%s %d", command, &code);

      // if (code != -2) {
      struct entry *result;
      struct entry in;
      in.str = command;
      // check buttons
      result = (entry *)bsearch(&in, dict_button,
                                sizeof(dict_button) / sizeof(dict_button[0]),
                                sizeof dict_button[0], compare);
      if ((result) && (i <= (int)sizeof(c8.sdl_buttons)) && (result->n != -1)) {
        c8.sdl_buttons[i] = result->n;
        c8.buttons2chip8[i] = code;
        i++;
      }
#ifndef PS3
      // check hats
      result = (entry *)bsearch(&in, dict_hat,
                                sizeof(dict_hat) / sizeof(dict_hat[0]),
                                sizeof dict_hat[0], compare);
      if ((result) && (j <= (int)sizeof(c8.sdl_hats)) && (result->n != -1)) {
        c8.sdl_hats[j] = result->n;
        c8.hat2chip8[j] = code;
        j++;
      }
#endif
    }
    fclose(fp);
  }
}