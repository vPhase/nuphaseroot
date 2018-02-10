#include "nuphaseCalibrationInfo.h" 

ClassImp(nuphase::CalibrationInfo); 

nuphase::CalibrationInfo::CalibrationInfo() 
{
  time_calibration = 1/1.5;
  t = UNITS_NS; 

  for (int board = 0; board < k::num_boards; board++)
  {
    for (int chan = 0; chan < k::num_chans_per_board; chan++)
    {
      voltage_calibration[board][chan] = 1; 
    }
  }

  v = UNITS_ADC; 
}
