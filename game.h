#pragma once

#include <GL/glew.h>
#include "fps_counter.h"
#include "vector.h"
#include "geometry.h"
#include "shader.h"
#include "mat4.h"
#include "quad.h"
#include "fbo.h"
#include "snake.h"

typedef struct food
{
	int x, y;
	quad* quad_ptr;
	void (*reset_pos)(struct food* this);
	void (*draw)(struct food* this, float time, const mat4* view_mat4_ptr, const mat4* ortho_mat4_ptr);
} food;

void food_reset_pos(food* this)
{
	this->x = rand() % 25 - 12;
	this->y = rand() % 25 - 12;
}

void food_draw(food* this, float time, const mat4* view_mat4_ptr, const mat4* ortho_mat4_ptr)
{
	mat4_set_to_identity(this->quad_ptr->model_mat4_ptr);
	mat4_scale(this->quad_ptr->model_mat4_ptr, 0.02f, 0.02f);
	mat4_translate(this->quad_ptr->model_mat4_ptr, this->x*2, this->y*2);
	glUseProgram(this->quad_ptr->shader_ptr->program);
	glUniform3f(this->quad_ptr->shader_ptr->u_color, 1.0f, 1.0f, 0.3f);
	this->quad_ptr->draw(this->quad_ptr, time, view_mat4_ptr, ortho_mat4_ptr);
}

food* new_food(const quad_model* quad_model_ptr, const shader* snake_shader_ptr)
{
	food* food_ptr = malloc(sizeof(food));
	food_ptr->quad_ptr = new_quad(quad_model_ptr, snake_shader_ptr);
	food_ptr->reset_pos = &food_reset_pos;
	food_ptr->draw = &food_draw;
	food_reset_pos(food_ptr);
	return food_ptr;
}

void delete_food(food* food_ptr)
{
	if (food_ptr == NULL) return;
	delete_quad(food_ptr->quad_ptr);
	free(food_ptr);
	food_ptr = NULL;
}

typedef struct game
{
	fps_counter* fps_counter_ptr;
	quad_model* quad_model_ptr;
	mat4* ortho_mat4_ptr;
	shader* snake_shader_ptr;
	shader* fbo_shader_ptr;
	fbo* fbo_ptr;
	snake* snake_ptr;
	food* food_ptr;
	float time;
	bool is_running;
	void (*init)(struct game* this);
	void (*update)(struct game* this, float delta);
	void (*draw)(struct game*);
	void (*send_key)(struct game* this, SDL_Keycode key, bool is_down);
} game;

void game_init(game* this)
{
}

void game_update(game* this, float delta)
{
	glClearColor(rand() % 100 / 1000.0f, rand() % 100 / 1000.0f, rand() % 100 / 1000.0f, 1.0f);
	this->time += delta;
	this->fps_counter_ptr->update(this->fps_counter_ptr, delta);
	this->snake_ptr->update(this->snake_ptr, delta);
	if ((this->food_ptr->x == this->snake_ptr->get_x(this->snake_ptr)) && (this->food_ptr->y == this->snake_ptr->get_y(this->snake_ptr)))
	{
		this->food_ptr->reset_pos(this->food_ptr);
		this->snake_ptr->increase(this->snake_ptr);
	}
	this->is_running &= this->snake_ptr->is_alive;
	if (!this->is_running) printf("Game over!\nYour final length: %d\nBetter luck next time!\n", this->snake_ptr->parts->size);
}

void game_draw(game* this)
{
	this->fbo_ptr->bind(this->fbo_ptr);
	glClear(GL_COLOR_BUFFER_BIT);
	this->snake_ptr->draw(this->snake_ptr, this->time, this->ortho_mat4_ptr);
	this->food_ptr->draw(this->food_ptr, this->time, this->snake_ptr->view_mat4_ptr, this->ortho_mat4_ptr);
	this->fbo_ptr->unbind(this->fbo_ptr);

	glClear(GL_COLOR_BUFFER_BIT);
	this->fbo_ptr->draw(this->fbo_ptr, this->time);
}

void game_send_key(game* this, SDL_Keycode key, bool is_down)
{
	if (key == SDLK_ESCAPE) this->is_running = false;
	if ((is_down) && (key == SDLK_z)) this->snake_ptr->turn(this->snake_ptr, false);
	if ((is_down) && (key == SDLK_x)) this->snake_ptr->turn(this->snake_ptr, true);
}

game* new_game(unsigned int width, unsigned int height)
{
	game* game_ptr = malloc(sizeof(game));
	game_ptr->fps_counter_ptr = new_fps_counter();
	game_ptr->quad_model_ptr = new_quad_model();
	game_ptr->ortho_mat4_ptr = new_mat4();
	mat4_set_ortho(game_ptr->ortho_mat4_ptr, 1.0f * width / height, 1);
	game_ptr->snake_shader_ptr = new_shader("snake");
	game_ptr->fbo_shader_ptr = new_shader("fbo");
	game_ptr->fbo_ptr = new_fbo(width, height, game_ptr->quad_model_ptr, game_ptr->fbo_shader_ptr);
	game_ptr->snake_ptr = new_snake(game_ptr->quad_model_ptr, game_ptr->snake_shader_ptr);
	game_ptr->food_ptr = new_food(game_ptr->quad_model_ptr, game_ptr->snake_shader_ptr);
	game_ptr->time = 0;
	game_ptr->is_running = true;
	game_ptr->update = &game_update;
	game_ptr->draw = &game_draw;
	game_ptr->send_key = &game_send_key;
	game_ptr->init = &game_init;
	game_ptr->init(game_ptr);
	return game_ptr;
}

void delete_game(game* game_ptr)
{
	if (game_ptr == NULL) return;
	delete_food(game_ptr->food_ptr);
	delete_snake(game_ptr->snake_ptr);
	delete_fbo(game_ptr->fbo_ptr);
	delete_shader(game_ptr->fbo_shader_ptr);
	delete_shader(game_ptr->snake_shader_ptr);
	delete_mat4(game_ptr->ortho_mat4_ptr);
	delete_quad_model(game_ptr->quad_model_ptr);
	delete_fps_counter(game_ptr->fps_counter_ptr);
	free(game_ptr);
	game_ptr = NULL;
}
