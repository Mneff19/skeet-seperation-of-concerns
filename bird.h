/***********************************************************************
 * Header File:
 *    Bird : Everything that can be shot
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Stuff that moves across the screen to be shot
 ************************************************************************/

#pragma once
#include "position.h"

enum class BirdType
{
   Standard = 0,
   Floater = 1,
   Crazy = 2,
   Sinker = 3,
};

/**********************
 * BIRD
 * Everything that can be shot
 **********************/
class BirdStorage
{
protected:
   BirdType birdType;         // The type of bird (determines behavior)
   PositionStorage pt;        // position of the flyer
   VelocityStorage v;         // velocity of the flyer
   double radius;             // the size (radius) of the flyer
   bool dead;                 // is this flyer dead?
   int points;                // how many points is this worth?

public:
   BirdStorage(BirdType birdType);

   // setters
   void operator=(const PositionStorage    & rhs) { pt = rhs;    }
   void operator=(const VelocityStorage & rhs) { v = rhs;     }
   void kill()                          { dead = true; }
   void setPoints(int pts)              { points = pts;}

   // getters
   bool isDead()           const { return dead;   }
   PositionStorage getPosition()     const { return pt;     }
   VelocityStorage getVelocity()  const { return v;      }
   double getRadius()      const { return radius; }
   int getPoints() const { return points; }




};

class BirdLogic
{
   bool isOutOfBounds(BirdStorage &bird) const
   {
      return (pt.getX() < -bird.getRadius() || pt.getX() >= dimensions.getX() + bird.getRadius() ||
              pt.getY() < -bird.getRadius() || pt.getY() >= dimensions.getY() + bird.getRadius());
   }

   // special functions
   virtual void advance() = 0;
};

class BirdInterface
{
   // special functions
   virtual void draw() = 0;
};
