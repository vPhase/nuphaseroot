#include "nuphaseCalibrationInfo.h" 

ClassImp(nuphase::CalibrationInfo); 

nuphase::CalibrationInfo::CalibrationInfo() 
{
  time_calibration = 1/1.5;
  t = UNITS_NS; 

  nuphase_fiber_delay = 4.880; //ns/m

  
  const static double temp_len[k::num_boards][k::num_chans_per_board] = {{2.0, 3.13827759, 3.98370041, 5.09002215, 6.0269715, 29.0401805, 8.16781324, 10.0752401},
                                                                         {12.0, 14.0, 23.4384754, -2.97043852, 23.3151191, -1.34546578, 23.8567027, -0.55532582}};
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
