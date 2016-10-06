#pragma once

typedef struct mat4
{
	float data[16];
} mat4;

mat4* new_mat4();
void delete_mat4(mat4* mat4_ptr);

float mat4_get(const mat4* this, unsigned int row, unsigned int col)
{
	return this->data[col * 4 + row];
}

void mat4_set(mat4* this, unsigned int row, unsigned int col, float value)
{
	this->data[col * 4 + row] = value;
}

void mat4_set_to_identity(mat4* this)
{
	memset(this->data, 0, 16 * sizeof(float));
	for (unsigned int i = 0; i < 4; i++) mat4_set(this, i, i, 1);
}

void mat4_mul(mat4* this, const struct mat4* right_matrix)
{
	mat4* tmp_mat4_ptr = new_mat4();
	for (unsigned int row = 0; row < 4; row++)
	{
		for (unsigned int col = 0; col < 4; col++)
		{
			float sum = 0;
			for (unsigned int i = 0; i < 4; i++) sum += mat4_get(this, row, i) * mat4_get(right_matrix, i, col);
			mat4_set(tmp_mat4_ptr, row, col, sum);
		}
	}
	memcpy(this->data, tmp_mat4_ptr->data, 16 * sizeof(float));
	delete_mat4(tmp_mat4_ptr);
}

void mat4_translate(mat4* this, float x, float y)
{
	mat4* translation_matrix_ptr = new_mat4();
	mat4_set(translation_matrix_ptr, 0, 3, x);
	mat4_set(translation_matrix_ptr, 1, 3, y);
	mat4_mul(this, translation_matrix_ptr);
	delete_mat4(translation_matrix_ptr);
}

void mat4_rotate(mat4* this, float angle)
{
	mat4* rotation_matrix_ptr = new_mat4();
	mat4_set(rotation_matrix_ptr, 0, 0,  cos(angle));
	mat4_set(rotation_matrix_ptr, 0, 1, -sin(angle));
	mat4_set(rotation_matrix_ptr, 1, 0,  sin(angle));
	mat4_set(rotation_matrix_ptr, 1, 1,  cos(angle));
	mat4_mul(this, rotation_matrix_ptr);
	delete_mat4(rotation_matrix_ptr);
}

void mat4_scale(mat4* this, float x, float y)
{
	mat4* scale_matrix_ptr = new_mat4();
	mat4_set(scale_matrix_ptr, 0, 0, x);
	mat4_set(scale_matrix_ptr, 1, 1, y);
	mat4_mul(this, scale_matrix_ptr);
	delete_mat4(scale_matrix_ptr);
}

void mat4_set_ortho(mat4* this, float aspect_ratio, float scale)
{
	mat4_set_to_identity(this);
	mat4_set(this, 0, 0,  2.0f / (scale * aspect_ratio));
	mat4_set(this, 1, 1,  2.0f /  scale);
	mat4_set(this, 2, 2, -1.0f);
}

void mat4_print(const mat4* this)
{
	for (unsigned char row = 0; row < 4; row++)
	{
		for (unsigned char col = 0; col < 4; col++) printf("%f ", mat4_get(this, row, col));
		printf("\n");
	}
}

mat4* new_mat4()
{
	mat4* mat4_ptr = malloc(sizeof(mat4));
	mat4_set_to_identity(mat4_ptr);
	return mat4_ptr;
}

void delete_mat4(mat4* mat4_ptr)
{
	free(mat4_ptr);
	mat4_ptr = NULL;
}
