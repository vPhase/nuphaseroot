#ifndef _NUPHASE_EVENT_H 
#define _NUPHASE_EVENT_H 


#include "TObject.h" 
#include "nuphaseConsts.h"
#include <stdint.h> 
#include <vector >

class TGraph; 

#ifdef HAVE_LIBNUPHASE
struct nuphase_event; 
#endif

namespace nuphase
{

  class CalibrationInfo; 

  class Event : public TObject 
  {

    public: 
      /** Default constructor */
      Event(); 


#ifdef HAVE_LIBNUPHASE
      /** Constructor from raw data (requires libnuphase.so) */ 
      Event (const nuphase_event *event); 
#endif 

      /** Return the raw data for a board and channel. 
       * If there is no data for that channel, return NULL. 
       * This is NOT baseline subtracted. 
       */ 
      UChar_t * getRawData(nuphase::board b, int channel) const; 

      /** This creates a calibrated, properly named,  TGraph of this event 
       *
       *  You may choose to provide a TGraph in useme to avoid allocating a new one, but 
       *  deleting it is the responsibility of the user. 
       *
       *  If calibration info is passed, then that will be used to calibrate the voltage scale. Otherwise,
       *  it will just be baseline-subtracted. 
       *
       **/ 
      TGraph * getData(int channel = 0, 
                       nuphase::board b = nuphase::BOARD_MASTER, 
                       TGraph * useme = 0, CalibrationInfo * info = 0) const; 


      /** The event number */ 
      uint64_t event_number; 


      /** The buffer length */
      uint16_t buffer_length;
      uint8_t board_id[nuphase::k::num_boards]; 

    private: 
      std::vector<UChar_t> data[nuphase::k::num_boards][nuphase::k::num_chans_per_board]; 

    ClassDef(Event,1); 

  }; 
} 

#endif
