/* This just reduces the number of entries in a tree by a factor of N */ 

#include <cstdio>

#include "TTree.h" 
#include "TFile.h" 
#include "nuphaseEvent.h" 
#include "nuphaseHeader.h" 
#include "nuphaseStatus.h" 
#include "nuphaseHk.h" 


int main(int nargs, char ** args) 
{
  char * what = nargs > 1 ? args[1] :  0; 

  if (nargs < 4 || (strcmp(what,"hk") && strcmp(what,"status") && strcmp(what,"header") && strcmp(what,"event")))
  {
    fprintf(stderr, "Usage: nuphaseroot-decimate what=[hk status header event] decimate_factor input_files\n"); 
    return -1; 
  }


  int decimate = atoi(args[2]); 


  for (int fileindex = 3; fileindex < nargs; fileindex++)
  {

    TFile fin(args[fileindex]); 
    TString fname(args[fileindex]); 
    fname.ReplaceAll(".root",".decimated.root"); 
    if (!strcmp(fname.Data(), args[fileindex]))
    {
      fprintf(stderr,"Input and output have same name...continuing..."); 
      continue; 
    }

    TTree * tin = (TTree*) fin.Get(what); 

    TFile fout(fname.Data(),"RECREATE"); 

    TTree * tout = new TTree(what,what); 
    TObject * ptr = 0; 
    tin->SetBranchAddress(what,&ptr); 
    tin->GetEntry(0); 
    tout->Branch(what,ptr); 
    int i = 0; 
    tout->Branch("original_entry", &i); 

    for (i = 0; i < tin->GetEntries(); i++)
    {
      if (i % decimate == 0) 
      {
        tin->GetEntry(i); 
        tout->Fill(); 
      }
    }

    fout.Write(); 
  }


  return 0; 


}
