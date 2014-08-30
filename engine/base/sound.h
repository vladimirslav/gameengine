#ifndef __SOUND_H__
#define __SOUND_H__

#include "..\SDL2\include\SDL_mixer.h"
#include <string>

namespace EngineSound
{
    typedef size_t sound_id;

    void InitAudio();
    sound_id LoadSound(std::string sound_file);
    sound_id LoadMusic(std::string music_file);
    void StopAudio();
    void ClearAudio();

    void PlayMusic(sound_id m_id);
}

#endif