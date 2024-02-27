#include <stdio.h>
#include <stdlib.h>
#include "csim.h"
#include "mem.h"

#define IN 0b00
#define OUT 0b01
#define NB_PINS 16

/****** Component GPIO ******/
extern csim_component_t gpio_component;
csim_word_t MODR;
csim_word_t IDR;
csim_word_t ODR;
int  indxxx =0; 

void gpio_destruct(csim_inst_t *inst) {
}

void gpio_reset(csim_inst_t *inst) {
}
// le parametre pin_index n'a aucun importance pour notre cas
void gpio_write_MODR(csim_inst_t *inst, int pin_index, csim_word_t value) {
    MODR = value ; 
}

csim_word_t gpio_read_MODR(csim_inst_t *inst, int pin_index) {
    return MODR;
}

void gpio_write_IDR(csim_inst_t *inst, int pin_index, csim_word_t value) {
    IDR = value ; 
}

csim_word_t gpio_read_IDR(csim_inst_t *inst, int pin_index) {
    return IDR;
}

void gpio_write_ODR(csim_inst_t *inst, int pin_index, csim_word_t value) {
    ODR = value  ;     
}

csim_word_t gpio_read_ODR(csim_inst_t *inst, int pin_index) {
    return ODR ; 
}
// le parametre pin_index n'a aucun importance pour notre cas
void gpio_set_MODR(csim_inst_t *inst, int pin_index, csim_word_t value) {
    MODR = value ; 
}

csim_word_t gpio_get_MODR(csim_inst_t *inst, int pin_index) {
    return MODR;
}

void gpio_set_IDR(csim_inst_t *inst, int pin_index, csim_word_t value) {
    IDR = value ; 
}

csim_word_t gpio_get_IDR(csim_inst_t *inst, int pin_index) {
    return IDR;
}

void gpio_set_ODR(csim_inst_t *inst, int pin_index, csim_word_t value) {
    ODR = value  ;     
}

csim_word_t gpio_get_ODR(csim_inst_t *inst, int pin_index) {
    return ODR ; 
}




csim_reg_t gpio_registers[3] = {
    {"MODR", 0x00, 4, 1, 1, 0, CSIM_INT, NULL, NULL, gpio_read_MODR, gpio_write_MODR, gpio_get_MODR, gpio_set_MODR},
    {"IDR", 0x10, 4, 1, 1, 0, CSIM_INT, NULL, NULL, gpio_read_IDR, gpio_write_IDR, gpio_get_IDR, gpio_set_IDR},
    {"ODR", 0x14, 4, 1, 1, 0, CSIM_INT, NULL, NULL, gpio_read_ODR, gpio_write_ODR, gpio_get_ODR, gpio_set_ODR},
};
void gpio_construct(csim_inst_t *inst) {
    gpio_registers[0].set(inst,0,0x01   );
    gpio_registers[1].set(inst,1,0xABCDEF01);
    gpio_registers[2].set(inst,2,0x98765432);
    MODR = gpio_registers[0].get(inst,0) ; 
    IDR = gpio_registers[1].get(inst,0) ;
    ODR = gpio_registers[2].get(inst,0) ;
}
void update_pins(csim_port_inst_t *port, csim_value_type_t type, csim_value_t val) {
    csim_inst_t *inst = port->inst;
    int pin= indxxx ; 
//la boucle est juste pour un petit test mais je vais faire une fonction pour chaque pin dons 16 update_pins ?
    if(pin<NB_PINS) { 
        MODR = gpio_registers[0].get(inst,0) ; 
        IDR = gpio_registers[1].get(inst,0) ;
        ODR = gpio_registers[2].get(inst,0) ;
        csim_word_t pin_value ; 
        csim_word_t pin_mode = (MODR >> (2 * pin)) & 1; //à voir
       // printf("____Debug____PINMODE: -----------------------%d\n",pin_mode);
        if (pin_mode == OUT) {

            printf("\t\t\t---OUT MODE---\n") ; 
            //printf("____Debug____ : odr_value-----------------------%0x%08x \n" ,gpio_read_ODR(inst,0));
            pin_value = (val.digital >> pin) & 1;
            //printf("____Debug____ : pin_value-----------------------%d\n",pin_value);
            ODR = (ODR & ~(1<< pin)) | (pin_value << pin); //à voir
            //printf("____Debug____ : odr_value-----------------------%0x%08x \n",ODR);
            csim_send_digital(inst, &port->port[pin], pin_value);

        } 
        else {

            printf("\t\t\t---IN MODE---\n") ; 

            pin_value = IDR ; 
           // printf("____Debug____ : pin_value-----------------------%d\n",pin_value);
            pin_value = (IDR >> pin) & 1;//(x>>0 ) & 1 oubien x & (1 << offset ) 
            //printf("____Debug____ : pin_value-----------------------%d\n",pin_value);

            csim_send_digital(inst, &port->port[pin], pin_value);

        }
        
    }
        gpio_write_ODR(inst, 0, ODR); 

}

void update_pins_global (csim_port_inst_t *port, csim_value_type_t type, csim_value_t val , int index_pin){
    indxxx = index_pin ; 
    update_pins(port,type,val) ; 
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

csim_component_t gpio_component = {
    "GPIO",
    CSIM_SIMPLE,
    3,
    gpio_registers,
    0,
    gpio_ports,
    NB_PINS,
    sizeof(csim_inst_t),
    gpio_construct,
    gpio_destruct,
    gpio_reset,
    NULL
};
int main(){


csim_memory_t *mem = csim_mem_new();
    csim_board_t *board = csim_new_board("my_board", mem);
    board->level = CSIM_DEBUG;

    csim_inst_t *gpio_instance1 = csim_new_component(board, &gpio_component, "gpio1", 0x80000000);
   



    csim_word_t modr_value = gpio_read_MODR(gpio_instance1, 0);
    printf("Valeur lue dans le registre MODR : 0x%08x\n", modr_value);
     
        

    csim_word_t odr_value = gpio_read_ODR(gpio_instance1, 0);
    printf("Valeur lue dans le registre ODR : 0x%08x\n", odr_value);

    // Test de la fonction update_pins

   
    csim_value_t pins_value;    
    pins_value.digital =0x01      ;
    update_pins_global(gpio_instance1->ports, CSIM_ELECTRIC, pins_value,0);
    update_pins_global(gpio_instance1->ports, CSIM_ELECTRIC, pins_value,15);
    update_pins_global(gpio_instance1->ports, CSIM_ELECTRIC, pins_value,3);    
    csim_word_t odr_after = gpio_read_ODR(gpio_instance1, 0);
    printf("Valeur lue dans le registre ODR MAj : 0x%08x\n", odr_after);

    csim_delete_board(board);
    csim_mem_delete(mem);

    return 0;

}