#pragma once

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
