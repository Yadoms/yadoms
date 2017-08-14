#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <mcp23s17.h>
#include "pifacedigital.h"


// PiFace Digital is always at /dev/spidev0.0
static const int bus = 0, chip_select = 0;
static int pfd_count = 0; // number of PiFace Digitals
/* MCP23S17 SPI file descriptor. All PiFace Digitals are connected to
 * the same SPI bus, only need 1 fd. Keeping this hiden from novice
 * PiFace Digital users.
 * If you want to make raw SPI transactions to the device then try using
 * libmcp23s17.
 */
static int mcp23s17_fd = 0;


int pifacedigital_open_noinit(uint8_t hw_addr)
{
    // if initialising the first PiFace Digital, open the fd
    if (pfd_count <= 0) {
        if ((mcp23s17_fd = mcp23s17_open(bus, chip_select)) < 0) {
            fprintf(stderr,
                    "pifacedigital_open_noinit: ERROR Could not open MCP23S17 "
                    "device.");
            return mcp23s17_fd; // JM. BERHAULT for debugging purpose
        }
    }
    pfd_count++;
    return mcp23s17_fd; // returns the fd, for advanced users.
}

int pifacedigital_open(uint8_t hw_addr)
{
    if ((mcp23s17_fd = pifacedigital_open_noinit(hw_addr)) < 0) {
        fprintf(stderr,
                "pifacedigital_open: ERROR Could not open MCP23S17 device.");
        return -1;
    }

    // set up config register
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

    // enable interrupts
    mcp23s17_write_reg(0xff, GPINTENB, hw_addr, mcp23s17_fd);

    return mcp23s17_fd; // returns the fd, for advanced users.
}

void pifacedigital_close(uint8_t hw_addr)
{
    if (pfd_count <= 0)
        return;

    pfd_count--;

    // disable interrupts if enabled
    const uint8_t intenb = mcp23s17_read_reg(GPINTENB, hw_addr, mcp23s17_fd);
    if (intenb) {
        mcp23s17_write_reg(0, GPINTENB, hw_addr, mcp23s17_fd);
        // now do some other interrupt stuff...
        // TODO
    }

    // if no more PiFace Digital's, close the fd
    if (pfd_count <= 0) {
        pfd_count = 0;
        close(mcp23s17_fd);
    }
}

uint8_t pifacedigital_read_reg(uint8_t reg, uint8_t hw_addr)
{
    return mcp23s17_read_reg(reg, hw_addr, mcp23s17_fd);
}

void pifacedigital_write_reg(uint8_t data, uint8_t reg, uint8_t hw_addr)
{
    mcp23s17_write_reg(data, reg, hw_addr, mcp23s17_fd);
}

uint8_t pifacedigital_read_bit(uint8_t bit_num, uint8_t reg, uint8_t hw_addr)
{
    return mcp23s17_read_bit(bit_num, reg, hw_addr, mcp23s17_fd);
}

void pifacedigital_write_bit(uint8_t data,
                             uint8_t bit_num,
                             uint8_t reg,
                             uint8_t hw_addr)
{
    mcp23s17_write_bit(data, bit_num, reg, hw_addr, mcp23s17_fd);
}

uint8_t pifacedigital_digital_read(uint8_t pin_num)
{
    return pifacedigital_read_bit(pin_num, INPUT, 0);
}

void pifacedigital_digital_write(uint8_t pin_num, uint8_t value)
{
    pifacedigital_write_bit(value, pin_num, OUTPUT, 0);
}

int pifacedigital_enable_interrupts()
{
   return mcp23s17_enable_interrupts();
}

int pifacedigital_disable_interrupts()
{
   return mcp23s17_disable_interrupts();
}

int pifacedigital_wait_for_input(uint8_t *data,
                                 int timeout,
                                 uint8_t hw_addr)
{
    // Flush any pending interrupts prior to wait
    pifacedigital_read_reg(0x11, hw_addr);

    // Wait for input state change
    int ret = mcp23s17_wait_for_interrupt(timeout);

    // Read & return input register, thus clearing interrupt
    *data = pifacedigital_read_reg(0x11, hw_addr);
    return ret;
}
