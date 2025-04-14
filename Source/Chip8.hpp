#pragma once

#include <cstdint>
#include <random>

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
