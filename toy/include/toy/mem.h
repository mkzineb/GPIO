/*
 *	$Id: fast_mem.h,v 1.4 2009/01/21 07:30:54 casse Exp $
 *	fast_mem module interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2008, IRIT UPS.
 *
 *	GLISS is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	GLISS is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef TOY_VFAST_MEM_H
#define TOY_VFAST_MEM_H

#include <stdint.h>
#include <stddef.h>
#include "config.h"

#if defined(__cplusplus)
    extern  "C" {
#endif

#define TOY_MEM_STATE
#define TOY_MEM_INIT(s)
#define TOY_MEM_DESTROY(s)

#define TOY_VFAST_MEM
#ifdef TOY_NO_PAGE_INIT
#	define TOY_NOBITS_INIT
#endif

typedef uint32_t toy_address_t;
typedef uint32_t toy_size_t;
typedef struct toy_memory_t toy_memory_t;

/* creation function */
toy_memory_t *toy_mem_new(void);
void toy_mem_delete(toy_memory_t *memory);
toy_memory_t *toy_mem_copy(toy_memory_t *memory);

/* read functions */
uint8_t toy_mem_read8(toy_memory_t *, toy_address_t);
uint16_t toy_mem_read16(toy_memory_t *, toy_address_t);
uint32_t toy_mem_read32(toy_memory_t *, toy_address_t);
uint64_t toy_mem_read64(toy_memory_t *, toy_address_t);
float toy_mem_readf(toy_memory_t *, toy_address_t);
double toy_mem_readd(toy_memory_t *, toy_address_t);
long double toy_mem_readld(toy_memory_t *, toy_address_t);
void toy_mem_read(toy_memory_t *memory, toy_address_t, void *buf, size_t size);


/* write functions */
void toy_mem_write8(toy_memory_t *, toy_address_t, uint8_t);
void toy_mem_write16(toy_memory_t *, toy_address_t, uint16_t);
void toy_mem_write32(toy_memory_t *, toy_address_t, uint32_t);
void toy_mem_write64(toy_memory_t *, toy_address_t, uint64_t);
void toy_mem_writef(toy_memory_t *, toy_address_t, float);
void toy_mem_writed(toy_memory_t *, toy_address_t, double);
void toy_mem_writeld(toy_memory_t *, toy_address_t, long double);
void toy_mem_write(toy_memory_t *memory, toy_address_t, void *buf, size_t size);

#ifdef TOY_MEM_SPY
typedef enum { toy_access_read, toy_access_write } toy_access_t;
typedef void (*toy_mem_spy_t)(toy_memory_t *mem, toy_address_t addr, toy_size_t size, toy_access_t access, void *data);
void toy_mem_set_spy(toy_memory_t *mem, toy_mem_spy_t fun, void *data);
#endif

#if defined(__cplusplus)
}
#endif

#endif	/* TOY_VFAST_MEM_H */
