#include "Openalclass.h"

bool OpenALClass::Initialize()
{
    ALCdevice* device = alcOpenDevice(nullptr);
    if (!device) {
        return false;
    }

    ALCcontext* context = alcCreateContext(device, nullptr);
    if (!context) {
        return false;
    }

    if (!alcMakeContextCurrent(context))
    {
        alcDestroyContext(context);
        alcCloseDevice(device);
        return false;
    }

    const float position[3] = { 0.0f, 0.0f, 0.0f };
    alGetError();
    alListenerfv(AL_POSITION, position);
    if (alGetError() != AL_NO_ERROR)
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        alcCloseDevice(device);
        return false;
    }
    return true;
}


void OpenALClass::Shutdown()
{
    ALCcontext* context = alcGetCurrentContext();
    if (context)
    {
        ALCdevice* device = alcGetContextsDevice(context);

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);

        if (device)
        {
            alcCloseDevice(device);
        }
    }
}