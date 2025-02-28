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
#include "constants.h"

enum class BirdType
{
   Standard = 0,
   Floater  = 1,
   Crazy    = 2,
   Sinker   = 3,
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
   void operator=(const PositionStorage    & rhs)       { pt = rhs;         }
   void operator=(const VelocityStorage & rhs)          { v = rhs;          }
   void kill()                                          { dead = true;      }
   void setPoints(int pts)                              { points = pts;     }

   // getters
   BirdType getBirdType()                         const { return birdType;  }
   bool isDead()                                  const { return dead;      }
   PositionStorage getPosition()                  const { return pt;        }
   VelocityStorage getVelocity()                  const { return v;         }
   double getRadius()                             const { return radius;    }
   int getPoints()                                const { return points;    }
   double getPositionX()                          const { return pt.getX(); }
   double getPositionY()                          const { return pt.getY(); }

   // Other
   void multiplyV(double multiplicationFactor)          { v *= multiplicationFactor;      }
   void multiplyPoints(double multiplicationFactor)     { points *= multiplicationFactor; }
   void addInertia()                                    { pt.add(v);                      }
   void addDy(double dy)                                { v.addDy(dy);                    }
   void addDx(double dx)                                { v.addDx(dx);                    }





};

class BirdLogic
{
public:
   bool isOutOfBounds(BirdStorage &bird) const
   {
      return (bird.getPositionX() < -bird.getRadius() || bird.getPositionX() >= WIDTH  + bird.getRadius() ||
              bird.getPositionY() < -bird.getRadius() || bird.getPositionY() >= HEIGHT + bird.getRadius());
   }

   // special functions
   void advance(BirdStorage &bird);
};

class BirdInterface
{
public:
   // special functions
   void draw(BirdStorage &bird) const;
};
