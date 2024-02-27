#ifndef  $(comp)_COMPONENT_H
#define  $(comp)_COMPONENT_H

#include <stdio.h>
#include <stdlib.h>
#include "csim.h"
#include "mem.h"

#define IN 0b00
#define OUT 0b01
#define NB_PINS 16

extern csim_component_t  $(comp)_component;


$(foreach registers) 
    void  $(comp)_write_$(name)(csim_inst_t *inst, int indX, csim_word_t value);
$(end)
$(foreach registers)
     csim_word_t  $(comp)_read_$(name)(csim_inst_t *inst, int indX);
$(end)


void  $(comp)_construct(csim_inst_t *inst);
void  $(comp)_reset(csim_inst_t *inst);
void  $(comp)_configure_pin_input( $(comp)_inst_t * $(comp)_inst, int pin);
void  $(comp)_configure_pin_output( $(comp)_inst_t * $(comp)_inst, int pin);
void update_pins(csim_port_inst_t *port, csim_value_type_t type, csim_value_t val);
void  $(comp)_destruct(csim_inst_t *inst);

#endif /*  $(comp)_COMPONENT_H */
