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
#include <tamtypes.h>
#include <unistd.h>
#include <usbhdfsd-common.h>

#define WIDTH 640
#define HEIGHT 480
#define MENU_ITENS_PER_PAGE 17
#define ROM_DIRECTORY "mass0:."
#define MENU_MAX_SIZE 30
#else
// LINUX
#define WIDTH 640
#define HEIGHT 480
#define MENU_ITENS_PER_PAGE 17
#define ROM_DIRECTORY "."
#define MENU_MAX_SIZE 30
#endif
