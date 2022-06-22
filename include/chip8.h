#include <cstdint>

#define KEY0 0
#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4
#define KEY5 5
#define KEY6 6
#define KEY7 7
#define KEY8 8
#define KEY9 9
#define KEYA 10
#define KEYB 11
#define KEYC 12
#define KEYD 13
#define KEYE 14
#define KEYF 15

#define CARRY 15

#define FONT_ADDRESS 0x50

#define NO_EXCEPTION 0
#define EXCEPTION_UNKNOWN_OPCODE 1
#define EXCEPTION_STACK_UNDERFLOW 2
#define EXCEPTION_STACK_OVERFLOW 3

class chip8 {
protected:
  uint8_t memory[4096];
  bool display[64 * 32];
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint16_t stack[16];
  uint8_t sp;
  uint16_t pc;
  uint8_t reg[16];
  uint16_t index;
  bool keyboard[16];
  uint8_t exception;
  void init();
  // debugger would like to override these
  virtual void set_RAM(uint16_t ptr, uint8_t val);
  virtual uint8_t get_RAM(uint16_t ptr);

private:
  void clear_screen();
  bool tick_delay();
  bool tick_sound();
  uint16_t fetch();
  bool decode_execute(uint16_t opcode);

public:
  chip8();
  bool load(char *rom);
  bool load(uint8_t *rom, uint16_t size);
  void key_press(int k);
  void key_release(int k);
  bool loop();
};
