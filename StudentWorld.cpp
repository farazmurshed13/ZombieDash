#include "StudentWorld.h"
#include "GameConstants.h"

#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_numCitizens = 0;
}

int StudentWorld::init()
{
    levelFinished = false;
    Level lev(assetPath());
    string opener = "level";
    if(getLevel()<10)
    {opener = "level0";}
    string levelFile = opener + to_string(getLevel()) + ".txt";
    Level::LoadResult result = lev.loadLevel(levelFile);
    if(result == Level::load_fail_file_not_found || getLevel()==100)
    {
        cerr << "Cannot find level0" + to_string(getLevel()) + ".txt" << endl;
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << "Your level was improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        for(int i = 0; i<SPRITE_HEIGHT; i++)
        {
            for(int j = 0; j<SPRITE_WIDTH; j++)
            {
                Level::MazeEntry ge = lev.getContentsOf(i,j); // level_x=5, level_y=10 switch (ge)
                switch (ge) // so x=80 and y=160
                {
                    case Level::empty:
                        cout << "Location " << i << "," << j << " is empty" << endl;
                        break;
                    case Level::smart_zombie:
                        cout << "Location " << i << "," << j << " starts with a smart zombie" << endl;
                        actors.push_back(new SmartZombie(this,i,j));
                        break;
                    case Level::dumb_zombie:
                        cout << "Location " << i << "," << j << " starts with a dumb zombie" << endl;
                        actors.push_back(new DumbZombie(this,i,j));
                        break;
                    case Level::player:
                        cout << "Location " << i << "," << j << " is where Penelope starts" << endl;
                        p = new Penelope(this,i,j);
                        break;
                    case Level::exit:
                        cout << "Location " << i << "," << j << " is where an exit is" << endl;
                        actors.push_back(new Exit(this,i,j));
                        break;
                    case Level::wall:
                        cout << "Location " << i << "," << j << " holds a Wall" << endl;
                        actors.push_back(new Wall(this,i,j));
                        break;
                    case Level::pit:
                        cout << "Location " << i << "," << j << " has a pit in the ground" << endl;
                        actors.push_back(new Pit(this,i,j));
                        break;
                    case Level::citizen:
                        cout << "Location " << i << "," << j << " has a citizen" << endl;
                        m_numCitizens++;
                        actors.push_back(new Citizen(this,i,j));
                        break;
                    case Level::gas_can_goodie:
                        cout << "Location " << i << "," << j << " has a gas can in the ground" << endl;
                        actors.push_back(new GasCanGoodie(this,i,j));
                        break;
                    case Level::vaccine_goodie:
                        cout << "Location " << i << "," << j << " has a vaccine in the ground" << endl;
                        actors.push_back(new VaccineGoodie(this,i,j));
                        break;
                    case Level::landmine_goodie:
                        cout << "Location " << i << "," << j << " has a landmine in the ground" << endl;
                        actors.push_back(new LandmineGoodie(this,i,j));
                        break;
        
                        // etc...
                }
            }
        }
        
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    //penelope does something
    p->doSomething();
    
    //actors perform actions
    vector<Actor*>::iterator it;
    it = actors.begin();
    while(it!=actors.end())
    {
        (*it)->doSomething();
        if(p->isDead())
        {return GWSTATUS_PLAYER_DIED;}
        if(levelFinished==true)
        {
            playSound(SOUND_LEVEL_FINISHED);
            return GWSTATUS_FINISHED_LEVEL;
        }
        it++;
    }
    //remove dead actors
    it = actors.begin();
    while(it!=actors.end())
    {
        if((*it)!=nullptr)
        {
            if((*it)->isDead())
            {
                
                cout << "delete" << endl;
                delete *it;
                it = actors.erase(it);
            }
            else
            {it++;}
        }
    }
    //update screen text
    displayText();
    return GWSTATUS_CONTINUE_GAME;
    
}

void StudentWorld::displayText()
{
    ostringstream oss;
    if(getScore()>=0)
    {
        oss << "Score: ";
    }
    else
    {oss << "Score: -";}
    
    oss.fill('0');
    oss << setw(6) << getScore() << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Vacc: " << p->getNumVaccines() << "  Flames: " << p->getNumFlameCharges() << "  Mines: " << p->getNumLandmines() <<  "  Infected: " << p->getInfectionDuration();
    setGameStatText(oss.str());
}

void StudentWorld::cleanUp()
{
    if(p!=nullptr)
    {
        delete p;
        p = nullptr;
    }
    vector<Actor*>::iterator dt;
    dt = actors.begin();
    while(dt!=actors.end())
    {
        if((*dt)!=nullptr)
        {
            delete *dt;
            dt = actors.erase(dt);
        }
        else
        {dt++;}
        
    }
}

//cant move onto walls, citizens, zombies, or penelope
bool StudentWorld::canMove(double x, double y, Actor* a)
{
    vector<Actor*>::iterator dt;
    dt = actors.begin();
    while(dt!=actors.end())
    {
        double xActor = (*dt)->getX();
        double yActor = (*dt)->getY();
        double xBound = xActor-x;
        double yBound = yActor - y;
        if(xBound < 0)
        {xBound = -1 * xBound;}
        if(yBound < 0)
        {yBound = -1 * yBound;}
        if(xBound < SPRITE_WIDTH && yBound < SPRITE_HEIGHT && (*dt)->blocksMovement() && (*dt)!=a && !(*dt)->isDead())
        {
            return false;
        }
        dt++;
    }
    //check with penelope
    double xBound = x-(p->getX());
    double yBound = y-(p->getY());
    if(xBound < 0)
    {xBound = -1 * xBound;}
    if(yBound < 0)
    {yBound = -1 * yBound;}
    if(xBound < SPRITE_WIDTH && yBound < SPRITE_HEIGHT && p->blocksMovement() && p!=a)
    {
        return false;
    }
    return true;
}

bool StudentWorld::flameBlocked(double x, double y)
{
    vector<Actor*>::iterator dt;
    dt = actors.begin();
    while(dt!=actors.end())
    {
        double deltaX = x - (*dt)->getX();
        double deltaY = y - (*dt)->getY();
        deltaX*=deltaX;
        deltaY*=deltaY;
        double euclidDist = deltaX + deltaY;
        if(euclidDist<=100 && (*dt)->blocksFlame() && !(*dt)->isDead())
        {return true;}
        dt++;
    }
    return false;
}



void StudentWorld::addActor(Actor* a)
{
    actors.push_back(a);
}

void StudentWorld::recordCitizenGone()
{
    m_numCitizens--;
}

int StudentWorld::citizensLeft() const
{
    return m_numCitizens;
}

void StudentWorld::recordLevelFinishedIfAllCitizensGone()
{
    
    levelFinished = true;
}

void StudentWorld::activateOnAppropriateActors(Actor* a)
{
    //actors interactions
    
    vector<Actor*>::iterator dt;
    dt = actors.begin();
    
    while(dt!=actors.end())
    {
        
        if((EuclideanDistance(a,(*dt)) <= 100 && (*dt)!=a))
        {
            a->activateIfAppropriate(*dt);
        }
        dt++;
    }
    //penelope interaction
    if((EuclideanDistance(a,p)) <= 100)
    {
        a->activateIfAppropriate(p);
    }
}

double StudentWorld::EuclideanDistance(Actor* a, Actor* b)
{
    double deltaX = b->getX() - (a->getX());
    double deltaY = b->getY() - (a->getY());
    deltaX*=deltaX;
    deltaY*=deltaY;
    double euclidDist = deltaX + deltaY;
    return euclidDist;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y) const
{
    double deltaX = x-(p->getX());
    double deltaY = y-(p->getY());
    deltaX*=deltaX;
    deltaY*=deltaY;
    double euclidDist = deltaX + deltaY;
    if(euclidDist<=100 && p->triggersZombieVomit())
    {return true;}
    for(int i = 0; i < actors.size(); i++)
    {
        double deltaX = x-(actors[i]->getX());
        double deltaY = y-(actors[i]->getY());
        deltaX*=deltaX;
        deltaY*=deltaY;
        double euclidDist = deltaX + deltaY;
        if(euclidDist<=100 && actors[i]->triggersZombieVomit())
        {return true;}
    }
    return false;
}

bool StudentWorld::locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance)
{
    //calculate distance
    double deltaX = x-(p->getX());
    double deltaY = y-(p->getY());
    deltaX*=deltaX;
    deltaY*=deltaY;
    double nearest = deltaX + deltaY;
    otherX=p->getX();
    otherY=p->getY();
    for(int i = 0; i < actors.size(); i++)
    {
        double deltaX = x-(actors[i]->getX());
        double deltaY = y-(actors[i]->getY());
        deltaX*=deltaX;
        deltaY*=deltaY;
        double euclidDist = deltaX + deltaY;
        if(euclidDist<nearest && actors[i]->triggersZombieVomit())
        {
            nearest = euclidDist;
            otherX = actors[i]->getX();
            otherY = actors[i]->getY();
        }
    }
    distance = nearest;
    return true;
}

bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const
{
    double tempDist = 0;
    bool triggerPresent = false;
    if(p->triggersCitizens())
    {triggerPresent=true;}
    //calculate distance
    double deltaX = x-(p->getX());
    double deltaY = y-(p->getY());
    deltaX*=deltaX;
    deltaY*=deltaY;
    double nearest = deltaX + deltaY;
    otherX=p->getX();
    otherY=p->getY();
    isThreat = p->threatensCitizens();
    
    //actors
    for(int i = 0; i<actors.size();i++)
    {
        if(actors[i]->triggersCitizens())
        {triggerPresent=true;}
        //calculate distance
        double deltaX = x-(actors[i]->getX());
        double deltaY = y-(actors[i]->getY());
        deltaX*=deltaX;
        deltaY*=deltaY;
        tempDist = deltaX + deltaY;
        if(tempDist<nearest && actors[i]->triggersCitizens())
        {
            nearest=tempDist;
            otherX=actors[i]->getX();
            otherY=actors[i]->getY();
            isThreat = actors[i]->threatensCitizens();
        }
    }
    distance = nearest;
    return triggerPresent;
}

//just for zombies
bool StudentWorld::locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const
{
    bool threatPresent = false;
    double tempDist = 0;
    double nearest = 1000000000;
    for(int i = 0; i<actors.size();i++)
    {
        double deltaX = x-(actors[i]->getX());
        double deltaY = y-(actors[i]->getY());
        deltaX*=deltaX;
        deltaY*=deltaY;
        tempDist = deltaX + deltaY;
        if(tempDist<nearest && actors[i]->threatensCitizens())
        {
            nearest=tempDist;
            otherX=actors[i]->getX();
            otherY=actors[i]->getY();
            threatPresent = true;
        }
    }
    distance = nearest;
    return threatPresent;
}

