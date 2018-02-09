#include "nuphaseEvent.h" 

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
      raw_data[b][c].insert(raw_data[b][c].begin(), event->data[b][c], event->data[b][c] + buffer_length); 
    }
  }
}





#endif













