#pragma once

typedef struct
{
	char* data;
} file_reader;

file_reader* new_file_reader(const char* file_name)
{
	FILE* file = fopen(file_name, "rb");
	if (file == NULL)
	{
		printf("Can't open file: %s\n", file_name);
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);
	file_reader* file_reader_ptr = malloc(sizeof(file_reader));
	file_reader_ptr->data = malloc(size+1);
	fread(file_reader_ptr->data, size, 1, file);
	fclose(file);
	file_reader_ptr->data[size] = 0;
	return file_reader_ptr;
}

void delete_file_reader(file_reader* file_reader_ptr)
{
	if (file_reader_ptr == NULL) return;
	free(file_reader_ptr->data);
	free(file_reader_ptr);
	file_reader_ptr = NULL;
}

typedef struct
{
	GLuint program;
	GLuint u_random_seed, u_time, u_model, u_view, u_projection, u_color, u_fbo_texture;
	GLuint a_pos;
} shader;

void shader_error_check(const char* shader_name, GLuint shader_id)
{
	GLint success;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint shader_type;
		glGetShaderiv(shader_id, GL_SHADER_TYPE, &shader_type);

		GLint log_length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

		GLchar* message = malloc(log_length * sizeof(GLchar));
		glGetShaderInfoLog(shader_id, log_length, &log_length, message);
		printf("%s(%s): %s\n", shader_name, (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
		free(message);
	}
}

shader* new_shader(const char* shader_name)
{
	unsigned int vs_size = snprintf(NULL, 0, "%s%s%s", "shaders/", shader_name, "/vertex.glsl");
	char* vertex_shader_path = malloc(vs_size+1);
	snprintf(vertex_shader_path, vs_size+1, "%s%s%s", "shaders/", shader_name, "/vertex.glsl");
	file_reader* vs_reader_ptr = new_file_reader(vertex_shader_path);
	free(vertex_shader_path);

	unsigned int fs_size = snprintf(NULL, 0, "%s%s%s", "shaders/", shader_name, "/fragment.glsl");
	char* fragment_shader_path = malloc(fs_size+1);
	snprintf(fragment_shader_path, fs_size+1, "%s%s%s", "shaders/", shader_name, "/fragment.glsl");
	file_reader* fs_reader_ptr = new_file_reader(fragment_shader_path);
	free(fragment_shader_path);

	if ((vs_reader_ptr == NULL) || (fs_reader_ptr == NULL)) return NULL;

	shader* shader_ptr = malloc(sizeof(shader));

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, ((const char**)&vs_reader_ptr->data), NULL);
	glCompileShader(vertex_shader);
	delete_file_reader(vs_reader_ptr);
	shader_error_check(shader_name, vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, ((const char**)&fs_reader_ptr->data), NULL);
	glCompileShader(fragment_shader);
	delete_file_reader(fs_reader_ptr);
	shader_error_check(shader_name, fragment_shader);

	shader_ptr->program = glCreateProgram();
	glAttachShader(shader_ptr->program, vertex_shader);
	glAttachShader(shader_ptr->program, fragment_shader);
	glLinkProgram(shader_ptr->program);
	glDetachShader(shader_ptr->program, fragment_shader);
	glDetachShader(shader_ptr->program, vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	shader_ptr->u_random_seed = glGetUniformLocation(shader_ptr->program, "random_seed");
	shader_ptr->u_time = glGetUniformLocation(shader_ptr->program, "time");
	shader_ptr->u_model = glGetUniformLocation(shader_ptr->program, "model");
	shader_ptr->u_view = glGetUniformLocation(shader_ptr->program, "view");
	shader_ptr->u_projection = glGetUniformLocation(shader_ptr->program, "projection");
	shader_ptr->u_color = glGetUniformLocation(shader_ptr->program, "color");
	shader_ptr->u_fbo_texture = glGetUniformLocation(shader_ptr->program, "fbo_texture");

	shader_ptr->a_pos = glGetAttribLocation(shader_ptr->program, "pos");

	return shader_ptr;
}

void delete_shader(shader* shader_ptr)
{
	if (shader_ptr == NULL) return;
	glDeleteProgram(shader_ptr->program);
	free(shader_ptr);
	shader_ptr = NULL;
}
