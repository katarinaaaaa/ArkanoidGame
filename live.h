#pragma once
#include "object.h"

class Live : public Object {
public:
    Live(float x, float y, float width, float height);

    void DeleteLive(float width, float height);
    void ReturnLive(float width, float height); //for game restart
};
