#ifndef _NUPHASE_CONSTS_H
#define _NUPHASE_CONSTS_H

namespace nuphase
{
  namespace k
  {
    const int num_boards = 2;
    const int num_chans_per_board = 8; 
    const int num_beams; 
  }

  enum board
  {
    BOARD_MASTER = 0,
    BOARD_SLAVE = 1
  }; 

  enum trig_type 
  {
    TRIGGER_NONE = 0, 
    TRIGGER_RF = 1, 
    TRIGGER_SW = 2, 
    TRIGGER_EXT = 4 
  }; 

  enum sync_problem
  {
    SYNC_PROBLEM_NONE = 0, 
    SYNC_PROBLEM_SW_HW_BUFFER_MISMATCH=1, 
    SYNC_PROBLEM_TRIGNUM_MISMATCH=2, 
    SYNC_PROBLEM_TRIGTYPE_MISMATCH=4, 
    SYNC_PROBLEM_MASTER_SLAVE_BUFFER_MISMATCH=8 
  }; 
}


#endif
