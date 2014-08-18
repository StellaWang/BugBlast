
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{
    m_SimultaneousSprayers = 2;
    int level = getLevel();
    ostringstream oss;
    string CurLevel;
    if(level < 10)
    {
        oss << "level0" << level << ".dat";
        CurLevel = oss.str();
    }
    else if(level >= 10)
    {
        oss << "level" << level << ".dat";
        CurLevel = oss.str();
    }
    
    Level lev;
    Level::LoadResult result = lev.loadLevel(CurLevel, "/Users/apple/Desktop/BugBlast/DerivedData/BugBlast/Build/Products/Debug");
    
    if (level == 0 && result == Level::load_fail_file_not_found)
    {
        return GWSTATUS_NO_FIRST_LEVEL;
    }
    if (level != 0 && result == Level::load_fail_file_not_found)
    {
        return GWSTATUS_PLAYER_WON;
    }
    if (result == Level::load_fail_bad_format)
    {
        return GWSTATUS_LEVEL_ERROR;
    }
    else
    {
        m_bonus = lev.getOptionValue(optionLevelBonus);
        m_ProbOfGoodieOverall = lev.getOptionValue(optionProbOfGoodieOverall);
        m_ProbOfExtraLifeGoodie = lev.getOptionValue(optionProbOfExtraLifeGoodie);
        m_ProbOfWalkThruGoodie = lev.getOptionValue(optionProbOfWalkThruGoodie);
        m_ProbOfMoreSprayersGoodie = lev.getOptionValue(optionProbOfMoreSprayersGoodie);
        m_TicksPerSimpleZumiMove = lev.getOptionValue(optionTicksPerSimpleZumiMove);
        m_TicksPerComplexZumiMove = lev.getOptionValue(optionTicksPerComplexZumiMove);
        m_GoodieLifetimeInTicks = lev.getOptionValue(optionGoodieLifetimeInTicks);
        m_WalkThruLifetimeTicks = lev.getOptionValue(optionWalkThruLifetimeTicks);
        m_BoostedSprayerLifetimeTicks = lev.getOptionValue(optionBoostedSprayerLifetimeTicks);
        m_MaxBoostedSprayers = lev.getOptionValue(optionMaxBoostedSprayers);
        m_ComplexZumiSearchDistance = lev.getOptionValue(optionComplexZumiSearchDistance);
        
        for (int i = 0; i < VIEW_WIDTH; i++)
            for (int j = 0; j < VIEW_HEIGHT; j++)
            {
                Level::MazeEntry item = lev.getContentsOf(i, j);
                switch (item) {
                    case Level::empty:
                        break;
                    case Level::player:
                    {
                        m_player = new Player(i, j, this);
                        break;
                    }
                    case Level::perma_brick:
                    {
                        Actor* PBrick = new PermaBrick(i, j, this);
                        Actors.push_back(PBrick);
                        break;
                    }
                    case Level::destroyable_brick:
                    {
                        Actor* DBrick = new DestroyableBrick(i,j,this);
                        Actors.push_back(DBrick);
                        break;
                    }
                    case Level::simple_zumi:
                    {
                        unsigned int k = m_TicksPerSimpleZumiMove;
                        Actor* SZumi = new SimpleZumi(i,j,this,k);
                        Actors.push_back(SZumi);
                        break;
                    }
                    case Level::complex_zumi:
                    {
                        unsigned int k = m_TicksPerComplexZumiMove;
                        Actor* CZumi = new ComplexZumi(i,j,this, k);
                        Actors.push_back(CZumi);
                        break;
                    }
                    case Level::exit:
                    {
                        Actor* exit = new Exit(i,j,this);
                        Actors.push_back(exit);
                        break;
                    }
                    default:
                        break;
                }
            }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    int score = getScore();
    int lives = getLives();
    int level = getLevel();
    ostringstream oss;
    string s;
        oss << "Score: " << score << "  Level: " << level << "  Lives: " << lives << "  Bonus: " << m_bonus;
        s = oss.str();
    setGameStatText(s);
    
    m_player->doSomething();
    
    for (int i = 0; i < Actors.size(); i++)
    {
        if (Actors[i]->isAlive() == false)
        {
            delete Actors[i];
            Actors.erase(Actors.begin() + i);
            i--;
            break;
        }
        Actors[i]->doSomething();
    }
    
    if (m_bonus > 0)
    {
        m_bonus--;
    }

    if (anyLiveZumi() == false)
    {
        Exit* exit;
        int EX, EY;
        for (int i = 0; i < Actors.size(); i++)
        {
                Exit *bp = dynamic_cast<Exit*>(Actors[i]);
                if (bp != nullptr)
                {
                    exit = bp;
                    Actors[i]->revealit();
                    EX = Actors[i]->getX();
                    EY = Actors[i]->getY();
                }
        }
        int PX = m_player->getX();
        int PY = m_player->getY();
        if (PX == EX && PY == EY)
        {
            exit->setDead();
            increaseScore(m_bonus);
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }

    if (m_player->isAlive() == false)
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < Actors.size(); i++)
    {
        delete Actors[i];
        Actors.erase(Actors.begin() + i);
        i--;
    }
    
    Actors.clear();
    delete m_player;
}

int StudentWorld::checkBricks(int x, int y)
{
    for (int i = 0; i < Actors.size(); i++)
    {
            if (Actors[i]->getX() == x && Actors[i]->getY() == y)
            {
                PermaBrick *bp = dynamic_cast<PermaBrick*>(Actors[i]);
                DestroyableBrick *cp = dynamic_cast<DestroyableBrick*>(Actors[i]);
                if (bp != nullptr)
                    return -1;
                if (cp != nullptr)
                    return -2;
            }
    }
    return 0;
}

bool StudentWorld::anyLiveZumi() const
{
    for (int i = 0; i < Actors.size(); i++)
    {
            Zumi *bp = dynamic_cast<Zumi*>(Actors[i]);
            if (bp != nullptr)
                return true;
    }
    return false;
}

void StudentWorld::addSprayerIfPossible(int x, int y)
{
    if (CheckBugSprayer(x, y) == true || checkBricks(x, y) != 0)
        return;

    if (m_player->canDropMoreSprayers() == true && m_SimultaneousSprayers > 0)
    {
        Actor* Sprayer = new BugSprayer(this, x, y);
        Actors.push_back(Sprayer);
        m_SimultaneousSprayers--;
    }
    if (m_player->canDropMoreSprayers() == false && m_SimultaneousSprayers > 2)
    {
        m_SimultaneousSprayers -= (m_MaxBoostedSprayers-2);
    }
    if (m_player->canDropMoreSprayers() == false && m_SimultaneousSprayers > 0)
    {
        Actor* Sprayer = new BugSprayer(this, x, y);
        Actors.push_back(Sprayer);
        m_SimultaneousSprayers--;
    }
}

bool StudentWorld::AnyZumiAtSpecificLocation(int x, int y)
{
    for (int i = 0; i < Actors.size(); i++)
    {
        if (Actors[i]->isAlive() == true)
        {
            if (Actors[i]->getX() == x && Actors[i]->getY() == y)
            {
                Zumi *bp = dynamic_cast<Zumi*>(Actors[i]);
                if (bp != nullptr)
                    return true;
            }
        }
    }
    return false;
}

bool StudentWorld::CheckBugSprayer(int x, int y)
{
    for (int i = 0; i < Actors.size(); i++)
    {
        if (Actors[i]->isAlive() == true)
        {
            if (Actors[i]->getX() == x && Actors[i]->getY() == y)
            {
                BugSprayer *bp = dynamic_cast<BugSprayer*>(Actors[i]);
                if (bp != nullptr)
                    return true;
            }
        }
    }
    return false;
}

void StudentWorld::addActor(Actor* a)
{
    Actors.push_back(a);
}

Actor* StudentWorld::getPointer(int x, int y)
{
    if (m_player->getX() == x && m_player->getY() == y)
    {
        return m_player;
    }
    for (int i = 0; i < Actors.size(); i++)
    {
            if (Actors[i]->getX() == x && Actors[i]->getY() == y)
                return Actors[i];
    }
}

void StudentWorld::getPlayerLocation(int& x, int& y) const
{
    x = m_player->getX();
    y = m_player->getY();
}

unsigned int StudentWorld::getWalkThruLifetimeTicks() const
{
    return m_WalkThruLifetimeTicks;
}

unsigned int StudentWorld::getBoostedSprayerLifetimeTicks() const
{
    return m_BoostedSprayerLifetimeTicks;
}

unsigned int StudentWorld::getMaxBoostedSprayers() const
{
    return m_MaxBoostedSprayers;
}

unsigned int StudentWorld::getGoodieLifetimeInTicks() const
{
    return m_GoodieLifetimeInTicks;
}

unsigned int StudentWorld::getProbOfGoodieOverall() const
{
    return m_ProbOfGoodieOverall;
}

unsigned int StudentWorld::getProbOfExtraLifeGoodie() const
{
    return m_ProbOfExtraLifeGoodie;
}

unsigned int StudentWorld::getProbOfWalkThruGoodie() const
{
    return m_ProbOfWalkThruGoodie;
}

unsigned int StudentWorld::getProbOfMoreSprayersGoodie() const
{
    return m_ProbOfMoreSprayersGoodie;
}

unsigned int StudentWorld::getComplexZumiSearchDistance() const
{
    return m_ComplexZumiSearchDistance;
}