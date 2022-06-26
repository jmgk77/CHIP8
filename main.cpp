// CHIP8 emulator for linux/PS2/PS3
// (c) JMGK 2022

#include <stdio.h>
#include <stdlib.h>

#define FRAMERATE 120

#include "defs.h"
#include "sdl1_chip8.h"

#ifdef PS2
#include "data/usbd.h"
#include "data/usbhdfsd.h"
#endif

int main(int argc, char *argv[]) {
#ifdef PS2
  SifInitRpc(0);

  while (!SifIopReset("", 0))
    ;
  while (!SifIopSync())
    ;
  ;
  // fioExit();
  SifExitIopHeap();
  SifLoadFileExit();
  SifExitRpc();
  SifExitCmd();

  SifInitRpc(0);
  FlushCache(0);
  FlushCache(2);

  SifLoadFileInit();
  SifInitIopHeap();

  sbv_patch_enable_lmb();
  sbv_patch_disable_prefix_check();

  // load usb IRXs
  int res = 0;
  res = SifLoadModule("rom0:SIO2MAN", 0, NULL);
  printf("SIO2MAN ret=%d\n", res);

  // load usb IRXs
  SifExecModuleBuffer((void *)usbd_array, usbd_array_length, 0, NULL, &res);
  printf("usbd_array ret=%d\n", res);
  SifExecModuleBuffer((void *)usbhdfsd_array, usbhdfsd_array_length, 0, NULL,
                      &res);
  printf("usbhdfsd_array ret=%d\n", res);
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
