
#ifdef HAVE_LIBNUPHASE

#include "nuphaseConverter.h" 
#include <vector> 
#include "nuphaseHk.h" 
#include "nuphaseEvent.h" 
#include "nuphaseStatus.h" 
#include "nuphaseHeader.h" 
#include "nuphase.h" 

#include "TTree.h" 
#include "<zlib.h>"
#include "TFile.h" 
#include <stdio.h> 

static bool allow_overwrite = false; 
void nuphase::convert::setAllowOverwrite(bool allow) { allow_overwrite = allow; } 

static template <typename RootType, typename RawType, typename RawReadFunction> 
int convert(int nfiles, const char ** infiles, const char * outfile, const char * treename) 
{
  int nprocessed = 0; 

  TFile of(outfile, allow_overwrite ? "RECREATE" : "CREATE"); 
  TTree * tree = new TTree(treename,treename); 
  RootType * ptr = new RootType; 
  RawType raw; 
  tree->Branch(treename,&ptr); 
  for (int i = 0; i < nfiles; i++) 
  {
    gzFile f = gzopen(infiles[i],"r"); 
    while(!RawReadFunction(f,&raw))
    {
      nprocessed++; 
      new (ptr) RootType(&raw); 
      tree->Fill(); 
    }
    gzclose(f); 
  }
  of.Write(); 

  return nprocessed;
}

static template<typename RootType, typename RawType, typename RawReadFunction>
int convertDir(const char * dir, const char * outfile, const char * treeName)
{
  std::vector<std::pair<int mtime, std::string> > files; 
  std::vector<const char * > > file_ptrs; 


  int nprocessed = convert<RootType,RawType,RawReadFunction>(nfiles,&files[0], outfile, treeName); 

  for (unsigned i = 0; i < nfiles; i++) free(files[i]); 
  return nprocessed; 
}


int nuphase::convert::convertHkFiles(int nfiles, const char ** infiles, const char * outfile, const char * treeName) 
{
  return convert<Hk, nuphase_hk,nuphase_hk_gzread> (nfiles, infiles, outfile, treeName)
}

int nuphase::convert::convertHeaderFiles(int nfiles, const char ** infiles, const char * outfile, const char * treeName) 
{
  return convert<Header, nuphase_header,nuphase_header_gzread> (nfiles, infiles, outfile, treeName)
}

int nuphase::convert::convertEventFiles(int nfiles, const char ** infiles, const char * outfile, const char * treeName) 
{
  return convert<Event, nuphase_event,nuphase_event_gzread> (nfiles, infiles, outfile, treeName)
}

int nuphase::convert::convertStatusFiles(int nfiles, const char ** infiles, const char * outfile, const char * treeName) 
{
  return convert<Status, nuphase_status,nuphase_status_gzread> (nfiles, infiles, outfile, treeName)
}


int nuphase::convert::convertHkDir(const char * dir, const char * outfile, const char * treeName) 
{
  return convertDir<Hk, nuphase_hk,nuphase_hk_gzread> (dir, outfile, treeName)
}

int nuphase::convert::convertHeaderDir(const char * dir, const char * outfile, const char * treeName) 
{
  return convertDir<Header, nuphase_header,nuphase_header_gzread> (dir, outfile, treeName)
}

int nuphase::convert::convertEventDir(const char * dir, const char * outfile, const char * treeName) 
{
  return convertDir<Event, nuphase_event,nuphase_event_gzread> (dir, outfile, treeName)
}

int nuphase::convert::convertStatusDir(const char * dir, const char * outfile, const char * treeName) 
{
  return convertDir<Status, nuphase_status,nuphase_status_gzread> (dir, outfile, treeName)
}













#endif
