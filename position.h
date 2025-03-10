/***********************************************************************
 * Header File:
 *    Position : The representation of a position on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/


#pragma once

#include <iostream>
#include <cmath>

class VelocityStorage;

/*********************************************
 * POINT
 * A single position.  
 *********************************************/
class PositionStorage
{
public:
   // constructors
   PositionStorage()            : x(0.0), y(0.0)  {}
   PositionStorage(double x, double y);
   PositionStorage(const PositionStorage & pt) : x(pt.x), y(pt.y) {}

   // getters
   double getX()       const { return x;              }
   double getY()       const { return y;              }
   bool operator == (const PositionStorage & rhs) const
   {
      return x == rhs.x && y == rhs.y;
   }
   bool operator != (const PositionStorage & rhs) const
   {
      return x != rhs.x || y != rhs.y;
   }

   // setters
   void setX(double x)       { this->x = x;           }
   void setY(double y)       { this->y = y;           }
   void addX(double dx)      { setX(getX() + dx);     }
   void addY(double dy)      { setY(getY() + dy);     }
   void add(const VelocityStorage & v);
   PositionStorage & operator += (const VelocityStorage & v);
   PositionStorage & operator = (const PositionStorage & rhs)
   {
      x = rhs.x;
      y = rhs.y;
      return *this;
   }

private:
   double x;           // horizontal position
   double y;           // vertical position
};

/*********************************************
 * VELOCITY
 * Movement
 *********************************************/
class VelocityStorage
{
public:
   // constructors
   VelocityStorage()            : dx(0.0), dy(0.0)  {}
   VelocityStorage(double dx, double dy);
   VelocityStorage(const VelocityStorage & v) : dx(v.dx), dy(v.dy) {}

   // getters
   double getDx()       const { return dx;              }
   double getDy()       const { return dy;              }
   bool operator == (const VelocityStorage & rhs) const
   {
      return dx == rhs.dx && dy == rhs.dy;
   }
   bool operator != (const VelocityStorage & rhs) const
   {
      return dx != rhs.dx || dy != rhs.dy;
   }
   double getSpeed() const
   {
      return sqrt(dx * dx + dy * dy);
   }

   // setters
   void setDx(double dx)       { this->dx = dx;   }
   void setDy(double dy)       { this->dy = dy;   }
   void addDx(double dx)       { this->dx += dx;  }
   void addDy(double dy)       { this->dy += dy;  }
   VelocityStorage & operator += (const VelocityStorage & v)
   {
      addDx(v.getDx());
      addDy(v.getDy());
      return *this;
   }
   void add(const VelocityStorage & v)
   {
      *this += v;
   }
   VelocityStorage & operator = (const VelocityStorage & rhs)
   {
      dx = rhs.dx;
      dy = rhs.dy;
      return *this;
   }
   VelocityStorage & operator *= (double mult)
   {
      dx *= mult;
      dy *= mult;
      return *this;
   }
   VelocityStorage operator* (double mult)
   {
      VelocityStorage v(*this);
      v *= mult;
      return v;
   }
   void set(double angle, double speed)
   {
      dx = sin(angle) * speed;
      dy = cos(angle) * speed;
   }
   void turn(double radians = 0.04)
   {
      set(atan2(dx, dy) + radians, getSpeed());      
   }

private:
   double dx;           // horizontal VelocityStorage
   double dy;           // vertical VelocityStorage
};

// stream I/O useful for debugging
std::ostream & operator << (std::ostream & out, const PositionStorage & pt);
std::istream & operator >> (std::istream & in,        PositionStorage & pt);

inline double max(double x, double y) { return (x > y) ? x : y; }
inline double min(double x, double y) { return (x > y) ? y : x; }

double minimumDistance(const PositionStorage & pt1, const VelocityStorage & v1,
                       const PositionStorage & pt2, const VelocityStorage & v2);
