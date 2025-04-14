#pragma once // Ensures this header is only included once during compilation to avoid redefinition errors.

#include <cstdint>   // Provides fixed-width integer types like uint8_t.
#include <SDL.h>     // Simple DirectMedia Layer library for handling windows, rendering, and input.
#include <glad/gl.h> // GLAD loader to access modern OpenGL functions.

/**
 * @class Platform
 * @brief Handles platform-specific operations such as window creation, input processing, and texture rendering.
 *
 * This class integrates SDL2 for windowing and input, and OpenGL for rendering.
 */
class Platform
{
    // Granting Imgui class access to private members of Platform
    friend class Imgui;

public:
    /**
     * @brief Constructor to initialize the Platform with a window and texture.
     *
     * @param title          Title of the window.
     * @param windowWidth    Width of the SDL window.
     * @param windowHeight   Height of the SDL window.
     * @param textureWidth   Width of the texture to render.
     * @param textureHeight  Height of the texture to render.
     */
    Platform(char const *title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);

    /**
     * @brief Destructor to clean up allocated resources (window, context, textures, etc.).
     */
    ~Platform();

    /**
     * @brief Updates the display with a new framebuffer.
     *
     * @param buffer  Pointer to pixel data to render.
     * @param pitch   Number of bytes in a row of pixel data.
     */
    void Update(void const *buffer, int pitch);

    /**
     * @brief Processes keyboard input and populates the provided key state array.
     *
     * @param keys  Pointer to an array where key states will be stored.
     * @return True if the application should continue running, false if it should quit.
     */
    bool ProcessInput(uint8_t *keys);

private:
    SDL_Window *window{};       ///< Pointer to the SDL window.
    SDL_GLContext gl_context{}; ///< OpenGL context associated with the SDL window.
    GLuint framebuffer_texture; ///< OpenGL texture ID for the framebuffer.
    SDL_Renderer *renderer{};   ///< SDL renderer used for rendering (optional depending on OpenGL usage).
    SDL_Texture *texture{};     ///< SDL texture that may be used for software rendering or display.
};
