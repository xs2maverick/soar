
#include <assert.h>
#include <vector>
#include <iostream>

// ORTS includes
#include "Object.H"

// our includes
#include "PerceptualGroup.h"
#include "general.h"
#include "Sorts.h"
#include "AttackManager.h"
#include "AttackManagerRegistry.h"
#include "AttackFSM.h"
#include "MineManager.h"
#include "SidedInfluenceERF.h"

#define msg cout << "PerceptualGroup.cpp: "

PerceptualGroup::PerceptualGroup (SoarGameObject* unit) {
  members.insert(unit);
  unit->setPerceptualGroup(this);
  setHasStaleMembers();
  hasStaleProperties= true;
  centerMember = unit;
  typeName = unit->gob->bp_name();
  owner = unit->getOwner();
  friendly = unit->isFriendly();
  world = unit->isWorld();
  mixedType = false;
  inSoar = false;
  old = false;

  fmSector = -1;

  minerals = (typeName == "mineral");
  friendlyWorker = (typeName == "worker");
  airUnits = (*(unit->gob->sod.zcat) == 1);
  landUnits = (*(unit->gob->sod.zcat) == 3);
  bbox.collapse(*unit->gob->sod.x, *unit->gob->sod.y);

  sticky = false;
  commandStatus = GRP_STATUS_IDLE;
  currentCommand = "none";
  if (typeName == "worker") {
    canMine = true;
  }
  else {
    canMine = false;
  }

  centerX = 0;
  centerY = 0;

  ///cout << "XXX created: " << (int) this << endl;
}

PerceptualGroup::~PerceptualGroup() {
  for(list<MapRegion*>::iterator
      i  = regionsOccupied.begin();
      i != regionsOccupied.end();
      i++)
  {
    (*i)->groupExit(this);
  }
  //cout << "XXX destroyed: " << (int) this << endl;
}

void PerceptualGroup::addUnit(SoarGameObject* unit) {

  assert(members.find(unit) == members.end());
  // don't group units from different teams together
  assert(unit->gob->get_int("owner") == owner);
  
  if (not mixedType and 
     (unit->gob->bp_name() != typeName)) {
    mixedType = true;
    minerals = false;
    friendlyWorker = false;
    airUnits = false;
    landUnits = false;
  } 

  members.insert(unit); 
  unit->setPerceptualGroup(this);
  setHasStaleMembers();
}

bool PerceptualGroup::removeUnit(SoarGameObject* unit) {
  assert(members.find(unit) != members.end());

  members.erase(unit);
  setHasStaleMembers();
  
  if (centerMember == unit) {
    // make sure center is a valid unit
    // it should be refreshed before use, though
    centerMember = *(members.begin());
  }
  return true;
}

void PerceptualGroup::updateBoundingBox() {
  bbox = (*members.begin())->getBoundingBox();
  for(set<SoarGameObject*>::iterator 
      i  = members.begin(); 
      i != members.end(); 
      i++)
  {
    bbox.accomodate((*i)->getBoundingBox());
  }
}

// This function calculates all the data that will go on the input link
void PerceptualGroup::generateData() {
  int x = 0;
  int y = 0;
  
  soarData.stringIntPairs.clear();
  soarData.stringFloatPairs.clear();
  soarData.stringStringPairs.clear();
  soarData.regionsOccupied.clear();
  soarData.stringFloatPairs.clear();

  int health = 0;
  int speed = 0;
  int size = members.size();
  int mineralCount = 0;
  int running = 0;
  int success = 0;
  int failure = 0;
  int idle = 0;
  int stuck = 0;
    
  double avg_heading_i = 0.0, avg_heading_j = 0.0;

  moving = false;
  
  int objStatus;

  for( set<SoarGameObject*>::iterator 
       currentObject =  members.begin();
       currentObject != members.end();
       currentObject++ )
  {
    if (canMine) {
      mineralCount += (*currentObject)->gob->get_int("minerals");
    }
    
    // not everything has health
    // if no hp, just set it to 0
    // get_int asserts if not valid, this is what it calls internally
    if ((*currentObject)->gob->get_int_ptr("hp") != 0) {
      health += (*currentObject)->gob->get_int("hp");
    }
    else {
      health += 0;
    }
    speed += *(*currentObject)->gob->sod.speed;
    x += *(*currentObject)->gob->sod.x;
    y += *(*currentObject)->gob->sod.y;

    // average heading over those units that could move
    if ((*currentObject)->gob->has_attr("heading")) {
      double hi, hj;
      getHeadingVector((*currentObject)->gob->get_int("heading"), &hi, &hj);
      avg_heading_i += hi;
      avg_heading_j += hj;
    }

    objStatus = (*currentObject)->getStatus();
    if (objStatus == OBJ_RUNNING) {
      running++;
    }
    else if (objStatus == OBJ_SUCCESS) {
      success++;
    }
    else if (objStatus == OBJ_FAILURE) {
      failure++;
    }
    else if (objStatus == OBJ_IDLE) {
      idle++;
    }
    else if (objStatus == OBJ_STUCK) {
      stuck++;
    }
  } 
 
  // average all attributes
  health /= size;
  speed /= size;
  x /= size;
  y /= size;

  centerX = x;
  centerY = y;

  updateBoundingBox();
  updateRegionsOccupied();

  // normalize the heading vector
  double heading_mag = sqrt(avg_heading_i * avg_heading_i + avg_heading_j * avg_heading_j);
  avg_heading_i /= heading_mag;
  avg_heading_j /= heading_mag;

  pair<string, int> stringIntWme;
  pair<string, float> stringFloatWme;
  pair<string, string> stringStringWme;

  stringIntWme.first = "health";
  stringIntWme.second = health;
  soarData.stringIntPairs.push_back(stringIntWme);

  stringIntWme.first = "speed";
  stringIntWme.second = speed;
  soarData.stringIntPairs.push_back(stringIntWme);

  // how do we want to represent position?
  stringIntWme.first = "x-pos";
  stringIntWme.second = x;
  soarData.stringIntPairs.push_back(stringIntWme);

  stringIntWme.first = "y-pos";
  stringIntWme.second = y;
  soarData.stringIntPairs.push_back(stringIntWme);

  stringIntWme.first = "x-min";
  stringIntWme.second = bbox.xmin;
  soarData.stringIntPairs.push_back(stringIntWme);

  stringIntWme.first = "x-max";
  stringIntWme.second = bbox.xmax;
  soarData.stringIntPairs.push_back(stringIntWme);

  stringIntWme.first = "y-min";
  stringIntWme.second = bbox.ymin;
  soarData.stringIntPairs.push_back(stringIntWme);

  stringIntWme.first = "y-max";
  stringIntWme.second = bbox.ymax;
  soarData.stringIntPairs.push_back(stringIntWme);


//// Heading info
  stringFloatWme.first = "heading-i";
  stringFloatWme.second = (float) avg_heading_i;
  soarData.stringFloatPairs.push_back(stringFloatWme);
  stringFloatWme.first = "heading-j";
  stringFloatWme.second = (float) avg_heading_j;
  soarData.stringFloatPairs.push_back(stringFloatWme);
  
  stringIntWme.first = "num_members";
  stringIntWme.second = size;
  soarData.stringIntPairs.push_back(stringIntWme);

  if (mixedType) {
    stringStringWme.first = "type";
    stringStringWme.second = "mixed";
  }
  else {
    stringStringWme.first = "type";
    stringStringWme.second = typeName;
  }
  soarData.stringStringPairs.push_back(stringStringWme);

//// Threats and support
  Circle approx = bbox.getCircumscribingCircle();
  int numPlayers = Sorts::OrtsIO->getNumPlayers();
  // this probably isn't right
  bool isGround = (*(*members.begin())->gob->sod.zcat == GameObj::ON_LAND);
  list<GameObj*> collisions;
  int threats = 0;
  int support = 0;
  msg << "-------------" << endl;
  msg << "Group at " << x << "," << y << " under owner " << owner << endl;
  for(int side = 0; side < numPlayers; side++) {
    msg << "Checking collisions with owner... " << side << endl;

    SidedInfluenceERF erf(10, side, isGround); // look ten viewframe ahead
    Sorts::satellite->getCollisions
      ((int) approx.x, (int) approx.y, (int) approx.r, &erf, collisions);

    set<GameObj*> temp;
    temp.clear();
    for(list<GameObj*>::iterator
        i  = collisions.begin();
        i != collisions.end();
        i++) 
    {
      assert(temp.find(*i) == temp.end());
      temp.insert(*i);
      msg << "Collides with obj at " << *(*i)->sod.x << ", " 
          << *(*i)->sod.y << " owned by " << (*i)->get_int("owner") << endl;
    }

    if (side != owner) {
      threats += collisions.size();
    }
    else {
      // of course you will always intersect yourself
      support += collisions.size() - members.size();
    }
  }
  stringIntWme.first = "threats";
  stringIntWme.second = threats;
  soarData.stringIntPairs.push_back(stringIntWme);
  stringIntWme.first = "support";
  stringIntWme.second = support;
  soarData.stringIntPairs.push_back(stringIntWme);

  // command info:
  // show last command, and as many status attributes as are applicable
  // if a group has one member succeed, fail, or still running, that 
  // status is there

  if (friendly) {
    stringStringWme.first = "command";
    stringStringWme.second = currentCommand;
    soarData.stringStringPairs.push_back(stringStringWme);

    stringIntWme.first = "command_running";
    stringIntWme.second = running;
    soarData.stringIntPairs.push_back(stringIntWme);

    stringIntWme.first = "command_success";
    stringIntWme.second = success;
    soarData.stringIntPairs.push_back(stringIntWme);
    
    stringIntWme.first = "command_failure";
    stringIntWme.second = failure;
    soarData.stringIntPairs.push_back(stringIntWme);
    
    stringIntWme.first = "command_stuck";
    stringIntWme.second = stuck;
    soarData.stringIntPairs.push_back(stringIntWme);
    
  }
  if (canMine) {
    stringIntWme.first = "minerals";
    stringIntWme.second = mineralCount;
    soarData.stringIntPairs.push_back(stringIntWme);
  }
  
  hasStaleProperties = true;
  hasStaleMembers = false;

  if (speed > 0) {
    moving = true;
  }

  old = true;
   
  return;
}

void PerceptualGroup::updateCenterLoc() {
  // used in lieu of generateData for internal groups
  // since the only data we actually need is the location of the center
  
  int x = 0;
  int y = 0;
  set<SoarGameObject*>::iterator currentObject;
  
  currentObject = members.begin();

  int size = members.size();

  while (currentObject != members.end()) {
    x += *(*currentObject)->gob->sod.x;
    y += *(*currentObject)->gob->sod.y;
    currentObject++;
  }
  
  x /= size;
  y /= size;

  centerX = x;
  centerY = y;

  hasStaleProperties = true;
  hasStaleMembers = false;

  return;
}

void PerceptualGroup::updateCenterMember() {
  double shortestDistance 
        = squaredDistance(centerX, centerY, 
          *centerMember->gob->sod.x, *centerMember->gob->sod.y);

  double currentDistance;
  
  set<SoarGameObject*>::iterator currentObject;
  currentObject = members.begin();
  while (currentObject != members.end()) {
    currentDistance = squaredDistance(centerX, centerY, 
                      *(*currentObject)->gob->sod.x, *(*currentObject)->gob->sod.y);
    if (currentDistance < shortestDistance) {
      shortestDistance = currentDistance;
      centerMember = *currentObject;
    }
    
    currentObject++;
  }

  return;
}

/**************************************************
 *                                                *
 * Update the regions the group occupies          *
 *                                                *
 **************************************************/
void PerceptualGroup::updateRegionsOccupied() {
  // for now, just leave all regions first and then recompute
  // which ones it enters, even if this may mean exiting and
  // entering the same region redundently
  for( list<MapRegion*>::iterator 
       i  = regionsOccupied.begin();
       i != regionsOccupied.end();
       i++ )
  {
 //   cout << "&&& Group " << (int) this << " has exited from region " << (*i)->getId() << endl;
    (*i)->groupExit(this);
  }
  regionsOccupied.clear();
  Sorts::mapManager->getRegionsIntersecting(getBoundingBox(), regionsOccupied);

  for( list<MapRegion*>::iterator
       i  = regionsOccupied.begin();
       i != regionsOccupied.end();
       i++ )
  {
    list<MapRegion*>::iterator j = i;
    j++;
    for( ; j != regionsOccupied.end(); j++) {
      if (*i == *j) {
        assert(false);
      }
    }
  }

  for( list<MapRegion*>::iterator 
       i  = regionsOccupied.begin();
       i != regionsOccupied.end();
       i++ )
  {
 //   cout << "&&& Group " << (int) this << " has entered region " << (*i)->getId() << endl;
    (*i)->groupEnter(this);
    soarData.regionsOccupied.push_back((*i)->getId());
  }
}

void PerceptualGroup::mergeTo(PerceptualGroup* target) {
  // move all members into other group

  // the group should be destructed after this is called.

  if (target == this) {
    cout << "WARNING: merge of group to self requested. Ignoring." << endl;
    return;
  }
  
  set<SoarGameObject*>::iterator currentObject = members.begin();
  
  while (currentObject != members.end()) {
    target->addUnit(*currentObject);
    currentObject++;
  }

  members.clear();

  setHasStaleMembers();

  return;
}

bool PerceptualGroup::assignAction(ObjectActionType type, list<int> params,
                                 list<PerceptualGroup*> targets) { 
  bool result = true;

  set<SoarGameObject*>::iterator currentObject;
  
  list<int>::iterator intIt;  
  list<PerceptualGroup*>::iterator targetGroupIt;  
  Vector<sint4> tempVec;
  list<SoarGameObject*> sgoList;
  
  switch (type) {
    case OA_MOVE:
      // the third param is speed, always use 3 (the max)
      assert(params.size() == 2);
      intIt = params.begin();  
      tempVec.push_back(*intIt);
      intIt++;
      tempVec.push_back(*intIt);
      tempVec.push_back(3);

      currentCommand = "move";
      sticky = true;
      // this group is stuck together from now on,
      // until Soar issues an unstick action
      for (currentObject = members.begin();
           currentObject != members.end();
           currentObject++) {
        (*currentObject)->issueCommand(type, tempVec);
      } 
      break;

    case OA_MINE:
      assert(targets.size() == 0);
      currentCommand = "mine";
      tempVec.clear();
      sticky = true;
      // this group is stuck together from now on,
      // until Soar issues an unstick action
      
      getMembers(sgoList);
      Sorts::mineManager->prepareRoutes(sgoList);
      
      for (currentObject = members.begin();
           currentObject != members.end();
           currentObject++) {
        (*currentObject)->issueCommand(type, tempVec);
      }
      break;

    case OA_FREE:
      sticky = false;
      cout << "UNSTUCK!\n";
      break;

    case OA_ATTACK: {
      vector<SoarGameObject*> myUnits;
      myUnits.insert(myUnits.begin(), members.begin(), members.end());

      int managerId = Sorts::amr->assignManager(targets);
      Vector<sint4> params(1);
      params[0] = managerId;

      for(set<SoarGameObject*>::iterator
          i =  members.begin();
          i != members.end();
          i++)
      {
        (*i)->issueCommand(type, params);
      }
      break;
    }

    default:
      assert(false);  
  }
  
  return result;
}

bool PerceptualGroup::isEmpty() {
  return (members.empty());
}

bool PerceptualGroup::getHasStaleMembers() {
  return hasStaleMembers;
}
void PerceptualGroup::setHasStaleMembers() {
  //cout << "stale: set so" << endl;
  hasStaleMembers = true;
}

groupPropertyStruct* PerceptualGroup::getSoarData() {
  return &soarData;
}

bool PerceptualGroup::getHasStaleProperties() {
  return hasStaleProperties;
}

void PerceptualGroup::setHasStaleProperties(bool val) {
  hasStaleProperties = val;
}

void PerceptualGroup::getMembers(list<SoarGameObject*>& memberList) {
  memberList.clear();
  memberList.insert(memberList.begin(), members.begin(), members.end());
}

SoarGameObject* PerceptualGroup::getCenterMember() {
  return centerMember;
}

int PerceptualGroup::getSize() {
  return members.size();
}

int PerceptualGroup::getOwner() {
  return owner;
}

bool PerceptualGroup::isFriendly() {
  return friendly;
}

bool PerceptualGroup::isWorld() {
  return world;
}

bool PerceptualGroup::isMinerals() {
  return minerals;
}
bool PerceptualGroup::isAirUnits() {
  return airUnits;
}
bool PerceptualGroup::isLandUnits() {
  return landUnits;
}

bool PerceptualGroup::isMoving() {
  return moving;
}

pair<string, int> PerceptualGroup::getCategory(bool ownerGrouping) {
  // get the group's category-
  // category is the type and owner, if ownerGrouping is not used,
  // otherwise just the owner alone
  
  pair<string, int> cat;
  if (not ownerGrouping) {
    cat.first = typeName;
  }
  else {
    cat.first = "";
  }
  cat.second = owner;

  return cat;
}

Rectangle PerceptualGroup::getBoundingBox() {
  // checking if everything is in the bounding box, because I'm not sure
  // if the bounding box is getting updated often enough
  for(set<SoarGameObject*>::iterator
      i  = members.begin();
      i != members.end();
      i++)
  {
    int x = *(*i)->gob->sod.x;
    int y = *(*i)->gob->sod.y;
   // FIXME: asserts in game1
    // assert(bbox.xmin <= x && x <= bbox.xmax &&
   //        bbox.ymin <= y && y <= bbox.ymax);
  }
  return bbox;
}

bool PerceptualGroup::getSticky() {
  return sticky;
}

void PerceptualGroup::setSticky(bool in) {
  sticky = in;
}
void PerceptualGroup::getCenterLoc(int& x, int& y) {
  x = centerX;
  y = centerY;
}

void PerceptualGroup::getLocNear(int x, int y, int& locX, int &locY) {
  // return a location on the bounding box of this group,
  // close to the given point (x,y)
  
  // if point is inside bounding box, just return it

  // better way to do this?
  int adjust = rand() % 50;
  int adjust2 = rand() % 50;

  if (x < bbox.xmin) {
    locX = bbox.xmin - adjust;
    if (y < bbox.ymin) {
      locY = bbox.ymin - adjust2;
    }
    else if (y > bbox.ymax) {
      locY = bbox.ymax + adjust2;
    }
    else {
      locY = y;
    }
  }
  else if (x > bbox.xmax) {
    locX = bbox.xmax + adjust;
    if (y < bbox.ymin) {
      locY = bbox.ymin - adjust2;
    }
    else if (y > bbox.ymax) {
      locY = bbox.ymax + adjust2;
    }
    else {
      locY = y;
    }
  }
  else {
    locX = x;
    if (y < bbox.ymin) {
      locY = bbox.ymin - adjust;
    }
    else if (y > bbox.ymax) {
      locY = bbox.ymax + adjust;
    }
    else {
      locY = y;
    }
  }

  return;
}

void PerceptualGroup::setFMSector(int num) {
  fmSector = num;
}

int PerceptualGroup::getFMSector() {
  return fmSector;
}

void PerceptualGroup::setFMaps(list <FeatureMap*> _fMaps) {
  fMaps = _fMaps;
  return;
}

list <FeatureMap*> PerceptualGroup::getFMaps() {
  return fMaps;
}

void PerceptualGroup::setFMFeatureStrength(int num) {
  fmFeatureStrength = num;
}

int PerceptualGroup::getFMFeatureStrength() {
  return fmFeatureStrength;
}

void PerceptualGroup::calcDistToFocus(int focusX, int focusY) {
  distToFocus = (int)squaredDistance(focusX, focusY, centerX, centerY);
  // make sure casting this to an int makes sense..
}

int PerceptualGroup::getDistToFocus() {
  return distToFocus;
}
  
bool PerceptualGroup::getInSoar() {
  return inSoar;
}

void PerceptualGroup::setInSoar(bool val) {
  inSoar = val;
  return;
}

bool PerceptualGroup::isOld() {
  return old;
}


bool PerceptualGroup::isFriendlyWorker() {
  return friendlyWorker;
}
