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
class Points : public ElementStorage
{
public:
   Points(const PositionStorage& pt, int value);
   void show() const;
   void update();
   bool isDead() const {return age <= 0.0; }
private:

};
