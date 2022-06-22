#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cstring>

chip8::chip8() { init(); }

void chip8::init() {
  // zero structures
  memset(memory, 0, sizeof(memory));
  clear_screen();

  // set timers
  delay_timer = 60;
  sound_timer = 60;

  memset(stack, 0, sizeof(stack));

  // set font
  uint8_t font[] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
  memcpy(memory + FONT_ADDRESS, font, sizeof(font));

  // set registers
  sp = 0;
  pc = 0x200;

  memset(reg, 0, sizeof(reg));
  index = 0;

  memset(keyboard, 0, sizeof(keyboard));

  // init rnd
  srand(time(NULL));

  exception = NO_EXCEPTION;
}

void chip8::clear_screen() { memset(display, 0, sizeof(display)); }

bool chip8::tick_delay() {
  if (delay_timer) {
    delay_timer--;
  }
  return delay_timer;
};

void chip8::set_RAM(uint16_t ptr, uint8_t val) {
  //
  memory[ptr] = val;
}

uint8_t chip8::get_RAM(uint16_t ptr) {
  //
  return memory[ptr];
}

bool chip8::tick_sound() {
  if (sound_timer) {
    sound_timer--;
  }
  return sound_timer;
};

bool chip8::loop() {
  bool redraw = decode_execute(fetch());
  // clock
  tick_delay();
  if (tick_sound()) {
    //!!! beep
  }
  return redraw;
}

bool chip8::load(char *rom) {
  FILE *f;

  if ((f = fopen(rom, "r")) == NULL) {
    return false;
  }

  // read ROM
  fread(memory + 0x200, sizeof(memory) - 0x200, 1, f);

  fclose(f);
  return true;
}

bool chip8::load(uint8_t *rom, uint16_t size) {
  // copy ROM
  memcpy(memory + 0x200, rom,
         (size < sizeof(memory) - 0x200) ? size : (sizeof(memory) - 0x200));
  return true;
}

void chip8::key_press(int k) { keyboard[k] = true; }

void chip8::key_release(int k) { keyboard[k] = false; }

uint16_t chip8::fetch() {
  uint16_t opcode = (memory[pc & 0xfff] << 8) + memory[(pc & 0xfff) + 1];
  pc += 2;
  return opcode;
}

bool chip8::decode_execute(uint16_t opcode) {
  // decode
  uint16_t _op = (opcode & 0xf000) >> 12;
  uint16_t _x = (opcode & 0x0f00) >> 8;
  uint16_t _y = (opcode & 0x00f0) >> 4;
  uint16_t _n = (opcode & 0x000f);
  uint16_t _nn = (opcode & 0x00ff);
  uint16_t _nnn = (opcode & 0x0fff);

  bool redraw = false;
  exception = NO_EXCEPTION;
  switch (_op) {
  case 0x00:
    switch (_nn) {
    case 0xe0:
      // CLS
      clear_screen();
      redraw = true;
      break;
    case 0xee:
      // RET
      if (sp == 0) {
        exception = EXCEPTION_STACK_UNDERFLOW;
        break;
      }
      sp--;
      pc = stack[sp];
      break;
    default:
      exception = EXCEPTION_UNKNOWN_OPCODE;
      break;
    }
    break;
  case 0x01:
    // JP addr
    pc = _nnn;
    break;
  case 0x02:
    // CALL addr
    if (sp == 16) {
      exception = EXCEPTION_STACK_OVERFLOW;
      break;
    }
    stack[sp] = pc;
    sp++;
    pc = _nnn;
    break;
  case 0x03:
    // SE Vx, byte
    if (reg[_x] == _nn) {
      pc += 2;
    }
    break;
  case 0x04:
    // SNE Vx, byte
    if (reg[_x] != _nn) {
      pc += 2;
    }
    break;
  case 0x05:
    // SE Vx, Vy
    if (reg[_x] == reg[_y]) {
      pc += 2;
    }
    break;
  case 0x06:
    // LD Vx, byte
    reg[_x] = _nn;
    break;
  case 0x07:
    // ADD Vx, byte
    reg[_x] += _nn;
    break;
  case 0x08:
    switch (_n) {
    case 0x0:
      // LD Vx, Vy
      reg[_x] = reg[_y];
      break;
    case 0x1:
      // OR Vx, Vy
      reg[_x] |= reg[_y];
      break;
    case 0x2:
      // AND Vx, Vy
      reg[_x] &= reg[_y];
      break;
    case 0x3:
      // XOR Vx, Vy
      reg[_x] ^= reg[_y];
      break;
    case 0x4:
      // ADD Vx, Vy
      reg[CARRY] = (reg[_x] > UINT8_MAX - reg[_y]);
      reg[_x] += reg[_y];
      break;
    case 0x5:
      // SUB Vx, Vy
      reg[CARRY] = (reg[_x] > reg[_y]);
      reg[_x] -= reg[_y];
      break;
    case 0x6:
      // SHR Vx {, Vy}
      reg[CARRY] = (reg[_x] & 0b00000001);
      //??? reg[_x] = reg[_y] >> 1;
      reg[_x] >>= 1;
      break;
    case 0x7:
      // SUBN Vx, Vy
      reg[CARRY] = (reg[_y] > reg[_x]);
      reg[_x] = reg[_y] - reg[_x];
      break;
    case 0xe:
      // SHL Vx {, Vy}
      reg[CARRY] = (reg[_x] >> 7);
      //??? reg[_x] = reg[_y] << 1;
      reg[_x] <<= 1;
      break;
    default:
      exception = EXCEPTION_UNKNOWN_OPCODE;
      break;
    }
    break;
  case 0x09:
    // SNE Vx, Vy
    if (reg[_x] != reg[_y]) {
      pc += 2;
    }
    break;
  case 0x0a:
    // LD I, addr
    index = _nnn;
    break;
  case 0x0b:
    // LJP V0, addr
    pc = _nnn + reg[0];
    break;
  case 0x0c:
    // RND Vx, byte
    reg[_x] = (rand() % 255) % _nn;
    break;
  case 0x0d:
    // DRW Vx, Vy, nibble
    reg[CARRY] = false;
    for (int pos_y = 0; pos_y < _n; pos_y++) {
      for (int pos_x = 0; pos_x < 8; pos_x++) {
        uint16_t ofs =
            (((reg[_y] + pos_y) % 32) * 64) + ((reg[_x] + pos_x) % 64);
        uint8_t pixel = get_RAM(index + pos_y) & (0x80 >> pos_x);
        if (pixel) {
          if (display[ofs]) {
            reg[CARRY] = true;
            display[ofs] = false;
          } else {
            display[ofs] = true;
          }
        }
      }
    }
    redraw = true;
    break;
  case 0x0e:
    switch (_nn) {
    case 0x9E:
      // SKP Vx
      if (keyboard[reg[_x]]) {
        pc += 2;
      }
      break;
    case 0xA1:
      // SKNP Vx
      if (!keyboard[reg[_x]]) {
        pc += 2;
      }
      break;
    default:
      exception = EXCEPTION_UNKNOWN_OPCODE;
      break;
    }
    break;
  case 0x0f:
    switch (_nn) {
    case 0x07:
      // LD Vx, DT
      reg[_x] = delay_timer;
      break;
    case 0x0a:
      // LD Vx, K
      {
        bool keyp = false;
        for (uint8_t i = 0; i < sizeof(keyboard); i++) {
          if (keyboard[i]) {
            reg[_x] = i;
            keyp = true;
          }
        }
        if (!keyp) {
          pc -= 2;
        }
      }
      break;
    case 0x15:
      // LD DT, Vx
      delay_timer = reg[_x];
      break;
    case 0x18:
      // LD ST, Vx
      sound_timer = reg[_x];
      break;
    case 0x1e:
      // ADD I, Vx
      index += reg[_x];
      break;
    case 0x29:
      // LD F, Vx
      index = FONT_ADDRESS + (reg[_x] * 5);
      break;
    case 0x33:
      // LD B, Vx
      set_RAM(index, (reg[_x] / 100));
      set_RAM(index + 1, (reg[_x] % 100) / 10);
      set_RAM(index + 2, (reg[_x] % 10) / 1);
      //??? index +=3;
      break;
    case 0x55:
      // LD [I], Vx
      for (int i = 0; i <= _x; i++) {
        set_RAM(index + i, reg[i]);
      }
      //??? index += _x + 1;
      break;
    case 0x65:
      //  LD Vx, [I]
      for (int i = 0; i <= _x; i++) {
        reg[i] = get_RAM(index + i);
      }
      //??? index += _x + 1;
      break;
    default:
      exception = EXCEPTION_UNKNOWN_OPCODE;
      break;
    }
    break;
  default:
    exception = EXCEPTION_UNKNOWN_OPCODE;
    break;
  }

  return redraw;
}
