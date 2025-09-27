/*  bad6502 A Raspberry Pi-based backend to a 65C02 CPU
    Copyright (C) 2025  D.Herrendoerfer

    MIT License
*/

/* MSBASIC 
 * using OSI msbasic from microsoft
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>

#include "../badX16/cpu/fake6502.h"
#include "msbasic/osi_msbasic.h"
#include "bios/osi_bios.h"

#define RAM_START 0xA000
#define COLD_START 0xBD11

// the ram
uint8_t mem[0x10000];

// terminal settings
struct termios previous_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &previous_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &previous_termios);
    memcpy(&new_termios, &previous_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        if (c == 0x18) {
          printf("\r\nCNTL+X pressed, exiting\r\n");
          exit(1);
        }
        return c;
    }
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
  if (address == 0xFF01) {
    if (!kbhit())
      return 0;
    return getch();
  }

  // mem read
  return mem[address];
}

void write6502(uint16_t address, uint8_t bank, uint8_t data)
{
  // mem write

  // Virtual hardware (writes char to stdout)
  if (address == 0xFF00) {
    printf("%c",(char)data);
    fsync(1);
  }

  if (address >= 0x8000) //simulated ROM above F000 
    return;

  mem[address] = data;
}

// Main prog
int main(int argc, char **argv)
{
  // setup terminal
  set_conio_terminal_mode();
  setbuf(stdout, NULL);

  for (uint16_t i=0; i != 0xFFFF; i++) {
    mem[i] = 0xFF;
  }

  //load msbasic @RAM_START
  printf("Loading OSI MSBASIC to :0x%04X\r\n",RAM_START);
  for (uint16_t i=0; i < msbasic_bin_len; i++) {
    mem[RAM_START+i] = msbasic_bin[i];
  }

  //load bios
  printf("Loading BIOS to 0x1E00\r\n");
  for (uint16_t i=0; i < bios_len; i++) {
    mem[0xFF10+i] = bios[i];
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

