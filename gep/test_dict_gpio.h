#ifndef  gpio_COMPONENT_H
#define  gpio_COMPONENT_H

#include <stdio.h>
#include <stdlib.h>
#include "csim.h"
#include "mem.h"

#define IN 0b00
#define OUT 0b01
#define NB_PINS 16

extern csim_component_t  gpio_component;

typedef struct  gpio_inst_t {
    csim_inst_t inst;
 
        csim_word_t MODER;
 
        csim_word_t IDR;
 
        csim_word_t BSRR;
 
        csim_word_t ODR;
 
        csim_word_t LCKR;

    csim_port_inst_t *gpio_port;
}  gpio_inst_t;

 
    void  gpio_write_MODER(csim_inst_t *inst, int indX, csim_word_t value);
 
    void  gpio_write_IDR(csim_inst_t *inst, int indX, csim_word_t value);
 
    void  gpio_write_BSRR(csim_inst_t *inst, int indX, csim_word_t value);
 
    void  gpio_write_ODR(csim_inst_t *inst, int indX, csim_word_t value);
 
    void  gpio_write_LCKR(csim_inst_t *inst, int indX, csim_word_t value);

     csim_word_t  gpio_read_MODER(csim_inst_t *inst, int indX);
     csim_word_t  gpio_read_IDR(csim_inst_t *inst, int indX);
     csim_word_t  gpio_read_BSRR(csim_inst_t *inst, int indX);
     csim_word_t  gpio_read_ODR(csim_inst_t *inst, int indX);
     csim_word_t  gpio_read_LCKR(csim_inst_t *inst, int indX);



void  gpio_construct(csim_inst_t *inst);
void  gpio_reset(csim_inst_t *inst);
void  gpio_configure_pin_input( gpio_inst_t * gpio_inst, int pin);
void  gpio_configure_pin_output( gpio_inst_t * gpio_inst, int pin);
void update_pins(csim_port_inst_t *port, csim_value_type_t type, csim_value_t val);
void  gpio_destruct(csim_inst_t *inst);

#endif /*  gpio_COMPONENT_H */
