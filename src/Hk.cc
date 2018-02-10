#include "nuphaseHk.h" 

ClassImp(nuphase::Hk); 

#ifdef HAVE_LIBNUPHASE

#include "nuphase.h" 

nuphase::Hk::Hk(const nuphase_hk * hk) 
{
    unixTime=hk->unixTime; 
    unixTimeMilliSecs=hk->unixTimeMillisecs; 
    temp_master=hk->temp_master; 
    temp_slave=hk->temp_slave; 
    temp_case=hk->temp_case; 
    temp_asps_uc=hk->temp_asps_uc; 
    current_master=hk->current_master; //mA
    current_slave=hk->current_slave; 
    current_frontend=hk->current_frontend; 
    current_sbc=hk->current_sbc; 
    current_switch=hk->current_switch; 
    on_state= (asps_power_state) hk->on_state; 
    fault_state= (asps_power_state) hk->fault_state; 
    gpio_state= (gpio_power_state) hk->gpio_state; 
    disk_space_kB=hk->disk_space_kB; 
    free_mem_kB=hk->free_mem_kB;  
    asps_heater_current=hk->asps_heater_current; 
}
#endif


nuphase::Hk::Hk() 
{
    unixTime=0; 
    unixTimeMilliSecs=0; 
    temp_master=0; 
    temp_slave=0; 
    temp_case=0; 
    temp_asps_uc=0; 
    current_master=0; //mA
    current_slave=0; 
    current_frontend=0; 
    current_sbc=0; 
    current_switch=0; 
    on_state=ASPS_POWER_NONE; 
    fault_state=ASPS_POWER_NONE; 
    gpio_state=GPIO_POWER_NONE; 
    disk_space_kB=0; 
    free_mem_kB=0;  
    asps_heater_current=0; 
}

