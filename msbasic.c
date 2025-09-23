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
#include "bios/bios.h"


#define RAM_START 0x2000
#define COLD_START 0x4065

// the stack
uint8_t mem[0x10000];

char m_getc()
{
  int str_read = fgetc(stdin);
  if (!str_read)
      return 0xff;

//  printf("%i\n",str_read);
  // Fixups
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
  
  if (address == 0xFF01)
    return m_getc();

  // mem read
  return mem[address];
}

void write6502(uint16_t address, uint8_t bank, uint8_t data)
{
  // mem write

  if (address == 0xFF00) {
    printf("%c",(char)data);
  }

  if ((address >= 0x1E00) & (address < 2000)) //simulated ROM above E000 
    return;

  if (address >= 0xF000) //simulated ROM above F000 
    return;

  mem[address] = data;
}

uint16_t pc;
uint16_t sleep_val = 0;

// Main prog
int main(int argc, char **argv)
{
  //load msbasic @BASIC_START

  for (uint16_t i=0; i < msbasic_bin_len; i++) {
    mem[RAM_START+i] = msbasic_bin[i];
  }

  for (uint16_t i=0; i < bios_len; i++) {
    mem[0x1E00+i] = bios[i];
  }


  //reset the cpu
  reset6502(0);

  for (int i=0 ; i<32 ; i++) {
    step6502();
    usleep(1000);
  }

  pc=bus_addr;

loop:  
  step6502();
  
  #ifndef FAST
  usleep(100000);
  #endif

  goto loop;

  printf("\n\nExecution stopped. (but we never get here)");
  return 0;
}

