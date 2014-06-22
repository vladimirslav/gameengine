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