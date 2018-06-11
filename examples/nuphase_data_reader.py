##Example for how to read nuphase data with python #

import ROOT
import numpy 
import sys

## you may need to hardcode the location here" 
ROOT.gSystem.Load("../build/libnuphaseroot.so"); 


class nuphase_data_reader:


  def __init__(base_dir, run):

    self.run = run; 
    self.base_dir = base_dir

    self.event_file = ROOT.TFile("%s/run%d/event.root" % (base_dir, run))
    self.event_tree = self.event_file.Get("eventTree") 
    self.event = ROOT.nuphase.Event() 
    self.event_entry = -1; 
    self.event_tree.SetBranchAddress("event",self.event) 

    self.head_file = ROOT.TFile("%s/run%d/head.root" % (base_dir, run))
    self.head_tree = self.event_file.Get("headTree") 
    self.head_tree.BuildIndex("eventNumber") 
    self.head = ROOT.nuphase.Head(); 
    self.head_entry = -1
    self.head_tree.SetBranchAddress("head",self.head) 

    self.status_file = ROOT.TFile("%s/run%d/status.root" % (base_dir, run))
    self.status_tree = self.event_file.Get("statusTree") 
    self.status = ROOT.nuphase.status(); 
    self.status_tree.SetBranchAddress("status",self.status) 
    self.status_tree.BuildIndex("readoutTime","readoutTimeNs"); 
    self.status_entry =-1; 

    self.current_entry = 0; 
    

  def setEntry(i): 
    if (i < 0 or i >= self.head_tree.GetEntries()):
      sys.stderr.write("Entry out of bounds!") 
    else: 
      self.current_entry = i; 

  def setEvent(i):
    setEntry(self.head_tree.GetEntryNumberWithIndex(i)) 


  def event(force_reload = False): 
    if (self.event_entry != self.current_entry or force_reload):
      self.event_tree.GetEntry(self.current_entry)
      self.event_entry = self.current_entry 
    return self.event 


  def wf(ch = 0):  
    return numpy.frombuffer(self.event().getData(ch % 8, ch/8), numpy.dtype(float64), self.event().getBufferLength()) 

  def header(force_reload = False): 
    if (self.header_entry != self.current_entry or force_reload): 
      self.head_tree.GetEntry(self.current_entry); 
      self.head_entry = self.current_entry 
    return self.head 

  def status(force_reload = False): 
    if (self.status_entry != self.current_entry or force_reload): 
      self.status_tree.GetEntryWithBestIndex(self.header().readoutTime, self.header().readoutTimeNs)
      self.status_entry = self.current_entry

    return self.status

  

  







