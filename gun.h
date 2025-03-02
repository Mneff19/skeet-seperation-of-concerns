/***********************************************************************
 * Header File:
 *    GUN
 * Author:
 *    Br. Helfrich
 * Summary:
 *    The gun at the bottom of the screen
 ************************************************************************/

#pragma once
#include "position.h"

/*********************************************
 * GUN
 * The gun at the bottom of the screen
 *********************************************/
class GunStorage
{
public:
   GunStorage(const PositionStorage & pt) : angle(0.78 /* 45 degrees */), pt(pt) {}  // 45 degrees initially
   double getAngle() const { return angle; }
   PositionStorage getPt() {return pt;}
   void incrementAngle(double amount);
   void decrementAngle(double amount);
   void setAngle(double newAngle) { angle = newAngle; }
   
private:
   double angle;
   PositionStorage pt;
};

/*********************************************
 * GUN LOGIC
 * Handles the gun's interactions
 *********************************************/
class GunLogic
{
public:
   GunLogic() {}
   void interact(int clockwise, int counterclockwise, GunStorage* pGun);
   
};

class GunInterface {
public:
   void display(GunStorage* pGun) ;
};



