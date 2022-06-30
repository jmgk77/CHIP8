# CHIP8 for PS2/PS3/Linux

This is a CHIP8 emulator for PlayStation 2, PlayStation 3 and Linux.

## Build

Use ps2sdk to compile for PS2, psl1ght to compile for PS3.

## Usage

Run `chip8.Linux` on linux, `chip8.ps2.elf` on PS2, or install and run `chip8.PS3.pkg` on PS3. The ROMs are read from current directory on Linux, or in the 1st USB port on PS2 and PS3

## Keys

Default keys are:

LINUX/PS2 (Joystick)
```
BUTTON_A 5
BUTTON_B 11
BUTTON_X 10
BUTTON_Y 0
BUTTON_L1 12
BUTTON_R1 13
BUTTON_L3 14
BUTTON_R3 15
BUTTON_UP 2
BUTTON_RIGHTUP 3
BUTTON_RIGHT 6
BUTTON_RIGHTDOWN 9
BUTTON_DOWN 8
BUTTON_LEFTDOWN 7
BUTTON_LEFT 4
BUTTON_LEFTUP 1
```

LINUX/PS3 (Keyboard)
```
1 1
2 2
3 3
4 12
Q 4
W 5
E 6
R 13
A 7
S 8
D 9
F 14
Z 10
X 0
C 11
V 15
```

PS3 (Joystick)
```
BUTTON_A 5
BUTTON_B 11
BUTTON_X 10
BUTTON_Y 0
BUTTON_L1 1
BUTTON_R1 3
BUTTON_L3 12
BUTTON_R3 13
BUTTON_LEFT 4
BUTTON_DOWN 8
BUTTON_RIGHT 6
BUTTON_UP 2
BUTTON_L2 7
BUTTON_R2 9
BUTTON_START 14
```

These input can be changed by a `chip8.ini` file in ROMs directory.
