#ifndef NUPHASE_DATASET_H
#define NUPHASE_DATASET_H

/** Class to simplify reading/writing of data */ 


class TTree; 
class TChain; 
class TFile; 

namespace nuphase
{

  class Hk; 
  class Event; 
  class Header; 
  class Status; 
  class Dataset
  {

    public: 
      Dataset(int run, const char * basedir = ""); 
      int loadRun(int run, const char * basedir = ""); 
      int getRun() { return current_run; } 

      /** Grabs an entry within a run */ 
      int setEntry(int entry); 

      Status * status(bool force_reload = false); 
      Header * header(bool force_reload = false); 
      Event  * event(bool force_reload = false); 

      TTree * statusTree() { return status_tree; }
      TTree * headerTree() { return header_tree; }
      TTree * eventTree()  { return  event_tree; }

      /** Returns the housekeeping data for current time */ 
      Hk * hk(bool force_reload = false); 

      static TTree * hkTree(const char * base_dir = ""); 

      /** Returns the housekeeping data for a given time. */
      static Hk * hk(double when, bool force_reload = false, const char * base_dir = ""); 



    private: 
      int current_run; 

      const char * data_dir; 
      int event_index; 
      int header_index; 
      int status_index; 
      int wanted_index; 

      TFile * event_file;
      TFile * header_file;
      TFile * status_file;

      Status * the_status; 
      Event * the_event; 
      Header * the_header; 

      TTree * status_tree; 
      TTree * event_tree;; 
      TTree * header_tree; 


  };

}


#endif
