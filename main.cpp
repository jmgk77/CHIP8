// CHIP8 emulator for linux/PS2/PS3
// (c) JMGK 2022

#include <stdio.h>
#include <stdlib.h>

#define FRAMERATE 120

#include "defs.h"
#include "sdl1_chip8.h"

#ifdef PS2
#include "data/fileXio.h"
#include "data/iomanX.h"
#include "data/sio2man.h"

#include "data/bdm.h"
#include "data/bdmfs_fatfs.h"
#include "data/bdmfs_vfat.h"
#include "data/usbd.h"
#include "data/usbhdfsd.h"
#include "data/usbmass_bd.h"
#endif

int main(int argc, char *argv[]) {
#ifdef PS2
  // init PS2
  /*   SifInitRpc(0);
    while (!SifIopReset(NULL, 0)) {
    };
    while (!SifIopSync()) {
    }; */
  SifInitRpc(0);
  sbv_patch_enable_lmb();
  sbv_patch_disable_prefix_check();
  sbv_patch_fileio();

  // load usb IRXs
  /*   int res = 0;
    res = SifLoadModule("rom0:SIO2MAN", 0, NULL);
    printf("!!!SIO2MAN RET=%d\n", res);
    SifExecModuleBuffer((void *)usbd_irx, usbd_irx_length, 0, NULL, &res);
    printf("!!!USBD_ARRAY RET=%d\n", res);
    SifExecModuleBuffer((void *)usbhdfsd_array, usbhdfsd_array_length, 0, NULL,
                        &res);
    printf("!!!USBHDFSD_ARRAY RET=%d\n", res); */

  // retroarch
  int r = 0;
  printf(
      "!!!iomanX_irx %d (%d)\n",
      SifExecModuleBuffer((void *)&iomanX_irx, iomanX_irx_length, 0, NULL, &r),
      r);
  printf("!!!fileXio_irx %d (%d)\n",
         SifExecModuleBuffer((void *)&fileXio_irx, fileXio_irx_length, 0, NULL,
                             &r),
         r);
  printf("!!!sio2man_irx %d (%d)\n",
         SifExecModuleBuffer((void *)&sio2man_irx, sio2man_irx_length, 0, NULL,
                             &r),
         r);
  printf("!!!usbd_irx %d (%d)\n",
         SifExecModuleBuffer((void *)&usbd_irx, usbd_irx_length, 0, NULL, &r),
         r);
  printf("!!!usbmass_bd_irx %d (%d)\n",
         SifExecModuleBuffer((void *)&usbmass_bd_irx, usbmass_bd_irx_length, 0,
                             NULL, &r),
         r);
  printf("!!!bdm_irx %d (%d)\n",
         SifExecModuleBuffer((void *)&bdm_irx, bdm_irx_length, 0, NULL, &r), r);
  printf("!!!bdmfs_fatfs_irx %d (%d)\n",
         SifExecModuleBuffer((void *)&bdmfs_fatfs_irx, bdmfs_fatfs_irx_length,
                             0, NULL, &r),
         r);
  printf("!!!bdmfs_vfat_irx %d (%d)\n",
         SifExecModuleBuffer((void *)&bdmfs_vfat_irx, bdmfs_vfat_irx_length, 0,
                             NULL, &r),
         r);
  printf("!!!usbhdfsd_array %d (%d)\n",
         SifExecModuleBuffer((void *)usbhdfsd_irx, usbhdfsd_irx_length, 0, NULL,
                             &r),
         r);
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
