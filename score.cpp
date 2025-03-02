/***********************************************************************
 * Source File:
 *    Score
 * Author:
 *    Br. Helfrich
 * Summary:
 *    How well the player is doing
 ************************************************************************/

#include "score.h"
#include <sstream>
using namespace std;

/************************
 * SCORE GET TEXT
 * Get the status text
 ************************/
string ScoreStorage::getText() const
{
    ostringstream sout;
    sout << "Score:  " << points;
    return sout.str();
}

/************************
 * HIT RATIO  GET TEXT
 * Get the status text
 ************************/
string HitRatioStorage::getText() const
{
    ostringstream sout;
    sout << "Birds:   ";
    
    // compute the percentage when possible
    if (numMissed + numKilled)
        sout << (numKilled * 100 / (numMissed + numKilled)) << "%";
    else
        sout << "--";

    return sout.str();
}

/************************
 * HIT RATIO  ADJUST
 * Adjust the score for a bird hit (positive) or missed (negative)
 ************************/
void HitRatioLogic::adjust(int value, HitRatioStorage* hitRatioStorage)
{
    if (value > 0)
       hitRatioStorage->incrementNumKilled();
    else if (value < 0)
       hitRatioStorage->incrementNumMissed();
}
