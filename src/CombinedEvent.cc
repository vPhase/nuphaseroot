
#include "nuphaseCombinedEvent.h" 

ClassImp(nuphase::CombinedEvent); 
#include "TPad.h" 
#include "TCanvas.h" 
#include "TGraph.h" 

#ifdef HAVE_ARAROOT
#include "RawAtriStationEvent.h" 
#include "UsefulAtriStationEvent.h" 
#include "AraEventCalibrator.h" 
#include <map> 
#include "TFile.h"
#include "nuphaseDataset.h" 
#include "nuphaseHeader.h" 
#include "nuphaseEvent.h" 
#include <fstream> 
#include "TTree.h" 
#include "TSystem.h" 
#include "FFTtools.h" 




const nuphase::CombinedEvent::Opts & nuphase::CombinedEvent::Opts::defaultOpts() 
{
  static Opts opts; 
  return opts; 
}


static std::map<int,std::string> a5_map; 



static TFile * openAraEventFile(int run, std::string a5dir) 
{
  if (a5dir == "") a5dir = getenv("ARA5_ROOT_DATA"); 

  TString str; 
  if(!a5_map.size()) 
  {
    str.Form("%s/share/nuphaseroot/a5.map", getenv("NUPHASE_INSTALL_DIR")); 


//    printf("%s\n", str.Data()); 
    std::ifstream map(str.Data()); 

    int run;
    std::string path; 
    while ((map >> run >> path))
    {
      a5_map[run] = path; 
//      printf("%d %s\n", run, path.c_str()); 
    }
  }

  if (!a5_map.count(run)) 
  {
    fprintf(stderr,"Count not find run %d in map :(\n", run); 
    return 0; 
  }

  str.Form("%s/%s/event%06d.root", a5dir.c_str(), a5_map[run].c_str(), run); 

  return TFile::Open(str.Data()); 
}




nuphase::CombinedEvent * 
nuphase::CombinedEvent::makeCombinedEvent(int nuphase_run, int nuphase_index, 
                                          int a5_run, int a5_index, const Opts & opts, nuphase::CombinedEvent * useme) 
{
  static nuphase::Dataset * nuphase_dataset = 0; 
  static int last_a5_run = -1; 



  if (!nuphase_dataset || nuphase_dataset->getRun() != nuphase_run) 
  {
    if (nuphase_dataset) delete nuphase_dataset; 

    nuphase_dataset = new nuphase::Dataset(nuphase_run, opts.nuphase_prefix); 
  }


  nuphase_dataset->setEntry(nuphase_index); 


  static TFile * a5file = 0;
  static TTree * a5tree = 0; 
  static RawAtriStationEvent * a5event = 0; 

  if (last_a5_run != a5_run) 
  {
    if (a5file) 
    {
      delete a5file; 
    }
    a5file = openAraEventFile(a5_run, opts.a5_prefix); 
    if (!a5file) return 0; 

    a5tree = (TTree*) a5file->Get("eventTree"); 
    a5tree->SetBranchAddress("event", &a5event); 


    //set the pedestal file, if we can 

    TString pedbase = opts.a5_pedestal_dir; 
    if (pedbase=="") pedbase = getenv("ARA5_PEDESTAL_DIR"); 
    TString pedfile;
    pedfile.Form("%s/pedFile_%d.dat", pedbase.Data(), a5_run); 

    if (gSystem->AccessPathName(pedfile.Data(),kReadPermission))
    {
      fprintf(stderr,"Could not find pedestal file for run %d (looked for %s). Maybe run repeder to generate it?\n", a5_run, pedfile.Data()); 

    }
    else
    {
      AraEventCalibrator::Instance()->setPedFile((char*) pedfile.Data(),5); 
    }

    last_a5_run = a5_run; 
  }

  if (a5_index > a5tree->GetEntries())
  {
    fprintf(stderr, "%d bigger than max entry %lld\n", a5_index, a5tree->GetEntries()); 
    return 0; 
  }

  a5tree->GetEntry(a5_index); 
  
  UsefulAtriStationEvent useful(a5event); 


  if (useme) 
  {
    useme->~CombinedEvent(); 
    return new (useme) CombinedEvent( nuphase_dataset->header(), nuphase_dataset->event(), &useful, opts); 
  }
  else
  {
    return new CombinedEvent(nuphase_dataset->header(), nuphase_dataset->event(), &useful, opts); 
  }
}
                                                                          


nuphase::CombinedEvent::CombinedEvent(const Header * nuphase_header, const Event * nuphase_event, 
                                      const UsefulAtriStationEvent  * ara_event, const Opts & opts)
{

  //TODO! 
  did_we_align  = false; 

  //let's fill in the phased array antennas first 

  nuphase_run = nuphase_header->event_number / 1000000000; ; 
  nuphase_entry = (nuphase_header->event_number % 100000000)-1; 

  TString str; 
  for (int bd = 0; bd< 2; bd++) 
  {
    for (int ch = 0; ch < 8; ch++) 
    { 
      const uint8_t * the_raw_data = nuphase_event->getRawData(ch,(nuphase::board) bd); 
      if (the_raw_data) 
      {
        const double * the_data = nuphase_event->getData(ch,(nuphase::board) bd); 

        types.push_back( bd == 0 ? NUPHASE_VPOL : 
                        nuphase_event->isSurface() || ch > 1 ? SURFACE :
                        NUPHASE_HPOL
                        ); 

        data.emplace_back( the_data, the_data + nuphase_event->getBufferLength()); 
        t0s.push_back(nuphase_event->getT0(ch,(nuphase::board) bd)); 
        sample_rate.push_back(1.5); //TODO: resample
        antenna_positions.push_back(TVector3(0,0,0)); //TODO: fix

        str.Form("NUPHASE, BD %s, CH %d", bd==0?"MASTER":"SLAVE", ch); 

        channel_names.push_back(str.Data()); //TODO, come up with a better name
      }
    }
  }


  //now do the ARA channels 
  ara_event_id = ara_event->eventId; 



  for (int chan = 0; chan < ((UsefulAtriStationEvent*) ara_event)->getNumRFChannels(); chan++)
  {
    TGraph * g = ((UsefulAtriStationEvent*) ara_event)->getGraphFromRFChan(chan); 

    //this needs to be interpoolated. TODO: allow choice of interpolator. 
    TGraph * gint = FFTtools::getInterpolatedGraph(g, 1./opts.sample_rate); 

    types.push_back(ARA5_VPOL); //TODO 
    data.emplace_back( gint->GetY(), gint->GetY() + gint->GetN()); 
    antenna_positions.push_back(TVector3(0,0,0)); //TODO
    sample_rate.push_back(3); 
    t0s.push_back(gint->GetX()[0]); 
    str.Form("A5 CH%d", chan); 
    channel_names.push_back(str.Data()); //TODO, come up with a better name


    delete g; 
    delete gint; 

  }

}


TGraph * nuphase::CombinedEvent::makeGraph(int chan, TGraph * useme) const
{
  TGraph * g = useme; 
  int nsamp = getNSamples(chan); 
  if (g) 
  {
    g->Set(nsamp); 
  }
  else
  {
    g = new TGraph(nsamp); 
  }
   memcpy(g->GetY(), getData(chan), nsamp*sizeof(double)); 
 
  double t0=getT0(chan); 
  double dt = getDeltaT(chan); 
  double * x = g->GetX() ; 

  for (int i = 0; i < nsamp; i++) 
  {
    x[i] = t0 + i*dt; 
  }

  g->SetTitle(getChannelName(chan)); 
  g->GetXaxis()->SetTitle("time (ns)"); 
  return g; 
}

void nuphase::CombinedEvent::drawAll(TVirtualPad * pad, int max_width) 
{
  if (!pad) pad = gPad ?: new TCanvas; 


  int nchan = getNChannels(); 
  if (max_width <= 0) max_width =ceil(sqrt(nchan)); 

  if (nchan < max_width) 
  {
    pad->Divide(nchan,1); 
  }
  else
  {
    pad->Divide(max_width, ceil(nchan/max_width)); 
  }


  for (int i = 0; i < nchan; i++) 
  {
    TGraph * g = makeGraph(i); 
    g->SetBit(kCanDelete); //make it owned by the pad 
    pad->cd(i+1); 
    g->Draw("al"); 
  }

}



#endif






