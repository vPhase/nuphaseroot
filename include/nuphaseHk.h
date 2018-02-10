#ifndef NUPHASE_HK_H
#define NUPHASE_HK_H


#include "nuphaseConsts.h" 
#include "TObject.h" 

#ifdef HAVE_LIBNUPHASE
struct nuphase_hk; 
#endif


namespace nuphase
{
  class Hk  : public TObject 
  {

    Hk(); 
#ifdef HAVE_LIBNUPHASE
    Hk (const nuphase_hk * hk); 
#endif
    uint32_t unixTime; 
    uint16_t unixTimeMilliSecs; 
    int8_t temp_master; 
    int8_t temp_slave; 
    int8_t temp_case; 
    int8_t temp_asps_uc; 
    uint16_t current_master; //mA
    uint16_t current_slave; 
    uint16_t current_frontend; 
    uint16_t current_sbc; 
    uint16_t current_switch; 
    asps_power_state on_state; 
    asps_power_state fault_state; 
    gpio_power_state gpio_state; 
    uint32_t disk_space_kB; 
    uint32_t free_mem_kB;  
    uint16_t asps_heater_current; 

    ClassDef(Hk,1); 
  }; 
}

#endif


