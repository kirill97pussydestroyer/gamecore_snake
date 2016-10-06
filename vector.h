#pragma once

typedef struct vector
{
	void** elements;
	unsigned int size;
	void* (*at)(struct vector* this, unsigned int index);
	void* (*begin)(struct vector* this);
	void* (*end)(struct vector* this);
	void (*push_back)(struct vector* this, void* element_ptr);
	void (*push_front)(struct vector* this, void* element_ptr);
	void (*pop_back)(struct vector* this);
	void (*for_each)(struct vector* this, bool (*processor)(void* element, unsigned int i));
} vector;

void* vector_at(vector* this, unsigned int index)
{
	if (index >= this->size)
	{
		printf("Vector error: Out of bounds(size: %d, index: %d)\n", this->size, index);
		return NULL;
	}
	return this->elements[index];
}

void* vector_begin(vector* this)
{
	return this->at(this, 0);
}

void* vector_end(vector* this)
{
	return this->at(this, this->size-1);
}

void vector_push_back(vector* this, void* element_ptr)
{
	this->elements = realloc(this->elements, ++this->size * sizeof(void*));
	this->elements[this->size-1] = element_ptr;
}

void vector_push_front(vector* this, void* element_ptr)
{
	this->elements = realloc(this->elements, ++this->size * sizeof(void*));
	memmove(&this->elements[1], &this->elements[0], (this->size - 1) * sizeof(void*));
	this->elements[0] = element_ptr;
}

void vector_pop_back(vector* this)
{
	if (this->size == 0) printf("Vector error: can't pop_back(size: %d)\n", this->size);
	else this->elements = realloc(this->elements, --this->size * sizeof(void*));
}

void vector_for_each(vector* this, bool (*processor)(void* element_ptr, unsigned int i))
{
	for (unsigned int i = 0; i < this->size; i++)
	{
		if (!processor(this->at(this, i), i)) break;
	}
}

vector* new_vector()
{
	vector* vector_ptr = malloc(sizeof(vector));
	vector_ptr->elements = NULL;
	vector_ptr->size = 0;
	vector_ptr->at = &vector_at;
	vector_ptr->begin = &vector_begin;
	vector_ptr->end = &vector_end;
	vector_ptr->push_back = &vector_push_back;
	vector_ptr->push_front = &vector_push_front;
	vector_ptr->pop_back = &vector_pop_back;
	vector_ptr->for_each = &vector_for_each;
	return vector_ptr;
}

void delete_vector(vector* vector_ptr)
{
	if (vector_ptr == NULL) return;
	free(vector_ptr->elements);
	free(vector_ptr);
	vector_ptr = NULL;
}
