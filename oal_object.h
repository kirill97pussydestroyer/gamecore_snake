#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

typedef struct oal_object
{
	ALCdevice* device_ptr;
	ALCcontext* context_ptr;
} oal_object;

oal_object* new_oal_object()
{
	oal_object* oal_object_ptr = malloc(sizeof(oal_object));
	oal_object_ptr->device_ptr = alcOpenDevice(NULL);
	if (oal_object_ptr->device_ptr == NULL)
	{
		printf("Unable to create ALCDevice object\n");
		return NULL;
	}

	oal_object_ptr->context_ptr = alcCreateContext(oal_object_ptr->device_ptr, NULL);
	if (oal_object_ptr->context_ptr == NULL)
	{
		printf("Unable to create ALCContext object\n");
		return NULL;
	}
	if (!alcMakeContextCurrent(oal_object_ptr->context_ptr))
	{
		printf("Unable to make context current\n");
		return NULL;
	}

	ALfloat listener_dir[] = {0, 0, 1, 0, 1, 0};
	alListener3f(AL_POSITION, 0, 0, 0);
	alListener3f(AL_VELOCITY, 0, 0, 0);
	alListenerfv(AL_ORIENTATION, listener_dir);

	return oal_object_ptr;
}

void delete_oal_object(oal_object* oal_object_ptr)
{
	oal_object_ptr->device_ptr = alcGetContextsDevice(oal_object_ptr->context_ptr);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(oal_object_ptr->context_ptr);
	alcCloseDevice(oal_object_ptr->device_ptr);
	free(oal_object_ptr);
	oal_object_ptr = NULL;
}
