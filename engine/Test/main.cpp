#include <stdio.h>
#include "..\base\routines.h"
#include "..\base\graph.h"

int main(int argc, char** argv){
    SDL_SetAssertionHandler(EngineRoutines::handler, NULL);
    Graph g(640, 480, "test.ttf", "test");
    return 0;
}