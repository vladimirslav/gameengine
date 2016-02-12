/*
Author: Vladimir Slav

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __SOUND_H__
#define __SOUND_H__

#include "..\SDL2\include\SDL_mixer.h"
#include <string>

namespace EngineSound
{
    typedef size_t sound_id;

    void InitAudio();
    void FadeOutMusic(int ms);
    void FadeInMusic(sound_id m_id, int ms);
    sound_id LoadSound(std::string sound_file);
    sound_id LoadMusic(std::string music_file);
	void PauseMusic();
	void ResumeMusic();
    void StopMusic();
    void ClearAudio();

    void EnableSound();
    void DisableSound();

    void PlaySound(sound_id s_id, int loop = 0);
    void PlayMusic(sound_id m_id);

	void StopSounds();

    void SetMusicVolume(size_t percent);
    void SetSoundVolume(size_t percent);
}

#endif
