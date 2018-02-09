#ifndef _NUPHASE_HEADER_H 
#define _NUPHASE_HEADER_H 


#include "TObject.h" 
#include "nuphaseConsts.h"
#include <stdint.h> 
#include <vector>

class TGraph; 

#ifdef HAVE_LIBNUPHASE
struct nuphase_header; 
#endif

namespace nuphase
{


  class Header : public TObject 
  {

    public: 
      /** Default constructor */
      Header(); 

#ifdef HAVE_LIBNUPHASE
      /** Constructor from raw data (requires libnuphase.so) */ 
      Header (const nuphase_header *event); 
#endif 


      /* Helper functions
       *
       **/ 

      /** Returns true if the given beam triggered */ 
      bool beamTriggered(uint8_t beam) const  { return (1 <<beam) & triggered_beams; } 


      /** The data members (mostly the same as the struct in libnuphase) */ 
      uint64_t event_number; 
      uint64_t trig_number; 
      uint16_t buffer_length;
      uint16_t pretrigger_samples; 
      uint32_t readout_time[k::num_boards];          //!< CPU time of readout, seconds
      uint32_t readout_time_ns[k::num_boards];       //!< CPU time of readout, nanoseconds 
      uint64_t trig_time[k::num_boards];             //!< Board trigger time (raw units) 
      uint32_t raw_approx_trigger_time;                  //!< Raw Board trigger time converted to real units (approx secs), master only
      uint32_t raw_approx_trigger_time_nsecs;            //!< Raw Board trigger time converted to real units (approx nnsecs), master only
      uint16_t triggered_beams;                      //!< The beams that triggered 
      uint16_t beam_mask;                            //!< The enabled beams
      uint32_t beam_power[k::num_beams];             //!< The power in each beam at the trigger time
      uint32_t deadtime[k::num_boards];              //!< ??? Will we have this available? If so, this will be a fraction. (store for slave board as well) 
      uint8_t buffer_number;                         //!< the buffer number (do we need this?) 
      uint8_t channel_mask;                          //!< The channels allowed to participate in the trigger
      uint8_t channel_read_mask[k::num_boards];      //!< The channels actually read
      uint8_t channel_overflow;                      //!< Bitmask of channels that overflowed the 5 bits 
      uint8_t buffer_mask;                           //!< The buffer mask at time of read out (do we want this?)   
      uint8_t board_id[k::num_boards];      //!< The board number assigned at startup. If board_id[1] == 0, no slave. 
      trig_type trig_type;                 //!< The trigger type? 
      uint8_t calpulser;                             //!< Was the calpulser on? 
      uint8_t sync_problem;                          //!< Various sync problems. TODO convert to enum 


      uint32_t corrected_trigger_time;             //!< The offline corrected triggertime
      uint32_t corrected_triger_time_ns;           //!< The offline corrected trigger time (ns) 

    ClassDef(Header,1); 
  }; 
} 

#endif
