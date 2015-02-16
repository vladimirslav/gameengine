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

#include "routines.h"

#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>

SDL_assert_state EngineRoutines::handler(const SDL_assert_data* data,
                                         void*                  userdata)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "An error has occured",
        SDL_GetError(),
        NULL);
#ifdef _DEBUG
    return SDL_ASSERTION_BREAK;
#else
    return SDL_ASSERTION_ABORT;
#endif
}

int EngineRoutines::GetRand(int min, int max)
{
    int res = min + rand() % (max - min + 1);
    return res;
}

void EngineRoutines::InitRand()
{
    srand(static_cast<int>(time(NULL)));
}

EngineRoutines::SettingsFile::SettingsFile(const std::string& filename, bool save_on_close)
    : fname(filename)
    , saveOnClose(save_on_close)
{

    std::ifstream f(filename);
    if (f.good() == false) {
        // create file
        // TODO: add extra checks if the path is good
        f.open(filename, std::ios_base::out);
    }
    else
    {
        // solution from http://stackoverflow.com/questions/6892754/creating-a-simple-configuration-file-and-parser-in-c
        std::string line;
        while (std::getline(f, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '='))
            {
                std::string value;
                if (std::getline(is_line, value))
                    settings[key] = value;
            }
        }
    }
    f.close();

    /*
    const char config[] = "url=http://mysite.com\n"
        "file=main.exe\n"
        "true=0";

    std::istringstream is_file(config);

    std::string line;
    while (std::getline(is_file, line))
    {
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '='))
        {
            std::string value;
            if (std::getline(is_line, value))
                store_line(key, value);
        }
    }
    */
}

std::string EngineRoutines::SettingsFile::GetValue(const std::string& key)
{
    if (settings.find(key) != settings.end())
    {
        return settings[key];
    }

    return "";
}

int EngineRoutines::SettingsFile::GetValueAsInt(const std::string& key, int defaultValue)
{
    if (settings.find(key) != settings.end())
    {
        return std::atoi(settings[key].c_str());
    }

    SetValue(key, std::to_string(defaultValue));
    return defaultValue;
}

void EngineRoutines::SettingsFile::SetValue(const std::string& key, const std::string& value)
{
    settings[key] = value;
}

void EngineRoutines::SettingsFile::Save()
{
    std::fstream f;
    f.open(fname, std::ios_base::out);
    if (f.good())
    {
        for (const auto& setting : settings)
        {
            f << setting.first + "=" + setting.second << std::endl;
        }
        f.close();
    }
}

EngineRoutines::SettingsFile::~SettingsFile()
{
    if (saveOnClose)
    {
        Save();
    }
}
