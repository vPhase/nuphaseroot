#include "nuphaseHeader.h" 

ClassImp(nuphase::Header); 

#ifdef HAVE_LIBNUPHASE

#include "nuphase.h" 

nuphase::Header::Header(const nuphase_header *hd) 
{
      memcpy(&event_number,&hd->event_number,sizeof(event_number));
      memcpy(&trig_number,&hd->trig_number,sizeof(trig_number));
      memcpy(&buffer_length,&hd->buffer_length,sizeof(buffer_length));
      memcpy(&pretrigger_samples,&hd->pretrigger_samples,sizeof(pretrigger_samples));
      memcpy(&readout_time,&hd->readout_time,sizeof(readout_time));
      memcpy(&readout_time_ns,&hd->readout_time_ns,sizeof(readout_time_ns))  ;
      memcpy(&trig_time,&hd->trig_time,sizeof(trig_time))  ;
      memcpy(&raw_approx_trigger_time,&hd->approx_trigger_time,sizeof(raw_approx_trigger_time));
      memcpy(&raw_approx_trigger_time_nsecs,&hd->approx_trigger_time_nsecs,sizeof(raw_approx_trigger_time_nsecs));
      memcpy(&triggered_beams,&hd->triggered_beams,sizeof(triggered_beams));
      memcpy(&beam_mask,&hd->beam_mask,sizeof(beam_mask));
      memcpy(&beam_power,&hd->beam_power,sizeof(beam_power));
      memcpy(&deadtime,&hd->deadtime,sizeof(deadtime));
      memcpy(&buffer_number,&hd->buffer_number,sizeof(buffer_number));
      memcpy(&channel_mask,&hd->channel_mask,sizeof(channel_mask));
      memcpy(&channel_read_mask,&hd->channel_read_mask,sizeof(channel_read_mask));
      memcpy(&gate_flag,&hd->gate_flag,sizeof(gate_flag));

      memcpy(&buffer_mask,&hd->buffer_mask,sizeof(buffer_mask));
      memcpy(&board_id,&hd->board_id,sizeof(board_id)); 
      memcpy(&trigger_type,&hd->trig_type,sizeof(trigger_type)); 
      memcpy(&calpulser,&hd->calpulser,sizeof(calpulser)); 
      memcpy(&sync_problem,&hd->sync_problem,sizeof(sync_problem)); 


      //this could be filled later 
      corrected_trigger_time = 0; 
      corrected_trigger_time_ns = 0; 

}
#endif


nuphase::Header::Header()
{
      memset(&trig_number,0,sizeof(trig_number));
      memset(&buffer_length,0,sizeof(buffer_length));
      memset(&pretrigger_samples,0,sizeof(pretrigger_samples));
      memset(&readout_time,0,sizeof(readout_time));
      memset(&readout_time_ns,0,sizeof(readout_time_ns))  ;
      memset(&trig_time,0,sizeof(trig_time))  ;
      memset(&raw_approx_trigger_time,0,sizeof(raw_approx_trigger_time));
      memset(&raw_approx_trigger_time_nsecs,0,sizeof(raw_approx_trigger_time_nsecs));
      memset(&triggered_beams,0,sizeof(triggered_beams));
      memset(&beam_mask,0,sizeof(beam_mask));
      memset(&beam_power,0,sizeof(beam_power));
      memset(&deadtime,0,sizeof(deadtime));
      memset(&buffer_number,0,sizeof(buffer_number));
      memset(&channel_mask,0,sizeof(channel_mask));
      memset(&channel_read_mask,0,sizeof(channel_read_mask));
      memset(&gate_flag,0,sizeof(gate_flag));
      memset(&buffer_mask,0,sizeof(buffer_mask));
      memset(&board_id,0,sizeof(board_id)); 
      memset(&trigger_type,0,sizeof(trigger_type)); 
      memset(&calpulser,0,sizeof(calpulser)); 
      memset(&sync_problem,0,sizeof(sync_problem)); 

      corrected_trigger_time = 0; 
      corrected_trigger_time_ns = 0; 
}

double nuphase::Header::getBoardTrigTimeOffsetNs(board which) 
  const 
{ 
  if(which == BOARD_MASTER) 
    return 0;

  int64_t offset = trig_time[BOARD_SLAVE] - trig_time[BOARD_MASTER];
  return offset/nominalMasterClockRate*1e9; 
}

uint32_t nuphase::Header::getLivetimeConfiguration(uint64_t event_no)
  const
{
    if(event_no < 1746*uint64_t(1e9))
      return 1;
    else if(event_no < 3836*uint64_t(1e9))
      return 2;
    else if(event_no < 4022*uint64_t(1e9))
      return 3;
    else if(event_no < 5929*uint64_t(1e9))
      return 4;
    else
      return 5;
}

// checks event quality based on basic consistency checks
bool nuphase::Header::isGoodEvent() 
  const
{

  // check if boards are out of sync
  uint64_t trigMaster = trig_time[BOARD_MASTER];
  uint64_t trigSlave = trig_time[BOARD_SLAVE];
  int64_t absDiff = (trigMaster > trigSlave)? trigMaster - trigSlave : trigSlave - trigMaster;
  if(absDiff > 5) 
    return false;

  return true;
}

