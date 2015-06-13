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

#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__

#include <vector>

namespace EventHandling
{

struct Event
{
    int type;
    void* data;
};

class EventHandler
{
public:
    EventHandler() {};
    virtual void Receive(Event& e) = 0;
};

class EventDispatcher
{
protected:
    std::vector<EventHandler*> handlers;
public:
    void AddHandler(EventHandler* new_handler);
    void RemoveHandler(EventHandler* handlerToRemove);
    EventDispatcher();
    void SendEvent(Event& e);
    void ClearHandlers();
};

void AddHandler(EventHandler* handler);
void RemoveHandler(EventHandler* handler);
void SendEvent(Event& e);
void ClearHandlers();

}

#endif