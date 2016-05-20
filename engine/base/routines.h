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

#ifndef __COMMIEENGINE_SDL_ROUTINES_H__
#define __COMMIEENGINE_SDL_ROUTINES_H__

#include <SDL.h>
#include <string>
#include <unordered_map>

namespace EngineRoutines
{

    SDL_assert_state handler(const SDL_assert_data* data,
                             void*                  userdata);
    void ShowSimpleMsg(const char* msg);

    void InitRand();
    int GetRand(int min, int max);

    class SettingsFile
    {
    private:

        std::string fname;

        std::unordered_map<std::string, std::string> settings;
        bool saveOnClose;

    public:

        SettingsFile(const std::string& filename, bool save_on_close);

        std::string GetValue(const std::string& key);
        int GetValueAsInt(const std::string& key, int defaultValue = 0);

        void SetValue(const std::string& key, const std::string& value);

        void Save();

        ~SettingsFile();

        SettingsFile(const SettingsFile&) = delete;
        SettingsFile(SettingsFile&&) = delete;
        SettingsFile& operator=(SettingsFile&&) = delete;
        SettingsFile& operator=(const SettingsFile&) = delete;
        
    };

}
#endif