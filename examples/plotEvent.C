gSystem->load("libnuphaseroot.so"); // this should be somewhere  findable! 

void plotEvent(int run, int i= 0)
{
  // get the data directory which we assume is stored in the environmental variable NUPHASE_ROOT_DATA
  const char * data_dir = getenv("NUPHASE_ROOT_DATA"); 
  if (!data_dir){ fprintf(stderr, "Please define NUPHASE_ROOT_DATA"); return; } 
  
  // Open up the event file and set up reading
  TFile fevent(Form("%s/event.root", data_dir)); 
  TTree * events = (TTree*) f.Get("event");
  nuphase::Event * ev  = 0; 
  events->SetBranchAddress("event",&ev); 
  
  // Open up the header file and set up reading. 
  // For partial datasets (from transmission), header.filtered.root will match events in event.root while header.root will have all events.
  // For full datasets, header.root and header.filtered.root are identical, so using header.filtered.root is safe
  TFile fhead(Form("%s/header.filtered.root", data_dir)); 
  TTree * headers = (TTree*) f.Get("header"); 
  nuphase::Header * head = 0; 
  headers->SetBranchAddress("header",&head); 
  
  //Load the ith entry 
  events->GetEntry(i); 
  headers->GetEntry(i); 

  head->Print(); //some evidence that we have the event
  event->getGraph(0)->Draw(); //just draw the first channel as a proof of concept 


}
