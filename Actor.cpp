#include "Actor.h"
#include "StudentWorld.h"

class StudentWorld;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor
Actor::Actor(StudentWorld *s, const int image, double x, double y, int dir, int depth) : GraphObject(image,SPRITE_WIDTH*x,SPRITE_HEIGHT*y,dir,depth)
{
    m_level = s->getLevel();
    sw = s;
    m_alive = true;
}

bool Actor::isDead() const
{
    if(m_alive == true)
    {return false;}
    else
    {return true;}
}

void Actor::setDead()
{m_alive = false;}

bool Actor::blocksMovement() const
{return false;}

bool Actor::blocksFlame() const
{return false;}

void Actor::activateIfAppropriate(Actor* a)
{}

bool Actor::triggersZombieVomit() const
{return false;}

void Actor::dieByFallOrBurnIfAppropriate()
{}

void Actor::beVomitedOnIfAppropriate()
{}

void Actor::pickUpGoodieIfAppropriate(Goodie* g)
{}

void Actor::useExitIfAppropriate()
{}

bool Actor::threatensCitizens() const
{return false;}

bool Actor::triggersCitizens() const
{return false;}

StudentWorld* Actor::getWorld() const
{
    return sw;
}

//Activating Objects
ActivatingObject::ActivatingObject(StudentWorld* w, int imageID, double x, double y, int dir, int depth) : Actor(w,imageID,x,y,dir,depth)
{}

Exit::Exit(StudentWorld* w, double x, double y) : ActivatingObject(w,IID_EXIT,x,y,0,1)
{}

bool Exit::blocksFlame() const
{return true;}

void Exit::doSomething()
{
    getWorld()->activateOnAppropriateActors(this);
    //determine citizen overlap:
    //give user 500 pts
    //set citizen to dead
    //play sound effect
    
    //determine penelope overlap and citizens gone:
    //tell studentWorld level finished
    
}

void Exit::activateIfAppropriate(Actor* a)
{
        if(a->blocksMovement())
        {
            //cout << "overlap exit!" << endl;
            a->useExitIfAppropriate();
        }
}

Pit::Pit(StudentWorld* w, double x, double y) : ActivatingObject(w,IID_PIT,x,y,0,0)
{}

void Pit::doSomething()
{
    //destroy person or zombie who overlaps
    getWorld()->activateOnAppropriateActors(this);
}

void Pit::activateIfAppropriate(Actor* a)
{
    if(a->blocksMovement())
    {
        a->dieByFallOrBurnIfAppropriate();
    }
}


Flame::Flame(StudentWorld* w, double x, double y, int dir) : ActivatingObject(w,IID_FLAME,x,y,dir,0)
{
    numTicks = 0;
}

void Flame::doSomething()
{
    //check if alive
    if(isDead())
    {
        cout << "flame dead" << endl;
        return;
    }
    //set to dead after two ticks
    numTicks++;
    if(numTicks==2)
    {
        cout << "flame dead" << endl;
        setDead();
        return;
    }
    //damage certain objects that overlap
    getWorld()->activateOnAppropriateActors(this);
    
    
}

void Flame::activateIfAppropriate(Actor* a)
{
    a->dieByFallOrBurnIfAppropriate();
}

Vomit::Vomit(StudentWorld* w, double x, double y) : ActivatingObject(w,IID_VOMIT,x,y,0,0)
{
    numTicks = 0;
}

void Vomit::doSomething()
{
    //check if alive
    if(isDead())
    {
        return;
    }
    //set to dead after two ticks
    numTicks++;
    if(numTicks==2)
    {
        setDead();
        return;
    }
    //infect Penelope and citizens if they overlap
    getWorld()->activateOnAppropriateActors(this);
    
    
}

void Vomit::activateIfAppropriate(Actor* a)
{
    a->beVomitedOnIfAppropriate();
}

Landmine::Landmine(StudentWorld* w, double x, double y) : ActivatingObject(w, IID_LANDMINE,x,y,0,1)
{
    safetyTicks = 30;
    active = false;
    cout << "make land" << endl;
}

void Landmine::doSomething()
{
    //check if alive
    if(isDead())
    {
        return;
    }
    //decrement if not yet active
    if(!active)
    {
        safetyTicks--;
    }
    if(safetyTicks==0)
    {
        active=true;
        getWorld()->activateOnAppropriateActors(this);
    }
    
    //perform overlap functions
    
}

void Landmine::dieByFallOrBurnIfAppropriate()
{
    setDead();
}

void Landmine::activateIfAppropriate(Actor* a)
{
    cout << "landmine" << endl;
    setDead();
    
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);

    if(!getWorld()->flameBlocked(getX(),getY()))
      {getWorld()->addActor(new Flame(getWorld(),getX()/SPRITE_WIDTH,getY()/SPRITE_HEIGHT, right));}
    if(!getWorld()->flameBlocked((getX()-16),(getY())))
    {getWorld()->addActor(new Flame(getWorld(),(getX()-16)/SPRITE_WIDTH,(getY())/SPRITE_HEIGHT, right));}
    if(!getWorld()->flameBlocked((getX()+16),(getY())))
    {getWorld()->addActor(new Flame(getWorld(),(getX()+16)/SPRITE_WIDTH,(getY())/SPRITE_HEIGHT, right));}
     if(!getWorld()->flameBlocked((getX()-16),(getY()-16)))
     {getWorld()->addActor(new Flame(getWorld(),(getX()-16)/SPRITE_WIDTH,(getY()-16)/SPRITE_HEIGHT, right));}
    if(!getWorld()->flameBlocked((getX()-16),(getY()+16)))
    {getWorld()->addActor(new Flame(getWorld(),(getX()-16)/SPRITE_WIDTH,(getY()+16)/SPRITE_HEIGHT, right));}
    if(!getWorld()->flameBlocked((getX()+16),(getY()+16)))
    {getWorld()->addActor(new Flame(getWorld(),(getX()+16)/SPRITE_WIDTH,(getY()+16)/SPRITE_HEIGHT, right));}
    if(!getWorld()->flameBlocked((getX()),(getY()-16)))
    { getWorld()->addActor(new Flame(getWorld(),(getX())/SPRITE_WIDTH,(getY()-16)/SPRITE_HEIGHT, right));}
    if(!getWorld()->flameBlocked((getX()),(getY()+16)))
    {getWorld()->addActor(new Flame(getWorld(),(getX())/SPRITE_WIDTH,(getY()+16)/SPRITE_HEIGHT, right));}
    if(!getWorld()->flameBlocked((getX()+16),(getY()-16)))
    {getWorld()->addActor(new Flame(getWorld(),(getX()+16)/SPRITE_WIDTH,(getY()-16)/SPRITE_HEIGHT, right));}
    
    //getWorld()->addActor(new Pit(getWorld(),getX()/SPRITE_WIDTH,getY()/SPRITE_HEIGHT));
}
//Walls
Wall::Wall(StudentWorld *s, double x, double y) : Actor(s, IID_WALL, x,y,0,0)
{}

void Wall::doSomething()
{}

bool Wall::blocksMovement() const
{return true;}

bool Wall::blocksFlame() const
{return true;}

//Goodies
Goodie::Goodie(StudentWorld* w, int imageID, double x, double y) : ActivatingObject(w,imageID,x,y,0,1)
{}

void Goodie::dieByFallOrBurnIfAppropriate()
{
    setDead();
}

void Goodie::activateIfAppropriate(Actor* a)
{
    a->pickUpGoodieIfAppropriate(this);
}

void Goodie::doSomething()
{
    //check if alive
    if(isDead())
    {return;}
    //determine overlap with Penelope
    getWorld()->activateOnAppropriateActors(this);
    
}

VaccineGoodie::VaccineGoodie(StudentWorld* w, double x, double y) : Goodie(w,IID_VACCINE_GOODIE,x,y)
{}


void VaccineGoodie::pickUp(Penelope* p)
{
    getWorld()->increaseScore(50);
    setDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    p->increaseVaccines();
}


GasCanGoodie::GasCanGoodie(StudentWorld* w, double x, double y) : Goodie(w,IID_GAS_CAN_GOODIE,x,y)
{}

void GasCanGoodie::pickUp(Penelope* p)
{
    getWorld()->increaseScore(50);
    setDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    p->increaseFlameCharges();
}

LandmineGoodie::LandmineGoodie(StudentWorld* w, double x, double y) : Goodie(w,IID_LANDMINE_GOODIE,x,y)
{}

void LandmineGoodie::pickUp(Penelope* p)
{
    getWorld()->increaseScore(50);
    setDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    p->increaseLandmines();
}

//Agents
Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir) : Actor(w,imageID,x,y,dir,0)
{}

bool Agent::blocksMovement() const
{return true;}

Human::Human(StudentWorld* w, int imageID, double x, double y) : Agent(w,imageID,x,y,0)
{
    isInfected = false;
    infectionCount = 0;
}

void Human::clearInfection()
{
    isInfected = false;
    infectionCount = 0;
}

int Human::getInfectionDuration() const
{
    return infectionCount;
}

bool Human::triggersZombieVomit() const
{return true;}

bool Human::infectionStatus() const
{
    return isInfected;
}

void Human::addInfection()
{infectionCount++;}

void Human::beVomitedOnIfAppropriate()
{
    isInfected = true;
}

//Penelope
Penelope::Penelope(StudentWorld *s, double x, double y) : Human(s, IID_PLAYER, x, y)
{
    m_numVaccines = 0;
    m_numFlameCharges = 0;
    m_numLandmines = 0;
}
void Penelope::doSomething()
{
    //check alive
    if(isDead())
    {return;}
    //check infection status
    if(infectionStatus())
    {
        addInfection();
        if(getInfectionDuration()==500)
        {
            setDead();
            getWorld()->playSound(SOUND_PLAYER_DIE);
            return;
        }
    }
    
    //respond to flamethrower input
    //respond to landmine input
    //respond to vaccine input
    //respond to movement input
    int ch;
    if (getWorld()->getKey(ch))
    {
        // user hit a key during this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                //... move Penelope to the left ...;
                //check for blocking
                setDirection(180);
                if(getWorld()->canMove(getX()-4, getY(),this))
                {moveTo(getX()-4, getY());}
                
                break;
            case KEY_PRESS_RIGHT:
                //... move Penelope to the right ...;
                setDirection(0);
                if(getWorld()->canMove(getX()+4, getY(),this))
                {moveTo(getX()+4, getY());}
                
                
                break;
            case KEY_PRESS_UP:
                //... move Penelope to the right ...;
                setDirection(90);
                if(getWorld()->canMove(getX(), getY()+4,this))
                {moveTo(getX(), getY()+4);}
                
                break;
            case KEY_PRESS_DOWN:
                //... move Penelope to the right ...;
                setDirection(270);
                if(getWorld()->canMove(getX(), getY()-4,this))
                {moveTo(getX(), getY()-4);}
                break;
            case KEY_PRESS_SPACE:
                //... add flames in front of Penelope...;
                if(getNumFlameCharges()>=1)
                {
                    m_numFlameCharges--;
                    switch(getDirection())
                    {
                        case up:
                            for(int i = 1; i<=3; i++)
                            {
                                double px = getX();
                                double py = getY() + i * SPRITE_HEIGHT;
                                if(getWorld()->flameBlocked(px, py))
                                {
                                    break;
                                }
                                getWorld()->playSound(SOUND_PLAYER_FIRE);
                                getWorld()->addActor(new Flame(getWorld(),px/SPRITE_WIDTH,py/SPRITE_HEIGHT,up));
                            }
                            break;
                        case down:
                            for(int i = 1; i<=3; i++)
                            {
                                double px = getX();
                                double py = getY() - i * SPRITE_HEIGHT;
                                if(getWorld()->flameBlocked(px, py))
                                {
                                    cout << "end" << endl;
                                    break;
                                }
                                getWorld()->playSound(SOUND_PLAYER_FIRE);
                                getWorld()->addActor(new Flame(getWorld(),px/SPRITE_WIDTH,py/SPRITE_HEIGHT,down));
                            }
                            
                            break;
                        case left:
                            for(int i = 1; i<=3; i++)
                            {
                                double px = getX() - i * SPRITE_WIDTH;
                                double py = getY();
                                if(getWorld()->flameBlocked(px, py))
                                {
                                    break;
                                }
                                getWorld()->playSound(SOUND_PLAYER_FIRE);
                                getWorld()->addActor(new Flame(getWorld(),px/SPRITE_WIDTH,py/SPRITE_HEIGHT,left));
                            }
                            break;
                        case right:
                            for(int i = 1; i<=3; i++)
                            {
                                double px = getX() + i * SPRITE_WIDTH;
                                double py = getY();
                                //if getworld flame blocked -> return
                                if(getWorld()->flameBlocked(px, py))
                                {
                                    break;
                                }
                                getWorld()->playSound(SOUND_PLAYER_FIRE);
                                getWorld()->addActor(new Flame(getWorld(),px/SPRITE_WIDTH,py/SPRITE_HEIGHT,right));
                            }
                            break;
                    }
                }
                break;
            case KEY_PRESS_TAB:
                //add landmines
                if(getNumLandmines()>0)
                {
                    getWorld()->addActor(new Landmine(getWorld(),getX()/SPRITE_WIDTH,getY()/SPRITE_HEIGHT));
                    cout << "land" << endl;
                    m_numLandmines--;
                }
                break;
            case KEY_PRESS_ENTER:
                //use vaccines
                if(getNumVaccines()>0)
                {
                    m_numVaccines--;
                    clearInfection();
                }
                break;
                // etc... }
        }
    }
}

void Penelope::dieByFallOrBurnIfAppropriate()
{
    //penelope dies if she falls or burns
    getWorld()->decLives();
    setDead();
    getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Penelope::useExitIfAppropriate()
{
    
    //tell studentWorld to end game if all citizens gone
    cout << getWorld()->citizensLeft()<< endl;
    if(getWorld()->citizensLeft()<=0)
    {getWorld()->recordLevelFinishedIfAllCitizensGone();}
    
}

void Penelope::increaseVaccines()
{
    m_numVaccines++;
}

void Penelope::increaseFlameCharges()
{
    m_numFlameCharges+=5;
}

void Penelope::increaseLandmines()
{
    m_numLandmines+=2;
}

int Penelope::getNumVaccines() const
{return m_numVaccines;}

int Penelope::getNumFlameCharges() const
{return m_numFlameCharges;}

int Penelope::getNumLandmines() const
{return m_numLandmines;}

bool Penelope::triggersCitizens() const
{return true;}

void Penelope::pickUpGoodieIfAppropriate(Goodie* g)
{
    g->pickUp(this);
    getWorld()->playSound(SOUND_GOT_GOODIE);
}



Citizen::Citizen(StudentWorld* w,  double x, double y) : Human(w,IID_CITIZEN,x,y)
{
    paralysisTicks = 0;
    dist = 0;
    trig_x = 0;
    trig_y = 0;
    threatTrigger = false;
}

void Citizen::beVomitedOnIfAppropriate()
{
    Human::beVomitedOnIfAppropriate();
    getWorld()->playSound(SOUND_CITIZEN_INFECTED);
}

void Citizen::doSomething()
{
    //check if alive
    if(isDead())
    {return;}
    
    //check infection status
    if(infectionStatus())
    {
        addInfection();
        if(getInfectionDuration()==500)
        {
            getWorld()->recordCitizenGone();
            setDead();
            getWorld()->playSound(SOUND_ZOMBIE_BORN);
            getWorld()->increaseScore(-1000);
            if(randInt(1, 10)>3)
            {
                getWorld()->addActor(new DumbZombie(getWorld(),getX()/SPRITE_WIDTH,getY()/SPRITE_HEIGHT));
            }
            else
            {
                getWorld()->addActor(new SmartZombie(getWorld(),getX()/SPRITE_WIDTH,getY()/SPRITE_HEIGHT));
            }
            return;
        }
    }
    //implement paralysis
    paralysisTicks++;
    if(paralysisTicks%2==0)
    {
        return;
    }
    //determine distance to nearest trigger
    //penelope trigger
    if(getWorld()->locateNearestCitizenTrigger(getX(), getY(), trig_x, trig_y, dist, threatTrigger))
    {

        //penelope trigger
        if(threatTrigger == false && dist <= 6400)
        {
            //same row
            if(getX()==trig_x)
            {
                if(getY()<trig_y)
                {
                    if(getWorld()->canMove(getX(), getY()+2,this))
                    {
                        setDirection(90);
                        moveTo(getX(), getY()+2);
                        return;
                    }
                }
                if(getY()>trig_y)
                {
                    if(getWorld()->canMove(getX(), getY()-2,this))
                    {
                        setDirection(270);
                        moveTo(getX(), getY()-2);
                        return;
                    }
                }
            }
            else if(getY()==trig_y) //same column
            {
                if(getX()<trig_x)
                {
                    if(getWorld()->canMove(getX()+2, getY(),this))
                    {
                        setDirection(0);
                        moveTo(getX()+2, getY());
                        return;
                    }
                }
                if(getX()>trig_x)
                {
                    if(getWorld()->canMove(getX()-2, getY(),this))
                    {
                        setDirection(180);
                        moveTo(getX()-2, getY());
                        return;
                    }
                }
            }
            else //diff row and column
            {
                int randDir = randInt(1,2);
                if(randDir==1) //horizontal selected
                {
                    if(getX()<trig_x)
                    {
                        if(getWorld()->canMove(getX()+2, getY(),this))
                        {
                            setDirection(0);
                            moveTo(getX()+2, getY());
                            return;
                        }
                    }
                    else
                    {
                        if(getWorld()->canMove(getX()-2, getY(),this))
                        {
                            setDirection(180);
                            moveTo(getX()-2, getY());
                            return;
                        }
                    }
                    //can't move horizontally
                    if(getY()<trig_y)
                    {
                        if(getWorld()->canMove(getX(), getY()+2,this))
                        {
                            setDirection(90);
                            moveTo(getX(), getY()+2);
                            return;
                        }
                    }
                    else if(getY()>trig_y)
                    {
                        if(getWorld()->canMove(getX(), getY()-2,this))
                        {
                            setDirection(270);
                            moveTo(getX(), getY()-2);
                            return;
                        }
                    }
                }
                else if(randDir==2) //vertical selected
                {
                    if(getY()<trig_y)
                    {
                        if(getWorld()->canMove(getX(), getY()+2,this))
                        {
                            setDirection(90);
                            moveTo(getX(), getY()+2);
                            return;
                        }
                    }
                    else if(getY()>trig_y)
                    {
                        if(getWorld()->canMove(getX(), getY()-2,this))
                        {
                            setDirection(270);
                            moveTo(getX(), getY()-2);
                            return;
                        }
                    }
                    //can't move vertically
                    if(getX()<trig_x)
                    {
                        if(getWorld()->canMove(getX()+2, getY(),this))
                        {
                            setDirection(0);
                            moveTo(getX()+2, getY());
                            return;
                        }
                    }
                    else
                    {
                        if(getWorld()->canMove(getX()-2, getY(),this))
                        {
                            setDirection(180);
                            moveTo(getX()-2, getY());
                            return;
                        }
                    }
                }
            }
        }
        //zombie trigger
        double jx;
        double jy;
        if(threatTrigger == true  && dist <= 6400)
        {
            if(getWorld()->canMove(getX(), getY()+2,this))
            {
                getWorld()->locateNearestCitizenThreat(getX(), getY()+2, jx, jy, z1);
            }
            if(getWorld()->canMove(getX(), getY()-2,this))
            {
                getWorld()->locateNearestCitizenThreat(getX(), getY()-2, jx, jy, z2);
            }
            if(getWorld()->canMove(getX()+2, getY(),this))
            {
                getWorld()->locateNearestCitizenThreat(getX()+2, getY(), jx, jy, z3);
            }
            if(getWorld()->canMove(getX()-2, getY(),this))
            {
                getWorld()->locateNearestCitizenThreat(getX()-2, getY(), jx, jy, z4);
            }
            
            double dz[4] = {z1,z2,z3,z4};
            if(dist>z1 && dist>z2 && dist>z3 && dist>z4)
            {
                return;
            }
            int maxDist = 0;
            for(int i = 0; i<4; i++)
            {
                if(maxDist<dz[i])
                {
                    maxDist = dz[i];
                }
            }
            if(maxDist==z1)
            {
                if(getWorld()->canMove(getX(), getY()+2,this))
                {
                    setDirection(90);
                    moveTo(getX(), getY()+2);
                }
                return;
            }
            else if(maxDist==z2)
            {
                if(getWorld()->canMove(getX(), getY()-2,this))
                {
                    setDirection(270);
                    moveTo(getX(), getY()-2);
                }
                return;
            }
            else if(maxDist==z3)
            {
                if(getWorld()->canMove(getX()+2, getY(),this))
                {
                    setDirection(0);
                    moveTo(getX()+2, getY());
                }
                return;
            }
            else if(maxDist==z4)
            {
                if(getWorld()->canMove(getX()-2, getY(),this))
                {
                    setDirection(180);
                    moveTo(getX()-2, getY());
                }
                return;
            }
        }
        
    }
    
}

void Citizen::dieByFallOrBurnIfAppropriate()
{
    //kill citizen and lose 1000 pts
    setDead();
    getWorld()->increaseScore(-1000);
    getWorld()->recordCitizenGone();
    getWorld()->playSound(SOUND_CITIZEN_DIE);
}

void Citizen::useExitIfAppropriate()
{
    //user gets 500 pts
    getWorld()->increaseScore(500);
    setDead();
    getWorld()->playSound(SOUND_CITIZEN_SAVED);
    getWorld()->recordCitizenGone();
}

Zombie::Zombie(StudentWorld* w,  double x, double y) : Agent(w,IID_ZOMBIE,x,y,0)
{
    m_movementPlan = 0;
    paralysisTicks = 0;
}

int Zombie::getPlan() const
{return m_movementPlan;}

void Zombie::setPlan(int m)
{
    m_movementPlan=m;
}

void Zombie::decreasePlan()
{
    m_movementPlan--;
}

int Zombie::getParalysis() const
{
    return paralysisTicks;
}

void Zombie::incrementParalysis()
{
    paralysisTicks++;
}

bool Zombie::triggersCitizens() const
{return true;}

bool Zombie::threatensCitizens() const
{return true;}

DumbZombie::DumbZombie(StudentWorld* w,  double x, double y) : Zombie(w,x,y)
{}

void DumbZombie::doSomething()
{
    //check if alive
    if(isDead())
    {return;}
    //implement paralysis
    incrementParalysis();
    if(getParalysis()%2==0)
    {
        return;
    }
    //check for person in direction
    switch(getDirection())
    {
        case up:
            if(getWorld()->isZombieVomitTriggerAt(getX(), getY()+SPRITE_HEIGHT) && randInt(1, 3)==1)
            {
                getWorld()->addActor(new Vomit(getWorld(),(getX())/16,(getY()+SPRITE_HEIGHT/16)));
                getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                return;
            }
            break;
        case down:
            if(getWorld()->isZombieVomitTriggerAt(getX(), getY()-SPRITE_HEIGHT) && randInt(1, 3)==1)
            {
                getWorld()->addActor(new Vomit(getWorld(),(getX())/16,(getY()-SPRITE_HEIGHT/16)));
                getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                return;
            }
            break;
        case left:
            if(getWorld()->isZombieVomitTriggerAt(getX()-SPRITE_WIDTH, getY()) && randInt(1, 3)==1)
            {
                getWorld()->addActor(new Vomit(getWorld(),(getX()-SPRITE_WIDTH)/16,(getY()/16)));
                getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                return;
            }
            break;
        case right:
            if(getWorld()->isZombieVomitTriggerAt(getX()+SPRITE_WIDTH, getY()) && randInt(1, 3)==1)
            {
                getWorld()->addActor(new Vomit(getWorld(),(getX()+SPRITE_WIDTH)/16,(getY()/16)));
                getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                return;
            }
            break;
    }
    //check movement plan
    if(getPlan()==0)
    {
        setPlan(randInt(3, 10));
        setDirection(randInt(0, 3)*90);
    }
    //move dumb zombie
    switch (getDirection())
    {
        case up:
            if(getWorld()->canMove(getX(), getY()+1, this))
            {
                moveTo(getX(), getY()+1);
                decreasePlan();
            }
            else
            {setPlan(0);}
            break;
        case down:
            if(getWorld()->canMove(getX(), getY()-1, this))
            {
                moveTo(getX(), getY()-1);
                decreasePlan();
            }
            else
            {setPlan(0);}
            break;
        case left:
            if(getWorld()->canMove(getX()-1, getY(), this))
            {
                moveTo(getX()-1, getY());
                decreasePlan();
            }
            else
            {setPlan(0);}
            break;
        case right:
            if(getWorld()->canMove(getX()+1, getY(), this))
            {
                moveTo(getX()+1, getY());
                decreasePlan();
            }
            else
            {setPlan(0);}
            break;
    }
}

void DumbZombie::dieByFallOrBurnIfAppropriate()
{
    //kill zombie and increase by 1000 pts
    setDead();
    getWorld()->increaseScore(1000);
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
}

SmartZombie::SmartZombie(StudentWorld* w,  double x, double y) : Zombie(w,x,y)
{}

void SmartZombie::doSomething()
{
    //check if alive
    if(isDead())
    {return;}
    //implement paralysis
    incrementParalysis();
    if(getParalysis()%2==0)
    {
        return;
    }
    //check for person in direction
    switch(getDirection())
    {
        case up:
            if(getWorld()->isZombieVomitTriggerAt(getX(), getY()+SPRITE_HEIGHT) && randInt(1, 3)==1)
            {
                getWorld()->addActor(new Vomit(getWorld(),(getX())/16,(getY()+SPRITE_HEIGHT/16)));
                getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                return;
            }
            break;
        case down:
            if(getWorld()->isZombieVomitTriggerAt(getX(), getY()-SPRITE_HEIGHT) && randInt(1, 3)==1)
            {
                getWorld()->addActor(new Vomit(getWorld(),(getX())/16,(getY()-SPRITE_HEIGHT/16)));
                getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                return;
            }
            break;
        case left:
            if(getWorld()->isZombieVomitTriggerAt(getX()-SPRITE_WIDTH, getY()) && randInt(1, 3)==1)
            {
                getWorld()->addActor(new Vomit(getWorld(),(getX()-SPRITE_WIDTH)/16,(getY()/16)));
                getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                return;
            }
            break;
        case right:
            if(getWorld()->isZombieVomitTriggerAt(getX()+SPRITE_WIDTH, getY()) && randInt(1, 3)==1)
            {
                getWorld()->addActor(new Vomit(getWorld(),(getX()+SPRITE_WIDTH)/16,(getY()/16)));
                getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                return;
            }
            break;
    }
    //check movement plan arrangements
    double j1,j2;
    double distance;
    if(getPlan()==0)
    {
        setPlan(randInt(3, 10));
        getWorld()->locateNearestVomitTrigger(getX(), getY(), j1, j2, distance);
        if(distance>6400)
        {setDirection(randInt(0, 3)*90);}
        else
        {
            if(getX()==j1)
            {
                if(j2>getY())
                {setDirection(up);}
                else if(j2<getY())
                {setDirection(down);}
            }
            else if(getY()==j2)
            {
                if(j1>getX())
                {setDirection(right);}
                else if(j1<getX())
                {setDirection(left);}
            }
            else
            {
                if(randInt(1, 2)==1)
                {
                    if(j2>getY())
                    {setDirection(up);}
                    else if(j2<getY())
                    {setDirection(down);}
                }
                else
                {
                    if(j1>getX())
                    {setDirection(right);}
                    else if(j1<getX())
                    {setDirection(left);}
                }
            }
        }
        
    }
    //move dumb zombie
    switch (getDirection())
    {
        case up:
            if(getWorld()->canMove(getX(), getY()+1, this))
            {
                moveTo(getX(), getY()+1);
                decreasePlan();
            }
            else
            {setPlan(0);}
            break;
        case down:
            if(getWorld()->canMove(getX(), getY()-1, this))
            {
                moveTo(getX(), getY()-1);
                decreasePlan();
            }
            else
            {setPlan(0);}
            break;
        case left:
            if(getWorld()->canMove(getX()-1, getY(), this))
            {
                moveTo(getX()-1, getY());
                decreasePlan();
            }
            else
            {setPlan(0);}
            break;
        case right:
            if(getWorld()->canMove(getX()+1, getY(), this))
            {
                moveTo(getX()+1, getY());
                decreasePlan();
            }
            else
            {setPlan(0);}
            break;
    }
}

void SmartZombie::dieByFallOrBurnIfAppropriate()
{
    //kill zombie and increase by 2000 pts
    cout << "killed smart" << endl;
    setDead();
    getWorld()->increaseScore(2000);
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
}
