#include "Chip8.hpp"
#include "Platform.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char **argv)
{
    // Validate command-line arguments: Expect Scale, Delay, and ROM file path
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        std::exit(EXIT_FAILURE);
    }

    // Extract and convert command-line parameters
    int videoScale = std::stoi(argv[1]); // Pixel scaling factor for display
    int cycleDelay = std::stoi(argv[2]); // Time (in ms) between CPU cycles
    char const *romFilename = argv[3];   // Path to CHIP-8 ROM file

    // Initialize the platform (window + rendering) with scaled resolution
    Platform platform(
        "CHIP-8 Emulator",
        VIDEO_WIDTH * videoScale,
        VIDEO_HEIGHT * videoScale,
        VIDEO_WIDTH,
        VIDEO_HEIGHT);

    // Create and initialize the CHIP-8 virtual machine
    Chip8 chip8;
    chip8.LoadROM(romFilename); // Load the specified ROM into memory

    // Calculate the number of bytes per row for the video buffer
    int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

    // Time tracking for cycle delay control
    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    bool quit = false;

    // Main application loop
    while (!quit)
    {
        // Process user input and update the emulator's keypad state
        quit = platform.ProcessInput(chip8.keypad);

        // Calculate time elapsed since the last cycle
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(
                       currentTime - lastCycleTime)
                       .count();

        // Execute a CHIP-8 cycle if enough time has passed
        if (dt > cycleDelay)
        {
            lastCycleTime = currentTime;

            chip8.Cycle(); // Run one CPU cycle (fetch, decode, execute)

            platform.Update(chip8.video, videoPitch); // Render the current video buffer to screen
        }
    }

    return 0;
}