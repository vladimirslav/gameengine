#include "gamescreen.h"

GameScreen::GameScreen(Graph& g)
    : g(&g)
{
    particle_timer.Reset();
}

GameScreen* GameScreen::Process()
{
    return this;
}

void GameScreen::Draw()
{

}

size_t GameScreen::GetCurrentFont()
{
    return 0;
}

int GameScreen::GetCurrentTime()
{
    return particle_timer.GetTicks();
}