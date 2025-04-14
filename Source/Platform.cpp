#include "Platform.hpp"
#include <glad/gl.h>
#include <SDL.h>


// Constructor: Initializes SDL, creates a window and OpenGL context, and sets up an OpenGL texture
Platform::Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
{
	// Initialize SDL video subsystem
	SDL_Init(SDL_INIT_VIDEO);

	// Set OpenGL version to 3.3 Core Profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Create an SDL window with OpenGL context and make it resizable
	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// Create an OpenGL rendering context associated with the window
	gl_context = SDL_GL_CreateContext(window);

	// Enable VSync (1 = enabled)
	SDL_GL_SetSwapInterval(1);

	// Load OpenGL function pointers using GLAD
	gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

	// Generate a texture that will serve as the framebuffer for rendering
	glGenTextures(1, &framebuffer_texture);
	glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

	// Set texture parameters for scaling and wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);       // No smoothing on downscaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);       // No smoothing on upscaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);     // Clamp horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);     // Clamp vertically

	// Allocate memory for the texture but do not fill it yet
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 320, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Destructor: Cleans up all SDL and OpenGL resources
Platform::~Platform()
{
	// Destroy the SDL texture and renderer if they were used
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);

	// Destroy the window and shut down SDL
	SDL_DestroyWindow(window);
	SDL_Quit();
}

// Update: Uploads pixel buffer to SDL texture and renders it to the screen
void Platform::Update(void const* buffer, int pitch)
{
	// Update texture with the new pixel data
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);

	// Clear the screen before drawing
	SDL_RenderClear(renderer);

	// Copy the texture to the rendering context
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);

	// Present the rendered frame to the screen
	SDL_RenderPresent(renderer);
}

// ProcessInput: Handles SDL events, maps specific keys to virtual keypad array, and returns quit status
bool Platform::ProcessInput(uint8_t* keys)
{
	bool quit = false; // Flag to determine whether the application should exit

	SDL_Event event;

	// Poll and handle all queued SDL events
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				quit = true; // Exit requested (e.g., window closed)
				break;

			case SDL_KEYDOWN:
				// If Escape is pressed, mark quit
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
					break;
				}

				// Map key presses to virtual keypad
				switch (event.key.keysym.sym)
				{
					case SDLK_x: keys[0] = 1; break;
					case SDLK_1: keys[1] = 1; break;
					case SDLK_2: keys[2] = 1; break;
					case SDLK_3: keys[3] = 1; break;
					case SDLK_q: keys[4] = 1; break;
					case SDLK_w: keys[5] = 1; break;
					case SDLK_e: keys[6] = 1; break;
					case SDLK_a: keys[7] = 1; break;
					case SDLK_s: keys[8] = 1; break;
					case SDLK_d: keys[9] = 1; break;
					case SDLK_z: keys[0xA] = 1; break;
					case SDLK_c: keys[0xB] = 1; break;
					case SDLK_4: keys[0xC] = 1; break;
					case SDLK_r: keys[0xD] = 1; break;
					case SDLK_f: keys[0xE] = 1; break;
					case SDLK_v: keys[0xF] = 1; break;
				}
				break;

			case SDL_KEYUP:
				// Map key releases to reset virtual keypad states
				switch (event.key.keysym.sym)
				{
					case SDLK_x: keys[0] = 0; break;
					case SDLK_1: keys[1] = 0; break;
					case SDLK_2: keys[2] = 0; break;
					case SDLK_3: keys[3] = 0; break;
					case SDLK_q: keys[4] = 0; break;
					case SDLK_w: keys[5] = 0; break;
					case SDLK_e: keys[6] = 0; break;
					case SDLK_a: keys[7] = 0; break;
					case SDLK_s: keys[8] = 0; break;
					case SDLK_d: keys[9] = 0; break;
					case SDLK_z: keys[0xA] = 0; break;
					case SDLK_c: keys[0xB] = 0; break;
					case SDLK_4: keys[0xC] = 0; break;
					case SDLK_r: keys[0xD] = 0; break;
					case SDLK_f: keys[0xE] = 0; break;
					case SDLK_v: keys[0xF] = 0; break;
				}
				break;
		}
	}

	return quit; // Return whether to quit the main application loop
}
