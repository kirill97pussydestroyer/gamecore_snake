#pragma once

typedef struct snake_part
{
	quad* quad_ptr;
	int x, y;
	void (*draw)(struct snake_part* this, float part_id, float time, const mat4* view_mat4_ptr, const mat4* ortho_mat4_ptr);
} snake_part;

void snake_part_draw(snake_part* this, float part_id, float time, const mat4* view_mat4_ptr, const mat4* ortho_mat4_ptr)
{
	mat4_set_to_identity(this->quad_ptr->model_mat4_ptr);
	mat4_scale(this->quad_ptr->model_mat4_ptr, 0.02f, 0.02f);
	mat4_translate(this->quad_ptr->model_mat4_ptr, this->x*2, this->y*2);
	glUseProgram(this->quad_ptr->shader_ptr->program);
	glUniform3f(this->quad_ptr->shader_ptr->u_color, 1, (part_id == 0 ? 0.3f : 1), (part_id == 0 ? 0.3f : 1));
	this->quad_ptr->draw(this->quad_ptr, time, view_mat4_ptr, ortho_mat4_ptr);
}

snake_part* new_snake_part(int x, int y, const quad_model* quad_model_ptr, const shader* snake_shader_ptr)
{
	snake_part* snake_part_ptr = malloc(sizeof(snake_part));
	snake_part_ptr->quad_ptr = new_quad(quad_model_ptr, snake_shader_ptr);
	snake_part_ptr->x = x;
	snake_part_ptr->y = y;
	snake_part_ptr->draw = &snake_part_draw;
	return snake_part_ptr;
}

void delete_snake_part(snake_part* snake_part_ptr)
{
	if (snake_part_ptr == NULL) return;
	delete_quad(snake_part_ptr->quad_ptr);
	free(snake_part_ptr);
	snake_part_ptr = NULL;
}

typedef struct snake
{
	bool is_alive;
	const quad_model* quad_model_ptr;
	const shader* snake_shader_ptr;
	mat4* view_mat4_ptr;
	float timer;
	float rotation_timer;
	float current_rotation;
	bool rotation_flag;
	bool can_turn;
	int rotation;
	vector* parts;
	int (*get_x)(struct snake* this);
	int (*get_y)(struct snake* this);
	int (*get_next_x)(struct snake* this);
	int (*get_next_y)(struct snake* this);
	void (*turn)(struct snake* this, bool right);
	void (*increase)(struct snake* this);
	void (*update)(struct snake* this, float delta);
	void (*draw)(struct snake* this, float time, const mat4* ortho_mat4_ptr);
} snake;

int snake_get_x(snake* this)
{
	return ((snake_part*)this->parts->begin(this->parts))->x;
}

int snake_get_y(snake* this)
{
	return ((snake_part*)this->parts->begin(this->parts))->y;
}

int snake_get_next_x(snake* this)
{
	return this->get_x(this) + (int)cos(this->rotation * M_PI_2);
}

int snake_get_next_y(snake* this)
{
	return this->get_y(this) + (int)sin(this->rotation * M_PI_2);
}

void snake_turn(snake* this, bool right)
{
	if (!this->can_turn) return;
	this->rotation += (right ? -1 : 1);
	this->rotation_timer = 0;
	this->rotation_flag = true;
	this->can_turn = false;
}

void snake_increase(snake* this)
{
	int new_x = this->get_x(this) - this->get_next_x(this) + ((snake_part*)this->parts->end(this->parts))->x;
	int new_y = this->get_y(this) - this->get_next_y(this) + ((snake_part*)this->parts->end(this->parts))->y;
	this->parts->push_back(this->parts, new_snake_part(new_x, new_y, this->quad_model_ptr, this->snake_shader_ptr));
}

void snake_update(snake* this, float delta)
{
	float interval = 0.1f + 0.4f * (M_PI_2 - atan(this->parts->size - 1)) / M_PI_2;
	this->timer += delta;
	if (this->timer >= interval)
	{
		this->timer = 0;
		snake_part* last_part_ptr = ((snake_part*)this->parts->end(this->parts));
		last_part_ptr->x = this->get_next_x(this);
		last_part_ptr->y = this->get_next_y(this);
		this->parts->pop_back(this->parts);
		this->parts->push_front(this->parts, last_part_ptr);
		this->can_turn = true;
	}

	if (this->rotation_flag)
	{
		float speed = interval / 2.0f;
		this->rotation_timer += delta;
		this->current_rotation += (this->current_rotation >= this->rotation * M_PI_2 ? -1 : 1) * M_PI_2 * delta / speed;
		if (this->rotation_timer >= speed) this->current_rotation = this->rotation * M_PI_2, this->rotation_flag = false;
		mat4_set_to_identity(this->view_mat4_ptr);
		mat4_rotate(this->view_mat4_ptr, this->current_rotation);
	}

	if ((abs(this->get_x(this)) > 12) || (abs(this->get_y(this)) > 12)) this->is_alive = false;
	bool check_collide(void* element_ptr, unsigned int i)
	{
		if (i == 0) return true;
		if ((this->get_x(this) == ((snake_part*)element_ptr)->x) && (this->get_y(this) == ((snake_part*)element_ptr)->y))
		{
			this->is_alive = false;
			return false;
		}
		return true;
	}
	this->parts->for_each(this->parts, &check_collide);
}

void snake_draw(snake* this, float time, const mat4* ortho_mat4_ptr)
{
	bool draw(void* element_ptr, unsigned int i) {
		((snake_part*)element_ptr)->draw((snake_part*)element_ptr, 1.0f * i / this->parts->size, time, this->view_mat4_ptr, ortho_mat4_ptr);
		return true;
	}
	this->parts->for_each(this->parts, &draw);
}

snake* new_snake(const quad_model* quad_model_ptr, const shader* snake_shader_ptr)
{
	snake* snake_ptr = malloc(sizeof(snake));
	snake_ptr->is_alive = true;
	snake_ptr->quad_model_ptr = quad_model_ptr;
	snake_ptr->snake_shader_ptr = snake_shader_ptr;
	snake_ptr->view_mat4_ptr = new_mat4();
	snake_ptr->timer = 0;
	snake_ptr->rotation_timer = 0;
	snake_ptr->current_rotation = 0;
	snake_ptr->rotation_flag = false;
	snake_ptr->rotation = 0;
	snake_ptr->parts = new_vector();
	snake_ptr->get_x = &snake_get_x;
	snake_ptr->get_y = &snake_get_y;
	snake_ptr->get_next_x = &snake_get_next_x;
	snake_ptr->get_next_y = &snake_get_next_y;
	snake_ptr->turn = &snake_turn;
	snake_ptr->increase = &snake_increase;
	snake_ptr->update = &snake_update;
	snake_ptr->draw = &snake_draw;

	snake_ptr->parts->push_back(snake_ptr->parts, new_snake_part(0, 0, snake_ptr->quad_model_ptr, snake_ptr->snake_shader_ptr));

	return snake_ptr;
}

void delete_snake(snake* snake_ptr)
{
	if (snake_ptr == NULL) return;
	bool clear(void* element_ptr, unsigned int i) { delete_snake_part((snake_part*)element_ptr); return true; }
	snake_ptr->parts->for_each(snake_ptr->parts, &clear);
	delete_vector(snake_ptr->parts);
	delete_mat4(snake_ptr->view_mat4_ptr);
	free(snake_ptr);
	snake_ptr = NULL;
}
