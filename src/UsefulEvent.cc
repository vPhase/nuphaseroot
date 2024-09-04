#include "nuphaseUsefulEvent.h"

ClassImp(nuphase::UsefulEvent);
#include "TGraph.h" 

nuphase::UsefulEvent::UsefulEvent(const std::string eventFile, const std::string headerFile)
{
  // load files
  fHeaderFile = new TFile(headerFile.c_str(), "OLD");
  if(!fHeaderFile || fHeaderFile->IsZombie())
    throw std::runtime_error("Cannot open header file: "+headerFile);

  fEventFile = new TFile(eventFile.c_str(), "OLD");
  if(!fEventFile || fEventFile->IsZombie())
    throw std::runtime_error("Cannot open event file: "+eventFile);

  // create header and event objects
  fHeaderTree = (TTree*) fHeaderFile->Get("header");
  fHeader = 0;
  fHeaderTree->SetBranchAddress("header", &fHeader);

  fEventTree = (TTree*) fEventFile->Get("event");
  fEvent = 0;
  fEventTree->SetBranchAddress("event", &fEvent);

  return;
}

nuphase::UsefulEvent::~UsefulEvent()
{

  delete fHeaderFile;
  delete fEventFile;

  fHeaderFile = nullptr;
  fEventFile = nullptr;
  fHeader = nullptr;
  fEvent = nullptr;
  fHeaderTree = nullptr;
  fEventTree = nullptr;

  return;
}

void nuphase::UsefulEvent::GetEntry(const int i)
{
  fHeaderTree->GetEntry(i);
  fEventTree->GetEntry(i);

  if(fEvent->getEventNumber() != fHeader->getEventNumber())
    throw std::runtime_error("Header file and event file are not aligned!");

  return;
}

TGraph * nuphase::UsefulEvent::getGraph(int channel, board b)
  const
{
  // get calibrated waveform graph
  TGraph * g = fEvent->getGraph(channel, b);

  // ensure right board is chosen for offset
  if(channel/k::num_chans_per_board == 1)
    b = BOARD_SLAVE;

  // correct board offset and convert to mV
  double offset = fHeader->getBoardTrigTimeOffsetNs(b);
  for (int i = 0; i < g->GetN(); ++i) {
    g->GetX()[i] += offset;
    g->GetY()[i] -= 64;
    g->GetY()[i] *= adcToVoltageConversion; 
  }

  return g; 
}
