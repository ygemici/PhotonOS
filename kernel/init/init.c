/* Initialization interface
 *
 * Copyright (c) 2015 Feraru Mihail (mihailferaru2000@gmail.com).
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <pic.h>
#include <vga.h>
#include <handlers.h>
#include <io.h>
#include <system.h>
#include <keyboard.h>
#include <time.h>
#include <kheap.h>
#include <paging.h>
#include <stddef.h>
#include <task.h>
#include <multiboot.h>
#include <init.h>
#include <vga.h>

extern int detect_cpu(void);

#define FREQ 100

void init_stdio() {
	inbuffer = (uint8_t*) kmalloc(sizeof(uint8_t) * STDIO_SIZE);
	outbuffer = (char*) kmalloc(sizeof(char) * STDIO_SIZE);

	stdin = (uint8_t*) inbuffer;
	stdout = (char*) outbuffer;
	
	for (int i = 0; i <= STDIO_SIZE; i++) {
		inbuffer[i] = 0;
		outbuffer[i] = 0;
	}
} 

void init(multiboot *mboot_ptr, uint32_t init_stack) {
	init_esp = init_stack;

	init_vga();
	init_stdio();
	init_gdt();
	init_idt();
	init_paging(mboot_ptr);
	init_timer(FREQ);
	install_keyboard();

	// mask some interrupts
	IRQ_set_mask(2);
	IRQ_set_mask(3);
	IRQ_set_mask(4);
	IRQ_set_mask(5);
	IRQ_set_mask(6);
	IRQ_set_mask(7);
	IRQ_set_mask(8);
	IRQ_set_mask(9);
	IRQ_set_mask(10);
	IRQ_set_mask(11);
	IRQ_set_mask(12);
	IRQ_set_mask(13);
	IRQ_set_mask(14);
	IRQ_set_mask(15);

	printk("%s %s (%s) by %s. Copyright C 2015 %s. All rights reserved.\n", OS_Name, Version, Relase_Date, Author, Author);
	detect_cpu();
	printk("\n-------------------------------------------------------------------\n");

	printk("VGA driver was installed!\n");
	printk("Initialize tty.   ");
	wstr_color("[OK]\n", COLOR_GREEN);

	printk("Initialize paging.   ");
	wstr_color("[OK]\n", COLOR_GREEN);
	printk("Memory info:\n");
	printk("\tKernel starts at: %x\n", (size_t)&kernel_start);
	printk("\tKernel ends at: %x\n", (size_t)&kernel_end);
	printk("\tRAM: %d MB\n", mem_size_mb);

	printk("Initialize stdio (allow using of stdio header).   ");
	wstr_color("[OK]\n", COLOR_GREEN);
	
	printk("Initialize GDT.   ");
	wstr_color("[OK]\n", COLOR_GREEN);
	
	printk("Initialize IDT and interrupts.   ");
	wstr_color("[OK]\n", COLOR_GREEN);
	
	printk("Install timer and clock.   ");
	wstr_color("[OK]\n", COLOR_GREEN);
	
	printk("Install keyboard support.   ");
	wstr_color("[OK]\n", COLOR_GREEN);
	
	/* tasking is useless, because switcher crashes every time
	init_tasking();
	
	uint32_t cr3, eflags;
	asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=r"(cr3)::"%eax");
	asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=r"(eflags)::"%eax");
	
	task_t *clock_task = create_task(clock_task, &update_time, eflags, cr3);
	*/

	wstr_color("\nDONE!\n", COLOR_GREEN);

	sti();
	
	getch();
}

void welcome() {
	clear_vga();
	
	wstr_color("      |\\     /|(  ____ \\( \\      (  ____ \\(  ___  )(       )(  ____ \\\n", COLOR_RED);
	wstr_color("      | )   ( || (    \\/| (      | (    \\/| (   ) || () () || (    \\/\n", COLOR_RED);
	wstr_color("      | | _ | || (__    | |      | |      | |   | || || || || (\\\n", COLOR_RED);
	wstr_color("      | |( )| ||  __)   | |      | |      | |   | || |(_)| ||  __)\\\n", COLOR_RED);   
	wstr_color("      | || || || (      | |      | |      | |   | || |   | || (\\\n", COLOR_RED);      
	wstr_color("      | () () || (____/\\| (____/\\| (____/\\| (___) || )   ( || (____/\\\n", COLOR_RED);
	wstr_color("      (_______)(_______/(_______/(_______/(_______)|/     \\|(_______/\n\n", COLOR_RED);
	
	wstr_color("                           \\__   __/(  ___  )\n", COLOR_YELLOW);
	wstr_color("                              ) (   | (   ) |\n", COLOR_YELLOW);
	wstr_color("                              | |   | |   | |\n", COLOR_YELLOW);
	wstr_color("                              | |   | |   | |\n", COLOR_YELLOW);
	wstr_color("                              | |   | |   | |\n", COLOR_YELLOW);
	wstr_color("                              | |   | (___) |\n", COLOR_YELLOW);
	wstr_color("                              )_(   (_______)\n\n", COLOR_YELLOW);
	
	wstr_color(" _______           _______ _________ _______  _        _______  _______ \n", COLOR_BLUE);
	wstr_color("(  ____ )|\\     /|(  ___  )\\__   __/(  ___  )( (    /|(  ___  )(  ____ \\\\\n", COLOR_BLUE);
	wstr_color("| (    )|| )   ( || (   ) |   ) (   | (   ) ||  \\  ( || (   ) || (    \\/\n", COLOR_BLUE);
	wstr_color("| (____)|| (___) || |   | |   | |   | |   | ||   \\ | || |   | || (_____ \n", COLOR_BLUE);
	wstr_color("|  _____)|  ___  || |   | |   | |   | |   | || (\\ \\) || |   | |(_____  )\n", COLOR_BLUE);
	wstr_color("| (      | (   ) || |   | |   | |   | |   | || | \\   || |   | |      ) |\n", COLOR_BLUE);
	wstr_color("| )      | )   ( || (___) |   | |   | (___) || )  \\  || (___) |/\\____) |\n", COLOR_BLUE);
	wstr_color("|/       |/     \\|(_______)   )_(   (_______)|/    )_)(_______)\\_______)\n", COLOR_BLUE);
	
	printk("                                                        by Feraru Mihail");
	
	getch();
	clear_vga();
}

void login() {
	wstr_color(" _______           _______ _________ _______  _        _______  _______ \n", COLOR_BLUE);
	wstr_color("(  ____ )|\\     /|(  ___  )\\__   __/(  ___  )( (    /|(  ___  )(  ____ \\\\\n", COLOR_BLUE);
	wstr_color("| (    )|| )   ( || (   ) |   ) (   | (   ) ||  \\  ( || (   ) || (    \\/\n", COLOR_BLUE);
	wstr_color("| (____)|| (___) || |   | |   | |   | |   | ||   \\ | || |   | || (_____ \n", COLOR_BLUE);
	wstr_color("|  _____)|  ___  || |   | |   | |   | |   | || (\\ \\) || |   | |(_____  )\n", COLOR_BLUE);
	wstr_color("| (      | (   ) || |   | |   | |   | |   | || | \\   || |   | |      ) |\n", COLOR_BLUE);
	wstr_color("| )      | )   ( || (___) |   | |   | (___) || )  \\  || (___) |/\\____) |\n", COLOR_BLUE);
	wstr_color("|/       |/     \\|(_______)   )_(   (_______)|/    )_)(_______)\\_______)\n", COLOR_BLUE);
	
	printk("Log in please.\n");
	printk("Username: ");
	gets(user);
	printk("Machine: ");
	gets(machine);
	printk("Loged in!\n");
}
