#include "csim.h"
#include <stdio.h>

extern csim_component_t gpio_component;


void e1_trigger(csim_evt_t *evt) {
	csim_board_t *b = evt->inst->board;
	b->log(b, CSIM_INFO, "e1 triggered!");
	static int cnt = 0;
	csim_send_digital(evt->inst, &gpio_component.ports[0], cnt++);
}

csim_evt_t e1 = {
	NULL,
	NULL,
	10,
	10,
	NULL,
	e1_trigger
};
void gpio_component_construct(csim_inst_t *i) {
    e1.inst = i;
	csim_record_event(i->board, &e1);
}

void gpio_component_destruct(csim_inst_t *i) {
}

void gpio_component_reset(csim_inst_t *i) {
}

/*GPIO_regs*/
csim_reg_t gpio_moder_reg = {"MODER",0x0,1,1,0,0,CSIM_BITS,NULL,NULL,NULL,NULL,NULL,NULL};
csim_reg_t gpio_odr_reg = {"ODR",0x14,1,1,0,0,CSIM_BITS,NULL,NULL,NULL,NULL,NULL,NULL};
csim_reg_t gpio_idr_reg = {"IDR",0x10,1,1,0,0,CSIM_BITS,NULL,NULL,NULL,NULL,NULL,NULL};
/*GPIO_port*/
csim_port_t gpio_port = {"GPIO",CSIM_ELECTRIC,NULL};

csim_component_t gpio_component = {
    "GPIO Component",CSIM_SIMPLE,1,
    {&gpio_moder_reg, &gpio_odr_reg, &gpio_idr_reg},
    3,&gpio_port,1,0,NULL,NULL,NULL,NULL};

void update_gpio_port(csim_port_inst_t *port, csim_value_type_t type, csim_value_t val) {
    printf("GPIO Port Updated: Type=%d, Value=%d\n", type, val.digital);
}

int main() {
    csim_memory_t *mem = csim_mem_new();
    csim_board_t *board = csim_new_board("GPIO Board", NULL);

    csim_inst_t *gpio_instance = csim_new_component(board, &gpio_component, "GPIO", 0x1000);
    csim_connect(gpio_instance, &gpio_port, gpio_instance, &gpio_port);
    csim_run(board, 50);
    csim_delete_component(gpio_instance);

/*
struct csim_component_t {
	const char *name;
	csim_ctype_t type;
	uint32_t version;
	csim_reg_t *regs;
	int reg_cnt;
	csim_port_t *ports;
	int port_cnt;
	csim_size_t size;
	void (*construct)(csim_inst_t *inst);
	void (*destruct)(csim_inst_t *inst);
	void (*reset)(csim_inst_t *inst);
	csim_core_t *core;
};
*/

/*
struct csim_reg_t {
	const char *name;
	uint32_t offset;
	uint32_t size;
	uint32_t count;
	uint32_t stride;
	uint32_t flags;
	csim_rtype_t type;
	void (*make_name)(int num, char buf, int size);
	void (*display)(csim_inst_t *inst, int num, char buf, int size);
	csim_word_t (*read)(csim_inst_t *inst, int num);
	void (*write)(csim_inst_t *inst, int num, csim_word_t val);
	csim_word_t (*get)(csim_inst_t *inst, int num);
	void (*set)(csim_inst_t *inst, int num, csim_word_t val);
};



*/