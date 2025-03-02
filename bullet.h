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

   // setters


   // getters


   // special functions
   virtual void death(std::list<BulletStorage *>* bullets) {}
   virtual void output() = 0;
   virtual void input(bool isUp, bool isDown, bool isB) {}
   virtual void move(std::list<Effect*>* effects);


};

/*********************
 * BULLET LOGIC
 * The logic for all bullets
 **********************/
class BulletLogic : public ElementLogic
{
public:
   virtual void advance(ElementStorage &element, std::list<ElementStorage*> &elementList) override {}
   virtual void turn(double angle) override {}

   void death(std::list<ElementStorage *>* bullets);

   void input(ElementStorage &element, bool isUp, bool isDown, bool isB)
   {
      if (element.getType() == ElementType::Missile)
      {
         if (isUp)
            v.turn(0.04);
         if (isDown)
            v.turn(-0.04);
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
