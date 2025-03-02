/***********************************************************************
 * Header File:
 *    Bird : Everything that can be shot
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Stuff that moves across the screen to be shot
 ************************************************************************/

#pragma once
#include "abstractElement.h"
#include "position.h"

// Forward Declarations
class BulletStorage;
class EffectStorage;
class PointsStorage;


/**********************
 * BIRD
 * Everything that can be shot
 **********************/
class BirdStorage : public ElementStorage
{
protected:

public:
   BirdStorage(ElementType birdType);
   BirdStorage(ElementType birdType, double radius);
   BirdStorage(ElementType birdType, double radius, double speed);
   BirdStorage(ElementType birdType, double radius, double speed, int value);

   // Accept visitor
   void accept(ElementLogic& logic, std::list<ElementStorage*>& elementList) override;

};

class BirdLogic : public ElementLogic
{
public:

   // Concrete Visitor implementations
   virtual void advance(BirdStorage   &bird, std::list<ElementStorage*>& elementList);

   // Unneeded, but unsure how to better apply visitor pattern
   virtual void advance(BulletStorage &bullet, std::list<ElementStorage*>& elementList) {};
   virtual void advance(EffectStorage &effect, std::list<ElementStorage*>& elementList) {};
   virtual void advance(PointsStorage &points, std::list<ElementStorage*> &elementList) {};
};

class BirdInterface : public ElementInterface
{
public:
   // special functions
   void draw(BirdStorage &bird) const;
};
