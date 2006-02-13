#ifndef MineFSM_H
#define MineFSM_H

#include "FSM.h"
#include "general.h"

#include "PlayerInfo.H"
#include "Game.H"
#include "GameStateModule.H"

class MineFSM : public FSM {
public:
  MineFSM() {}

  bool update() {

    // TO DO: check if the mineral patch is out. If it is, return false

    switch (state) {
      case IDLE:
        // figure out what to do first
        if (gob->get_int("res_full") == 1) {
          state = MOVING_TO_BASE;
        }
        else {
          state = MOVING_TO_MINE;
        }
        break;

      case MINING:
        if (gob->get_int("is_mobile") == 1) {
          // finished mining
          state = MOVING_TO_BASE;
        }
        // otherwise don't change anything
        break;

      case MOVING_TO_MINE:
        if (squaredDistance(*gob->sod.x, *gob->sod.y, mine_x, mine_y) < 4) {
          // close to mine. The 4 is from the blueprint files
          gob->set_action("mine", mineParams);
          state = MINING;

        }
        else {
          gob->set_action("move", moveToMineParams);
        }
        break;

      case MOVING_TO_BASE:
        if (squaredDistance(*gob->sod.x, *gob->sod.y, base_x, base_y) < 4) {
          // close to base. The 4 is from the blueprint files
          gob->set_action("return_resources", depositParams);
          state = MOVING_TO_MINE;

        }
        else {
          gob->set_action("move", moveToBaseParams);
        }
        break;
    } // switch (state)

    return true;
  }

  /* the order of the parameters will be:
   *
   * mine id, mine x, mine y, base id, base x, base y
   */

  void setParams(vector<signed long> p) {
    FSM::setParams(p);

    // set up the parameter vectors for later use
    moveToMineParams.push_back(p[0]);
    moveToMineParams.push_back(p[1]);
    mineParams.push_back(p[2]);
    moveToBaseParams.push_back(p[3]);
    moveToBaseParams.push_back(p[4]);
    depositParams.push_back(p[5]);
  }

private:
  enum MineState { IDLE, MINING, MOVING_TO_MINE, MOVING_TO_BASE };
  MineState state;

  int mine_id, mine_x, mine_y;
  int base_id, base_x, base_y;

  Vector<sint4> mineParams, moveToMineParams, depositParams, moveToBaseParams;
};

#endif
