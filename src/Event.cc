#include "nuphaseEvent.h" 

ClassImp(nuphase::Event);
#include "TGraph.h" 
#include "TAxis.h" 


#ifdef HAVE_LIBNUPHASE
#include "nuphase.h" 

nuphase::Event::Event(const nuphase_event * event) 
{
  event_number = event->event_number; 
  buffer_length = event->buffer_length; 
  memcpy(board_id,&event->board_id, sizeof(board_id)); 


  for (int b = 0; b < k::num_boards; b++)
  {
    if (!board_id[b]) continue; 

    for (int c = 0; c < k::num_chans_per_board; c++)
    {
      raw_data[b][c].resize(buffer_length); 
      for (int i = 0; i < buffer_length; i++)
      {
        raw_data[b][c][i] = event->data[b][c][i]; 
      }
    }
  }
}

#endif



nuphase::Event::Event() 
{
  event_number = 0;
  buffer_length = 0; 
  memset(board_id,0,sizeof(board_id)); 
}


void nuphase::Event::dumpCalibrated() 
{
  for (int b = 0; b < k::num_boards; b++)
  {
    for (int c = 0; c < k::num_chans_per_board; c++)
    {
      data[b][c].clear(); 
    }
  }
}

const double * nuphase::Event::getData(int c, board b) const 
{

  //This is the calibration part 
  if (!data[b][c].size() && raw_data[b][c].size())
  {
    data[b][c].resize(buffer_length); 
    for (int i = 0; i < buffer_length; i++) 
    {
      data[b][c][i] = raw_data[b][c][i] * calibration.getVoltageCalibration(b,c); 
    }
  }

  return data[b][c].size() ? &data[b][c][0] : 0; 
}


double * nuphase::Event::copyData(int c, board b, double * destination) const
{
  if (!destination) destination = new double[buffer_length]; 

  const double * the_data = getData(c,b); 
  if (!the_data)
  {
    memset(destination,0,buffer_length*sizeof(double)); 
  }
  else
  {
    memcpy(destination, the_data, buffer_length * sizeof(double)); 
  }

  return destination; 
}

TGraph * nuphase::Event::getGraph(int channel, board b, TGraph * g) const
{
  if (!g) g = new TGraph(buffer_length); 
  else g->Set(buffer_length); 

  copyData(channel,b, g->GetY()); 


  for (int i = 0; i < g->GetN(); i++) 
  {
    g->GetX()[i] = i * calibration.getTimeCalibration(); 
  }

  g->GetYaxis()->SetTitle(getUnitString(calibration.getVoltageUnits())); 
  g->GetXaxis()->SetTitle(getUnitString(calibration.getTimeUnits())); 

  TString title;
  title.Form("Board %s, Channel %d\n", nuphase::getBoardString(b),channel); 
  g->SetTitle(title.Data()); 
  return g; 

}








