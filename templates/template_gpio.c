#include <stdio.h>
#include <stdlib.h>
#include "csim.h"
#include "mem.h"
#include "template_gpio.h"

#define IN 0b00
#define OUT 0b01
#define NB_PINS 16
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/****** Component  $(comp) ******/
extern csim_component_t  $(comp)_component;
int indxxx = 0;

typedef struct  $(comp)_inst_t {
    csim_inst_t inst;
    $(foreach registers) 
        csim_word_t  $(name) ; 
    $(end)
    csim_port_inst_t *$(comp)_port;
}  $(comp)_inst_t;


// READ - WRITE
$(foreach registers) 
    void  $(comp)_write_$(name)(csim_inst_t *inst, int indX, csim_word_t value) {
        $(comp)_inst_t * $(comp)_inst = ( $(comp)_inst_t *)inst;
        $(comp)_inst->$(name) = value;
} 
$(end)
$(foreach registers)
    csim_word_t  $(comp)_read_$(name)(csim_inst_t *inst, int indX) {
        $(comp)_inst_t * $(comp)_inst = ( $(comp)_inst_t *)inst;
        return  $(comp)_inst->$(name);
    }
$(end)
  
csim_reg_t  $(comp)_registers[3] = {
    $(foreach registers)
        {"$(name)", $(offset), 32, 1, 1, 0, CSIM_INT, NULL, NULL,  $(comp)_read_$(name),  $(comp)_write_$(name), NULL, NULL},
    $(end)
};

void  $(comp)_construct(csim_inst_t *inst) {
    $(comp)_inst_t * $(comp)_inst = ( $(comp)_inst_t *)inst;
    $(foreach registers)
        $(comp)_inst->$(name) = 0 ;
    $(end)
     $(comp)_inst->$(comp)_port = inst->ports;
}

void  $(comp)_reset(csim_inst_t *inst) {
    $(comp)_inst_t * $(comp)_inst = ( $(comp)_inst_t *)inst;
    $(foreach registers) 
        $(comp)_inst->$(name) = 0;
    $(end)
}

// FONCTIONS POUR FORCER LE MODE OUTPUT/INPUT ---- MODIF MODR
void  $(comp)_configure_pin_input( $(comp)_inst_t * $(comp)_inst, int pin) {
    $(comp)_inst->MODR  = ( $(comp)_inst->MODR & ~(1 << (2*pin))) | (IN << (2*pin)) ; 
}// trash -------- juste un petit test de MODR 

void  $(comp)_configure_pin_output( $(comp)_inst_t * $(comp)_inst, int pin) {
    $(comp)_inst->MODR = ( $(comp)_inst->MODR & ~(1 << (2*pin))) | (OUT << (2*pin)) ; 
    
}// trash -------- juste un petit test de MODR 
void update_pins(csim_port_inst_t *port, csim_value_type_t type, csim_value_t val) {
    $(comp)_inst_t * $(comp)_inst = ( $(comp)_inst_t *)(port->inst);
    int pin = indxxx;
    if (pin < NB_PINS) {
        csim_word_t pin_value;
        csim_word_t pin_mode = ( $(comp)_inst->MODR >> (2 * pin)) & 1;
        //printf("____Debug____PINMODE: -----------------------%d\n",pin_mode);
        if (pin_mode == OUT) {
            printf("\t\t\t---OUT MODE---\n") ; 
            pin_value = (val.digital >> pin) & 1;
            //printf("____Debug____ : pin_value-----------------------%d\n",pin_value);
             $(comp)_inst->ODR = ( $(comp)_inst->ODR & ~(1 << pin)) | (pin_value << pin);
            //printf("____Debug____ : odr_value-----------------------%d \n", $(comp)_inst->ODR);
            csim_send_digital(port->inst, &(port->port[pin]), pin_value);
        } else if (pin_mode == IN) {
            printf("\t\t\t---IN MODE---\n") ; 
            pin_value = ( $(comp)_inst->IDR >> pin) & 1;
            //printf("____Debug____ : pin_value-----------------------%d\n",pin_value);
            csim_send_digital(port->inst, &(port->port[pin]), pin_value);
        }
    } else {
        printf("\t\t\tOutOfBounds\n");
    }
    $(comp)_write_ODR(&($(comp)_inst->inst), 0, $(comp)_inst->ODR);// ??????????????????????
}

csim_port_t  $(comp)_ports[NB_PINS] = {
    {"Pin1", CSIM_ELECTRIC, update_pins},
    {"Pin0", CSIM_ELECTRIC, update_pins},
    {"Pin2", CSIM_ELECTRIC, update_pins},
    {"Pin3", CSIM_ELECTRIC, update_pins},
    {"Pin4", CSIM_ELECTRIC, update_pins},
    {"Pin5", CSIM_ELECTRIC, update_pins},
    {"Pin6", CSIM_ELECTRIC, update_pins},
    {"Pin7", CSIM_ELECTRIC, update_pins},
    {"Pin8", CSIM_ELECTRIC, update_pins},
    {"Pin9", CSIM_ELECTRIC, update_pins},
    {"Pin10", CSIM_ELECTRIC, update_pins},
    {"Pin11", CSIM_ELECTRIC, update_pins},
    {"Pin12", CSIM_ELECTRIC, update_pins},
    {"Pin13", CSIM_ELECTRIC, update_pins},
    {"Pin14", CSIM_ELECTRIC, update_pins},
    {"Pin15", CSIM_ELECTRIC, update_pins}
};
void  $(comp)_destruct(csim_inst_t *inst) {

}

csim_component_t  $(comp)_component = {
    "$(comp)",
    CSIM_SIMPLE,
    0,
    $(comp)_registers,
    3,
    $(comp)_ports,
    NB_PINS,
    sizeof( $(comp)_inst_t),
    $(comp)_construct,
    $(comp)_destruct,
    $(comp)_reset,
    
};

void update_pins_global(csim_port_inst_t *port, csim_value_type_t type, csim_value_t val, int index_pin) {
    indxxx = index_pin;
    update_pins(port, type, val);
}
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/****** Component  $(compSec) ******/
extern csim_component_t  $(compSec)_component;

typedef struct  $(compSec)_inst_t {
    csim_inst_t inst;
    $(foreach registersSec) 
        csim_word_t  $(nameSec) ; 
    $(end)
}  $(compSec)_inst_t;


// READ - WRITE
$(foreach registersSec) 
    void  $(compSec)_write_$(nameSec)(csim_inst_t *inst, int indX, csim_word_t value) {
        $(compSec)_inst_t * $(compSec)_inst = ( $(compSec)_inst_t *)inst;
        $(compSec)_inst->$(nameSec) = value;
} 
$(end)
$(foreach registersSec)
    csim_word_t  $(compSec)_read_$(nameSec)(csim_inst_t *inst, int indX) {
        $(compSec)_inst_t * $(compSec)_inst = ( $(compSec)_inst_t *)inst;
        return  $(compSec)_inst->$(nameSec);
    }
$(end)
  
csim_reg_t  $(compSec)_registersSec[6] = {
    $(foreach registersSec)
        {"$(nameSec)", $(offsetSec), 32, 1, 1, 0, CSIM_INT, NULL, NULL,  $(compSec)_read_$(nameSec),  $(compSec)_write_$(nameSec), NULL, NULL},
    $(end)
};

void  $(compSec)_construct(csim_inst_t *inst) {
    $(compSec)_inst_t * $(compSec)_inst = ( $(compSec)_inst_t *)inst;
    $(foreach registersSec)
        $(compSec)_inst->$(nameSec) = 0 ;
    $(end)

}

void  $(compSec)_reset(csim_inst_t *inst) {
    $(compSec)_inst_t * $(compSec)_inst = ( $(compSec)_inst_t *)inst;
    $(foreach registersSec) 
        $(compSec)_inst->$(nameSec) = 0;
    $(end)
}


csim_component_t  $(compSec)_component = {
    "$(compSec)",
    CSIM_SIMPLE,
    0,
    $(compSec)_registers,
    6,
    NULL
    NULL,
    NULL?
    $(compSec)_construct,
    $(compSec)_destruct,
    $(compSec)_reset,
};

void  $(compSec)_destruct(csim_inst_t *inst) {

}












/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/

int main() {
    csim_memory_t *mem = csim_mem_new();
    csim_board_t *board = csim_new_board("my_board", mem);
    board->level = CSIM_DEBUG;

     $(comp)_inst_t * $(comp)_instance = ( $(comp)_inst_t *)csim_new_component(board, & $(comp)_component, "$(comp)1", 0x80000000);

    csim_word_t modr_value =  $(comp)_read_MODR(&( $(comp)_instance->inst), 0);
    printf("Valeur lue dans le registre MODR : 0x%08x\n", modr_value);

    csim_word_t odr_value =  $(comp)_read_ODR(&( $(comp)_instance->inst), 0);
    printf("Valeur lue dans le registre ODR : 0x%08x\n", odr_value);

    csim_value_t pins_value;
    pins_value.digital = 0b1010101010101010;
    for (int i=0 ; i<NB_PINS ; i++){
            update_pins_global($(comp)_instance->$(comp)_port, CSIM_ELECTRIC, pins_value, i);

    }
    
    csim_word_t odr_after =  $(comp)_read_ODR(&( $(comp)_instance->inst), 0);
    printf("Valeur lue dans le registre ODR MAj : 0x%08x\n", odr_after);

    return 0;
}
