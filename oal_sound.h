#pragma once

#include <AL/al.h>
#include <AL/alut.h>

typedef struct oal_sound
{
	ALuint source;
	ALuint buffer;
	void (*set_pitch)(struct oal_sound* this, float pitch);
	void (*set_pos)(struct oal_sound* this, float x, float y, float z);
	void (*play)(struct oal_sound* this);
} oal_sound;

void oal_sound_play(oal_sound* this)
{
	alSourcePlay(this->source);
}

void oal_sound_set_pitch(oal_sound* this, float pitch)
{
	alSourcef(this->source, AL_PITCH, pitch);
}

void oal_sound_set_pos(oal_sound* this, float x, float y, float z)
{
	alSource3f(this->source, AL_POSITION, x, y, z);
}

oal_sound* new_oal_sound(const char* file_name)
{
	oal_sound* oal_sound_ptr = malloc(sizeof(oal_sound));
	alGenSources(1, &oal_sound_ptr->source);
	alSourcef(oal_sound_ptr->source, AL_PITCH, 1);
	alSourcef(oal_sound_ptr->source, AL_GAIN, 1);
	alSource3f(oal_sound_ptr->source, AL_POSITION, 0, 0, 0);
	alSource3f(oal_sound_ptr->source, AL_VELOCITY, 0, 0, 0);
	alSourcei(oal_sound_ptr->source, AL_LOOPING, AL_FALSE);

	ALenum format;
	ALvoid* data;
	ALsizei size;
	ALsizei freq;
	ALboolean loop = AL_FALSE;
	alGenBuffers(1, &oal_sound_ptr->buffer);
	alutLoadWAVFile((signed char*)file_name, &format, &data, &size, &freq, &loop);
	alBufferData(oal_sound_ptr->buffer, format, data, size, freq);
	alSourcei(oal_sound_ptr->source, AL_BUFFER, oal_sound_ptr->buffer);

	oal_sound_ptr->set_pitch = &oal_sound_set_pitch;
	oal_sound_ptr->set_pos = &oal_sound_set_pos;
	oal_sound_ptr->play = oal_sound_play;

	return oal_sound_ptr;
}

void delete_oal_sound(oal_sound* oal_sound_ptr)
{
	alDeleteSources(1, &oal_sound_ptr->source);
	alDeleteBuffers(1, &oal_sound_ptr->buffer);
	free(oal_sound_ptr);
	oal_sound_ptr = NULL;
}
