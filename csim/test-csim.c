#include <stdio.h>
#include <stdlib.h>
#include "csim.h"
#include "mem.h"

#define IN 0b00
#define OUT 0b01
#define NB_PINS 16

/****** Component GPIO ******/
extern csim_component_t gpio_component;
int indxxx = 0;

typedef struct gpio_inst_t {
    csim_inst_t inst;
    csim_word_t MODR;
    csim_word_t IDR;
    csim_word_t ODR;
    csim_port_inst_t *gp_ports;
} gpio_inst_t;


// READ - WRITE
void gpio_write_MODR(csim_inst_t *inst, int indX, csim_word_t value) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)inst;
    gpio_inst->MODR = value;
}
void gpio_write_IDR(csim_inst_t *inst, int indX, csim_word_t value) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)inst;
    gpio_inst->IDR = value;
}
void gpio_write_ODR(csim_inst_t *inst, int indX, csim_word_t value) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)inst;
    gpio_inst->ODR = value;
}
csim_word_t gpio_read_MODR(csim_inst_t *inst, int indX) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)inst;
    return gpio_inst->MODR;
}
csim_word_t gpio_read_IDR(csim_inst_t *inst, int indX) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)inst;
    return gpio_inst->IDR;
}
csim_word_t gpio_read_ODR(csim_inst_t *inst, int indX) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)inst;
    return gpio_inst->ODR;
}

csim_reg_t gpio_registers[3] = {
    {"MODR", 0x00, 32, 1, 1, 0, CSIM_INT, NULL, NULL, gpio_read_MODR, gpio_write_MODR, NULL, NULL},
    {"IDR", 0x10, 32, 1, 1, 0, CSIM_INT, NULL, NULL, gpio_read_IDR, gpio_write_IDR, NULL, NULL},
    {"ODR", 0x14, 32 , 1, 1, 0, CSIM_INT, NULL, NULL, gpio_read_ODR, gpio_write_ODR, NULL, NULL},
};

void gpio_construct(csim_inst_t *inst) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)inst;
    gpio_inst->MODR = 0x5555; //0101010101010101   0-IN 1-OUT
    gpio_inst->IDR = 0xAAAA; //1010101010101010    0-LOW 1-HIGH
    gpio_inst->ODR = 0x0000;
    gpio_inst->gp_ports = inst->ports;
}

void gpio_reset(csim_inst_t *inst) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)inst;
    gpio_inst->MODR = 0;
    gpio_inst->IDR = 0;
    gpio_inst->ODR = 0;
}

// FONCTIONS POUR FORCER LE MODE OUTPUT/INPUT ---- MODIF MODR
void gpio_configure_pin_input(gpio_inst_t *gpio_inst, int pin) {
    gpio_inst->MODR  = (gpio_inst->MODR & ~(1 << (2*pin))) | (IN << (2*pin)) ; 
}

void gpio_configure_pin_output(gpio_inst_t *gpio_inst, int pin) {
    gpio_inst->MODR = (gpio_inst->MODR & ~(1 << (2*pin))) | (OUT << (2*pin)) ; 
    
}
void update_pins(csim_port_inst_t *port, csim_value_type_t type, csim_value_t val) {
    gpio_inst_t *gpio_inst = (gpio_inst_t *)(port->inst);
    int pin = indxxx;
    if (pin < NB_PINS) {
        csim_word_t pin_value;
        csim_word_t pin_mode = (gpio_inst->MODR >> (2 * pin)) & 1;
        //printf("____Debug____PINMODE: -----------------------%d\n",pin_mode);
        if (pin_mode == OUT) {
            printf("\t\t\t---OUT MODE---\n") ; 
            pin_value = (val.digital >> pin) & 1;
            //printf("____Debug____ : pin_value-----------------------%d\n",pin_value);
            gpio_inst->ODR = (gpio_inst->ODR & ~(1 << pin)) | (pin_value << pin);
            //printf("____Debug____ : odr_value-----------------------%d \n",gpio_inst->ODR);
            csim_send_digital(port->inst, &(port->port[pin]), pin_value);
        } else if (pin_mode == IN) {
            printf("\t\t\t---IN MODE---\n") ; 
            pin_value = (gpio_inst->IDR >> pin) & 1;
            //printf("____Debug____ : pin_value-----------------------%d\n",pin_value);
            csim_send_digital(port->inst, &(port->port[pin]), pin_value);
        }
    } else {
        printf("\t\t\tOutOfBounds\n");
    }
    gpio_write_ODR(&(gpio_inst->inst), 0, gpio_inst->ODR);
}

csim_port_t gpio_ports[NB_PINS] = {
    {"Port0", CSIM_ELECTRIC, update_pins},
    {"Port1", CSIM_ELECTRIC, update_pins},
    {"Port2", CSIM_ELECTRIC, update_pins},
    {"Port3", CSIM_ELECTRIC, update_pins},
    {"Port4", CSIM_ELECTRIC, update_pins},
    {"Port5", CSIM_ELECTRIC, update_pins},
    {"Port6", CSIM_ELECTRIC, update_pins},
    {"Port7", CSIM_ELECTRIC, update_pins},
    {"Port8", CSIM_ELECTRIC, update_pins},
    {"Port9", CSIM_ELECTRIC, update_pins},
    {"Port10", CSIM_ELECTRIC, update_pins},
    {"Port11", CSIM_ELECTRIC, update_pins},
    {"Port12", CSIM_ELECTRIC, update_pins},
    {"Port13", CSIM_ELECTRIC, update_pins},
    {"Port14", CSIM_ELECTRIC, update_pins},
    {"Port15", CSIM_ELECTRIC, update_pins}
};
void gpio_destruct(csim_inst_t *inst) {

}

csim_component_t gpio_component = {
    "GPIO",
    CSIM_SIMPLE,
    3,
    gpio_registers,
    0,
    gpio_ports,
    NB_PINS,
    sizeof(gpio_inst_t),
    gpio_construct,
    gpio_destruct,
    gpio_reset,
    NULL
};

void update_pins_global(csim_port_inst_t *port, csim_value_type_t type, csim_value_t val, int index_pin) {
    indxxx = index_pin;
    update_pins(port, type, val);
}

int main() {
    csim_memory_t *mem = csim_mem_new();
    csim_board_t *board = csim_new_board("my_board", mem);
    board->level = CSIM_DEBUG;

    gpio_inst_t *gpio_instance = (gpio_inst_t *)csim_new_component(board, &gpio_component, "gpio1", 0x80000000);

    csim_word_t modr_value = gpio_read_MODR(&(gpio_instance->inst), 0);
    printf("Valeur lue dans le registre MODR : 0x%08x\n", modr_value);

    csim_word_t odr_value = gpio_read_ODR(&(gpio_instance->inst), 0);
    printf("Valeur lue dans le registre ODR : 0x%08x\n", odr_value);

    csim_value_t pins_value;
    pins_value.digital = 0b1010101010101010;
    for (int i=0 ; i<NB_PINS ; i++){
            update_pins_global(gpio_instance->gp_ports, CSIM_ELECTRIC, pins_value, i);

    }
    
    csim_word_t odr_after = gpio_read_ODR(&(gpio_instance->inst), 0);
    printf("Valeur lue dans le registre ODR MAj : 0x%08x\n", odr_after);

    return 0;
}
