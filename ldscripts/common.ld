/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 *	Aneesh V <aneesh@ti.com>
 *
 * (C) Copyright 2020
 * Icenowy Zheng <icenowy@aosc.io>
 */

MEMORY { hvram : ORIGIN = 0x40010000,
		LENGTH = 0x70000 }

ENTRY(_start)
SECTIONS
{
	.text : {
		. = ALIGN(8);
		*(.__image_copy_start)
		start.o (.text*)
		*(.text*)
	} > hvram

	.rodata : {
		. = ALIGN(8);
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	} > hvram

	.data : {
		. = ALIGN(8);
		*(.data*)
	} > hvram

	.u_boot_list : {
		. = ALIGN(8);
		KEEP(*(SORT(.u_boot_list*)));
	} > hvram

	.image_copy_end : {
		. = ALIGN(8);
		*(.__image_copy_end)
	} > hvram

	.end : {
		. = ALIGN(8);
		*(.__end)
	} > hvram

	_image_binary_end = .;

	.bss_start (NOLOAD) : {
		. = ALIGN(8);
		KEEP(*(.__bss_start));
	} > hvram

	.bss (NOLOAD) : {
		*(.bss*)
		 . = ALIGN(8);
	} > hvram

	.bss_end (NOLOAD) : {
		KEEP(*(.__bss_end));
	} > hvram

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.comment*) }
	/DISCARD/ : { *(.eh_frame*) }
	/DISCARD/ : { *(.note*) }
}
