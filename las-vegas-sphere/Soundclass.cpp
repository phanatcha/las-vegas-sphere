#define DR_MP3_IMPLEMENTATION
#include  "dr_mp3.h"
#include "Soundclass.h"
#include <string>



bool SoundClass::LoadMP3File(std::string_view filename, int& outChannels, int& outSampleRate)
{
    drmp3_config config;
    drmp3_uint64 totalPCMFrameCount;

    std::string safePath(filename);

    drmp3_int16* pSampleData = drmp3_open_file_and_read_pcm_frames_s16(safePath.c_str(), &config, &totalPCMFrameCount, nullptr);

    if (pSampleData == nullptr)
    {
        return false;
    }

    size_t totalSamples = totalPCMFrameCount * config.channels;
    m_audioData.assign(pSampleData, pSampleData + totalSamples);

    drmp3_free(pSampleData, nullptr);

    outChannels = config.channels;
    outSampleRate = config.sampleRate;

    return true;
}

bool SoundClass::LoadTrack(std::string_view filename, float volume)
{
    alGetError();

    alGenBuffers(1, &m_audioBufferId);
    alGenSources(1, &m_audioSourceId);
    if (alGetError() != AL_NO_ERROR) return false;

    // ~~~~~~~~~>Decode the MP3<~~~~~~~~~~~~~
    int channels = 0;
    int sampleRate = 0;
    if (!LoadMP3File(filename, channels, sampleRate)) 
    {
        return false;
    }

    // ~~~~~~~~~>MP3 = Mono or Stereo?<~~~~~~~~~~~~~
    ALenum format = (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

    // ~~~~~~~~~>Upload vector data<~~~~~~~~~~~~~
    alBufferData(m_audioBufferId, format, m_audioData.data(), m_audioData.size() * sizeof(short), sampleRate);
    if (alGetError() != AL_NO_ERROR ) return false;

    // ~~~~~~~~~>Empty RAM<~~~~~~~~~~~~~
    m_audioData.clear();

    // ~~~~~~~~~>Attach buffer to source and set the volume<~~~~~~~~~~~~~
    alSourcei(m_audioSourceId, AL_BUFFER, m_audioBufferId);
    alSourcef(m_audioSourceId, AL_GAIN, volume);

    return true;

}

void SoundClass::ReleaseTrack()
{
    if (m_audioSourceId != 0)
    {
        alDeleteSources(1, &m_audioSourceId);
        m_audioSourceId = 0;
    }

    if(m_audioBufferId != 0) 
    {
        alDeleteBuffers(1, &m_audioBufferId);
        m_audioBufferId = 0;
    }
}

bool SoundClass::PlayTrack(bool looping)
{
    alGetError();
    alSourcei(m_audioSourceId, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
    alSourcePlay(m_audioSourceId);

    return (alGetError() == AL_NO_ERROR);
}

bool SoundClass::StopTrack()
{
    alGetError();
    alSourceStop(m_audioSourceId);
    return (alGetError() == AL_NO_ERROR);
}

void SoundClass::SetPosition(float x, float y, float z)
{
    alGetError();
    alSource3f(m_audioSourceId, AL_POSITION, x, y, z);
}

void SoundClass::SetAttenuation(float referenceDistance, float rolloffFactor)
{
    alSourcef(m_audioSourceId, AL_REFERENCE_DISTANCE, referenceDistance);
    alSourcef(m_audioSourceId, AL_ROLLOFF_FACTOR, rolloffFactor);
}