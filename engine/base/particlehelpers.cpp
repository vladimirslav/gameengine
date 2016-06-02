#include "particlehelpers.h"
#include <math.h>
using namespace EngineParticles;

void EngineParticles::CreateSplash(int particleAmnt, GLfloat _x, GLfloat _y, int lifetime, GLfloat dxMin, GLfloat dxMax, GLfloat dyMin, GLfloat dyMax, const GraphColor& _color, bool applyPhysics, int currentTime, GLfloat ddy)
{
    for (int i = 0; i < particleAmnt; i++)
    {
        Add(new SparkParticle(_x,
            _y,
            lifetime,
            dxMin + EngineRoutines::GetRandF() * (dxMax - dxMin),
            dyMin + EngineRoutines::GetRandF() * (dyMax - dyMin),
            currentTime,
            _color,
            applyPhysics,
            ddy));
    }
}

void EngineParticles::CreateTexturedSplash(size_t texture,
    int particleAmnt,
    GLfloat _xMin, 
    GLfloat _xMax,
    GLfloat _yMin,
    GLfloat _yMax,
    int lifetime,
    GLfloat dxMin,
    GLfloat dxMax,
    GLfloat dyMin,
    GLfloat dyMax,
    const GraphColor& _color,
    bool applyPhysics,
    int currentTime,
    GLfloat ddy)
{
    for (int i = 0; i < particleAmnt; i++)
    {
        Add(new SparkParticle(texture,
            _xMin + EngineRoutines::GetRandF() * (_xMax - _xMin),
            _yMin + EngineRoutines::GetRandF() * (_yMax - _yMin),
            lifetime,
            dxMin + EngineRoutines::GetRandF() * (dxMax - dxMin),
            dyMin + EngineRoutines::GetRandF() * (dyMax - dyMin),
            currentTime,
            _color,
            applyPhysics,
            ddy));
    }
}

void EngineParticles::CreateConvergingParticleRow(size_t texture,
                                                  int particleAmnt,
                                                  MovementNodeCollection& c,
                                                  const GraphColor& _color,
                                                  GLfloat xStart,
                                                  GLfloat xEnd,
                                                  GLfloat yStart,
                                                  GLfloat yEnd,
                                                  int currentTime)
{
    for (int i = 0; i < particleAmnt; i++)
    {
        Add(new TargetedMovingParticle(texture, 
            xStart + ((xEnd - xStart) / particleAmnt) * i,
            yStart + ((yEnd - yStart) / particleAmnt) * i,
            currentTime,
            c,
            _color));
    }
}

static const float PI = 3.14159f;// good enough
static const float RADS_IN_DEGREE = 2 * PI / 360.0f;

void EngineParticles::CreateConvergingParticleCircle(size_t texture,
    int particleAmnt,
    MovementNodeCollection& c,
    const GraphColor& _color,
    GLfloat xCenter,
    GLfloat yCenter,
    GLfloat minAngle,
    GLfloat maxAngle,
    GLfloat radius,
    int currentTime,
    int delay)
{
    float minAngleRadians = ((minAngle >= -0.01f && minAngle <= 0.01f) ? 1 : minAngle) * RADS_IN_DEGREE;
    float maxAngleRadians = maxAngle * RADS_IN_DEGREE;

    for (int i = 0; i < particleAmnt; i++)
    {
        if (delay != 0)
        {
            float ptX = xCenter + radius * std::cos(((maxAngleRadians - minAngleRadians) / particleAmnt) * i);
            float ptY = yCenter + radius * std::sin(((maxAngleRadians - minAngleRadians) / particleAmnt) * i);
            MovementNodeCollection v = c;
            v.insert(v.begin(), MovementNode{ ptX, ptY, delay * i, false});

            Add(new TargetedMovingParticle(texture,
                ptX,
                ptY,
                currentTime,
                v,
                _color));
        }
        else
        {
            Add(new TargetedMovingParticle(texture,
                xCenter + radius * std::cos(((maxAngleRadians - minAngleRadians) / particleAmnt) * i),
                yCenter + radius * std::sin(((maxAngleRadians - minAngleRadians) / particleAmnt) * i),
                currentTime,
                c,
                _color));
        }

    }
}

void EngineParticles::CreateSquare(
    int squaresInRow,
    int squaresInColumn,
    size_t squareW,
    size_t squareH,
    GLfloat leftX,
    GLfloat leftY,
    int currentTime,
    GLfloat radius,
    GraphColor color)
{
    float minAngleRadians = 0;
    float maxAngleRadians = 2 * PI;

    GLfloat xCenter = leftX + ((squaresInRow) * squareW / 2.0f);
    GLfloat yCenter = leftY + ((squaresInColumn) * squareH / 2.0f);
    size_t particleAmnt = squaresInRow * squaresInColumn;

    for (int i = 0; i < squaresInRow; i++)
    {
        for (int j = 0; j < squaresInColumn; j++)
        {
            MovementNodeCollection c{};
            c.push_back({leftX + squareW * i, leftY + squareH * j, 850, false});
            c.push_back({ leftX + squareW * i, leftY + squareH * j, 250, true });

            Add(new TargetedMovingParticle(xCenter + radius * std::cos(((maxAngleRadians - minAngleRadians) / particleAmnt) * ((j + 1) * (i + 1))),
                yCenter + radius * std::sin(((maxAngleRadians - minAngleRadians) / particleAmnt) * ((j + 1) * (i + 1))),
                currentTime,
                c,
                color,
                squareW,
                squareH));
        }
    }
}