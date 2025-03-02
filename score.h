/***********************************************************************
 * Header File:
 *    Score
 * Author:
 *    Br. Helfrich
 * Summary:
 *    How well the player is doing
 ************************************************************************/

#pragma once
#include <string>

/**********************
 * STATUS
 * How well the player is doing
 **********************/
class StatusStorage
{
public:
    StatusStorage() {}
    virtual std::string getText() const = 0;
    virtual void reset() = 0;
};

/**********************
 * SCORE
 * Points earned vs lost
 **********************/
class ScoreStorage : public StatusStorage
{
public:
    ScoreStorage() { reset(); }
    std::string getText() const;
    void reset() { points = 0; }
   void adjustPoints(int value) { points += value; }
private:
    int points;
};

class ScoreLogic
{
public:
   ScoreLogic() {};
   void adjust(int value, ScoreStorage* scoreStorage) { scoreStorage->adjustPoints(value); };
};

/**********************
 * HIT RATIO
 * Bird hit ratio
 **********************/
class HitRatioStorage : public StatusStorage
{
public:
    HitRatioStorage()  { reset(); }
    std::string getText() const;
    void reset() { numKilled = numMissed = 0; }
   void incrementNumKilled() { numKilled++; }
   void incrementNumMissed() { numMissed++; }
private:
    int numKilled;
    int numMissed;
};

class HitRatioLogic
{
public:
   HitRatioLogic() {};
   void adjust(int value, HitRatioStorage* hitRatioStorage);
};
