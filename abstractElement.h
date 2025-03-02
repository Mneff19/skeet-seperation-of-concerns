/***********************************************************************
 * Header File:
 *    Abstract Element Classes
 * Author:
 *    Jarom Anderson
 * Summary:
 *    Abstract Element Classs for all moving elements in the game
 ************************************************************************/
#pragma once
#include "position.h"
#include "constants.h"
#include <list>

enum ElementType
{
   // Birds
   Standard = 0,
   Floater  = 1,
   Crazy    = 2,
   Sinker   = 3,

   // Bullets
   Pellet   = 4,
   Bomb     = 5,
   Shrapnel = 6,
   Missile  = 7,

   // Effects
   Fragment = 8,
   Streek   = 9,
   Exhaust  = 10,

   // Points
   Points   = 11,
};

// Forward Declarations
class ElementLogic;
class BirdStorage;
class BulletStorage;
class EffectStorage;
class PointsStorage;

class ElementStorage
{
public:
   // CONSTRUCTORS

   // element Constructor
   ElementStorage(bool dead, ElementType elementType) : dead(dead), elementType(elementType) {}

   // Effect Constructor
   ElementStorage
   (
      PositionStorage pt = PositionStorage(),
      double age = 1.0
   )
   : pt(pt), age(age)
   {}

   // ATTRIBUTES
   ElementType elementType;
   PositionStorage pt;    // Position of the element
   VelocityStorage v;     // Velocity of the element
   double radius;         // Radius (size) of the element
   bool dead;             // is the element dead?
   int value;             // value of the element
   double age;            // 1.0 = new, 0.0 = dead

   // METHODS
public:
   // Getters
   virtual bool isDead()           const { return dead;        }
   PositionStorage getPosition()   const { return pt;          }
   VelocityStorage getVelocity()   const { return v;           }
   double getPositionX()           const { return pt.getX();   }
   double getPositionY()           const { return pt.getY();   }
   double getRadius()              const { return radius;      }
   int getValue()                  const { return value;       }
   ElementType getType()           const { return elementType; }

   // Setters
   void kill()                                 { dead = true;      }
   void setValue(int newValue)                 { value = newValue; }
   void operator=(const PositionStorage & rhs) { pt = rhs;         }
   void operator=(const VelocityStorage & rhs) { v = rhs;          }

   // Other Methods
   void multiplyV(double multiplicationFactor)          { v *= multiplicationFactor;      }
   void multiplyPoints(double multiplicationFactor)     { value *= multiplicationFactor;  }
   void addInertia()                                    { pt.add(v);                      }
   void addDy(double dy)                                { v.addDy(dy);                    }
   void addDx(double dx)                                { v.addDx(dx);                    }

   // Virtual Methods
   virtual void accept(ElementLogic& logic, std::list<ElementStorage*>& elementList) = 0; // The Visitor pattern's "accept" method
   virtual ~ElementStorage() {} // Virtual destructor

};


// Abstract Visitor
class ElementLogic
{
public:
   bool isOutOfBounds(ElementStorage &element) const
   {
      return (element.getPositionX() < -element.getRadius() || element.getPositionX() >= WIDTH  + element.getRadius() ||
         element.getPositionY() < -element.getRadius() || element.getPositionY() >= HEIGHT + element.getRadius());
   }

   virtual void turn(ElementStorage &element, double radians) = 0;
   virtual void input(ElementStorage &element, bool isUp, bool isDown, bool isB) = 0;

   // Specific overloads for each type
   virtual void advance(BirdStorage   &bird,   std::list<ElementStorage*>& elementList) = 0;
   virtual void advance(BulletStorage &bullet, std::list<ElementStorage*>& elementList) = 0;
   virtual void advance(EffectStorage &effect, std::list<ElementStorage*>& elementList) = 0;
   virtual void advance(PointsStorage &points, std::list<ElementStorage*> &elementList) = 0;

};

class ElementInterface
{
   virtual void draw(ElementStorage &element) const = 0;

};
