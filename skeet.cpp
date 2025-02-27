/**********************************************************************
 * Skeet
 * No actual birds were killed in the making of this game.
 **********************************************************************/

#include <string>
#include <sstream>
#include "skeet.h"
using namespace std;


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


/************************
 * SKEET INTERACT
 * handle all user input
 ************************/
void SkeetInterface::interact(const UserInput & ui)
{
   // reset the game
   if (skeetLogic.isGameOver() && ui.isSpace())
   {
      skeetLogic.getTime().reset();
      skeetLogic.getScore().reset();
      skeetLogic.getHitRatio().reset();
      return;
   }

   // gather input from the interface
   skeetLogic.pGun()->interact(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft());

   // a pellet can be shot at any time
   if (ui.isSpace())
      skeetLogic.newPellet();
   // missiles can be shot at level 2 and higher
   else if (ui.isM() && skeetLogic.getLevel() > 1)
      skeetLogic.newMissile();
   // bombs can be shot at level 3 and higher
   else if (ui.isB() && skeetLogic.getLevel() > 2)
      skeetLogic.newBomb();
   
   skeetLogic.setBullseye(ui.isShift());
   
   // send movement information to all the bullets. Only the missile cares.
   for (auto bullet : skeetLogic.getBullets())
      bullet->input(ui.isUp() + ui.isRight(), ui.isDown() + ui.isLeft(), ui.isB());
   
   
   // move the stuff
   skeetLogic.animate();

   // output the stuff
   if (skeetLogic.isPlaying())
      drawLevel();
   else
      drawStatus();
}

/************************************************************************
 * DRAW Background
 * Fill in the background
 *  INPUT color   Background color
 *************************************************************************/
void SkeetInterface::drawBackground(double redBack, double greenBack, double blueBack) const
{
   glBegin(GL_TRIANGLE_FAN);

   // two rectangles is the fastest way to fill the screen.
   glColor3f((GLfloat)redBack /* red % */, (GLfloat)greenBack /* green % */, (GLfloat)blueBack /* blue % */);
   glVertex2f((GLfloat)0.0, (GLfloat)0.0);
   glVertex2f((GLfloat)skeetLogic.getDimensionsX(), (GLfloat)0.0);
   glVertex2f((GLfloat)skeetLogic.getDimensionsX(), (GLfloat)skeetLogic.getDimensionsY());
   glVertex2f((GLfloat)0.0, (GLfloat)skeetLogic.getDimensionsY());

   glEnd();
}

/************************************************************************
 * DRAW Timer
 * Draw a large timer on the screen
 *  INPUT percent     Amount of time left
 *        Foreground  Foreground color
 *        Background  Background color
 *************************************************************************/
void SkeetInterface::drawTimer(double percent,
                     double redFore, double greenFore, double blueFore,
                     double redBack, double greenBack, double blueBack) const
{
   double radians;

   GLfloat length = (GLfloat)skeetLogic.getDimensionsX();
   GLfloat half = length / (GLfloat)2.0;

   // do the background stuff
   drawBackground(redBack, greenBack, blueBack);

   // foreground stuff
   radians = percent * M_PI * 2.0;
   GLfloat x_extent = half + length * (GLfloat)sin(radians);
   GLfloat y_extent = half + length * (GLfloat)cos(radians);

   // get read to draw the triangles
   glBegin(GL_TRIANGLE_FAN);
   glColor3f((GLfloat)redFore /* red % */, (GLfloat)greenFore /* green % */, (GLfloat)blueFore /* blue % */);
   glVertex2f(half, half);

   // fill in the triangles, one eight at a time
   switch ((int)(percent * 8.0))
   {
   case 7: // 315 - 360
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, length);
      break;
   case 6: // 270 - 315
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      glVertex2f(0.0, half);
      break;
   case 5: // 225 - 270
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(0.0, 0.0);
      break;
   case 4: // 180 - 225
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, 0.0);
      glVertex2f(half, 0.0);
      break;
   case 3: // 135 - 180
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, half);
      glVertex2f(length, 0.0);
      break;
   case 2: // 90 - 135 degrees
      glVertex2f(half, length);
      glVertex2f(length, length);
      glVertex2f(length, half);
      break;
   case 1: // 45 - 90 degrees
      glVertex2f(half, length);
      glVertex2f(length, length);
      break;
   case 0: // 0 - 45 degrees
      glVertex2f(half, length);
      break;
   }
   glVertex2f(x_extent, y_extent);

   // complete drawing
   glEnd();

   // draw the red line now
   glBegin(GL_LINES);
   glColor3f((GLfloat)0.6, (GLfloat)0.0, (GLfloat)0.0);
   glVertex2f(half, half);
   glVertex2f(x_extent, y_extent);
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);
   glEnd();
}

/*************************************************************************
 * DRAW TEXT
 * Draw text using a simple bitmap font
 *   INPUT  topLeft   The top left corner of the text
 *          text      The text to be displayed
 ************************************************************************/
void drawText(const Position& topLeft, const char* text) 
{
   void* pFont = GLUT_TEXT;
   glColor3f((GLfloat)1.0 /* red % */, (GLfloat)1.0 /* green % */, (GLfloat)1.0 /* blue % */);

   // prepare to output the text from the top-left corner
   glRasterPos2f((GLfloat)topLeft.getX(), (GLfloat)topLeft.getY());

   // loop through the text
   for (const char* p = text; *p; p++)
      glutBitmapCharacter(pFont, *p);
}
void drawText(const Position & topLeft, const string & text)
{
   drawText(topLeft, text.c_str());
}

/************************
 * DRAW BULLSEYE
 * Put a bullseye on the screen
 ************************/
void SkeetInterface::drawBullseye(double angle) const
{
   // find where we are pointing
   double distance = skeetLogic.getDimensionsX();
   GLfloat x = skeetLogic.getDimensionsX() - distance * cos(angle);
   GLfloat y = distance * sin(angle);

   // draw the crosshairs
   glBegin(GL_LINES);
   glColor3f((GLfloat)0.6, (GLfloat)0.6, (GLfloat)0.6);

   // Draw the actual lines
   glVertex2f(x - 10.0, y);
   glVertex2f(x + 10.0, y);

   glVertex2f(x, y - 10.0);
   glVertex2f(x, y + 10.0);

   glColor3f((GLfloat)0.2, (GLfloat)0.2, (GLfloat)0.2);
   glVertex2f(skeetLogic.getDimensionsX(), 0.0);
   glVertex2f(x, y);

   // Complete drawing
   glEnd();
}

/************************
 * SKEET DRAW LEVEL
 * output everything that will be on the screen
 ************************/
void SkeetInterface::drawLevel() const
{
   // output the background
   drawBackground(skeetLogic.getLevel() * .1, 0.0, 0.0);
   
   // draw the bullseye
   if (skeetLogic.getBullseye())
      drawBullseye(skeetLogic.getGunAngle());

   // output the gun
   skeetLogic.getGun().display();
         
   // output the birds, bullets, and fragments
   for (auto& pts : skeetLogic.getPoints())
      pts.show();
   for (auto effect : skeetLogic.getEffects())
      effect->render();
   for (auto bullet : skeetLogic.getBullets())
      bullet->output();
   for (auto element : skeetLogic.getBirds())
      element->draw();
   
   // status
   drawText(Position(10,                         skeetLogic.getDimensionsY() - 30), skeetLogic.getScore().getText()  );
   drawText(Position(skeetLogic.getDimensionsX() / 2 - 30, skeetLogic.getDimensionsY() - 30), skeetLogic.getTime().getText()   );
   drawText(Position(skeetLogic.getDimensionsX() - 110,    skeetLogic.getDimensionsY() - 30), skeetLogic.getHitRatio().getText());
}

/************************
 * SKEET DRAW STATUS
 * place the status message on the center of the screen
 ************************/
void SkeetInterface::drawStatus() const
{
   // output the text information
   ostringstream sout;
   if (skeetLogic.isGameOver())
   {
      // draw the end of game message
      drawText(Position(skeetLogic.getDimensionsX() / 2 - 30, skeetLogic.getDimensionsY() / 2 + 10),
               "Game Over");

      // draw end of game status
      drawText(Position(skeetLogic.getDimensionsX() / 2 - 30, skeetLogic.getDimensionsY() / 2 - 10),
               skeetLogic.getScore().getText());
   }
   else
   {
      // output the status timer
      drawTimer(1.0 - skeetLogic.percentLeft(),
                     (skeetLogic.getLevel() - 0.0) * .1, 0.0, 0.0,
                     (skeetLogic.getLevel() - 1.0) * .1, 0.0, 0.0);

      // draw the message giving a countdown
      sout << "Level " << skeetLogic.getLevel()
           << " begins in " << skeetLogic.secondsLeft() << " seconds";
      drawText(Position(skeetLogic.getDimensionsX() / 2 - 110, skeetLogic.getDimensionsY() / 2 - 10),
         sout.str());
   }
}

/******************************************************************
 * RANDOM
 * This function generates a random number.
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min < max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}

/************************
 * SKEET ANIMATE
 * move the gameplay by one unit of time
 ************************/
void SkeetLogic::animate()
{
   skeetStorage.incrementTime();
   
   // if status, then do not move the game
   if (getTime().isStatus())
   {
      // get rid of the bullets and the birds without changing the score
      skeetStorage.clearBirds();
      skeetStorage.clearBullets();
      skeetStorage.clearEffects();
      skeetStorage.clearPoints();
      return;
   }
   
   // spawn
   spawn();
   
   // move the birds and the bullets
   for (auto element : getBirds())
   {
      element->advance();
      skeetStorage.adjustHitRatio(element->isDead() ? -1 : 0);
   }
   for (auto bullet : getBullets())
      bullet->move(skeetStorage.pEffects());
   for (auto effect : getEffects())
      effect->fly();
   for (auto it = skeetStorage.getPointsStart(); it != skeetStorage.getPointsEnd(); ++it)
      it->update();
      
   // hit detection
   for (auto element : getBirds())
      for (auto bullet : getBullets())
         if (!element->isDead() && !bullet->isDead() &&
             element->getRadius() + bullet->getRadius() >
             minimumDistance(element->getPosition(), element->getVelocity(),
                             bullet->getPosition(),  bullet->getVelocity()))
         {
            for (int i = 0; i < 25; i++)
               skeetStorage.newEffect(new Fragment(bullet->getPosition(), bullet->getVelocity()));
            element->kill();
            bullet->kill();
            skeetStorage.adjustHitRatio(1);
            bullet->setValue(-(element->getPoints()));
            element->setPoints(0);
         }
   
   // remove the zombie birds
   for (auto it = skeetStorage.getBirdsStart(); it != skeetStorage.getBirdsEnd();)
      if ((*it)->isDead())
      {
         if ((*it)->getPoints())
            skeetStorage.newPoints(Points((*it)->getPosition(), (*it)->getPoints()));
         skeetStorage.adjustScore((*it)->getPoints());
         it = skeetStorage.removeBird(it);
      }
      else
         ++it;
       
   // remove zombie bullets
   for (auto it = skeetStorage.getBulletsStart(); it != skeetStorage.getBulletsEnd(); )
      if ((*it)->isDead())
      {
         (*it)->death(skeetStorage.pBullets());
         int value = -(*it)->getValue();
         skeetStorage.addPoints(Points((*it)->getPosition(), value));
         skeetStorage.adjustScore(value);
         it = skeetStorage.removeBullet(it);
      }
      else
         ++it;
   
   // remove zombie fragments
   for (auto it = skeetStorage.getEffectsStart(); it !=  skeetStorage.getEffectsEnd();)
      if ((*it)->isDead())
         it = skeetStorage.removeEffect(it);
      else
         ++it;

   // remove expired points
   for (auto it = skeetStorage.getPointsStart(); it != skeetStorage.getPointsEnd();)
      if ((*it).isDead())
         it = skeetStorage.removePoints(it);
      else
         ++it;
}

/************************
 * SKEET SPAWN
 * lanuch new birds
 ************************/
void SkeetLogic::spawn()
{
   double size;
   switch (getTime().level())
   {
      // in level 1 spawn big birds occasionally
      case 1:
         size = 30.0;
         // spawns when there is nothing on the screen
         if (skeetStorage.birdsSize() == 0 && random(0, 15) == 1)
            skeetStorage.addBirds(new Standard(size, 7.0));
         
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Standard(size, 7.0));
         break;
         
      // two kinds of birds in level 2
      case 2:
         size = 25.0;
         // spawns when there is nothing on the screen
         if (skeetStorage.birdsSize() == 0 && random(0, 15) == 1)
            skeetStorage.addBirds(new Standard(size, 7.0, 12));

         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Standard(size, 5.0, 12));
         // spawn every 3 seconds
         if (random(0, 3 * 30) == 1)
            skeetStorage.addBirds(new Sinker(size));
         break;
      
      // three kinds of birds in level 3
      case 3:
         size = 20.0;
         // spawns when there is nothing on the screen
         if (skeetStorage.birdsSize() == 0 && random(0, 15) == 1)
            skeetStorage.addBirds(new Standard(size, 5.0, 15));

         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Standard(size, 5.0, 15));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Sinker(size, 4.0, 22));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Floater(size));
         break;
         
      // three kinds of birds in level 4
      case 4:
         size = 15.0;
         // spawns when there is nothing on the screen
         if (skeetStorage.birdsSize() == 0 && random(0, 15) == 1)
            skeetStorage.addBirds(new Standard(size, 4.0, 18));

         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Standard(size, 4.0, 18));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Sinker(size, 3.5, 25));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Floater(size, 4.0, 25));
         // spawn every 4 seconds
         if (random(0, 4 * 30) == 1)
            skeetStorage.addBirds(new Crazy(size));
         break;
         
      default:
         break;
   }
}
