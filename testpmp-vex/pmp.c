
#include "pmp.h"


/**
 * @brief Addressing a memory area with NAPOT mode
 * 
 * @param address 
 * @param size 
 * @return size_t 
 */
size_t addressingNAPOT(size_t address, size_t size){

        size_t return_addr = (address) >> 2; /* PMP addresses are 4-byte aligned, drop the bottom two bits */
        return_addr &= ~(size >> 3); /* Clear the bit corresponding with alignment */
        return_addr |= ((size >> 3) - 1); /* Set the bits up to the alignment bit */
        return return_addr;
}

/**
 * @brief Addressing a memory area with TOR mode
 * 
 * @param address 
 * @param size 
 * @return size_t 
 */
size_t addressingTOR(size_t address){

        size_t return_addr = (address) >> 2; /* PMP addresses are 4-byte aligned, drop the bottom two bits */
        return return_addr;
}

/*!
 * @brief Configure a address PMP for addressing mode TOR
 * @param region The PMP region to configure
 * @param address The desired address of the PMP region
 * @return void
 */
void pmp_set_addressTOR( unsigned int region,
                         size_t address)
{
        region = region - 1;

        if ((region > 15) || (region < 0)) {
                printf("ERROR : 16 entries PMPs between 0 to 15\n");
                exit(0);
        }

        switch(region) {
                case 0:
                __asm__("csrw pmpaddr0, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 1:
                __asm__("csrw pmpaddr1, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 2:
                __asm__("csrw pmpaddr2, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 3:
                __asm__("csrw pmpaddr3, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 4:
                __asm__("csrw pmpaddr4, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 5:
                __asm__("csrw pmpaddr5, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 6:
                __asm__("csrw pmpaddr6, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 7:
                __asm__("csrw pmpaddr7, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 8:
                __asm__("csrw pmpaddr8, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 9:
                __asm__("csrw pmpaddr9, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 10:
                __asm__("csrw pmpaddr10, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 11:
                __asm__("csrw pmpaddr11, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 12:
                __asm__("csrw pmpaddr12, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 13:
                __asm__("csrw pmpaddr13, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 14:
                __asm__("csrw pmpaddr14, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 15:
                __asm__("csrw pmpaddr15, %[addr]"
                        :: [addr] "r" (address) :);
                break;
        }
}

/*!
 * @brief Configure a PMP region
 * @param region The PMP region to configure
 * @param config The desired configuration of the PMP region
 * @param address The desired address of the PMP region
 * @return void
 */
void pmp_set_region(   unsigned int region,
                       struct pmp_config config,
                       size_t address)
{

        if (region > 15) {
                printf("ERROR : 16 entries PMPs between 0 to 15\n");
                exit(0);
        }

        switch(region) {
                case 0:
                __asm__("csrw pmpaddr0, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 1:
                __asm__("csrw pmpaddr1, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 2:
                __asm__("csrw pmpaddr2, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 3:
                __asm__("csrw pmpaddr3, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 4:
                __asm__("csrw pmpaddr4, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 5:
                __asm__("csrw pmpaddr5, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 6:
                __asm__("csrw pmpaddr6, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 7:
                __asm__("csrw pmpaddr7, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 8:
                __asm__("csrw pmpaddr8, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 9:
                __asm__("csrw pmpaddr9, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 10:
                __asm__("csrw pmpaddr10, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 11:
                __asm__("csrw pmpaddr11, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 12:
                __asm__("csrw pmpaddr12, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 13:
                __asm__("csrw pmpaddr13, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 14:
                __asm__("csrw pmpaddr14, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 15:
                __asm__("csrw pmpaddr15, %[addr]"
                        :: [addr] "r" (address) :);
                break;
        }

        /* Mask to clear old pmpcfg */
        size_t cfgmask = (0xFF);
        size_t pmpcfg = (CONFIG_TO_INT(config));
                                
        switch(region / 8) {
        case 0:
                __asm__("csrc pmpcfg0, %[mask]"
                        :: [mask] "r" (cfgmask << (8 * (region % 8))) :);

                __asm__("csrs pmpcfg0, %[cfg]"
                        :: [cfg] "r" (pmpcfg << (8 * (region % 8))) :);

                break;
        case 1:
                __asm__("csrc pmpcfg2, %[mask]"
                        :: [mask] "r" (cfgmask << (8 * (region % 8))) :);

                __asm__("csrs pmpcfg2, %[cfg]"
                        :: [cfg] "r" (pmpcfg << (8 * (region % 8))) :);
                break;
        }
}


/*!
 * @brief Configure a PMP region
 * @param region The PMP region to configure
 * @param config The desired configuration of the PMP region
 * @param address The desired address of the PMP region
 * @return void
 */
void pmp_set_region_init(   unsigned int region,
                       unsigned int config,
                       size_t address)
{
        

        if (region > 15) {
                printf("ERROR : 16 entries PMPs between 0 to 15\n");
                exit(0);
        }

        switch(region) {
                case 0:
                __asm__("csrw pmpaddr0, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 1:
                __asm__("csrw pmpaddr1, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 2:
                __asm__("csrw pmpaddr2, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 3:
                __asm__("csrw pmpaddr3, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 4:
                __asm__("csrw pmpaddr4, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 5:
                __asm__("csrw pmpaddr5, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 6:
                __asm__("csrw pmpaddr6, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 7:
                __asm__("csrw pmpaddr7, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 8:
                __asm__("csrw pmpaddr8, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 9:
                __asm__("csrw pmpaddr9, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 10:
                __asm__("csrw pmpaddr10, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 11:
                __asm__("csrw pmpaddr11, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 12:
                __asm__("csrw pmpaddr12, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 13:
                __asm__("csrw pmpaddr13, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 14:
                __asm__("csrw pmpaddr14, %[addr]"
                        :: [addr] "r" (address) :);
                break;
                case 15:
                __asm__("csrw pmpaddr15, %[addr]"
                        :: [addr] "r" (address) :);
                break;
        }

        /* Mask to clear old pmpcfg */
        size_t cfgmask = (0xFF);
        size_t pmpcfg = (CONFIG_TO_INT(config));
                                
        switch(region / 8) {
        case 0:
                __asm__("csrc pmpcfg0, %[mask]"
                        :: [mask] "r" (cfgmask << (8 * (region % 8))) :);

                __asm__("csrs pmpcfg0, %[cfg]"
                        :: [cfg] "r" (pmpcfg << (8 * (region % 8))) :);

                break;
        case 1:
                __asm__("csrc pmpcfg2, %[mask]"
                        :: [mask] "r" (cfgmask << (8 * (region % 8))) :);

                __asm__("csrs pmpcfg2, %[cfg]"
                        :: [cfg] "r" (pmpcfg << (8 * (region % 8))) :);
                break;
        }
}