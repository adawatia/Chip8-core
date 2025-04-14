#pragma once // Ensure the file is included only once during compilation

#include <cstdint>   // For standard integer types
#include <SDL.h>     // For SDL library functionality
#include <glad/gl.h> // For OpenGL functions (loaded via glad)

// Platform class handles window creation, rendering, and input processing
// for the CHIP-8 emulator using SDL and OpenGL
class Platform
{
    friend class Imgui; // Allow Imgui class to access private members

public:
    // Constructor - Initializes the platform with window and texture dimensions
    // Parameters:
    //   title - Window title
    //   windowWidth - Width of the application window
    //   windowHeight - Height of the application window
    //   textureWidth - Width of the texture used for CHIP-8 display
    //   textureHeight - Height of the texture used for CHIP-8 display
    Platform(char const *title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);

    // Destructor - Cleans up SDL and OpenGL resources
    ~Platform();

    // Updates the display with new frame data
    // Parameters:
    //   buffer - Pointer to the pixel data to render
    //   pitch - Number of bytes per row in the pixel data
    void Update(void const *buffer, int pitch);

    // Processes input events and updates the key state array
    // Parameters:
    //   keys - Array representing the CHIP-8 key states (1 = pressed, 0 = released)
    // Returns:
    //   bool - True if application should continue running, false if should quit
    bool ProcessInput(uint8_t *keys);

private:
    SDL_Window *window{};       // SDL window handle
    SDL_GLContext gl_context{}; // OpenGL context handle
    GLuint framebuffer_texture; // OpenGL texture for CHIP-8 display
    SDL_Renderer *renderer{};   // SDL renderer (alternative to OpenGL rendering)
    SDL_Texture *texture{};     // SDL texture (alternative to OpenGL texture)
};