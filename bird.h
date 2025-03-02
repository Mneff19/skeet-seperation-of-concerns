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


/**********************
 * BIRD
 * Everything that can be shot
 **********************/
class BirdStorage : public ElementStorage
{
protected:

public:
   BirdStorage(ElementType birdType);

   // Other
   void multiplyV(double multiplicationFactor)          { v *= multiplicationFactor;      }
   void multiplyPoints(double multiplicationFactor)     { value *= multiplicationFactor;  }
   void addDy(double dy)                                { v.addDy(dy);                    }
   void addDx(double dx)                                { v.addDx(dx);                    }





};

class BirdLogic : public ElementLogic
{
public:


   // special functions
   void advance(BirdStorage &bird);
};

class BirdInterface : public ElementInterface
{
public:
   // special functions
   void draw(BirdStorage &bird) const;
};
