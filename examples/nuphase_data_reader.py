##Example for how to read nuphase data with python #

import ROOT
import numpy 
import sys

## you may need to hardcode the location here" 
ROOT.gSystem.Load("../build/libnuphaseroot.so"); 


class nuphase_data_reader:


  def __init__(self,base_dir, run):

    self.run = run; 
    self.base_dir = base_dir

    self.event_file = ROOT.TFile("%s/run%d/event.root" % (base_dir, run))
    self.event_tree = self.event_file.Get("event") 
    self.evt = ROOT.nuphase.Event() 
    self.event_entry = -1; 
    self.event_tree.SetBranchAddress("event",self.evt) 

    self.head_file = ROOT.TFile("%s/run%d/header.root" % (base_dir, run))
    self.head_tree = self.head_file.Get("header") 
    self.head = ROOT.nuphase.Header(); 
    self.head_entry = -1
    self.head_tree.SetBranchAddress("header",self.head) 
    self.head_tree.BuildIndex("header.event_number") 

    self.status_file = ROOT.TFile("%s/run%d/status.root" % (base_dir, run))
    self.status_tree = self.status_file.Get("status") 
    self.stat= ROOT.nuphase.Status(); 
    self.status_tree.SetBranchAddress("status",self.stat) 
    self.status_tree.BuildIndex("status.readout_time","status.readout_time_ns"); 
    self.status_entry =-1; 

    self.current_entry = 0; 
    

  def setEntry(self,i): 
    if (i < 0 or i >= self.head_tree.GetEntries()):
      sys.stderr.write("Entry out of bounds!") 
    else: 
      self.current_entry = i; 

  def setEvent(self,i):
    setEntry(self.head_tree.GetEntryNumberWithIndex(i)) 


  def event(self,force_reload = False): 
    if (self.event_entry != self.current_entry or force_reload):
      self.event_tree.GetEntry(self.current_entry)
      self.event_entry = self.current_entry 
    return self.evt 


  def wf(self,ch = 0):  
    return numpy.frombuffer(self.event().getRawData(ch % 8, ch/8), numpy.dtype('int8'), self.event().getBufferLength()) 

  def header(self,force_reload = False): 
    if (self.header_entry != self.current_entry or force_reload): 
      self.head_tree.GetEntry(self.current_entry); 
      self.head_entry = self.current_entry 
    return self.head 

  def status(self,force_reload = False): 
    if (self.status_entry != self.current_entry or force_reload): 
      self.status_tree.GetEntryWithBestIndex(self.header().readoutTime, self.header().readoutTimeNs)
      self.status_entry = self.current_entry

    return self.stat

  

  







