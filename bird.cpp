/***********************************************************************
 * Source File:
 *    Bird : Everything that can be shot
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Stuff that moves across the screen to be shot
 ************************************************************************/

#include <cassert>
#include "bird.h"
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


/***************************************************************/
/***************************************************************/
/*                             MISC.                           */
/***************************************************************/
/***************************************************************/


/******************************************************************
 * RANDOM
 * These functions generate a random number.
 ****************************************************************/
int randomInt(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);
   return num;
}
double randomFloat(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   assert(min <= num && num <= max);
   return num;
}

/***************************************************************/
/***************************************************************/
/*                         CONSTRUCTORS                         */
/***************************************************************/
/***************************************************************/

/***************************************************************
* BIRD STORAGE constructor
***************************************************************/
BirdStorage::BirdStorage(ElementType birdType)
: ElementStorage(false, birdType)
{
   double speed;

   switch (birdType)
   {
      case ElementType::Standard:
         radius = 25.0;
         speed  = 5.0;
         value = 10;

         // set the position: standard birds start from the middle
         pt.setY(randomFloat(HEIGHT * 0.25, HEIGHT * 0.75));
         pt.setX(0.0);

         // set the velocity
         v.setDx(randomFloat(speed - 0.5, speed + 0.5));
         v.setDy(randomFloat(-speed / 5.0, speed / 5.0));

         break;
      case ElementType::Floater:
         radius = 30.0;
         speed  = 5.0;
         value = 15;

         // floaters start on the lower part of the screen because they go up with time
         pt.setY(randomFloat(HEIGHT * 0.01, HEIGHT * 0.5));
         pt.setX(0.0);

         // set the velocity
         v.setDx(randomFloat(speed - 0.5, speed + 0.5));
         v.setDy(randomFloat(0.0, speed / 3.0));

         break;
      case ElementType::Crazy:
         radius = 30.0;
         speed  = 4.5;
         value = 30;

         // crazy birds start in the middle and can go any which way
         pt.setY(randomFloat(HEIGHT * 0.25, HEIGHT * 0.75));
         pt.setX(0.0);

         // set the velocity
         v.setDx(randomFloat(speed - 0.5, speed + 0.5));
         v.setDy(randomFloat(-speed / 5.0, speed / 5.0));

         break;
      case ElementType::Sinker:
         radius = 30.0;
         speed  = 4.5;
         value = 20;

         // sinkers start on the upper part of the screen because they go down with time
         pt.setY(randomFloat(HEIGHT * 0.50, HEIGHT * 0.95));
         pt.setX(0.0);

         // set the velocity
         v.setDx(randomFloat(speed - 0.5, speed + 0.5));
         v.setDy(randomFloat(-speed / 3.0, 0.0));

         break;
   }
}

 /***************************************************************/
 /***************************************************************/
 /*                            ADVANCE                          */
 /***************************************************************/
 /***************************************************************/
void BirdLogic::advance(BirdStorage &bird)
{
   switch (bird.getType())
   {
      case ElementType::Standard:
         // small amount of drag
         bird.multiplyV(0.995);

         // inertia
         bird.addInertia();

         // out of bounds checker
         if (isOutOfBounds(bird))
         {
            bird.kill();
            bird.multiplyPoints(-1); // points go negative when it is missed!
         }

         break;
      case ElementType::Floater:
         // large amount of drag
         bird.multiplyV(0.990);

         // inertia
         bird.addInertia();

         // anti-gravity
         bird.addDy(0.05);

         // out of bounds checker
         if (isOutOfBounds(bird))
         {
            bird.kill();
            bird.multiplyPoints(-1); // points go negative when it is missed!
         }

         break;
      case ElementType::Crazy:
         // How the crazy bird moves, every half a second it changes direciton
         // erratic turns eery half a second or so
         if (randomInt(0, 15) == 0)
         {
            bird.addDy(randomFloat(-1.5, 1.5));
            bird.addDx(randomFloat(-1.5, 1.5));
         }

         // inertia
         bird.addInertia();


         break;
      case ElementType::Sinker:
         // gravity
         bird.addDy(-0.07);

         // inertia
         bird.addInertia();
         break;
   }

   // out of bounds checker
   if (isOutOfBounds(bird))
   {
      bird.kill();
      bird.multiplyPoints(-1); // points go negative when it is missed!
   }
}

/***************************************************************/
/***************************************************************/
/*                             DRAW                            */
/***************************************************************/
/***************************************************************/

/************************************************************************
 * DRAW Disk
 * Draw a filled circule at [center] with size [radius]
 *************************************************************************/
void drawDisk(const PositionStorage& center, double radius,
              double red, double green, double blue)
{
   assert(radius > 1.0);
   const double increment = M_PI / radius;  // bigger the circle, the more increments

   // begin drawing
   glBegin(GL_TRIANGLES);
   glColor3f((GLfloat)red /* red % */, (GLfloat)green /* green % */, (GLfloat)blue /* blue % */);

   // three points: center, pt1, pt2
   PositionStorage pt1;
   pt1.setX(center.getX() + (radius * cos(0.0)));
   pt1.setY(center.getY() + (radius * sin(0.0)));
   PositionStorage pt2(pt1);

   // go around the circle
   for (double radians = increment;
      radians <= M_PI * 2.0 + .5;
      radians += increment)
   {
      pt2.setX(center.getX() + (radius * cos(radians)));
      pt2.setY(center.getY() + (radius * sin(radians)));

      glVertex2f((GLfloat)center.getX(), (GLfloat)center.getY());
      glVertex2f((GLfloat)pt1.getX(), (GLfloat)pt1.getY());
      glVertex2f((GLfloat)pt2.getX(), (GLfloat)pt2.getY());

      pt1 = pt2;
   }

   // complete drawing
   glEnd();
}

/************************************************************************
 * DRAW
 * Draw a bird
 *************************************************************************/
void BirdInterface::draw(BirdStorage &bird) const
{
   if (!bird.isDead())
   {
      switch (bird.getType())
      {
         case ElementType::Standard:
            // Draw a standard bird: blue center and white outline
            drawDisk(bird.getPosition(), bird.getRadius() - 0.0, 1.0, 1.0, 1.0); // white outline
            drawDisk(bird.getPosition(), bird.getRadius() - 3.0, 0.0, 0.0, 1.0); // blue center
            break;
         case ElementType::Floater:
            // Draw a floating bird: white center and blue outline
            drawDisk(bird.getPosition(), bird.getRadius() - 0.0, 0.0, 0.0, 1.0); // blue outline
            drawDisk(bird.getPosition(), bird.getRadius() - 4.0, 1.0, 1.0, 1.0); // white center
            break;
         case ElementType::Crazy:
            // Draw a crazy bird: concentric circles in a course gradient
            drawDisk(bird.getPosition(), bird.getRadius() * 1.0, 0.0, 0.0, 1.0); // bright blue outside
            drawDisk(bird.getPosition(), bird.getRadius() * 0.8, 0.2, 0.2, 1.0);
            drawDisk(bird.getPosition(), bird.getRadius() * 0.6, 0.4, 0.4, 1.0);
            drawDisk(bird.getPosition(), bird.getRadius() * 0.4, 0.6, 0.6, 1.0);
            drawDisk(bird.getPosition(), bird.getRadius() * 0.2, 0.8, 0.8, 1.0); // almost white inside
            break;
         case ElementType::Sinker:
            // Draw a sinker bird: black center and dark blue outline
            drawDisk(bird.getPosition(), bird.getRadius() - 0.0, 0.0, 0.0, 0.8);
            drawDisk(bird.getPosition(), bird.getRadius() - 4.0, 0.0, 0.0, 0.0);
            break;

      }
   }
}
