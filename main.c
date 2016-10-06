#include <SDL2/SDL.h>

#define bool char
#define true 1
#define false 0

#include "time.h"
#include "sdl_window.h"
#include "sdl_clock.h"
#include "game.h"

int main()
{
	srand(time(0));
	sdl_gl_window* sdl_gl_window_ptr = new_sdl_gl_window(600, 600);
	sdl_clock* sdl_clock_ptr = new_sdl_clock();
	game* game_ptr = new_game(sdl_gl_window_ptr->width, sdl_gl_window_ptr->height);
	if ((sdl_gl_window_ptr != NULL) && (game_ptr != NULL) && (sdl_clock_ptr != NULL))
	{
		while (game_ptr->is_running)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					case SDL_KEYDOWN: game_ptr->send_key(game_ptr, event.key.keysym.sym, true); break;
					case SDL_KEYUP: game_ptr->send_key(game_ptr, event.key.keysym.sym, false); break;
				}
			}
			sdl_clock_ptr->update(sdl_clock_ptr);
			game_ptr->update(game_ptr, sdl_clock_ptr->delta);
			game_ptr->draw(game_ptr);
			SDL_GL_SwapWindow(sdl_gl_window_ptr->window_ptr);
		}
	} else printf("Something gone wrong\n");
	delete_game(game_ptr);
	delete_sdl_clock(sdl_clock_ptr);
	delete_sdl_gl_window(sdl_gl_window_ptr);
	return 0;
}
