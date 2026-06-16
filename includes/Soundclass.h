#pragma once

#include <string_view>
#include <vector>
#include "Openalclass.h"

class SoundClass
{
    public:
        SoundClass() = default;
        ~SoundClass() = default;

        SoundClass(const SoundClass&) = delete;
        SoundClass& operator=(const SoundClass&) = delete;
        SoundClass(SoundClass&&) = delete;
        SoundClass& operator=(SoundClass&&) = delete;

        [[nodiscard]] bool LoadTrack(std::string_view filename, float volume);
        void ReleaseTrack();
        bool PlayTrack(bool loop);
        bool StopTrack();

    private:
        [[nodiscard]] bool LoadMP3File(std::string_view filename, int& outChannels, int& outSampleRate);

    private:
        unsigned int m_audioBufferId = 0;
        unsigned int m_audioSourceId = 0;

        std::vector<short> m_audioData;
};