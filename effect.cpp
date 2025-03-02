/***********************************************************************
 * Source File:
 *    Fragment : Pieces that fly off a dead bird
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Pieces that fly off a dead bird
 ************************************************************************/

#include "effect.h"
#include <cassert>
#include "abstractElement.h"
#include "position.h"

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
double random(double min, double max)
{
   assert(min <= max);
   double num = min + ((double)rand() / (double)RAND_MAX * (max - min));
   assert(min <= num && num <= max);
   return num;
}

/************************************************************************
 * EFFECT STORAGE ACCEPT
 * Accepts a visitor
 *************************************************************************/
void EffectStorage::accept(ElementLogic &logic, std::list<ElementStorage*>& elementList) {
   // Second Dispatch
   logic.advance(*this, elementList);
}

/***************************************************************/
/***************************************************************/
/*                       CONSTRUCTORS                          */
/***************************************************************/
/***************************************************************/

EffectStorage::EffectStorage(ElementType effectType, const PositionStorage &pt, const VelocityStorage &velocity)
: ElementStorage(true, effectType)
{
   switch (elementType)
   {
      case ElementType::Fragment:
         // the velocity is a random kick plus the velocity of the thing that died
         this->v.setDx(v.getDx() * 0.5 + random(-6.0, 6.0));
         this->v.setDy(v.getDy() * 0.5 + random(-6.0, 6.0));

         // age
         age = random(0.4, 1.0);

         // size
         radius = random(1.0, 2.5);
         break;

      case ElementType::Streek:
         break;

      case ElementType::Exhaust:
         break;
   }

}

/************************************************************************
 * FRAGMENT constructor
 *************************************************************************/
Fragment::Fragment(const PositionStorage & pt, const VelocityStorage & v) : Effect(pt)
{

}

/************************************************************************
 * STREEK constructor
 *************************************************************************/
Streek::Streek(const PositionStorage & pt, VelocityStorage v) : Effect(pt)
{
   ptEnd = pt;
   v *= -1.0;
   ptEnd += v;

    // age
    age = 0.5;
}

/************************************************************************
 * EXHAUST constructor
 *************************************************************************/
Exhaust::Exhaust(const PositionStorage & pt, VelocityStorage v) : Effect(pt)
{
    ptEnd = pt;
    v *= -1.0;
    ptEnd += v;

    // age
    age = 0.5;
}

/***************************************************************/
/***************************************************************/
/*                           DRAW                              */
/***************************************************************/
/***************************************************************/

void EffectLogic::draw(const EffectStorage &effect)
{
    // Don't render if the effect is dead
    if (effect.isDead())
        return;

    EffectType type = effect.getEffectType();
    if (type == ElementType::Fragment)
    {
        glBegin(GL_TRIANGLE_FAN);
            glColor3f((GLfloat)effect.getAge(), (GLfloat)effect.getAge(), (GLfloat)effect.getAge());

            glVertex2f((GLfloat)(effect.getPosition().getX() - effect.getSize()),
                       (GLfloat)(effect.getPosition().getY() - effect.getSize()));
            glVertex2f((GLfloat)(effect.getPosition().getX() + effect.getSize()),
                       (GLfloat)(effect.getPosition().getY() - effect.getSize()));
            glVertex2f((GLfloat)(effect.getPosition().getX() + effect.getSize()),
                       (GLfloat)(effect.getPosition().getY() + effect.getSize()));
            glVertex2f((GLfloat)(effect.getPosition().getX() - effect.getSize()),
                       (GLfloat)(effect.getPosition().getY() + effect.getSize()));

            glColor3f(1.0f, 1.0f, 1.0f); // Reset color
            glEnd();
    }
    else if (type == ElementType::Exhaust || type == ElementType::Exhaust)
    {
            glBegin(GL_LINES);
            glColor3f((GLfloat)effect.getAge(), (GLfloat)effect.getAge(), (GLfloat)effect.getAge());

            glVertex2f((GLfloat)effect.getPosition().getX(),
                       (GLfloat)effect.getPosition().getY());
            glVertex2f((GLfloat)effect.getEndPosition().getX(),
                       (GLfloat)effect.getEndPosition().getY());

            glColor3f(1.0f, 1.0f, 1.0f); // Reset color
            glEnd();
    }
}

/***************************************************************/
 /***************************************************************/
 /*                            ADVANCE                          */
 /***************************************************************/
 /***************************************************************/
 void EffectLogic::advance(EffectStorage &effect)
{
    switch (effect.getEffectType())
    {
        case EffectType::Fragment:
        {
            effect.getPosition() += effect.getVelocity(); // Apply inertia
            effect.setAge(effect.getAge() - 0.02);
            effect.setSize(effect.getSize() * 0.95);
            break;
        }
        case EffectType::Streek:
        {
            effect.setAge(effect.getAge() - 0.10);
            break;
        }
        case EffectType::Exhaust:
        {
            effect.setAge(effect.getAge() - 0.025);
            break;
        }
        default:
            break;
    }

    // out of bounds checker
    if (isOutOfBounds(effect))
    {
       effect.kill();
       effect.multiplyPoints(-1); // points go negative when it is missed!
    }
}
