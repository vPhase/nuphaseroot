#ifndef NUPHASE_CALIBRATION_INFO_HH
#define NUPHASE_CALIBRATION_INFO_HH

#include <vector>
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

      std::vector<double> getChanPosition(int chan, board b = BOARD_MASTER) const;
      double getChanPositionX(int chan, board b = BOARD_MASTER) const { return chan_position[b][chan][0]; }
      double getChanPositionY(int chan, board b = BOARD_MASTER) const { return chan_position[b][chan][1]; }
      double getChanPositionZ(int chan, board b = BOARD_MASTER) const { return chan_position[b][chan][2]; }

    private: 
      double time_calibration; 
      double voltage_calibration[k::num_boards][k::num_chans_per_board]; 
      units t; 
      units v; 
      
      double nuphase_fiber_length[k::num_boards][k::num_chans_per_board];
      double nuphase_fiber_delay;

      double chan_position[k::num_boards][k::num_chans_per_board][3];

      ClassDef(CalibrationInfo,3); 
  }; 

}

#endif
