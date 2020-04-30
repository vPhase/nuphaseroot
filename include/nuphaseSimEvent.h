#ifndef _NUPHASE_SIM_EVENT_H
#define _NUPHASE_SIM_EVENT_H
#include "nuphaseEvent.h" 


namespace nuphase
{

  class SimEvent : public Event
  { 
    public: 
      SimEvent(); 
      SimEvent(uint64_t event_number,
               uint16_t buffer_length, 
               int nchannels, 
               const  board * boards,
               const int * channels,  
               const double ** values, 
               double adc_to_mV = 10,
               double dt = 1./1.5 
             ); 

      const double * getSimData(int channel = 0, board b = BOARD_MASTER) const { return sim_data[b][channel].size() ?  &sim_data[b][channel][0]: 0 ; }
    protected:
      std::vector<double> sim_data[k::num_boards][k::num_chans_per_board]; 


      ClassDef(SimEvent,1); 
  }; 




}

#endif 
