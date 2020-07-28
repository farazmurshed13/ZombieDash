#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include "Level.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip> 
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld()
    {
        cleanUp();
        cleanUp();
    }
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool canMove(double x, double y, Actor* a);
    bool flameBlocked(double x, double y);
    int citizensLeft() const;
    void activateOnAppropriateActors(Actor* a);
    void recordLevelFinishedIfAllCitizensGone();
    void displayText();
    // Add an actor to the world.
    void addActor(Actor* a);
    // Record that one more citizen on the current level is gone (exited,
    // died, or turned into a zombie).
    void recordCitizenGone();
    double EuclideanDistance(Actor* a, Actor* b);
  
    // Is there something at the indicated location that might cause a
    // zombie to vomit (i.e., a human)?
    bool isZombieVomitTriggerAt(double x, double y) const;
    
    // Return true if there is a living human, otherwise false.  If true,
    // otherX, otherY, and distance will be set to the location and distance
    // of the human nearest to (x,y).
    bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);

    
    // Return true if there is a living zombie or Penelope, otherwise false.
    // If true, otherX, otherY, and distance will be set to the location and
    // distance of the one nearest to (x,y), and isThreat will be set to true
    // if it's a zombie, false if a Penelope.
    bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const;
    
    // Return true if there is a living zombie, false otherwise.  If true,
    // otherX, otherY and distance will be set to the location and distance
    // of the one nearest to (x,y).
    bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const;
private:
    vector<Actor*> actors;
    Penelope *p;
    int m_numCitizens;
    bool levelFinished = false;
};

#endif // STUDENTWORLD_H_
