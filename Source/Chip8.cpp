#include "Chip8.hpp"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>

// Constants for the CHIP-8 system
const unsigned int FONTSET_SIZE = 80;            // Size of the fontset in bytes
const unsigned int FONTSET_START_ADDRESS = 0x50; // Starting memory address for fontset
const unsigned int START_ADDRESS = 0x200;        // Starting memory address for ROMs

// CHIP-8 fontset. Each character is 5 bytes long (representing 5x8 pixels)
uint8_t fontset[FONTSET_SIZE] =
    {
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

// Constructor: Initializes the CHIP-8 system
Chip8::Chip8()
    : randGen(std::chrono::system_clock::now().time_since_epoch().count()) // Seed random number generator
{
    // Initialize Program Counter to start address
    pc = START_ADDRESS;

    // Load fonts into memory starting at FONTSET_START_ADDRESS
    for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
    {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    // Initialize random number generator
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);

    // Set up the main instruction table (indexed by the first nibble of opcode)
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1nnn;
    table[0x2] = &Chip8::OP_2nnn;
    table[0x3] = &Chip8::OP_3xkk;
    table[0x4] = &Chip8::OP_4xkk;
    table[0x5] = &Chip8::OP_5xy0;
    table[0x6] = &Chip8::OP_6xkk;
    table[0x7] = &Chip8::OP_7xkk;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9xy0;
    table[0xA] = &Chip8::OP_Annn;
    table[0xB] = &Chip8::OP_Bnnn;
    table[0xC] = &Chip8::OP_Cxkk;
    table[0xD] = &Chip8::OP_Dxyn;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    // Initialize sub-tables with NULL operations by default
    for (size_t i = 0; i <= 0xE; i++)
    {
        table0[i] = &Chip8::OP_NULL;
        table8[i] = &Chip8::OP_NULL;
        tableE[i] = &Chip8::OP_NULL;
    }

    // Set up table0 (for opcodes starting with 0x0)
    table0[0x0] = &Chip8::OP_00E0; // Clear screen
    table0[0xE] = &Chip8::OP_00EE; // Return from subroutine

    // Set up table8 (for opcodes starting with 0x8)
    table8[0x0] = &Chip8::OP_8xy0; // Store VY in VX
    table8[0x1] = &Chip8::OP_8xy1; // Set VX to VX OR VY
    table8[0x2] = &Chip8::OP_8xy2; // Set VX to VX AND VY
    table8[0x3] = &Chip8::OP_8xy3; // Set VX to VX XOR VY
    table8[0x4] = &Chip8::OP_8xy4; // Add VY to VX, set VF on carry
    table8[0x5] = &Chip8::OP_8xy5; // Subtract VY from VX, set VF on NOT borrow
    table8[0x6] = &Chip8::OP_8xy6; // Right shift VX, set VF to LSB
    table8[0x7] = &Chip8::OP_8xy7; // Set VX to VY minus VX, set VF on NOT borrow
    table8[0xE] = &Chip8::OP_8xyE; // Left shift VX, set VF to MSB

    // Set up tableE (for opcodes starting with 0xE)
    tableE[0x1] = &Chip8::OP_ExA1; // Skip if key not pressed
    tableE[0xE] = &Chip8::OP_Ex9E; // Skip if key pressed

    // Initialize tableF (for opcodes starting with 0xF) with NULL operations
    for (size_t i = 0; i <= 0x65; i++)
    {
        tableF[i] = &Chip8::OP_NULL;
    }

    // Set up tableF operations
    tableF[0x07] = &Chip8::OP_Fx07; // Set VX to delay timer value
    tableF[0x0A] = &Chip8::OP_Fx0A; // Wait for key press, store in VX
    tableF[0x15] = &Chip8::OP_Fx15; // Set delay timer to VX
    tableF[0x18] = &Chip8::OP_Fx18; // Set sound timer to VX
    tableF[0x1E] = &Chip8::OP_Fx1E; // Add VX to index register
    tableF[0x29] = &Chip8::OP_Fx29; // Set index to font character in VX
    tableF[0x33] = &Chip8::OP_Fx33; // Store BCD representation of VX in memory
    tableF[0x55] = &Chip8::OP_Fx55; // Store registers V0-VX in memory
    tableF[0x65] = &Chip8::OP_Fx65; // Read registers V0-VX from memory
}

// Load a ROM file into memory starting at START_ADDRESS
void Chip8::LoadROM(char const *filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        // Get file size
        std::streampos size = file.tellg();
        char *buffer = new char[size];

        // Read file into buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        // Copy buffer contents into CHIP-8 memory
        for (long i = 0; i < size; ++i)
        {
            memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
    }
}

// Emulate one CPU cycle
void Chip8::Cycle()
{
    // Fetch opcode (2 bytes) from memory at PC
    opcode = (memory[pc] << 8u) | memory[pc + 1];

    // Increment PC before execution
    pc += 2;

    // Decode and execute the opcode using the instruction table
    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    // Decrement the delay timer if it's been set
    if (delayTimer > 0)
    {
        --delayTimer;
    }

    // Decrement the sound timer if it's been set
    if (soundTimer > 0)
    {
        --soundTimer;
    }
}

// Handle opcodes starting with 0x0 (uses sub-table)
void Chip8::Table0()
{
    ((*this).*(table0[opcode & 0x000Fu]))();
}

// Handle opcodes starting with 0x8 (uses sub-table)
void Chip8::Table8()
{
    ((*this).*(table8[opcode & 0x000Fu]))();
}

// Handle opcodes starting with 0xE (uses sub-table)
void Chip8::TableE()
{
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

// Handle opcodes starting with 0xF (uses sub-table)
void Chip8::TableF()
{
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

// Null operation (used for unimplemented opcodes)
void Chip8::OP_NULL()
{
}

// 00E0 - Clear the screen
void Chip8::OP_00E0()
{
    memset(video, 0, sizeof(video));
}

// 00EE - Return from subroutine
void Chip8::OP_00EE()
{
    --sp;           // Decrement stack pointer
    pc = stack[sp]; // Set PC to return address
}

// 1nnn - Jump to address nnn
void Chip8::OP_1nnn()
{
    uint16_t address = opcode & 0x0FFFu;
    pc = address;
}

// 2nnn - Call subroutine at nnn
void Chip8::OP_2nnn()
{
    uint16_t address = opcode & 0x0FFFu;

    stack[sp] = pc; // Store current PC on stack
    ++sp;           // Increment stack pointer
    pc = address;   // Jump to subroutine
}

// 3xkk - Skip next instruction if Vx == kk
void Chip8::OP_3xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] == byte)
    {
        pc += 2; // Skip next instruction
    }
}

// 4xkk - Skip next instruction if Vx != kk
void Chip8::OP_4xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] != byte)
    {
        pc += 2; // Skip next instruction
    }
}

// 5xy0 - Skip next instruction if Vx == Vy
void Chip8::OP_5xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] == registers[Vy])
    {
        pc += 2; // Skip next instruction
    }
}

// 6xkk - Set Vx = kk
void Chip8::OP_6xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    registers[Vx] = byte;
}

// 7xkk - Set Vx = Vx + kk
void Chip8::OP_7xkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    registers[Vx] += byte;
}

// 8xy0 - Set Vx = Vy
void Chip8::OP_8xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] = registers[Vy];
}

// 8xy1 - Set Vx = Vx OR Vy
void Chip8::OP_8xy1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] |= registers[Vy];
}

// 8xy2 - Set Vx = Vx AND Vy
void Chip8::OP_8xy2()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];
}

// 8xy3 - Set Vx = Vx XOR Vy
void Chip8::OP_8xy3()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];
}

// 8xy4 - Set Vx = Vx + Vy, set VF = carry
void Chip8::OP_8xy4()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = registers[Vx] + registers[Vy];

    // Set carry flag if sum exceeds 8 bits
    registers[0xF] = sum > 255U ? 1 : 0;

    // Store lower 8 bits in Vx
    registers[Vx] = sum & 0xFFu;
}

// 8xy5 - Set Vx = Vx - Vy, set VF = NOT borrow
void Chip8::OP_8xy5()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    // Set borrow flag if Vx > Vy
    registers[0xF] = registers[Vx] > registers[Vy] ? 1 : 0;

    registers[Vx] -= registers[Vy];
}

// 8xy6 - Set Vx = Vx SHR 1, set VF = LSB before shift
void Chip8::OP_8xy6()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    // Save least significant bit in VF
    registers[0xF] = (registers[Vx] & 0x1u);

    // Shift right by 1
    registers[Vx] >>= 1;
}

// 8xy7 - Set Vx = Vy - Vx, set VF = NOT borrow
void Chip8::OP_8xy7()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    // Set borrow flag if Vy > Vx
    registers[0xF] = registers[Vy] > registers[Vx] ? 1 : 0;

    registers[Vx] = registers[Vy] - registers[Vx];
}

// 8xyE - Set Vx = Vx SHL 1, set VF = MSB before shift
void Chip8::OP_8xyE()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    // Save most significant bit in VF
    registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

    // Shift left by 1
    registers[Vx] <<= 1;
}

// 9xy0 - Skip next instruction if Vx != Vy
void Chip8::OP_9xy0()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    if (registers[Vx] != registers[Vy])
    {
        pc += 2; // Skip next instruction
    }
}

// Annn - Set index register to nnn
void Chip8::OP_Annn()
{
    uint16_t address = opcode & 0x0FFFu;
    index = address;
}

// Bnnn - Jump to address nnn + V0
void Chip8::OP_Bnnn()
{
    uint16_t address = opcode & 0x0FFFu;
    pc = registers[0] + address;
}

// Cxkk - Set Vx = random byte AND kk
void Chip8::OP_Cxkk()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    registers[Vx] = randByte(randGen) & byte;
}

// Dxyn - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void Chip8::OP_Dxyn()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;

    // Wrap coordinates if they go beyond screen boundaries
    uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
    uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

    registers[0xF] = 0; // Reset collision flag

    for (unsigned int row = 0; row < height; ++row)
    {
        uint8_t spriteByte = memory[index + row]; // Get sprite data

        for (unsigned int col = 0; col < 8; ++col)
        {
            uint8_t spritePixel = spriteByte & (0x80u >> col); // Get current pixel
            uint32_t *screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

            // If sprite pixel is on
            if (spritePixel)
            {
                // Check if screen pixel is already on (collision)
                if (*screenPixel == 0xFFFFFFFF)
                {
                    registers[0xF] = 1; // Set collision flag
                }

                // XOR with sprite pixel (CHIP-8 drawing is XOR-based)
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

// Ex9E - Skip next instruction if key with value Vx is pressed
void Chip8::OP_Ex9E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if (keypad[key])
    {
        pc += 2; // Skip next instruction
    }
}

// ExA1 - Skip next instruction if key with value Vx is not pressed
void Chip8::OP_ExA1()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = registers[Vx];

    if (!keypad[key])
    {
        pc += 2; // Skip next instruction
    }
}

// Fx07 - Set Vx = delay timer value
void Chip8::OP_Fx07()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    registers[Vx] = delayTimer;
}

// Fx0A - Wait for a key press, store the value in Vx
void Chip8::OP_Fx0A()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    // Check each key (0-15)
    for (uint8_t i = 0; i < 16; ++i)
    {
        if (keypad[i])
        {
            registers[Vx] = i;
            return;
        }
    }

    // If no key pressed, decrement PC to repeat this instruction
    pc -= 2;
}

// Fx15 - Set delay timer = Vx
void Chip8::OP_Fx15()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    delayTimer = registers[Vx];
}

// Fx18 - Set sound timer = Vx
void Chip8::OP_Fx18()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    soundTimer = registers[Vx];
}

// Fx1E - Set I = I + Vx
void Chip8::OP_Fx1E()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    index += registers[Vx];
}

// Fx29 - Set I = location of sprite for digit Vx
void Chip8::OP_Fx29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = registers[Vx];

    // Each character is 5 bytes long, starting at FONTSET_START_ADDRESS
    index = FONTSET_START_ADDRESS + (5 * digit);
}

// Fx33 - Store BCD representation of Vx in memory locations I, I+1, I+2
void Chip8::OP_Fx33()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = registers[Vx];

    // Extract and store hundreds digit
    memory[index + 2] = value % 10;
    value /= 10;

    // Extract and store tens digit
    memory[index + 1] = value % 10;
    value /= 10;

    // Extract and store ones digit
    memory[index] = value % 10;
}

// Fx55 - Store registers V0 through Vx in memory starting at location I
void Chip8::OP_Fx55()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for (uint8_t i = 0; i <= Vx; ++i)
    {
        memory[index + i] = registers[i];
    }
}

// Fx65 - Read registers V0 through Vx from memory starting at location I
void Chip8::OP_Fx65()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for (uint8_t i = 0; i <= Vx; ++i)
    {
        registers[i] = memory[index + i];
    }
}