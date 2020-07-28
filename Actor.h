#ifndef ACTOR_H_
#define ACTOR_H_
//#include "StudentWorld.h"
#include "GraphObject.h"

class StudentWorld;
class Penelope;
class Goodie;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//Actor
class Actor : public GraphObject
{
public:
    virtual void doSomething() = 0;
    Actor(StudentWorld *s, const int image, double x, double y, int dir, int depth);
    
    // Is this actor dead?
    bool isDead() const;
    
    // Mark this actor as dead.
    void setDead();

    
    //actor blocks or doesn't movement
    virtual bool blocksMovement() const;
    
    // If this is an activated object, perform its effect on a (e.g., for an
    // Exit have a use the exit).
    virtual void activateIfAppropriate(Actor* a);
    
    // If this object can die by falling into a pit or burning, die.
    virtual void dieByFallOrBurnIfAppropriate();
    
    // If this object can be infected by vomit, get infected.
    virtual void beVomitedOnIfAppropriate();
    
    // If this object uses exits, use the exit.
    virtual void useExitIfAppropriate();
    
    // If this object can pick up goodies, pick up g
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    
    // Does this object block flames?
    virtual bool blocksFlame() const;
    
    //causes zombie to vomit
    virtual bool triggersZombieVomit() const;
    
    // Is this object a threat to citizens?
    virtual bool threatensCitizens() const;
    
    // Does this object trigger citizens to follow it or flee it?
    virtual bool triggersCitizens() const;
    
    //get this actor's world
    StudentWorld* getWorld() const;
    
private:
    int m_level;
    StudentWorld *sw;
    bool m_alive;
};

//Wall
class Wall : public Actor
{
public:
    virtual void doSomething();
    Wall(StudentWorld *s, double x, double y);
    virtual bool blocksMovement() const;
    virtual bool blocksFlame() const;
};

//Activating Objects
class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir);
};

class Exit : public ActivatingObject
{
public:
    Exit(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual bool blocksFlame() const;
};

class Pit : public ActivatingObject
{
public:
    Pit(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
};

class Flame : public ActivatingObject
{
public:
    Flame(StudentWorld* w, double x, double y, int dir);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual bool blocksFlame() const
    {return true;}
private:
    int numTicks;
};

class Vomit : public ActivatingObject
{
public:
    Vomit(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
private:
    int numTicks;
};

class Landmine : public ActivatingObject
{
public:
    Landmine(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
private:
    int safetyTicks;
    bool active;
};

//Goodies
class Goodie : public ActivatingObject
{
public:
    Goodie(StudentWorld* w, int imageID, double x, double y);
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
    virtual void doSomething();
    virtual void pickUp(Penelope* p) = 0;
};

class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(StudentWorld* w, double x, double y);
    virtual void pickUp(Penelope* p);
};

class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(StudentWorld* w, double x, double y);
    virtual void pickUp(Penelope* p);
};

class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(StudentWorld* w, double x, double y);
    virtual void pickUp(Penelope* p);
};

//Agents
class Agent : public Actor
{
public:
    Agent(StudentWorld* w, int imageID, double x, double y, int dir);
    virtual bool blocksMovement() const;
};

class Human : public Agent
{
public:
    Human(StudentWorld* w, int imageID, double x, double y);
    virtual void beVomitedOnIfAppropriate();
    virtual bool triggersZombieVomit() const;
    // Make this human uninfected by vomit.
    void clearInfection();
    // How many ticks since this human was infected by vomit?
    int getInfectionDuration() const;
    
    bool infectionStatus() const;
    void addInfection();
private:
    bool isInfected;
    int infectionCount;
    
};

class Penelope : public Human
{
public:
    virtual void doSomething();
    Penelope(StudentWorld *s, double x, double y);
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    virtual bool triggersCitizens() const;
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    
    // Increase the number of vaccines the object has.
    void increaseVaccines();
    
    // Increase the number of flame charges the object has.
    void increaseFlameCharges();
    
    // Increase the number of landmines the object has.
    void increaseLandmines();
    
    // How many vaccines does the object have?
    int getNumVaccines() const;
    
    // How many flame charges does the object have?
    int getNumFlameCharges() const;
    
    // How many landmines does the object have?
    int getNumLandmines() const;
    
   
    
    
private:
    int m_numVaccines;
    int m_numFlameCharges;
    int m_numLandmines;
};

class Citizen : public Human
{
public:
    Citizen(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    virtual void beVomitedOnIfAppropriate();
private:
    int paralysisTicks;
    double dist;
    double trig_x;
    double trig_y;
    bool threatTrigger;
    double z1;
    double z2;
    double z3;
    double z4;
};

class Zombie : public Agent
{
public:
    Zombie(StudentWorld* w,  double x, double y);
    virtual bool triggersCitizens() const;
    virtual bool threatensCitizens() const;
    
    int getPlan() const;
    void setPlan(int m);
    void decreasePlan();
    
    int getParalysis() const;
    void incrementParalysis();
private:
    int m_movementPlan;
    int paralysisTicks;
};

class DumbZombie : public Zombie
{
public:
    DumbZombie(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();

};

class SmartZombie : public Zombie
{
public:
    SmartZombie(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();
};

#endif // ACTOR_H_
