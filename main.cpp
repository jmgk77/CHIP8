// CHIP8 emulator for linux/PS2/PS3
// (c) JMGK 2022

#include <stdio.h>
#include <stdlib.h>

#define FRAMERATE 120

#include "defs.h"
#include "sdl1_chip8.h"

int main(int argc, char *argv[]) {
#ifdef PS2
  // #ifndef DEBUG
  SifInitRpc(0);
  while (!SifIopReset("", 0)) {
  };
  while (!SifIopSync()) {
  };
  // #endif

  // Initialize SIF services
  SifInitRpc(0);
  SifLoadFileInit();
  SifInitIopHeap();
  sbv_patch_enable_lmb();
  sbv_patch_fileio();

  // change priority to make SDL audio thread run properly
  ChangeThreadPriority(GetThreadId(), 72);
#endif

  sdl1_chip8 c8;

  c8.init_screen();

  // fps cap
  unsigned int framerate, framerate_old;
  framerate_old = framerate = c8.get_ticks();

  // sdl loop
  bool quit = false;
  while (!quit) {
    //
    framerate = c8.get_ticks();

    // keep FPS
    if ((framerate - framerate_old) > 1000 / FRAMERATE) {
      framerate_old = framerate;
      quit = c8.handle_input();
      if (c8.stage == c8.STAGE_MENU) {
        // update menu
        c8.show_menu();
      } else {
        if (c8.loop()) {
          c8.show_display();
        }
      }
    } else {
      // Wait remaining time
      c8.delay((1000 / FRAMERATE) - (framerate - framerate_old));
    }
  }

  c8.end_screen();

  exit(0);
}
