
/**
 * @file main.c
 * @author Kévin QUENEHERVE
 * @brief Fonctions à attaquer avec le ChipWhisperer en Horloge. 
 * @version 0.1
 * @date 2022-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <libbase/uart.h>
#include <libbase/console.h>
#include <generated/csr.h>

#include "simpleserial.h"

#include "pmp.h"

#include "ControlRegisterStatus.h"

#define NAPOT_SIZE 128
#define PROTECTED_ARRAY_LENGTH 32

/* declaration region memory analysis in write and read */
volatile uint32_t protected_global[PROTECTED_ARRAY_LENGTH] __attribute__((aligned(NAPOT_SIZE)));


/* Configure PMP 0 to only allow reads to protected_global. The
	 * PMP region is locked so that the configuration applies to M-mode
	 * accesses. */
struct pmp_config configNAPOT = {
	.L = PMP_LOCKED, /* LOCKED*/
	.A = PMP_NAPOT , /* Naturally-aligned power of two */
	.X = 0,
	.W = 0,
	.R = 1,
};

/* Configure PMP 1 to only allow reads to protected_global. The
	 * PMP region is locked so that the configuration applies to M-mode
	 * accesses. */
struct pmp_config configTOR = {
	.L = PMP_LOCKED, /* LOCKED*/
	.A = PMP_TOR , /* Top-of-Range */
	.X = 0,
	.W = 0,
	.R = 1,
};

/*** EN-TÊTES ***/

struct registers store_regs(void);

uint8_t PMP_Lib(uint8_t* in, uint8_t len);

/**
 * @brief main
 * 
 * @return void
 */
int main(void)
{

    #ifdef CONFIG_CPU_HAS_INTERRUPT
		irq_setmask(0);
		irq_setie(1);
	#endif

	#ifdef CSR_UART_BASE
		uart_init();
	#endif
	
	putchar('r');
    putchar('R');
    putchar('E');
    putchar('S');
    putchar('E');
    putchar('T');
    putchar(' ');
    putchar(' ');
    putchar(' ');
    putchar('\n');

    //leds_out_write(1);

    simpleserial_init();
    simpleserial_addcmd('s', 0, PMP_Lib);

    while(1){
        simpleserial_get();
		//leds_out_write(1);
	}
}


/**
 * @brief Configuration PMP in read-only, checking there is not possible to write
 * 
 * @param in empty
 * @param len 0
 * @return uint8_t protected_global[0]
 */
uint8_t PMP_Lib(uint8_t* in, uint8_t len)
{

	// /* Perform a write to the memory we're about to protect access to */
	protected_global[0] = 0xa;

    /* choose region pmp between 0 - 15 */
	unsigned int region = 0;


	//for(int i=0;i<10000000;i++){
	//	leds_out_write(2);
	//	}
    TRIGGER_out_write(0x1);
	//for(int i=0;i<10000000;i++){
		//leds_out_write(8);
	//}
    __asm__ volatile ("nop"); // 1
	__asm__ volatile ("nop"); // 2
	__asm__ volatile ("nop"); // 3
    __asm__ volatile ("nop"); // 4
	__asm__ volatile ("nop"); // 5
	__asm__ volatile ("nop"); // 6
    __asm__ volatile ("nop"); // 7
	__asm__ volatile ("nop"); // 8
	__asm__ volatile ("nop"); // 9
    __asm__ volatile ("nop"); // 10

	// configure pmp0 
    pmp_set_region(region, configNAPOT, addressingNAPOT((size_t) &protected_global, NAPOT_SIZE));

	__asm__ volatile ("nop");
	__asm__ volatile ("nop");
	__asm__ volatile ("nop");

	/* Attempt to write to protected_global. This should generate a store
	 * access fault exception. */
	protected_global[0] = 0xc;
	
	__asm__ volatile ("nop");
	__asm__ volatile ("nop");
	__asm__ volatile ("nop");

    TRIGGER_out_write(0x0);
    //uint32_t trigger_value;
    //trigger_value = TRIGGER_out_read();
    //printf("trigger_value (normalement 1) : %lx\n",trigger_value);
    
    //TRIGGER_out_write(0x0);
    //trigger_value = TRIGGER_out_read();
    //printf("trigger_value (normalement 0) : %lx\n",trigger_value);
    
    
    // retourne la valeur de protected_global[0], non modifié '0', modifié '1'
    simpleserial_put('r', 1, (uint8_t*)&protected_global[0]);

    printf("%lx,%lx,%lx,%lx,%lx,%lx,%lx,%lx,%lx,%lx\n",
        read_csr(pmpcfg0),read_csr(pmpcfg2),
        read_csr(pmpaddr0), read_csr(pmpaddr1), read_csr(pmpaddr2), read_csr(pmpaddr3), 
        read_csr(pmpaddr4), read_csr(pmpaddr5), read_csr(pmpaddr6), read_csr(pmpaddr7));

    return protected_global[0];
}
