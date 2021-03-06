#define WIDTH 640
#define HEIGHT 480
#define MENU_ITENS_PER_PAGE 17
#define MENU_MAX_SIZE 30

#ifdef PS2
// PS2
#include <debug.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <hdd-ioctl.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <kernel.h>
#include <libcdvd.h>
#include <libmc.h>
#include <libpad.h>
#include <libpwroff.h>
#include <limits.h>
#include <loadfile.h>
#include <malloc.h>
#include <math.h>
#include <netman.h>
#include <osd_config.h>
#include <ps2ips.h>
#include <ps2smb.h>
#include <sbv_patches.h>
#include <sifrpc.h>
#include <smem.h>
#include <smod.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <tamtypes.h>
#include <time.h>
#include <unistd.h>
#include <usbhdfsd-common.h>
#define ROM_DIRECTORY "mass:"
#elif defined(PS3)
// PS3
#include <io/pad.h>
#include <ppu-types.h>
#include <sys/process.h>
#include <sysutil/msg.h>
#include <sysutil/sysutil.h>
#define ROM_DIRECTORY "/dev_usb000"
#else
// LINUX
#define ROM_DIRECTORY "."
#endif

#ifdef PS2
// buttons in PS2
#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_X 0
#define BUTTON_Y 3
#define BUTTON_L1 6
#define BUTTON_R1 7
#define BUTTON_BACK 4
#define BUTTON_START 5
#define BUTTON_L3 10
#define BUTTON_R3 11
#define BUTTON_L2 -1
#define BUTTON_R2 -1
#define BUTTON_LEFT -1
#define BUTTON_DOWN -1
#define BUTTON_RIGHT -1
#define BUTTON_UP -1
#elif defined(PS3)
#define BUTTON_A 9
#define BUTTON_B 10
#define BUTTON_X 8
#define BUTTON_Y 11
#define BUTTON_L1 13
#define BUTTON_R1 12
#define BUTTON_BACK 7
#define BUTTON_START 4
#define BUTTON_L3 6
#define BUTTON_R3 5
#define BUTTON_LEFT 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_UP 3
#define BUTTON_L2 15
#define BUTTON_R2 14
#else
// buttons in linux
#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_X 2
#define BUTTON_Y 3
#define BUTTON_L1 4
#define BUTTON_R1 5
#define BUTTON_BACK 6
#define BUTTON_START 7
#define BUTTON_SYS 8
#define BUTTON_L3 9
#define BUTTON_R3 10
#define BUTTON_L2 -1
#define BUTTON_R2 -1
#define BUTTON_LEFT -1
#define BUTTON_DOWN -1
#define BUTTON_RIGHT -1
#define BUTTON_UP -1
#endif