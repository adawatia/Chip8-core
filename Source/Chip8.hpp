#pragma once

#include <cstdint>
#include <random>
#include <sys/types.h>

// === CHIP-8 Constants ===

// Number of keys on the CHIP-8 keypad (0-F)
const unsigned int KEY_COUNT = 16;

// Total memory size (4KB)
const unsigned int MEMORY_SIZE = 4096;

// Number of general-purpose registers (V0 to VF)
const unsigned int REGISTER_COUNT = 16;

// Maximum call stack depht
const unsigned int STACK_LEVELS = 16;

// Display dimensions (64 * 32 monochrome)
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

// === CHIP-8 Emulator Class ===

class Chip8
{
public:
  // Constructor: Initializes emulator  stateand opcode tables
  Chip8();

  // Load a CHIP-8 ROM into memory starting at 0x200
  void LoadROM(char const *filename);

  // Performs one emulation cycle: fetch, decode, execute
  void Cycle();

  // === Publically Exposed State ===
  uint8_t keypad[KEY_COUNT]{};

  // Framebuffer  for  rendering display (each pixel is  32-t ARGB)
  uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{};

private:
  // === Opcode Dispatch Tables ===

  // Handler for opcodes beginning with 0x0
  void Table0();

  // Handler for opcodes beginning with 0x8
  void Table8();

  // Handler for opcodes beginning with 0xE
  void TableE();

  // Handler for opcodes beginning with 0xF
  void TableF();

  // === Opcode Implementations ===

  // No operation (used as default)
  void OP_NULL();

  // 00E0 - CLS: Clear the display
  void OP_00E0();

  // 00EE - RET: Return from a subroutine
  void OP_00EE();

  // 1nnn - JP addr: Jump to address nnn
  void OP_1nnn();

  // 2nnn - CALL addr: Call subroutine at address nnn
  void OP_2nnn();

  // 3xkk - SE Vx, byte: Skip next instruction if Vx == kk
  void OP_3xkk();

  // 4xkk - SNE Vx, byte: Skip next instruction if Vx != kk
  void OP_4xkk();

  // 5xy0 - SE Vx, Vy: Skip next instruction if Vx == Vy
  void OP_5xy0();

  // 6xkk - LD Vx, byte: Set Vx = kk
  void OP_6xkk();

  // 7xkk - ADD Vx, byte: Set Vx = Vx + kk
  void OP_7xkk();

  // 8xy0 - LD Vx, Vy: Set Vx = Vy
  void OP_8xy0();

  // 8xy1 - OR Vx, Vy: Set Vx = Vx OR Vy
  void OP_8xy1();

  // 8xy2 - AND Vx, Vy: Set Vx = Vx AND Vy
  void OP_8xy2();

  // 8xy3 - XOR Vx, Vy: Set Vx = Vx XOR Vy
  void OP_8xy3();

  // 8xy4 - ADD Vx, Vy: Set Vx = Vx + Vy, VF = carry
  void OP_8xy4();

  // 8xy5 - SUB Vx, Vy: Set Vx = Vx - Vy, VF = NOT borrow
  void OP_8xy5();

  // 8xy6 - SHR Vx: Shift Vx right by 1, VF = LSB
  void OP_8xy6();

  // 8xy7 - SUBN Vx, Vy: Set Vx = Vy - Vx, VF = NOT borrow
  void OP_8xy7();

  // 8xyE - SHL Vx: Shift Vx left by 1, VF = MSB
  void OP_8xyE();

  // 9xy0 - SNE Vx, Vy: Skip next instruction if Vx != Vy
  void OP_9xy0();

  // Annn - LD I, addr: Set I = nnn
  void OP_Annn();

  // Bnnn - JP V0, addr: Jump to nnn + V0
  void OP_Bnnn();

  // Cxkk - RND Vx, byte: Set Vx = rand() & kk
  void OP_Cxkk();

  // Dxyn - DRW Vx, Vy, nibble: Display n-byte sprite at (Vx, Vy)
  void OP_Dxyn();

  // Ex9E - SKP Vx: Skip next instruction if key Vx is pressed
  void OP_Ex9E();

  // ExA1 - SKNP Vx: Skip next instruction if key Vx is not pressed
  void OP_ExA1();

  // Fx07 - LD Vx, DT: Set Vx = delay timer
  void OP_Fx07();

  // Fx0A - LD Vx, K: Wait for key press, store in Vx
  void OP_Fx0A();

  // Fx15 - LD DT, Vx: Set delay timer = Vx
  void OP_Fx15();

  // Fx18 - LD ST, Vx: Set sound timer = Vx
  void OP_Fx18();

  // Fx1E - ADD I, Vx: Set I = I + Vx
  void OP_Fx1E();

  // Fx29 - LD F, Vx: Set I = location of sprite for digit Vx
  void OP_Fx29();

  // Fx33 - LD B, Vx: Store BCD of Vx in memory at I, I+1, I+2
  void OP_Fx33();

  // Fx55 - LD [I], Vx: Store V0 through Vx in memory starting at I
  void OP_Fx55();

  // Fx65 - LD Vx, [I]: Read V0 through Vx from memory starting at I
  void OP_Fx65();

  // === Internal State ===

  // CHIP-8 memory (RAM), including font set and loaded ROM
  uint8_t memory[MEMORY_SIZE]{};

  // 16 general-purpose 8-bit registers: V0 to VF
  uint8_t registers[REGISTER_COUNT]{};

  // Index register (used for memory addressing)
  uint16_t index{};

  // Program counter (PC), points to current instruction
  uint16_t pc{};

  // Delay timer, counts down at 60Hz
  uint8_t delayTimer{};

  // Sound timer, also counts down at 60Hz and beeps when non-zero
  uint8_t soundTimer{};

  // Stack for subroutine calls
  uint16_t stack[STACK_LEVELS]{};

  // Stack pointer
  uint8_t sp{};

  // Current 16-bit opcode
  uint16_t opcode{};

  // === Random Number Generator ===

  // Mersenne Twister engine for generating random bytes
  std::default_random_engine randGen;

  // Uniform distribution in range [0, 255]
  std::uniform_int_distribution<uint8_t> randByte;

  // === Opcode Dispatch Mechanism ===

  // Function pointer type for opcode handlers
  typedef void (Chip8::*Chip8Func)();

  // Main opcode function table (by high nibble of opcode)
  Chip8Func table[0xF + 1];

  // Subtables for extended decoding
  Chip8Func table0[0xE + 1];
  Chip8Func table8[0xE + 1];
  Chip8Func tableE[0xE + 1];
  Chip8Func tableF[0x65 + 1];
};
