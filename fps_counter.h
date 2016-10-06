#pragma once

typedef struct fps_counter
{
	float time;
	unsigned int frames;
	void (*update)(struct fps_counter* this, float delta);
} fps_counter;

void fps_counter_update(fps_counter* this, float delta)
{
	this->time += delta;
	this->frames++;
	if (this->time >= 1)
	{
		printf("FPS: %d\n", this->frames);
		this->time = 0;
		this->frames = 0;
	}
}

fps_counter* new_fps_counter()
{
	fps_counter* fps_counter_ptr = malloc(sizeof(fps_counter));
	fps_counter_ptr->time = 0;
	fps_counter_ptr->frames = 0;
	fps_counter_ptr->update = &fps_counter_update;
	return fps_counter_ptr;
}

void delete_fps_counter(fps_counter* fps_counter_ptr)
{
	if (fps_counter_ptr == NULL) return;
	free(fps_counter_ptr);
	fps_counter_ptr = NULL;
}
