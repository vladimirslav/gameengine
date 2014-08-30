#include "sound.h"

#include "..\SDL2\include\SDL.h"
#include <unordered_map>

using namespace EngineSound;
typedef std::unordered_map<std::string, sound_id> SoundIdMap;

static SoundIdMap preloadedSounds;
static std::vector<Mix_Chunk*> sounds;

static SoundIdMap preloadedMusic;
static std::vector<Mix_Music*> music;

void EngineSound::InitAudio()
{
    ClearAudio();
    SDL_assert_release(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) == 0);
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

void EngineSound::StopAudio()
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

    Mix_CloseAudio();
}

void EngineSound::PlayMusic(sound_id m_id)
{
    if (music.size() > m_id)
    {
        Mix_PlayMusic(music[m_id], -1);
    }
}
