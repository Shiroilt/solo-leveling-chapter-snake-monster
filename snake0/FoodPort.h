#ifndef FOOD_PORT_H
#define FOOD_PORT_H

#include "../snake0/Common.h"

class FoodPort {
public:
    virtual ~FoodPort() = default;
    virtual Point getPosition() = 0;
    virtual bool isActive() = 0;
    virtual void deactivate() = 0;
};

#endif
