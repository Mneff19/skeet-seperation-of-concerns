/***********************************************************************
 * Header File:
 *    SKEET
 * Author:
 *    Br. Helfrich
 * Summary:
 *    The game class
 ************************************************************************/

#pragma once

#include "abstractElement.h"
#include "position.h"
#include "uiInteract.h"
#include "bird.h"
#include "bullet.h"
#include "effect.h"
#include "gun.h"
#include "time.h"
#include "score.h"
#include "points.h"

#include <list>

/*************************************************************************
 * Skeet
 * The game class
 *************************************************************************/
class SkeetStorage
{
public:
    SkeetStorage(PositionStorage & dimensions) : dimensions(dimensions),
        gun(PositionStorage(800.0, 0.0)), time(), score(), hitRatio(), bullseye(false) {}

    // is the game currently playing right now?
    bool isPlaying() const { return time.isPlaying();  }

   double getDimensionsX() const { return dimensions.getX(); }
   double getDimensionsY() const { return dimensions.getY(); }
   int getLevel() const { return time.level(); }
   bool getBullseye() const { return bullseye; }
   bool isGameOver() const { return time.isGameOver(); }
   double percentLeft() const { return time.percentLeft(); }
   int secondsLeft() const { return time.secondsLeft(); }
   double getGunAngle() const { return gun.getAngle(); }
   void setBullseye(bool newBullseye) { bullseye = newBullseye; }
   bool isStatus() const { return time.isStatus(); }
   std::string timeText() const { return time.getText(); }
   void incrementTime() { time++; }
   void clearBullets() { bullets.clear(); }
   void clearBirds() { birds.clear(); }
   void clearEffects() { effects.clear(); }
   void clearPoints() { points.clear(); }
   void addBirds(Bird* bird) { birds.push_back(bird); }
   void addEffects(Effect* effect) { effects.push_back(effect); }
   void addPoints(Points point) { points.push_back(point); }
   void addEffect(Effect* effect) { effects.push_back(effect); }
   int birdsSize() const { return (int)birds.size(); }

   // Birds
   std::list<BirdStorage*>::const_iterator getBirdsStart() const { return birds.begin(); }
   // std::list<BirdStorage*>::iterator getBirdsEnd() const  { return birds.end();   }
   // std::list<BirdStorage*>::iterator removeBird(std::list<BirdStorage*>::iterator it) { return birds.erase(it); }
   void newBird(BirdStorage* bird) { birds.push_back(bird); }

   std::list<Bullet*>::iterator getBulletsStart() { return bullets.begin(); }
   std::list<Bullet*>::iterator getBulletsEnd() { return bullets.end(); }
   std::list<Effect*>::iterator getEffectsStart() { return effects.begin(); }
   std::list<Effect*>::iterator getEffectsEnd() { return effects.end(); }
   std::list<Points>::iterator getPointsStart() { return points.begin(); }
   std::list<Points>::iterator getPointsEnd() { return points.end(); }

   std::list<Bullet*>::iterator removeBullet(std::list<Bullet*>::iterator it) { return bullets.erase(it); }
   std::list<Effect*>::iterator removeEffect(std::list<Effect*>::iterator it) { return effects.erase(it); }
   std::list<Points>::iterator removePoints(std::list<Points>::iterator it) { return points.erase(it); }
   void newBullet(Bullet* newBullet) { bullets.push_back(newBullet); }

   void newEffect(Effect* newEffect) { effects.push_back(newEffect); }
   void newPoints(Points newPoints) { points.push_back(newPoints); }

   void reset() {
      time.reset();
      hitRatio.reset();
      score.reset();
   }
   HitRatioStorage* pHitRatio() { return &hitRatio; }
   ScoreStorage* pScore() { return &score; }
   std::string hitRatioText() const { return hitRatio.getText(); }
   std::string scoreText() const { return score.getText(); }


   // TEMP PLEASE DO NOT KEEP:(((
   Gun getGun() const { return gun; }
   Gun* pGun() { return &gun; }
   std::list<Points> getPoints() const { return points;}
   std::list<Points>* pPoints() { return &points;}
   std::list<Effect*> getEffects() const { return effects;}
   std::list<Bullet*> getBullets() const { return bullets;}

   std::list<Effect*>* pEffects() { return &effects; }
   std::list<Bullet*>* pBullets() { return &bullets; }
private:

    Gun gun;                              // the gun
    std::list<ElementStorage*> elements;  // The elements
    // std::list<BirdStorage*> birds;     // logic for the birds
    // std::list<Bullet*>      bullets;   // the bullets
    // std::list<Effect*>      effects;   // the fragments of a dead bird.
    std::list<Points>       points;       // point values;
    TimeStorage time;                     // how many frames have transpired since the beginning
    ScoreStorage score;                   // the player's score
    HitRatioStorage hitRatio;             // the hit ratio for the birds
    PositionStorage dimensions;           // size of the screen
    bool bullseye;
};


class SkeetLogic
{
public:
   SkeetLogic(PositionStorage & dimensions) : skeetStorage(SkeetStorage(dimensions)) {}

   // move the gameplay by one unit of time
   void animate();
   // generate new birds
   void spawn();
   double getDimensionsX() const { return skeetStorage.getDimensionsX(); }
   double getDimensionsY() const { return skeetStorage.getDimensionsY(); }
   int getLevel() const { return skeetStorage.getLevel(); }
   bool getBullseye() const { return skeetStorage.getBullseye(); }
   double getGunAngle() const { return skeetStorage.getGunAngle(); }
   bool isGameOver() const { return skeetStorage.isGameOver(); }
   double percentLeft() const { return skeetStorage.percentLeft(); }
   int secondsLeft() const { return skeetStorage.secondsLeft(); }
   bool isPlaying() const { return skeetStorage.isPlaying(); }
   std::string timeText() const { return skeetStorage.timeText(); }
   void setBullseye(bool newBullseye) { skeetStorage.setBullseye(newBullseye); }
   void newPellet() { skeetStorage.newBullet(new Pellet(getGunAngle())); }
   void newMissile() { skeetStorage.newBullet(new Missile(getGunAngle())); }
   void newBomb() { skeetStorage.newBullet(new Bomb(getGunAngle())); }

   void newStandardBird(double radius, double speed, int points) { skeetStorage.newBird(new Standard(radius, speed, points)); }
   void newSinkerBird(double radius, double speed, int points) { skeetStorage.newBird(new Sinker(radius, speed, points)); }
   void newFloaterBird(double radius, double speed, int points) { skeetStorage.newBird(new Floater(radius, speed, points)); }
   void newCrazyBird(double radius, double speed, int points) { skeetStorage.newBird(new Crazy(radius, speed, points)); }
   void reset() { skeetStorage.reset(); }
   std::string hitRatioText() const { return skeetStorage.hitRatioText(); }
   std::string scoreText() const { return skeetStorage.scoreText(); }


   Gun getGun() const { return skeetStorage.getGun(); }
   Gun* pGun() { return skeetStorage.pGun(); }
   std::list<Points> getPoints() const { return skeetStorage.getPoints(); }
   std::list<Effect*> getEffects() const { return skeetStorage.getEffects(); }
   std::list<Bullet*> getBullets() const { return skeetStorage.getBullets(); }
   std::list<BirdStorage*> getBirds() const { return skeetStorage.getBirds(); }

private:
   SkeetStorage skeetStorage;

   HitRatioLogic hitRatioLogic;
   ScoreLogic scoreLogic;

};

class SkeetInterface
{
public:
   // inherited method draw()
   SkeetInterface(PositionStorage & dimensions) : skeetLogic(SkeetLogic(dimensions)) {}
   // handle all user input
   void interact(const UserInput& ui);
private:
    // output everything on the screen
    void drawLevel()  const;    // output the game
    void drawStatus() const;    // output the status information
   void drawBackground(double redBack, double greenBack, double blueBack) const;
   void drawTimer(double percent,
                  double redFore, double greenFore, double blueFore,
                  double redBack, double greenBack, double blueBack) const;
   void drawBullseye(double angle) const;

private:
   SkeetLogic skeetLogic;

   // Interfaces
   BirdInterface birdInterface;
};
