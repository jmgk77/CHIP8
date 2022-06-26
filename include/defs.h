#ifdef PS2
// PS2
#include <debug.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
// #include <fileXio_rpc.h>
// #include <fileio.h>
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

#define WIDTH 640
#define HEIGHT 448
#define MENU_ITENS_PER_PAGE 17
#define ROM_DIRECTORY "mass:"
#define MENU_MAX_SIZE 30
#else
// LINUX
#define WIDTH 640
#define HEIGHT 480
#define MENU_ITENS_PER_PAGE 17
#define ROM_DIRECTORY "."
#define MENU_MAX_SIZE 30
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
#define BUTTON_SYS -1
#define BUTTON_L3 10
#define BUTTON_R3 11
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
#endif