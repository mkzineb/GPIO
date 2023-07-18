/*
 *	$Id: old_elf.h,v 1.10 2009/07/21 13:17:58 barre Exp $
 *	old_elf module interface
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
#ifndef TOY_OLD_ELF_H
#define TOY_OLD_ELF_H

#include "grt.h"
#include "mem.h"
#include "api.h"

#if defined(__cplusplus)
    extern  "C" {
#endif

#define TOY_LOADER_STATE
#define TOY_LOADER_INIT(s)
#define TOY_LOADER_DESTROY(s)

/* toy_loader_t type */
typedef struct toy_loader_t toy_loader_t;


/* loader management */
toy_loader_t *toy_loader_open(const char *path);
void toy_loader_close(toy_loader_t *loader);
void toy_loader_load(toy_loader_t *loader, toy_platform_t *pf);
toy_address_t toy_loader_start(toy_loader_t *loader);


/* system initialization (used internally during platform and state initialization) */
toy_address_t toy_brk_init(toy_loader_t *loader);

/* section access */
typedef struct toy_loader_sect_t {
	const char *name;
	toy_address_t addr;
	int size;
	enum {
		TOY_LOADER_SECT_UNKNOWN = 0,
		TOY_LOADER_SECT_TEXT,
		TOY_LOADER_SECT_DATA,
		TOY_LOADER_SECT_BSS
	} type;
} toy_loader_sect_t;
int toy_loader_count_sects(toy_loader_t *loader);
void toy_loader_sect(toy_loader_t *loader, int sect, toy_loader_sect_t *data);

/* symbol access */
typedef struct {
	const char *name;
	toy_address_t value;
	int size;
	int sect;
	enum {
		TOY_LOADER_SYM_NO_TYPE,
		TOY_LOADER_SYM_DATA,
		TOY_LOADER_SYM_CODE
	} type;
	enum {
		TOY_LOADER_NO_BINDING,
		TOY_LOADER_LOCAL,
		TOY_LOADER_GLOBAL,
		TOY_LOADER_WEAK
	} bind;
} toy_loader_sym_t;
int toy_loader_count_syms(toy_loader_t *loader);
void toy_loader_sym(toy_loader_t *loader, int sym, toy_loader_sym_t *data);



#if defined(__cplusplus)
}
#endif

#endif	/* TOY_OLD_ELF_H */
