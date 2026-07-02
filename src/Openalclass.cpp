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

    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
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

void OpenALClass::UpdateListener(float posX, float posY, float posZ,
                                                                                            float forwardX, float forwardY, float forwardZ,
                                                                                            float upX, float upY, float upZ)
{
    alListener3f(AL_POSITION, posX, posY, posZ);
    const float orientation[6] = { forwardX, forwardY, forwardZ, upX, upY, upZ };
    alListenerfv(AL_ORIENTATION, orientation);
}
                                                                                        