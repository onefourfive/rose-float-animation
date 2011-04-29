#ifndef __MAESTROSPI_H
#define __MAESTROSPI_H

/*
	*
	*	custom made functions for 
	*	SPI port functions on both
	*	PORT1 & PORT2 on the PIC18F46K22
	*	
	
	
	Word on the street was that the libraries
	for the PIC18F46K22 were built incorrectly
	and include only UART functions. So here's SPI
	stuff.
	
	Based on SPI library functions already written
	by Microchip

	Derek Sorenson	4/29/11
	
*/

#include <pconfig.h>