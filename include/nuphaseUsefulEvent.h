#ifndef _NUPHASE_USEFULEVENT_H
#define _NUPHASE_USEFULEVENT_H

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include <string>
#include "nuphaseConsts.h"
#include "nuphaseHeader.h"
#include "nuphaseEvent.h"

class TGraph;

namespace nuphase
{

  class UsefulEvent : public TObject
  {

    public:
      UsefulEvent(const std::string eventFile, const std::string headerFile);
      ~UsefulEvent();
  
      Header* getHeader() { return fHeader; };
      Event* getEvent() { return fEvent; };

      TTree* getHeaderTree() { return fHeaderTree; };
      TTree* getEventTree() { return fEventTree; };

      void GetEntry(const int i);

      uint64_t getEventNumber() { return fHeader->getEventNumber(); };
      uint32_t getLivetimeConfiguration(const int run) { return fHeader->getLivetimeConfiguration(run*uint64_t(1e9)); }; 
      double getReadoutTimeFloat() const { return fHeader->getReadoutTimeFloat(); };     
 
      trig_type getRawTriggerType() { return fHeader->trigger_type; };
      // corrected trigger info
      bool isCalpulser() const { return fHeader->gate_flag=='\1'; };
      bool isSoftTrigger() const { return fHeader->isRFTrigger(); }; // RF & SW trigs flipped in data
      bool isRFTrigger() const { return fHeader->trigger_type == nuphase::TRIGGER_SW; }; // RF & SW trigs flipped in data
      bool isGoodEvent() { return fHeader->isGoodEvent(); };
      
      // This creates a calibrated TGraph of this event, correcting for board offset.
      TGraph * getGraph(int channel = 0, board b = BOARD_MASTER) const;

    private:
      Header* fHeader;
      Event* fEvent; 

      TFile* fHeaderFile;
      TFile* fEventFile; 

      TTree* fHeaderTree;
      TTree* fEventTree; 

    ClassDef(UsefulEvent,1);

  };

}

#endif
