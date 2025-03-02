/***********************************************************************
 * Header File:
 *    Points : Points graphic drawn on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Inert point values on the screen.
 ************************************************************************/


#pragma once

#include "abstractElement.h"
#include "position.h"

 /*********************************************
  * POINTS
  * Points graphic drawn on the screen
  *********************************************/
class PointsStorage : public ElementStorage
{
public:
   PointsStorage(const PositionStorage& pt, int value);

   // Accept Visitor
   void accept(ElementLogic& logic, std::list<ElementStorage*>& elementList) override;

   void show() const;

   bool isDead() const override {return age <= 0.0; }
private:

};

class PointsLogic : public ElementLogic
{
public:

   virtual void advance(PointsStorage &points, std::list<ElementStorage*> &elementList);
   // Specific overloads for each type
   virtual void advance(BirdStorage   &bird,   std::list<ElementStorage*>& elementList) {}
   virtual void advance(BulletStorage &bullet, std::list<ElementStorage*>& elementList) {}
   virtual void advance(EffectStorage &effect, std::list<ElementStorage*>& elementList) {}
};

class PointsInterface : public ElementInterface
{
public:
   virtual void draw(ElementStorage &element) const override;

}
