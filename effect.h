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
   PositionStorage ptEnd;
public:
    // create a fragment based on the velocity and position of the bullet
    EffectStorage(ElementType effectType, const PositionStorage & pt) : ElementStorage( pt, 0.5) {}

    void accept(ElementLogic& logic, std::list<ElementStorage*>& elementList) override;

    // it is dead when age goes to 0.0
    bool isDead() const override { return age <= 0.0; }
};


class EffectLogic : public ElementLogic
{
public:


   // special functions
   void advance(ElementStorage &element);
   void draw(const EffectStorage &effect);
};
