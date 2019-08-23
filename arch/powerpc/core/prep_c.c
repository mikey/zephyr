/*
 * Copyright (c) 2019 Michael Neuling <mikey@neuling.org>
 * Copyright (c) 2019 Anton Blanchard <anton@linux.ibm.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <stdint.h>
#include <toolchain.h>
#include <kernel_structs.h>
#include <kernel_internal.h>

#define PROC_FREQ       50000000
#define UART_FREQ       115200
#define UART_BASE       0xc0002000
#define LPC_UART_BASE   0x60300d00103f8

/* Taken from skiboot */
#define REG_RBR         0
#define REG_THR         0
#define REG_DLL         0
#define REG_IER         1
#define REG_DLM         1
#define REG_FCR         2
#define REG_IIR         2
#define REG_LCR         3
#define REG_MCR         4
#define REG_LSR         5
#define REG_MSR         6
#define REG_SCR         7

#define LSR_DR          0x01  /* Data ready */
#define LSR_OE          0x02  /* Overrun */
#define LSR_PE          0x04  /* Parity error */
#define LSR_FE          0x08  /* Framing error */
#define LSR_BI          0x10  /* Break */
#define LSR_THRE        0x20  /* Xmit holding register empty */
#define LSR_TEMT        0x40  /* Xmitter empty */
#define LSR_ERR         0x80  /* Error */

#define LCR_DLAB        0x80  /* DLL access */

#define IER_RX          0x01
#define IER_THRE        0x02
#define IER_ALL         0x0f

static uint64_t lpc_uart_base;

static void lpc_uart_reg_write(uint64_t offset, uint8_t val) {
    uint64_t addr;

    addr = lpc_uart_base + offset;

    *(volatile uint8_t *)addr = val;
}

static uint8_t lpc_uart_reg_read(uint64_t offset) {
    uint64_t addr;
    uint8_t val;

    addr = lpc_uart_base + offset;

    val = *(volatile uint8_t *)addr;

    return val;
}

static int lpc_uart_tx_full(void) {
    return !(lpc_uart_reg_read(REG_LSR) & LSR_THRE);
}

static int lpc_uart_rx_empty(void) {
    return !(lpc_uart_reg_read(REG_LSR) & LSR_DR);
}

void lpc_uart_init(void) {
    lpc_uart_base = LPC_UART_BASE;
}

char lpc_uart_read(void) {
    while (lpc_uart_rx_empty()) ;
    return lpc_uart_reg_read(REG_THR);
}

void lpc_uart_write(char c) {
    while (lpc_uart_tx_full());
    lpc_uart_reg_write(REG_RBR, c);
}

/*
 * Core UART functions to implement for a port
 */

static uint64_t potato_uart_base;

#define PROC_FREQ 50000000
#define UART_FREQ 115200
#define UART_BASE 0xc0002000

#define POTATO_CONSOLE_TX		0x00
#define POTATO_CONSOLE_RX		0x08
#define POTATO_CONSOLE_STATUS		0x10
#define   POTATO_CONSOLE_STATUS_RX_EMPTY		0x01
#define   POTATO_CONSOLE_STATUS_TX_EMPTY		0x02
#define   POTATO_CONSOLE_STATUS_RX_FULL			0x04
#define   POTATO_CONSOLE_STATUS_TX_FULL			0x08
#define POTATO_CONSOLE_CLOCK_DIV	0x18
#define POTATO_CONSOLE_IRQ_EN		0x20

static uint64_t potato_uart_reg_read(int offset)
{
	uint64_t addr;
	uint64_t val;

	addr = potato_uart_base + offset;

	val = *(volatile uint64_t *)addr;

	return val;
}

static void potato_uart_reg_write(int offset, uint64_t val)
{
	uint64_t addr;

	addr = potato_uart_base + offset;

	*(volatile uint64_t *)addr = val;
}

static int potato_uart_rx_empty(void)
{
	uint64_t val;

	val = potato_uart_reg_read(POTATO_CONSOLE_STATUS);

	if (val & POTATO_CONSOLE_STATUS_RX_EMPTY)
		return 1;

	return 0;
}

static int potato_uart_tx_full(void)
{
	uint64_t val;

	val = potato_uart_reg_read(POTATO_CONSOLE_STATUS);

	if (val & POTATO_CONSOLE_STATUS_TX_FULL)
		return 1;

	return 0;
}

static char potato_uart_read(void)
{
	uint64_t val;

	val = potato_uart_reg_read(POTATO_CONSOLE_RX);

	return (char)(val & 0x000000ff);
}

static void potato_uart_write(char c)
{
	uint64_t val;

	val = c;

	potato_uart_reg_write(POTATO_CONSOLE_TX, val);
}

static unsigned long potato_uart_divisor(unsigned long proc_freq, unsigned long uart_freq)
{
	return proc_freq / (uart_freq * 16) - 1;
}

static void potato_uart_init(void)
{
	potato_uart_base = UART_BASE;

	potato_uart_reg_write(POTATO_CONSOLE_CLOCK_DIV, potato_uart_divisor(PROC_FREQ, UART_FREQ));
}

int lpc_console;

int microwatt_getchar(void)
{
	if (lpc_console)
		return (int)lpc_uart_read();

	while (potato_uart_rx_empty())
		/* Do nothing */ ;

	return potato_uart_read();
}



void microwatt_putchar(unsigned char c)
{
	while (potato_uart_tx_full())
		/* Do Nothing */;

	potato_uart_write(c);
}

static int lpc_console_out(int c)
{
	if (c == '\n') {
		lpc_uart_write('\r');
	}

	lpc_uart_write(c);
	return c;
}

static int microwatt_console_out(int c)
{
	if (c == '\n') {
		microwatt_putchar('\r');
	}

	microwatt_putchar(c);
	return c;
}

extern void __printk_hook_install(int (*fn)(int));
extern void __stdout_hook_install(int (*hook)(int));

void _PrepC(int qemu)
{
	z_bss_zero();
#ifdef CONFIG_XIP
	z_data_copy();
#endif

	lpc_console = qemu;
	if (lpc_console) {
		lpc_uart_init();
		__printk_hook_install(lpc_console_out);
		__stdout_hook_install(lpc_console_out);
	}
	else {
		potato_uart_init();
		__printk_hook_install(microwatt_console_out);
		__stdout_hook_install(microwatt_console_out);
	}

        /* Will be replaced later when a real serial driver comes up */

	z_cstart();
	CODE_UNREACHABLE;
}
