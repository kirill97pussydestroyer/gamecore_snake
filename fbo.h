#pragma once

typedef struct fbo
{
	GLuint id, tex_id;
	quad* fbo_quad_ptr;
	void (*bind)(struct fbo* this);
	void (*unbind)(struct fbo* this);
	void (*draw)(struct fbo* this, float time);
} fbo;

void fbo_bind(fbo* this)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
}

void fbo_unbind(fbo* this)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void fbo_draw(fbo* this, float time)
{
	glUseProgram(this->fbo_quad_ptr->shader_ptr->program);
	glUniform1i(this->fbo_quad_ptr->shader_ptr->u_fbo_texture, 0);
	this->fbo_quad_ptr->draw(this->fbo_quad_ptr, time, NULL, NULL);
}

fbo* new_fbo(unsigned int width, unsigned int height, const quad_model* quad_model_ptr, const shader* fbo_shader_ptr)
{
	fbo* fbo_ptr = malloc(sizeof(fbo));

	glGenTextures(1, &fbo_ptr->tex_id);
	glBindTexture(GL_TEXTURE_2D, fbo_ptr->tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glGenFramebuffers(1, &fbo_ptr->id);
	fbo_bind(fbo_ptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_ptr->tex_id, 0);
	GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers);
	fbo_unbind(fbo_ptr);

	fbo_ptr->fbo_quad_ptr = new_quad(quad_model_ptr, fbo_shader_ptr);
	fbo_ptr->bind = &fbo_bind;
	fbo_ptr->unbind = &fbo_unbind;
	fbo_ptr->draw = &fbo_draw;

	return fbo_ptr;
}

void delete_fbo(fbo* fbo_ptr)
{
	if (fbo_ptr == NULL) return;
	glDeleteFramebuffers(1, &fbo_ptr->id);
	free(fbo_ptr);
	fbo_ptr = NULL;
}
