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

#ifndef __GAME_INVENTORY_H__
#define __GAME_INVENTORY_H__

namespace GameInventory
{
    template <typename T> class Inventory
    {
    protected:
        std::vector<T> items;
        size_t maxSize;
    public:
        Inventory() : maxSize(0) {}

        Inventory(size_t size)
            : maxSize(size)
        {
            
        }

        void SetSize(size_t newSize)
        {
            maxSize = newSize;
        }

        size_t GetMaxSize()
        {
            return items.size();
        }

        // return false if inventory is full
        bool Add(T newItem) 
        {
            if (items.size() < maxSize)
            {
                items.push_back(newItem);
                return true;
            }

            return false;
        }

        bool GetItem(int index, T** targetItem)
        {
            if (index < (int)items.size())
            {
                *targetItem = &items[index];
                return true;
            }

            *targetItem = NULL;
            return false;
        }

        // return 
        bool Remove(int index)
        {
            if (index < (int)items.size())
            {
                items.erase(items.begin() + index);
                return true;
            }
            return false;
        }

        int GetRemainingSlotAmount() const
        {
            return maxSize - items.size();
        }

        size_t GetUsedSlotAmount() const
        {
            return items.size();
        }

        void Clean()
        {
            items.clear();
        }

    };
}

#endif