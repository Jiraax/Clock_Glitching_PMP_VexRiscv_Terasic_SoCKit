/**
 * @file trap_handler.c
 * @author Kévin QUENEHERVE
 * @brief File trap handler
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <generated/csr.h>
#include <generated/soc.h>
#include <irq.h>
#include <libbase/uart.h>
#include <stdio.h>

#include "ControlRegisterStatus.h"

void trap_handler (void);


void send_data(uint8_t *src, uint8_t size);


void send_data(uint8_t *src, uint8_t size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    uart_rxtx_write(src[i]);
	  busy_wait(1);
  }
}

/**
 * @brief Exception handling 
 * @details manages the following exception codes : 
 *  - 0x1 : Instruction access fault
 *  - 0x2 : Illegal instruction
 *  - 0x5 : Load access fault
 *  - 0x7 : Store access fault
 *  - 0x8 : Environment Call from U-mode
 *  - 0xb : Environment Call from M-mode
 * @param void
 * @return void
 * 
 */
void trap_handler (void) {

    uint8_t IAF[]   = "Instruction access fault\n\r";
    uint8_t Ii[]    = "Illegal instruction\n\r";
    uint8_t LAF[]   = "Load access fault\n\r";
    uint8_t SAF[]   = "Store access fault\n\r";
    uint8_t ECUm[]  = "Environment Call from U-Mode\n\r";
    uint8_t ECMm[]  = "Environment Call from M-Mode\n\r"; 

    // uint64_t mcause_reg = read_csr(mcause);
    // uint64_t mepc_reg = read_csr(mepc);
    unsigned int mcause_reg = read_csr(mcause);
    unsigned int mepc_reg = read_csr(mepc);
    unsigned int claim; 
    __attribute__((unused)) unsigned int irqs;	

   
        // switch(mcause_reg & 0x7f) {
        switch (mcause_reg) {
            case 0x1: // Instruction access fault
                
                // send_data(IAF, sizeof(IAF));
                csrw(mepc, mepc_reg + (((*(char *)mepc_reg) & 0b11) == 0b11 ? 4 : 2)); // skip faulty instruction
                break;

            case 0x2: // Illegal instruction	
                
                // send_data(Ii, sizeof(Ii));
                csrw(mepc, mepc_reg + (((*(char *)mepc_reg) & 0b11) == 0b11 ? 4 : 2)); // skip faulty instruction
                break;

            case 0x5: // Load access fault
                
                // send_data(LAF, sizeof(LAF));
                // printf("%016lx\n", mepc_reg);
                csrw(mepc, mepc_reg + (((*(char *)mepc_reg) & 0b11) == 0b11 ? 4 : 2)); // skip faulty instruction
                // write_csr(mepc, mepc_reg + (((*(char *)mepc_reg) & 0b11) == 0b11 ? 4 : 2)); // skip faulty instruction
                break;

            case 0x7: // Store access fault
                
                // send_data(SAF, sizeof(SAF));
                // printf("%016lx\n", mepc_reg);
                csrw(mepc, mepc_reg + (((*(char *)mepc_reg) & 0b11) == 0b11 ? 4 : 2)); // skip faulty instruction
                // write_csr(mepc, mepc_reg + (((*(char *)mepc_reg) & 0b11) == 0b11 ? 4 : 2)); // skip faulty instruction
                break;

            case 0x8: // Environment Call from U-mode 
                // send_data(ECUm, sizeof(ECUm));
                break;

            case 0xb: // Environment Call from M-mode
                // send_data(ECMm, sizeof(ECMm));
                break;

            default:

               /* #if defined(__cva6__)
                    leds_out_write(0xf);
                    while ((claim = *((unsigned int *)PLIC_M_CLAIM))) {

                       switch (claim - 1) {
                            case UART_INTERRUPT:
                                leds_out_write(0x9);
                                uart_isr();
                                break;
                            default:
                                printf("## PLIC: Unhandled claim: %d\n", claim);
                                printf("# plic_enabled:    %08x\n", irq_getmask());
                                printf("# plic_pending:    %08x\n", irq_pending());
                                printf("# mepc:    %016lx\n", csrr(mepc));
                                printf("# mcause:  %016lx\n", csrr(mcause));
                                printf("# mtval:   %016lx\n", csrr(mtval));
                                printf("# mie:     %016lx\n", csrr(mie));
                                printf("# mip:     %016lx\n", csrr(mip));
                                printf("###########################\n\n");
                                break;
                        }
                        *((unsigned int *)PLIC_M_CLAIM) = claim;
                    }    
                #else*/
                    irqs = irq_pending() & irq_getmask();

                    #ifdef CSR_UART_BASE
                    #ifndef UART_POLLING
                        if(irqs & (1 << UART_INTERRUPT))
                            uart_isr();
                    #endif
                    #endif
                    leds_out_write(0xA);
                //#endif
        }
     
}

