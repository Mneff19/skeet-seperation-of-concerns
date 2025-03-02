/***********************************************************************
 * Header File:
 *    Bullet
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Stuff that kills other things
 ************************************************************************/

#pragma once
#include "abstractElement.h"
// #include "position.h"
#include "effect.h"
#include <list>
#include <cassert>


/*********************************************
 * BULLET
 * Something to shoot something else
 *********************************************/
class BulletStorage : public ElementStorage
{
protected:
   int timeToDie;

public:
   BulletStorage(double angle, ElementType bulletType);
   BulletStorage(BulletStorage &bomb);

   // Accept Visitor
   void accept(ElementLogic& logic, std::list<ElementStorage*>& elementList) override;

};

/*********************
 * BULLET LOGIC
 * Concrete Visitor
 * The logic for all bullets
 **********************/
class BulletLogic : public ElementLogic
{
public:

   // Specific overloads for each type
   virtual void advance(BulletStorage &bullet, std::list<ElementStorage*>& elementList) override;
   // Unneeded
   virtual void advance(BirdStorage   &bird,   std::list<ElementStorage*>& elementList) override {}
   virtual void advance(EffectStorage &effect, std::list<ElementStorage*>& elementList) override {}
   virtual void advance(PointsStorage &points, std::list<ElementStorage*> &elementList) override {}

   virtual void turn(ElementStorage &element, double angle) override { element.v.turn(angle); }

   virtual void death(std::list<ElementStorage *>* elementList);

   virtual void input(ElementStorage &element, bool isUp, bool isDown, bool isB) override
   {
      if (element.getType() == ElementType::Missile)
      {
         if (isUp)
            turn(element, 0.04);
         if (isDown)
            turn(element, -0.04);
      }
   }

};

/*********************
 * BULLET Interface
 * The interface for all bullets
 **********************/
class BulletInterface : public ElementInterface
{

public:
   virtual void draw(ElementStorage &element) override;
protected:
   void drawLine(const PositionStorage& begin, const PositionStorage& end,
                 double red = 1.0, double green = 1.0, double blue = 1.0) const;

   void drawDot(const PositionStorage& point, double radius = 2.0,
                double red = 1.0, double green = 1.0, double blue = 1.0) const;
   // int    random(int    min, int    max);
   // double random(double min, double max);
};
