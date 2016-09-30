/**
 * @file  pifacedigital-cmd.c
 * @brief Command line tool for PiFace Digital.
 *
 * Takes two to three positional arguments:
 * pifacedigital [cmd] [reg] {value}
 * pifacedigital read output
 * pifacedigital read input
 * pifacedigital write output 0xaa
 *
 * Can also take optional flags:
 * pifacedigital --bit 1 write output 1
 * pifacedigital --hw-addr 1 --bit=1 write output 1
 *
 * Or shorthand:
 * pifacedigital -b 1 write output 1
 * pifacedigital -h 1 -b 1 write output 1
 */
#include <stdlib.h>
#include <argp.h>
#include <strings.h>
#include <mcp23s17.h>
#include <pifacedigital.h>


/**********************************************************************/
/* argp stuff from:
 * http://www.gnu.org/software/libc/manual/html_node/Argp.html#Argp
 */
const char *argp_program_version = "pifacedigital v0.1.0";
const char *argp_program_bug_address = "<thomas.preston@openlx.org.uk>";

/* Program documentation. */
static char doc[] = "Command line tool for PiFace Digital.";

/* A description of the arguments we accept. */
static char args_doc[] = "CMD REG";

/* The options we understand. */
static struct argp_option options[] = {
    {"bit-num", 'b', "BITNUM", 0, "Bit number to read/write to." },
    {"hw-addr", 'h', "HWADDR", 0, "Hardware address of PiFace Digital." },
    { 0 },
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char * cmd;
    char * reg;
    int value, bit_num, hw_addr;
};

/* Parse a single option. */
static error_t parse_opt(int key, char * arg, struct argp_state * state)
{
    /* Get the input argument from argp_parse, which we
      know is a pointer to our arguments structure. */
    struct arguments * arguments = state->input;

    switch (key)
    {
    case 'b':
        arguments->bit_num = atoi(arg);
        break;
    case 'h':
        arguments->hw_addr = atoi(arg);
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 3) {
            argp_usage(state); /* Too many arguments. */
        } else if (state->arg_num == 0) {
            arguments->cmd = arg;
        } else if (state->arg_num == 1) {
            arguments->reg = arg;
        } else if (state->arg_num == 2) {
            arguments->value = (int) strtol(arg, NULL, 0);
        }
        break;

    case ARGP_KEY_END:
        if (state->arg_num < 2)
             /* Not enough arguments. */
             argp_usage (state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};
/**********************************************************************/

uint8_t str2reg(char * reg_str);
void pfd_read(int bit_num, uint8_t reg, uint8_t hw_addr);
void pfd_write(uint8_t value, int bit_num, uint8_t reg, uint8_t hw_addr);


int main(int argc, char **argv)
{
    struct arguments arguments;

    // Default values
    arguments.cmd = NULL;
    arguments.reg = NULL;
    arguments.value = -1;
    arguments.bit_num = -1;
    arguments.hw_addr = -1;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // printf("cmd = %s\nreg = %s\nbit_num = %d\nhw_addr = %d\n",
    //        arguments.cmd, arguments.reg, arguments.bit_num, arguments.hw_addr);

    // hw_addr defaults to 0
    const uint8_t hw_addr = arguments.hw_addr > 0 ? arguments.hw_addr : 0;
    const uint8_t reg = str2reg(arguments.reg);
    if (arguments.bit_num > 7) {
        fprintf(stderr, "pifacedigital: bit num must in range 0-7.\n");
        exit(1);
    }

    if (strcmp(arguments.cmd, "read") == 0) {
        pfd_read(arguments.bit_num, reg, hw_addr);
    } else if (strcmp(arguments.cmd, "write") == 0) {
        pfd_write((uint8_t) (arguments.value & 0xff),
                  arguments.bit_num,
                  reg,
                  hw_addr);
    }

    exit(0);
}

uint8_t str2reg(char * reg_str)
{
    // convert to lower case
    int i;
    for(i = 0; reg_str[i]; i++) {
        reg_str[i] = tolower(reg_str[i]);
    }

    // get the real value
    if (strcmp(reg_str, "output") == 0 || strcmp(reg_str, "gpioa") == 0) {
        return GPIOA;
    } else if (strcmp(reg_str, "input") == 0 || strcmp(reg_str, "gpiob") == 0) {
        return GPIOB;
    } else {
        fprintf(stderr, "pifacedigital: no such register '%s'\n", reg_str);
        exit(1);
    }
}

void pfd_read(int bit_num, uint8_t reg, uint8_t hw_addr)
{
    pifacedigital_open(hw_addr);
    uint8_t value;
    if (bit_num >= 0) {
        value = pifacedigital_read_bit(bit_num, reg, hw_addr);
    } else {
        value = pifacedigital_read_reg(reg, hw_addr);
    }
    printf("%d\n", value);
    pifacedigital_close(hw_addr);
}

void pfd_write(uint8_t value, int bit_num, uint8_t reg, uint8_t hw_addr)
{
    pifacedigital_open(hw_addr);
    if (bit_num >= 0) {
        pifacedigital_write_bit(value, bit_num, reg, hw_addr);
    } else {
        pifacedigital_write_reg(value, reg, hw_addr);
    }
    pifacedigital_close(hw_addr);
}
