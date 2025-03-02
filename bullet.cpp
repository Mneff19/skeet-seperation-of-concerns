/
***********************************************************************
 * Source File:
 *    Bullet
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Stuff that kills other things
 ************************************************************************/

#include "bullet.h"
#include "abstractElement.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_18
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>         // OpenGL library we copied
#define _USE_MATH_DEFINES
#include <math.h>
#define GLUT_TEXT GLUT_BITMAP_HELVETICA_12
#endif // _WIN32


/******************************************************************
 * RANDOM
 * This function generates a random number.
 ****************************************************************/
int random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);
   return num;
}
double random(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   assert(min <= num && num <= max);
   return num;
}

/*********************************************
 * BULLET STORAGE constructor
 *********************************************/
BulletStorage::BulletStorage(double angle, ElementType elementType)
: ElementStorage(false), bulletType(bulletType)
{
   double speed;

   switch (elementType)
   {
      case BulletType::Pellet:
         speed = 15.0;
         radius = 1.0;
         value = 1;
         break;
      case BulletType::Bomb:
         speed = 10.0;
         radius = 4.0;
         value = 4;
         timeToDie = 60;
         break;
      case BulletType::Missile:
         speed = 10.0;
         radius = 1.0;
         value = 3;
         break;

      
      assert(elementType != ElementType::Shrapnel);
      
      // set the initial position
      pt.setX(WIDTH - 1.0);
      pt.setY(1.0);
      assert(pt.getX() > 100.0);

      // set the initial velocity
      v.setDx(-speed * cos(angle));
      v.setDy(speed * sin(angle));
      assert(v.getDx() <= 0.0);
      assert(v.getDy() >= 0.0);
      
   }

}

BulletStorage::BulletStorage(BulletStorage &bomb)
: ElementStorage(false), bulletType(ElementType::Shrapnel)
{
   assert(bomb.getType() == ElementType::Bomb);

   radius = 3.0;
   value = 0;

   // how long will this one live?
   timeToDie = random(5, 15);

   // The speed and direction is random
   v.set(random(0.0, 6.2), random(10.0, 15.0));
   // handle this
   pt = bomb.getPosition();

}

/*********************************************
 * BULLET LOGIC
 *********************************************/

/*********************************************
 * BULLET LOGIC ADVANCE
 *********************************************/
void BulletLogic::advance(ElementStorage &element, std::list<ElementStorage*> &elementList) override
{
   switch (element.elementType)
   {   
      case ElementType::Bomb:
         // kill if it has been around too long
         timeToDie--;
         if (!timeToDie)
            kill();
      break;
   
      case ElementType::Shrapnel:
         // kill if it has been around too long
         timeToDie--;
         if (!timeToDie)
            kill();
         
         // Add a streak
         elements.push_back(new Effect(ElementType::Streek, element.getPosition(), element.getVelocity()));
   
         break;
      case ElementType::Missile:
         // kill if it has been around too long
         effects->push_back(new Exhaust(pt, v));
         break;
   }

   // add inertia
   element.addInertia()
   // out of bounds checker
   if (isOutOfBounds())
      kill();

}

/*********************************************
 * BULLET LOGIC TURN
 *********************************************/

/*********************************************
 * BULLET INTERFACE
 *********************************************/




/*********************************************
 * BOMB DEATH
 * Bombs have a tendency to explode!
 *********************************************/
void Bomb::death(std::list<Bullet*>* bullets)
{
   for (int i = 0; i < 20; i++)
      bullets->push_back(new Shrapnel(*this));
}


/***************************************************************/
/***************************************************************/
/*                            OUTPUT                           */
/***************************************************************/
/***************************************************************/

/*************************************************************************
 * GL VERTEXT POINT
 * Just a more convenient format of glVertext2f
 *************************************************************************/
inline void glVertexPoint(const PositionStorage& point)
{
   glVertex2f((GLfloat)point.getX(), (GLfloat)point.getY());
}

/*********************************************
 * BULLET INTERFACE DRAW
 *********************************************/
void BulletInterface::Draw(ElementStorage &element)
{
   if (!element.isDead())
   {
      switch (element.elementType)
      {
         case ElementType::Pelet:
           drawDot(pt, 3.0, 1.0, 1.0, 0.0);
           break;
         case ElementType::Bomb:
            // Bomb actually has a gradient to cut out the harsh edges
            drawDot(pt, radius + 2.0, 0.50, 0.50, 0.00);
            drawDot(pt, radius + 1.0, 0.75, 0.75, 0.00);
            drawDot(pt, radius + 0.0, 0.87, 0.87, 0.00);
            drawDot(pt, radius - 1.0, 1.00, 1.00, 0.00);
            break;
         case ElementType::Shrapnel:
            drawDot(pt, radius, 1.0, 1.0, 0.0);
            break;
         case ElementType::Missile:
            // missile is a line with a dot at the end so it looks like fins.
            PositionStorage ptNext(pt);
            ptNext.add(v);
            drawLine(pt, ptNext, 1.0, 1.0, 0.0);
            drawDot(pt, 3.0, 1.0, 1.0, 1.0);
            break;
      }
   }
}

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *************************************************************************/
void BulletInterface::drawLine(const PositionStorage& begin, const PositionStorage& end,
                      double red, double green, double blue) const
{
   // Get ready...
   glBegin(GL_LINES);
   glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);

   // Draw the actual line
   glVertexPoint(begin);
   glVertexPoint(end);

   // Complete drawing
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/************************************************************************
 * DRAW DOT
 * Draw a single point (square actually on the screen, r pixels by r pixels
 *************************************************************************/
void BulletInterface::drawDot(const PositionStorage& point, double radius,
                     double red, double green, double blue) const
{
   // Get ready, get set...
   glBegin(GL_TRIANGLE_FAN);
   glColor3f((GLfloat)red, (GLfloat)green, (GLfloat)blue);
   double r = radius / 2.0;

   // Go...
   glVertex2f((GLfloat)(point.getX() - r), (GLfloat)(point.getY() - r));
   glVertex2f((GLfloat)(point.getX() + r), (GLfloat)(point.getY() - r));
   glVertex2f((GLfloat)(point.getX() + r), (GLfloat)(point.getY() + r));
   glVertex2f((GLfloat)(point.getX() - r), (GLfloat)(point.getY() + r));

   // Done!  OK, that was a bit too dramatic
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}