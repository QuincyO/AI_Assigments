#pragma once
#include "rlImGui.h"
#include "Math.h"




struct Rigidbody
{

    Rigidbody(float angularSpeed, Vector2 position)
        :angularSpeed{ angularSpeed }
        , pos{ position }
        , accel{ 0 }
        , velo{ 0 }
        , rotation{ 0 }
    {
    }

    Vector2 pos = {};
    Vector2 accel = {};
    Vector2 velo = {};
    float rotation;
    float angularSpeed;

};
