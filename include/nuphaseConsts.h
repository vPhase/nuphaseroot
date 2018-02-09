#ifndef _NUPHASE_CONSTS_H
#define _NUPHASE_CONSTS_H

namespace nuphase
{
  namespace k
  {
    const int num_boards = 2;
    const int num_chans_per_board = 8; 
    const int num_beams = 15; 
    const int num_scalers = 3; 
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
  } ;

  enum sync_problem
  {
    SYNC_PROBLEM_NONE = 0, 
    SYNC_PROBLEM_SW_HW_BUFFER_MISMATCH=1, 
    SYNC_PROBLEM_TRIGNUM_MISMATCH=2, 
    SYNC_PROBLEM_TRIGTYPE_MISMATCH=4, 
    SYNC_PROBLEM_MASTER_SLAVE_BUFFER_MISMATCH=8 
  } ;

  enum scaler_type
  {
    SCALER_SLOW, 
    SCALER_SLOW_GATED,
    SCALER_FAST
  } ;

  enum asps_power_state
  {
    ASPS_POWER_FRONTEND = 1,
    ASPS_POWER_SBC      = 2, 
    ASPS_POWER_SLAVE    = 4, 
    ASPS_POWER_MASTER   = 8, 
    ASPS_POWER_SWITCH   = 16  
  };

  enum gpio_power_state
  {
    GPIO_FPGA_POWER_MASTER = 1, 
    GPIO_FPGA_POWER_SLAVE = 2,
    GPIO_SPI_ENABLE = 4,
//    GPIO_DOWNHOLE_POWER= 8 , 
    GPIO_AUX_HEATER=16 

  }; 

  enum units
  {
    UNITS_NS, 
    UNITS_SAMPLE, 
    UNITS_ADC, 
    UNITS_MV 
  }

  const char * getUnitString(units u); 

}

#endif
