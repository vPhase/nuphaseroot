#ifndef NUPHASE_DATASET_H
#define NUPHASE_DATASET_H

/** Class to simplify reading/writing of data */ 


class TTree; 

namespace nuphase
{

  class Hk; 
  class Event; 
  class Header; 
  class Status; 
  class Dataset
  {

    public: 
      Dataset(int run =-1, int detector = 1); 

      /** Loads a particular run */ 
      int loadRun(int run, int detector = 1); 

      /* Gets an event. Will load a new run if necessary*/ 
      int getEvent(int event, int detector = 1); 

      /** Grabs an entry within a run */ 
      int getEntry(int entry); 

      Status * status(bool force_reload = false); 
      Header * header(bool force_reload = false); 
      Event  * event(bool force_reload = false); 

      TTree * statusTree(); 
      TTree * headerTree(); 
      TTree * eventTree(); 


      /** Returns the housekeeping data for a given time. If 0 is passed, the time of the current event (if any) is used.*/ 
      Hk * hk(double when = 0, bool force_reload = false); 



    private: 
      int current_run; 
      int current_det; 
      int current_event_index; 


      Status * the_status; 
      Event * the_event; 
      Header * the_header; 

      Hk * the_hk; 
  };

}


#endif
