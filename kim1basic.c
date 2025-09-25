/*  bad6502 A Raspberry Pi-based backend to a 65C02 CPU
    Copyright (C) 2025  D.Herrendoerfer

    MIT License
*/

/* MSBASIC 
 * using KIM-1 msbasic from microsoft
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "../badX16/cpu/fake6502.h"
#include "msbasic/kb9_msbasic.h"
#include "bios/kim_bios.h"

#define RAM_START 0x2000
#define COLD_START 0x4065

// the ram
uint8_t mem[0x10000];

char m_getc()
{
  int str_read = fgetc(stdin);
  if (!str_read)
      return 0xff;

//  printf("%i\n",str_read);

  // Keyboard input Fixups
  if (str_read==10)
    str_read=13;

  return (char)str_read;
}

uint8_t read6502(uint16_t address, uint8_t bank)
{
  // Manual reset vector
  if (address == 0xFFFC)
    return COLD_START & 0xff; 
  if (address == 0xFFFD)
    return COLD_START >> 8;

  //2348: NMI ?
  
  // Virtual hardware (reads a char from stdin)
  if (address == 0xFF01)
    return m_getc();

  // mem read
  return mem[address];
}

void write6502(uint16_t address, uint8_t bank, uint8_t data)
{
  // mem write

  // Virtual hardware (writes char to stdout)
  if (address == 0xFF00) {
    printf("%c",(char)data);
  }

  if ((address >= 0x1E00) & (address < 2000)) //simulated ROM above E000 
    return;

  if (address >= 0xF000) //simulated ROM above F000 
    return;

  mem[address] = data;
}

// Main prog
int main(int argc, char **argv)
{
  for (uint16_t i=0; i != 0xFFFF; i++) {
    mem[i] = 0xFF;
  }

  //load msbasic @RAM_START
  printf("Loading KIM-1 MSBASIC to :0x%04X\n",RAM_START);
  for (uint16_t i=0; i < msbasic_bin_len; i++) {
    mem[RAM_START+i] = msbasic_bin[i];
  }

  //load bios
  printf("Loading BIOS to 0x1E00\n");
  for (uint16_t i=0; i < bios_len; i++) {
    mem[0x1E00+i] = bios[i];
  }

  //reset the cpu
  reset6502(0);

  // step past the reset code
  for (int i=0 ; i<32 ; i++) {
    step6502();
    usleep(1000);
  }

loop:  
  step6502();
  
  #ifndef FAST
  usleep(100000);
  #endif

  goto loop;

  printf("\n\nExecution stopped. (but we never get here)");
  return 0;
}

