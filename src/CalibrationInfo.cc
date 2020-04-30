#include "nuphaseCalibrationInfo.h" 

ClassImp(nuphase::CalibrationInfo); 

nuphase::CalibrationInfo::CalibrationInfo() 
{
  time_calibration = 1/1.5;
  t = UNITS_NS; 

  nuphase_fiber_delay = 4.880; //ns/m

  
  const static double temp_len[k::num_boards][k::num_chans_per_board] = {{2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 10.0},
									{12.0, 14.0, 1.0, 10.0, 0.0, 0.0, 0.0, 0.0}};
  memcpy(nuphase_fiber_length, temp_len, sizeof(temp_len)); 


  for (int board = 0; board < k::num_boards; board++)
  {
    for (int chan = 0; chan < k::num_chans_per_board; chan++)
    {
      voltage_calibration[board][chan] = 1; 
    }
  }

  v = UNITS_ADC; 
}

nuphase::CalibrationInfo::CalibrationInfo(double tc, double vc) 
{
  time_calibration = tc; 
  t = UNITS_NS; 

  memset(nuphase_fiber_length,0,sizeof(nuphase_fiber_length)); 
  nuphase_fiber_delay = 0; 
  for (int board = 0; board < k::num_boards; board++)
  {
    for (int chan = 0; chan < k::num_chans_per_board; chan++)
    {
      voltage_calibration[board][chan] = v; 
    }
  }

  v = UNITS_MV; 

}


