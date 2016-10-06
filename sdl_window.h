#pragma once

#include <GL/glew.h>

typedef struct sdl_gl_window
{
	unsigned int width, height;
	SDL_Window* window_ptr;
	SDL_GLContext gl_context;
} sdl_gl_window;

sdl_gl_window* new_sdl_gl_window(unsigned int width, unsigned int height)
{
	sdl_gl_window* sdl_gl_window_ptr = malloc(sizeof(sdl_gl_window));
	sdl_gl_window_ptr->width = width;
	sdl_gl_window_ptr->height = height;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return NULL;
	}

	SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	sdl_gl_window_ptr->window_ptr = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (sdl_gl_window_ptr->window_ptr == NULL)
	{
		printf("Unable to create window: %s\n", SDL_GetError());
		return NULL;
	}

	sdl_gl_window_ptr->gl_context = SDL_GL_CreateContext(sdl_gl_window_ptr->window_ptr);

	glewExperimental = true;
	glewInit();

	return sdl_gl_window_ptr;
}

void delete_sdl_gl_window(sdl_gl_window* sdl_gl_window_ptr)
{
	if (sdl_gl_window_ptr == NULL) return;
	SDL_GL_DeleteContext(sdl_gl_window_ptr->gl_context);
	SDL_DestroyWindow(sdl_gl_window_ptr->window_ptr);
	free(sdl_gl_window_ptr);
	sdl_gl_window_ptr = NULL;
}
