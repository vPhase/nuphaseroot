#ifndef _NUPHASE_STATUS_H 
#define _NUPHASE_STATUS_H 

#include "TObject.h" 
#include "nuphaseConsts.h" 

#ifdef HAVE_LIBNUPHASE
struct nuphase_status; 
#endif


namespace nuphase
{
  class Status : public TObject
  {
    public:
      /** Default constructor */ 
      Status(); 

#ifdef HAVE_LIBNUPHASE
        /** Constructor from raw data (requires libnuphase.so) */ 
      Status (const nuphase_status *status); 
#endif 

      uint16_t global_scalers[k::num_scalers];  //!< The overall scalers 
      uint16_t beam_scalers[k::num_scalers][k::num_beams]; //!< The scalers for each beam 
      uint32_t deadtime; //!< Deadtime fraction (units tbd) 
      uint32_t readout_time; //!< CPU time of readout
      uint32_t readout_time_ns; //!< CPU time ns of readout
      uint32_t trigger_thresholds[k::num_beams]; //!< Trigger thresholds 
      uint64_t latched_pps_time; //!< timestamp corresponding to a pps time
      uint8_t  board_id[2]; //!< the board number assigned at startup 
      uint16_t surface_scalers[k::num_scalers]; //!< Surface scalers, if present 

      ClassDef(Status,2); 
  };
}








#endif
