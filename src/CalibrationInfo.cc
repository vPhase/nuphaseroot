#include "nuphaseCalibrationInfo.h" 

ClassImp(nuphase::CalibrationInfo); 

nuphase::CalibrationInfo::CalibrationInfo() 
{
  time_calibration = 1/1.5;
  t = UNITS_NS; 

  nuphase_fiber_delay = 4.880; //ns/m

  
  const static double temp_len[k::num_boards][k::num_chans_per_board] = {{2.0, 3.13827759, 3.98370041, 5.09002215, 6.0269715, 29.0401805, 8.16781324, 10.0752401},
                                                                         {12.0, 14.0, 30.0108607, 3.60796107, 29.8881148, 5.26577869, 30.4872951, 6.01639344}};
  memcpy(nuphase_fiber_length, temp_len, sizeof(temp_len)); 

  for (int board = 0; board < k::num_boards; board++)
  {
    for (int chan = 0; chan < k::num_chans_per_board; chan++)
    {
      voltage_calibration[board][chan] = 1; 
    }
  }

  v = UNITS_ADC;

  // channel positions are relative to top of PA borehole in meters
  const static double temp_pos[k::num_boards][k::num_chans_per_board][3] = {{ // board 0
                                                                             {0,0,-172.635}, // 0 (chan 0)
                                                                             {0,0,-173.65}, // 1 (chan 1)
                                                                             {0,0,-174.66}, // 2 (chan 2)
                                                                             {0,0,-175.68}, // 3 (chan 3)
                                                                             {0,0,-176.70}, // 4 (chan 4)
                                                                             {12.3309,-31.8967,-190.8559}, // 5 (chan 5 = ARA RFchan 7 =  ARA elecChan 24)
                                                                             {0,0,-178.75}, // 6 (chan 6)
                                                                             {0,0,-180.79}}, // 7 (chan 7) 
                                                                            { // board 1
                                                                             {0,0,-182.79}, // 0 (chan 8, Hpol positions are a guesstimate ^-^)
                                                                             {0,0,-184.79}, // 1 (chan 9, Hpol positions are a guesstimate ^-^)
                                                                             {-12.9616,-8.6511,-177.7466}, // 2 (chan 10 = ARA RFchan 6 =  ARA elecChan 16)
                                                                             {12.3428,-31.8885,-161.02376}, // 3 (chan 11 = ARA RFchan 3 = ARA elecChan 25)
                                                                             {1.5493,15.6665,-196.2045}, // 4 (chan 12 = ARA RFchan 5 = ARA elecChan 0)
                                                                             {1.5458,15.6587,-166.5366}, // 5 (chan 13 = ARA RFchan 1 = ARA elecChan 1)
                                                                             {29.6505,-3.2845,-194.739}, // 6 (chan 14 = ARA RFchan 4 = ARA elecChan 8)
                                                                             {29.6347,-3.3004,-165.0948}}}; // 7 (chan 15 = ARA RFchan 0 = ARA elecChan 9)
  memcpy(chan_position, temp_pos, sizeof(temp_pos));
}

std::vector<double> nuphase::CalibrationInfo::getChanPosition(int chan, board b)
  const
{
  std::vector<double> v(chan_position[b][chan], chan_position[b][chan] + 3);
  return v;
}

double nuphase::CalibrationInfo::getCableLen(board b, int chan)
  const
{
  // double check board/chan numbers are right
  b = (chan/k::num_chans_per_board == 0)? board::BOARD_MASTER : board::BOARD_SLAVE;
  chan = chan%k::num_chans_per_board;
 
  return nuphase_fiber_length[b][chan];
}
