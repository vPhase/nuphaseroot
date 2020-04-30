#include "nuphaseSimEvent.h" 
ClassImp(nuphase::SimEvent); 

nuphase::SimEvent::SimEvent() 
  : Event() 
{

}

nuphase::SimEvent::SimEvent(uint64_t event,
               uint16_t length, 
               int nchannels, 
               const  board * boards,
               const int * channels,  
               const double ** values,
               double adc_to_mV , double dt
             ) 

{
  event_number =event; 
  buffer_length = length;
  calibration = CalibrationInfo(dt, adc_to_mV); 
  board_id[0] = BOARD_MASTER;
  board_id[1] = BOARD_SLAVE;

  for (int i = 0; i < nchannels; i++) 
  {
    std::vector<double> & v = sim_data[boards[i]][channels[i]]; 
    v.insert(v.begin(), values[i], values[i] + buffer_length); 
    std::vector<UChar_t> & rv = raw_data[boards[i]][channels[i]]; 
    rv.resize(buffer_length); 
    for (int j = 0; j < buffer_length; j++) 
    {
      rv[i] = v[i]/adc_to_mV + 64; 
    }
  }
}




