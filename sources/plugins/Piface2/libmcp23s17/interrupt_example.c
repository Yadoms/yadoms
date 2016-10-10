#include <stdio.h>
#include <unistd.h>
#include "mcp23s17.h"


int main(void)
{
    const int bus = 0;
    const int chip_select = 0;
    const int hw_addr = 0;

    int mcp23s17_fd = mcp23s17_open(bus, chip_select);

    // config register
    const uint8_t ioconfig = BANK_OFF | \
                             INT_MIRROR_OFF | \
                             SEQOP_OFF | \
                             DISSLW_OFF | \
                             HAEN_ON | \
                             ODR_OFF | \
                             INTPOL_LOW;
    mcp23s17_write_reg(ioconfig, IOCON, hw_addr, mcp23s17_fd);

    // I/O direction
    mcp23s17_write_reg(0x00, IODIRA, hw_addr, mcp23s17_fd);
    mcp23s17_write_reg(0xff, IODIRB, hw_addr, mcp23s17_fd);

    // GPIOB pull ups
    mcp23s17_write_reg(0xff, GPPUB, hw_addr, mcp23s17_fd);

    // Write 0xaa to GPIO Port A
    mcp23s17_write_reg(0xaa, GPIOA, hw_addr, mcp23s17_fd);

    printf("Waiting for interrupt...\n");
    if (mcp23s17_wait_for_interrupt(10)) {
        // print the input state
        uint8_t input = mcp23s17_read_reg(GPIOB, hw_addr, mcp23s17_fd);
        printf("Inputs: 0x%x\n", input);
    } else {
        printf("Too long waiting for inputs!\n");
    }

    close(mcp23s17_fd);
}
