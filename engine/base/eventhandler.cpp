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

#include "eventhandler.h"
#include <algorithm>

namespace EventHandling
{

static EventDispatcher mainDispatcher;

EventDispatcher::EventDispatcher()
{

}

void EventDispatcher::AddHandler(EventHandler* new_handler)
{
    handlers.push_back(new_handler);
}

void EventDispatcher::RemoveHandler(EventHandler* handlerToRemove)
{
    if (handlers.size() > 0)
    {
        handlers.erase(std::remove(handlers.begin(), handlers.end(), handlerToRemove), handlers.end());
    }
}

void EventDispatcher::SendEvent(Event& e)
{
    for (auto handler : handlers)
    {
        handler->Receive(e);
    }
}

void EventDispatcher::ClearHandlers()
{
    handlers.clear();
}

void AddHandler(EventHandler* handler)
{
    mainDispatcher.AddHandler(handler);
}

void SendEvent(Event& e)
{
    mainDispatcher.SendEvent(e);
}

void ClearHandlers()
{
    mainDispatcher.ClearHandlers();
}

void RemoveHandler(EventHandler* handler)
{
    mainDispatcher.RemoveHandler(handler);
}

}