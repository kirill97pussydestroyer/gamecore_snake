#pragma once

typedef struct sdl_clock
{
	clock_t last, now;
	float delta;
	void (*update)(struct sdl_clock* this);
} sdl_clock;

void sdl_clock_update(sdl_clock* this)
{
	this->last = this->now;
	this->now = SDL_GetPerformanceCounter();
	this->delta = ((this->now - this->last) / (float)SDL_GetPerformanceFrequency());
}

sdl_clock* new_sdl_clock()
{
	sdl_clock* sdl_clock_ptr = malloc(sizeof(sdl_clock));
	sdl_clock_ptr->now = SDL_GetPerformanceCounter();
	sdl_clock_ptr->last = 0;
	sdl_clock_ptr->delta = 0;
	sdl_clock_ptr->update = &sdl_clock_update;
	return sdl_clock_ptr;
}

void delete_sdl_clock(sdl_clock* sdl_clock_ptr)
{
	if (sdl_clock_ptr == NULL) return;
	free(sdl_clock_ptr);
	sdl_clock_ptr = NULL;
}
