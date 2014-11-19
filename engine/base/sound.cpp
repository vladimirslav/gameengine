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

#include "sound.h"

#include "..\SDL2\include\SDL.h"
#include <unordered_map>
#include <vector>

using namespace EngineSound;
typedef std::unordered_map<std::string, sound_id> SoundIdMap;

static SoundIdMap preloadedSounds;
static std::vector<Mix_Chunk*> sounds;

static SoundIdMap preloadedMusic;
static std::vector<Mix_Music*> music;

static bool soundEnabled = true;

static int currentMusicVolume = MIX_MAX_VOLUME;
static int currentSoundVolume = MIX_MAX_VOLUME;

void EngineSound::InitAudio()
{
    ClearAudio();
    SDL_assert_release(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) == 0);
    //Mix_AllocateChannels(8);
}

void EngineSound::EnableSound()
{
    soundEnabled = true;
}

void EngineSound::DisableSound()
{
    soundEnabled = false;
}

sound_id EngineSound::LoadSound(std::string sound_file)
{
    if (preloadedSounds.count(sound_file) != 0)
    {
        return preloadedSounds.at(sound_file);
    }

    Mix_Chunk* result = Mix_LoadWAV(sound_file.c_str());
    SDL_assert_release(result != NULL);
    sounds.push_back(result);
    return sounds.size() - 1;
}

sound_id EngineSound::LoadMusic(std::string music_file)
{
    if (preloadedMusic.count(music_file) != 0)
    {
        return preloadedMusic.at(music_file);
    }

    Mix_Music* result = Mix_LoadMUS(music_file.c_str());
    SDL_assert_release(result != NULL);
    music.push_back(result);
    return music.size() - 1;
}

void EngineSound::StopMusic()
{
    Mix_PausedMusic();
    Mix_HaltMusic();
}

void EngineSound::ClearAudio()
{
    for (auto sound : sounds)
    {
        Mix_FreeChunk(sound);
    }
    sounds.clear();
    preloadedSounds.clear();

    for (auto sound : music)
    {
        Mix_FreeMusic(sound);
    }
    music.clear();
    preloadedMusic.clear();
    //Mix_AllocateChannels(0);
    Mix_CloseAudio();
}

void EngineSound::PlayMusic(sound_id m_id)
{
    if (music.size() > m_id)
    {
        SDL_assert_release(Mix_PlayMusic(music[m_id], -1) == 0);
        Mix_VolumeMusic(currentMusicVolume);
    }
}

void EngineSound::PlaySound(sound_id s_id)
{
    if (sounds.size() > s_id && soundEnabled)
    {
        Mix_PlayChannel(-1, sounds[s_id], 0);
    }
}

void EngineSound::FadeOutMusic(int ms)
{
    Mix_FadeOutMusic(ms);
}

void EngineSound::FadeInMusic(sound_id m_id, int ms)
{
    if (music.size() > m_id)
    {
        SDL_assert_release(Mix_FadeInMusic(music[m_id], -1, ms) == 0);
        Mix_VolumeMusic(currentMusicVolume);
    }
}

void EngineSound::SetMusicVolume(size_t percent)
{
    currentMusicVolume = static_cast<int>(MIX_MAX_VOLUME * static_cast<double>(percent) / 100);
    Mix_VolumeMusic(currentMusicVolume);
}
void EngineSound::SetSoundVolume(size_t percent)
{
    currentSoundVolume = static_cast<int>(MIX_MAX_VOLUME * static_cast<double>(percent) / 100);
    Mix_Volume(-1, currentSoundVolume);
}
