#ifndef __PARTICLE_HELPERS__H__
#define __PARTICLE_HELPERS__H__

#include "particles.h"

namespace EngineParticles
{
    void CreateSplash(int particleAmnt,
        GLfloat _x,
        GLfloat _y,
        int lifetime,
        GLfloat dxMin,
        GLfloat dxMax,
        GLfloat dyMin,
        GLfloat dyMax,
        const GraphColor& _color,
        bool applyPhysics,
        int currentTime,
        GLfloat ddy);

    void CreateTexturedSplash(
        size_t texture,
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
        GLfloat ddy);

    void CreateConvergingParticleRow(
        size_t texture,
        int particleAmnt,
        MovementNodeCollection& c,
        const GraphColor& _color,
        GLfloat xStart,
        GLfloat xEnd,
        GLfloat yStart,
        GLfloat yEnd,
        int currentTime);

    // angle in degrees
    void CreateConvergingParticleCircle(
        size_t texture,
        int particleAmnt,
        MovementNodeCollection& c,
        const GraphColor& _color,
        GLfloat xCenter,
        GLfloat yCenter,
        GLfloat minAngle,
        GLfloat maxAngle,
        GLfloat radius,
        int currentTime,
        int delay = 0);

    void CreateSquare(
        int squaresInRow,
        int squaresInColumn,
        size_t squareW,
        size_t squareH,
        GLfloat leftX,
        GLfloat leftY,
        int currentTime,
        GLfloat radius,
        GraphColor color);
}

#endif