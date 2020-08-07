#ifndef NUPHASE_COMBINED_EVENT_H
#define NUPHASE_COMBINED_EVENT_H

class UsefulAtriStationEvent; 
class TGraph; 
class TVirtualPad; 

#include "TVector3.h" 
#include "TObject.h" 
#include <vector> 

namespace nuphase
{



  class Header; 
  class Event; 

  class CombinedEvent : public TObject
  {
    public: 
      CombinedEvent() { ; }


      //There are a LOT of options here, since a lot of the logic is moved elsewhere. 
      struct Opts
      {

        //Used by factory method, but usually will get from environment! 
        const char * nuphase_prefix = ""; // get from environment, NUPHASE_ROOT_DATA
        const char * a5_prefix = ""; // get from environment, ARA5_ROOT_DATA 
        const char * a5_pedestal_dir = ""; // get from environment, ARA5_PEDESTAL_DATA; 


        // will set time delays based on cross-correlation of these channels if they're both non-negative. TODO. 
        int align_channel_nuphase = -1; 
        int align_channel_ara5 = -1; 


        enum 
        {
          INTERPOLATE_LINEAR, 
          INTERPOLATE_AKIMA
        } interpolation_method = INTERPOLATE_AKIMA; 

        double sample_rate = 3; //GHz

        //returns a default instantiated version of this 
        static const Opts & defaultOpts(); 


      }; 


      enum AntennaType
      {
        NUPHASE_VPOL, 
        NUPHASE_HPOL, 
        ARA5_VPOL, 
        ARA5_HPOL,
        SURFACE, 
        NONE 
      }; 




      //factory method. NOT THREAD SAFE DUE TO CALIBRATOR 
      static CombinedEvent * makeCombinedEvent( int nuphase_run, int nuphase_index, 
                                                int ara5_run, int ara5_index, const Opts & opts = Opts::defaultOpts(), 
                                                CombinedEvent * useme = 0); 


      CombinedEvent(const Header * nuphase_header, const Event * nuphase_event, 
                    const UsefulAtriStationEvent  * ara_event, const Opts & opts = Opts::defaultOpts()); 
                    

      const double * getData(int chan) const { return &data[chan][0] ; }
      unsigned getNSamples(int chan) const { return data[chan].size(); }
      double getT0(int chan) const { return t0s.at(chan) ; }
      double getDeltaT(int chan) const { return 1./ sample_rate.at(chan); };
      double getSampleRate(int chan) const { return sample_rate.at(chan); }
      const char * getChannelName(int chan) const { return channel_names.at(chan).c_str(); }
      TGraph * makeGraph(int chan, TGraph *useme = 0) const; 

      bool stationsTimeAligned() const { return did_we_align; } 

      int getA5EventId() const { return ara_event_id; } 
      int getNuphaseRun() const { return nuphase_run; } 
      int getNuphaseEntry() const { return nuphase_entry; } 
      int getNChannels() { return channel_names.size(); } 

      void drawAll(TVirtualPad * pad = 0, int max_width = 5); 

    private: 

      std::vector<AntennaType>  types; 
      std::vector<std::string> channel_names; //stores 
      std::vector<std::vector<double> > data; ; 
      std::vector<double> sample_rate; 
      std::vector<double> t0s; 
      std::vector<TVector3> antenna_positions; 
      bool did_we_align; 
      int ara_event_id; 
      int nuphase_run; 
      int nuphase_entry; 
      Opts opts; 

      ClassDef(CombinedEvent,1); 
  };




}


#endif
