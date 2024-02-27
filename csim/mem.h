/*
 * GLISS Component simulator memory manager
 * Copyright (c) 2019, IRIT - UPS <casse@irit.fr>
 *
 * This file is part of GLISS2.
 *
 * GLISS2 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GLISS2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLISS2; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef GLISS_CSIM_MEM_H
#define GLISS_CSIM_MEM_H

#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
    extern  "C" {
#endif

#define CSIM_MEM_STATE
#define CSIM_MEM_INIT(s)
#define CSIM_MEM_DESTROY(s)

#define CSIM_MEM_IO

#define CSIM_PAGE_SIZE 4096

typedef uint32_t csim_addr_t;
typedef uint32_t csim_size_t;
typedef struct csim_memory_t csim_memory_t;
 
/* creation function */
csim_memory_t *csim_mem_new();
void csim_mem_delete(csim_memory_t *memory);
csim_memory_t *csim_mem_copy(csim_memory_t *memory);

/* read functions */
uint8_t csim_mem_read8(csim_memory_t *, csim_addr_t);
uint16_t csim_mem_read16(csim_memory_t *, csim_addr_t);
uint32_t csim_mem_read32(csim_memory_t *, csim_addr_t);
uint64_t csim_mem_read64(csim_memory_t *, csim_addr_t);
float csim_mem_readf(csim_memory_t *, csim_addr_t);
double csim_mem_readd(csim_memory_t *, csim_addr_t);
long double csim_mem_readld(csim_memory_t *, csim_addr_t);
void csim_mem_read(csim_memory_t *memory, csim_addr_t, void *buf, size_t size);
 
/* write functions */
void csim_mem_write8(csim_memory_t *, csim_addr_t, uint8_t);
void csim_mem_write16(csim_memory_t *, csim_addr_t, uint16_t);
void csim_mem_write32(csim_memory_t *, csim_addr_t, uint32_t);
void csim_mem_write64(csim_memory_t *, csim_addr_t, uint64_t);
void csim_mem_writef(csim_memory_t *, csim_addr_t, float);
void csim_mem_writed(csim_memory_t *, csim_addr_t, double);
void csim_mem_writeld(csim_memory_t *, csim_addr_t, long double);
void csim_mem_write(csim_memory_t *memory, csim_addr_t, void *buf, size_t size);

/* callback related functions */
/* in the prototype of a typical callback function, arguments are the address of the memory transfer,
 * the size of the transfer in bytes, the address (as a void* to be cast) of the data (typically a register)
 * to be read or written from or to memory and the access type (read or write memory) */
/* !!WARNING!! the memory is divided into pages and callback functions are defined for an entire page, not only for a few addresses */

/* type access values */
#define CSIM_MEM_READ	0
#define CSIM_MEM_WRITE	1

/* callback function prototype */
typedef void (*csim_callback_fun_t)(csim_addr_t addr, int size, void *data, int type_access, void *cdata);
void csim_set_range_callback(csim_memory_t *mem, csim_addr_t start, csim_addr_t end, csim_callback_fun_t f, void *data);
void csim_unset_range_callback(csim_memory_t *mem, csim_addr_t start, csim_addr_t end);
void *csim_get_callback_data(csim_memory_t *mem, csim_addr_t a);

/* spy function prototypes */
#ifdef CSIM_MEM_SPY
typedef enum {
	csim_access_read = CSIM_MEM_READ,
	csim_access_write = CSIM_MEM_WRITE
} csim_access_t;
typedef void (*csim_mem_spy_t)(csim_memory_t *mem, csim_address_t addr, csim_size_t size, gliss_access_t access, void *data);
void csim_mem_set_spy(csim_memory_t *mem, csim_mem_spy_t fun, void *data);
#endif

#if defined(__cplusplus)
}
#endif

#endif /* GLISS_CSIM_MEM_H */

