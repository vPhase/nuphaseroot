#include "nuphaseDataset.h" 

#include "TFile.h" 
#include "TTree.h" 
#include "nuphaseHeader.h" 
#include "nuphaseStatus.h" 
#include "nuphaseEvent.h" 
#include "nuphaseHk.h" 


nuphase::Dataset::Dataset(int run, const char * basedir) 
{
  the_status= 0; 
  the_event = 0; 
  the_header = 0;

  
  status_tree = 0; 
  header_tree = 0; 
  event_tree = 0; 

  current_run =-1; 
  loadRun(run,basedir); 
}

int nuphase::Dataset::loadRun(int run, const char * basedir) 
{
  data_dir = basedir; 
  if (!basedir || !strcmp(basedir,"")) basedir = getenv("NUPHASE_ROOT_DATA"); 
  if (!basedir)
  {
    fprintf(stderr,"Could not load basedir. Specify or define NUPHASE_ROOT_DATA"); 
    return -1; 
  }
 
  TString str; 

  str.Form("%s/run%d/event.root", basedir, run); 

  if (event_file) delete event_file; 
  event_file = TFile::Open(str.Data()); 
  if (!event_file) return -1; 

  event_tree = (TTree*) event_file->Get("event"); 
  if (!event_tree)
  {
    fprintf(stderr,"Could not find event tree in %s", str.Data()); 
    return -1; 
  }
  event_tree->SetBranchAddress("event",&the_event); 


  str.Form("%s/run%d/header.filtered.root", basedir, run); 

  if (header_file) delete header_file; 
  header_file = TFile::Open(str.Data()); 
  if (!header_file) return -1; 

  header_tree = (TTree*) header_file->Get("header"); 
  if (!header_tree)
  {
    fprintf(stderr,"Could not find header tree in %s", str.Data()); 
    return -1; 
  }
  header_tree->SetBranchAddress("header",&the_header); 

  str.Form("%s/run%d/status.root", basedir, run); 

  if (status_file) delete status_file; 
  status_file = TFile::Open(str.Data()); 
  if (!status_file) return -1; 

  status_tree = (TTree*) status_file->Get("status"); 
  if (!status_tree)
  {
    fprintf(stderr,"Could not find status tree in %s", str.Data()); 
    return -1; 
  }
  status_tree->SetBranchAddress("status",&the_status); 
  status_tree->BuildIndex("status.readout_time","status.readout_time_ns"); 
  event_index = -1; 
  status_index = -1; 
  header_index = -1; 
  wanted_index = 0; 

  current_run = run; 
  return 0; 

}


int nuphase::Dataset::setEntry(int entry) 
{
  if (!header_tree) 
  {
    fprintf(stderr,"No run loaded!\n"); 
    return -1; 
  }

  if (entry < 0 || entry >= header_tree->GetEntries()) 
  {
    fprintf(stderr,"Entry %d is out of bounds\n", entry); 
    return -1; 
  }
  wanted_index = entry; 
  return wanted_index; 
}


nuphase::Header * nuphase::Dataset::header(bool force) 
{

  if (force || wanted_index != header_index) 
  {
    header_tree->GetEntry(wanted_index); 
    header_index = wanted_index; 
  }
  return the_header; 
}

nuphase::Event * nuphase::Dataset::event(bool force) 
{

  if (force || wanted_index != event_index) 
  {
    event_tree->GetEntry(wanted_index); 
    event_index = wanted_index; 
  }
  return the_event; 
}

nuphase::Status * nuphase::Dataset::status(bool force) 
{

  int wanted_status = status_tree->GetEntryNumberWithBestIndex(header()->readout_time[0], header()->readout_time_ns[0]); 

  if (force || status_index != wanted_status) 
  {
    status_tree->GetEntry(wanted_status); 
    status_index = wanted_status; 
  }
  return the_status; 

}


nuphase::Hk * nuphase::Dataset::hk( bool force) 
{

  return hk( header()->readout_time[0] + header()->readout_time_ns[0]*1e-9, force); 
}


static TFile * hkfile =0; 
static TTree * hktree =0; 
static nuphase::Hk * the_hk = 0; 
static int hk_entry;


TTree * nuphase::Dataset::hkTree(const char * basedir) 
{

  if (!hkfile || (basedir && *basedir && !strstr(basedir, hkfile->GetName())))
  {

    if (!basedir || !*basedir) 
    {
      basedir = getenv("NUPHASE_ROOT_DATA"); 
    }

    if (hkfile) delete hkfile; 

    TString str; 
    str.Form("%s/hk/all_hk.root", basedir); 

    hkfile = TFile::Open(str.Data()); 
    hktree = (TTree*) hkfile->Get("hk"); 
    hktree->SetBranchAddress("hk",&the_hk); 
    hktree->BuildIndex("unixTime","unixTimeMilliSeconds"); 
    hk_entry =-1; 
  }
  return hktree; 
}


nuphase::Hk * nuphase::Dataset::hk(double when, bool force, const char * basedir) 
{
  

  int wanted_entry = hkTree()->GetEntryNumberWithBestIndex(floor(when), floor(1000*(when-floor(when)))); 

  if (force || hk_entry != wanted_entry) 
  {

    hktree->GetEntry(wanted_entry); 
    hk_entry = wanted_entry; 
  }

  return the_hk; 
}



