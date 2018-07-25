#ifndef NUPHASE_CALIBRATION_INFO_HH
#define NUPHASE_CALIBRATION_INFO_HH

#include "TObject.h" 
#include "nuphaseConsts.h" 

namespace nuphase
{

  /** The CalibrationInfo knows the conversions between
   *  raw and physical units 
   *  */ 
  class CalibrationInfo   : public TObject
  {

    public: 
      /* Construct the default CalibrationInfo */ 
      CalibrationInfo(); 

      double getVoltageCalibration(board b, int chan) const { return voltage_calibration[b][chan]; } 
      double getTimeCalibration() const { return time_calibration; } 
      double getLenDelayConst() const { return nuphase_fiber_delay; } 
      double getCableLen(board b, int chan) const { return nuphase_fiber_length[b][chan]; } 

      units getTimeUnits()const  { return t; }
      units getVoltageUnits()const  { return v; }

    private: 
      double time_calibration; 
      double voltage_calibration[k::num_boards][k::num_chans_per_board]; 
      units t; 
      units v; 
      
      double nuphase_fiber_length[k::num_boards][k::num_chans_per_board];
      double nuphase_fiber_delay;

      ClassDef(CalibrationInfo,2); 
  }; 

}

#endif
