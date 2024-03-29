/* Generated by gep (123/06/18 13:34:17) copyright (c) 2008 IRIT - UPS */

#ifndef GLISS_TOY_INCLUDE_TOY_CODE_TABLE_H
#define GLISS_TOY_INCLUDE_TOY_CODE_TABLE_H


#if defined(__cplusplus)
extern  "C"
{
#endif

#include <toy/api.h>
#include <toy/macros.h>
#include <toy/grt.h>

/* module headers */


#include <toy/mem.h>
#include <toy/grt.h>
#include <toy/error.h>
#include <toy/gen_int.h>
#include <toy/loader.h>
#include <toy/code.h>


/* switch optimisation tables */


/* TODO: add some error messages when malloc fails */
#define gliss_error(e) fprintf(stderr, "%s\n", (e))

#define TOY__SIZE	16
static void toy_instr_UNKNOWN_code(toy_state_t *state, toy_inst_t *inst) {
	/* must not be executed ! */
	toy_execute_unknown(state, TOY_UNKNOWN___IADDR);
}

/* beq %s, %s, %d */
static void toy_instr_BEQ_S__S__D_code(toy_state_t *state, toy_inst_t *inst) {
	uint32_t __gtmp_0;
	if((((uint32_t)(TOY_BEQ_S__S__D_x_r2_i)) == 0))
		__gtmp_0 = 0;
	else
		__gtmp_0 = ((uint32_t)(TOY_R[TOY_BEQ_S__S__D_x_r2_i]));
	if((((uint32_t)(TOY_R[TOY_BEQ_S__S__D_x_r1_i])) == __gtmp_0))
		TOY_PC = ((((int16_t)(TOY_PC)) + ((int16_t)(TOY_BEQ_S__S__D_x_off))) << 1);
	TOY_PC = ((uint16_t)((((uint32_t)(TOY_PC)) + 2)));

}

/* add %s, %s, %s */
static void toy_instr_ADD_S__S__S_code(toy_state_t *state, toy_inst_t *inst) {
	TOY_R[TOY_ADD_S__S__S_x_d_i] = (TOY_R[TOY_ADD_S__S__S_x_s1_i] + TOY_R[TOY_ADD_S__S__S_x_s2_i]);
	TOY_PC = ((uint16_t)((((uint32_t)(TOY_PC)) + 2)));

}

/* set %s, #%d */
static void toy_instr_SET_S___ID_code(toy_state_t *state, toy_inst_t *inst) {
	TOY_R[TOY_SET_S___ID_x_r_i] = ((int16_t)(TOY_SET_S___ID_x_imm));
	TOY_PC = ((uint16_t)((((uint32_t)(TOY_PC)) + 2)));

}




typedef void (*toy_code_function_t)(toy_state_t *, toy_inst_t *);

toy_code_function_t toy_code_table[] =
{
	toy_instr_UNKNOWN_code,
	toy_instr_BEQ_S__S__D_code,
	toy_instr_ADD_S__S__S_code,
	toy_instr_SET_S___ID_code
};



#if defined(__cplusplus)
}
#endif

#endif /* GLISS_TOY_INCLUDE_TOY_DECODE_TABLE_H */
