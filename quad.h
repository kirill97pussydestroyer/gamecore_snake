#pragma once

typedef struct
{
	triangle triangles[2];
	GLuint vbo, vao;
} quad_model;

quad_model* new_quad_model()
{
	quad_model* quad_model_ptr = malloc(sizeof(quad_model));
	quad_model_ptr->triangles[0] = (triangle){{-1, -1}, {-1, 1}, {1,  1}};
	quad_model_ptr->triangles[1] = (triangle){{-1, -1}, { 1, 1}, {1, -1}};

	glGenBuffers(1, &quad_model_ptr->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_model_ptr->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_model_ptr->triangles), quad_model_ptr->triangles, GL_STATIC_DRAW);

	glGenVertexArrays(1, &quad_model_ptr->vao);
	glBindVertexArray(quad_model_ptr->vao);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	return quad_model_ptr;
}

void delete_quad_model(quad_model* quad_model_ptr)
{
	if (quad_model_ptr == NULL) return;
	free(quad_model_ptr);
	quad_model_ptr = NULL;
}

typedef struct quad
{
	mat4* model_mat4_ptr;
	const quad_model* model_ptr;
	const shader* shader_ptr;
	void (*draw)(struct quad* this, float time, const mat4* view_mat4_ptr, const mat4* ortho_mat4_ptr);
} quad;

void quad_draw(quad* this, float time, const mat4* view_mat4_ptr, const mat4* ortho_mat4_ptr)
{
	glUseProgram(this->shader_ptr->program);
	glUniform1f(this->shader_ptr->u_random_seed, rand() % 10000 / 10000.0f);
	glUniform1f(this->shader_ptr->u_time, time);
	glUniformMatrix4fv(this->shader_ptr->u_model, 1, GL_FALSE, this->model_mat4_ptr->data);
	if (view_mat4_ptr != NULL) glUniformMatrix4fv(this->shader_ptr->u_view, 1, GL_FALSE, view_mat4_ptr->data);
	if (ortho_mat4_ptr != NULL) glUniformMatrix4fv(this->shader_ptr->u_projection, 1, GL_FALSE, ortho_mat4_ptr->data);
	glBindBuffer(GL_ARRAY_BUFFER, this->model_ptr->vbo);
	glBindVertexArray(this->model_ptr->vao);
	glEnableVertexAttribArray(this->shader_ptr->a_pos);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(this->model_ptr->triangles));
	glDisableVertexAttribArray(this->shader_ptr->a_pos);
}

quad* new_quad(const quad_model* model_ptr, const shader* shader_ptr)
{
	quad* quad_ptr = malloc(sizeof(quad));
	quad_ptr->model_mat4_ptr = new_mat4();
	quad_ptr->model_ptr = model_ptr;
	quad_ptr->shader_ptr = shader_ptr;
	quad_ptr->draw = &quad_draw;
	return quad_ptr;
}

void delete_quad(quad* quad_ptr)
{
	if (quad_ptr == NULL) return;
	delete_mat4(quad_ptr->model_mat4_ptr);
	free(quad_ptr);
	quad_ptr = NULL;
}
