/***********************************************************************
 * Header File:
 *    Fragment : Pieces that fly off a dead bird
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Pieces that fly off a dead bird
 ************************************************************************/

#pragma once
#include "abstractElement.h"
#include "position.h"

/**********************
 * Effect: 
 * Stuff that is not interactive
 **********************/
class EffectStorage : public ElementStorage
{
protected:

public:
    // create a fragment based on the velocity and position of the bullet
    EffectStorage(const PositionStorage & pt) : ElementStorage( pt, 0.5) {}

    // it is dead when age goes to 0.0
    bool isDead() const { return age <= 0.0; }
};


class EffectLogic : public ElementLogic
{
public:


   // special functions
   void advance(ElementStorage &element);
   void draw(const EffectStorage &effect);
};
