#pragma once

#include <AL/al.h>
#include <AL/alc.h>

class OpenALClass
{
    public:
        OpenALClass() = default;
        ~OpenALClass() = default;

        OpenALClass(const OpenALClass&) = delete;
        OpenALClass& operator=(const OpenALClass&) = delete;
        OpenALClass(OpenALClass&&) = delete;
        OpenALClass& operator=(OpenALClass&&) = delete;

        [[nodiscard]] bool Initialize();

        void Shutdown();

    private:

};