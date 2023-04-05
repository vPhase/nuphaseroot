#ifndef _NUPHASE_EVENT_H 
#define _NUPHASE_EVENT_H 


#include "TObject.h" 
#include "nuphaseConsts.h"
#include <stdint.h> 
#include <vector>
#include "nuphaseCalibrationInfo.h" 

class TGraph; 

#ifdef HAVE_LIBNUPHASE
struct nuphase_event; 
#endif

namespace nuphase
{

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
       */ 
      const UChar_t * getRawData(int channel, board b = BOARD_MASTER) const { return raw_data[b][channel].size() ?  &raw_data[b][channel][0]: 0 ; }

      /** Sets the calibration for this event. This is used to convert from adc to Volts*/ 
      void setCalibrationInfo(const CalibrationInfo & info) { dumpCalibrated(); calibration = info; }  


      /** This creates a calibrated, properly named,  TGraph of this event 
       *
       *  You may choose to provide a TGraph in useme to avoid allocating a new one, but 
       *  deleting it is the responsibility of the user. 
       *
       **/ 
      TGraph * getGraph(int channel = 0, 
                       board b = BOARD_MASTER, 
                       TGraph * useme = 0) const; 

      /** Retrieves calibrated data. 
       * This returns an internal copy that might get modified if setCalibrationInfo is called and will
       * be lost if this event is deallocated.. 
       **/ 
      const double * getData(int channel = 0, board b = BOARD_MASTER) const; 

      /** Returns a copy of calibrated data. Useful if you want to do something with it or w*/ 
      double * copyData(int channel = 0, board b = BOARD_MASTER, double * dest = 0 ) const; 

      uint16_t getBufferLength() const { return buffer_length; } 

      uint64_t getEventNumber() const { return event_number; } 
      bool isSurface() const { return !board_id[0] && board_id[1]; } 

    private: 
      /** The event number */ 
      uint64_t event_number; 
      /** The buffer length */
      uint16_t buffer_length;
      uint8_t board_id[k::num_boards]; 
      std::vector<UChar_t> raw_data[k::num_boards][k::num_chans_per_board]; 
      mutable std::vector<double> data[k::num_boards][k::num_chans_per_board];  //!  
      void dumpCalibrated() const;   
      mutable uint64_t calibrated_event_number; //! 
      CalibrationInfo calibration; //! 

    ClassDef(Event,3); 

  }; 
} 

#endif
