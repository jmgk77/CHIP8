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
struct entry dict[] = {
    "BUTTON_A",    BUTTON_A,       "BUTTON_B",    BUTTON_B,
    "BUTTON_BACK", BUTTON_BACK,    "BUTTON_DOWN", BUTTON_DOWN,
    "BUTTON_L1",   BUTTON_L1,      "BUTTON_L2",   BUTTON_L2,
    "BUTTON_L3",   BUTTON_L3,      "BUTTON_LEFT", BUTTON_LEFT,
    "BUTTON_R1",   BUTTON_R1,      "BUTTON_R2",   BUTTON_R2 "BUTTON_R3",
    BUTTON_R3,     "BUTTON_RIGHT", BUTTON_RIGHT,  "BUTTON_START",
    BUTTON_START,  "BUTTON_UP",    BUTTON_UP,     "BUTTON_X",
    BUTTON_X,      "BUTTON_Y",     BUTTON_Y,
};
//

int compare(const void *s1, const void *s2) {
  const struct entry *e1 = (const entry *)s1;
  const struct entry *e2 = (const entry *)s2;
  return strcmp(e1->str, e2->str);
}

void read_config() {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(ROM_DIRECTORY "/chip8.ini", "r");
  if (fp != NULL) {
    int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
      if (i > sizeof(c8.sdl_buttons)) {
        continue;
      }
      char command[256];
      int code;
      sscanf(line, "%s %d", command, &code);

      struct entry *result;
      struct entry in;
      in.str = command;
      result = (entry *)bsearch(&in, dict, sizeof(dict) / sizeof(dict[0]),
                                sizeof dict[0], compare);
      if (result) {
        c8.sdl_buttons[i] = result->n;
        c8.buttons2chip8[i] = code;
        i++;
      }
    }
    fclose(fp);
  }
}