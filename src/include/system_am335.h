/*
 * AM33XX-CM3 firmware
 *
 * Cortex-M3 (CM3) firmware for power management on Texas Instruments' AM33XX series of SoCs
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  This software is licensed under the  standard terms and conditions in the Texas Instruments  Incorporated
 *  Technology and Software Publicly Available Software License Agreement , a copy of which is included in the
 *  software download.
*/

#ifndef __SYSTEM_AM335_H__
#define __SYSTEM_AM335_H__

#include <stdint.h>
#include <low_power.h>

struct ipc_data {
	int reg1;
	int reg2;
	int reg3;
	int reg4;
	int reg5;
	int reg6;
	int reg7;
	int reg8;
};

enum cmd_ids {
	CMD_ID_INVALID		= 0x0,
	CMD_ID_RTC		= 0x1,
	CMD_ID_RTC_FAST		= 0x2,
	CMD_ID_DS0		= 0x3,
	CMD_ID_DS1		= 0x5,
	CMD_ID_DS2		= 0x7,
	CMD_ID_STANDALONE	= 0x9,
	CMD_ID_STANDBY		= 0xb,
	CMD_ID_RESET		= 0xe,
	CMD_ID_VERSION		= 0xf,
	CMD_ID_COUNT,
};

struct cmd_data {
	enum cmd_ids cmd_id;
	union state_data *data;
	unsigned short i2c_sleep_offset;
	unsigned short i2c_wake_offset;
};

struct cmd_data cmd_global_data;
struct ipc_data a8_m3_data_r;

/* Board specifics populated in IPC_REG4 */
int mem_type;			/* Memory Type 2 = DDR2, 3 = DDR3 */
int vtt_toggle; 		/* VTT Toggle  1 = required */
int vtt_gpio_pin; 		/* VTT GPIO Pin */

/* Debug info */
int halt_on_resume;

int cmd_wake_sources;
int pd_mpu_stctrl_next_val;
int pd_mpu_stctrl_prev_val;
int pd_per_stctrl_next_val;
int pd_per_stctrl_prev_val;
int pd_mpu_pwrstst_prev_val;
int pd_per_pwrstst_prev_val;

unsigned int soc_id;
unsigned int soc_rev;
unsigned int soc_type;

/* Placeholder for storing PLL mode */
unsigned int clk_mode_per_val;
unsigned int clk_mode_disp_val;
unsigned int clk_mode_ddr_val;
unsigned int clk_mode_mpu_val;
unsigned int clk_mode_core_val;

/* PD_PER registers */
unsigned int am33xx_per[85];

unsigned int watermark;

void pm_init(void);

void system_init(void);
void system_core_clock_update(void);

void msg_init(void);
unsigned int msg_read(char);
void msg_read_all(void);
void msg_write(unsigned int, char);

void msg_cmd_read_id(void);
int msg_cmd_is_valid(void);
int msg_cmd_needs_trigger(void);
void msg_cmd_dispatcher(void);
void msg_cmd_stat_update(int);
void msg_cmd_wakeup_reason_update(int);

void a8_notify(int);
void a8_m3_low_power_sync(int);

void a8_lp_rtc_handler(struct cmd_data *);
void a8_lp_rtc_fast_handler(struct cmd_data *);
void a8_lp_ds0_handler(struct cmd_data *);
void a8_lp_ds1_handler(struct cmd_data *);
void a8_lp_ds2_handler(struct cmd_data *);
void a8_standalone_handler(struct cmd_data *);
void a8_standby_handler(struct cmd_data *);

void generic_wake_handler(int);
void a8_wake_rtc_handler(void);
void a8_wake_rtc_fast_handler(void);
void a8_wake_ds0_handler(void);
void a8_wake_ds1_handler(void);
void a8_wake_ds2_handler(void);
void a8_wake_standby_handler(void);

void m3_firmware_version(void);
void init_m3_state_machine(void);

void trace_init(void);
void trace_update(void);
void trace_get_current_pos(void);
void trace_set_current_pos(void);

int rtc_enable_check(void);
int rtc_reg_read(int);
void rtc_reg_write(int, int);

int i2c_write(const unsigned char *);

void setup_soc(void);

int a8_i2c_sleep_handler(unsigned short);
int a8_i2c_wake_handler(unsigned short);

#define BITBAND_SRAM_REF 	UMEM_ALIAS
#define BITBAND_SRAM_BASE 	0x22000000
#define BITBAND_SRAM(a,b) 	((BITBAND_SRAM_BASE + ((int)(a))*32 + (b*4)))

#define BITBAND_PERI_REF 	DMEM_ALIAS
#define BITBAND_PERI_BASE 	0x42000000
#define BITBAND_PERI(a,b) 	((BITBAND_PERI_BASE + (*(a) - BITBAND_PERI_REF)*32 + (b*4)))

#define BB_USB_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 0)))
#define BB_I2C0_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 1)))
#define BB_RTC_ALARM_WAKE	*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 2)))
#define BB_TIMER1_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 3)))
#define BB_UART0_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 4)))
#define BB_GPIO0_WAKE0		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 5)))
#define BB_GPIO0_WAKE1		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 6)))
#define BB_WDT1_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 7)))
#define BB_ADTSC_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 8)))
/* Not used currently */
#define BB_RTC_TIMER_WAKE	*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 9)))
#define BB_USBWOUT0		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 10)))
#define BB_MPU_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 11)))
#define BB_USBWOUT1		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 12)))

#define PD_MPU	0x1
#define PD_PER	0x2

#define MODULE_DISABLE	0x0
#define MODULE_ENABLE	0x2

#define RESUME_REG	0x0
#define STAT_ID_REG	0x1
#define PARAM1_REG	0x2
#define PARAM2_REG	0x3
#define PARAM3_REG	0x4
#define PARAM4_REG	0x5
#define TRACE_REG	0x6
#define CUST_REG	0x7

#define DS_IPC_DEFAULT	0xffffffff

#define CMD_STAT_PASS		0x0
#define CMD_STAT_FAIL		0x1
#define CMD_STAT_WAIT4OK	0x2

#define SET_BIT(x)		(1<<x)
#define CLR_BIT(x)		(0<<x)

#define AM335X_SOC_ID		0xB944
#define AM335X_REV_ES1_0	0
#define AM335X_REV_ES2_0	1

#define AM43XX_SOC_ID		0xB98C

#define SOC_TYPE_TEST	0
#define SOC_TYPE_EMU	1
#define SOC_TYPE_HS	2
#define SOC_TYPE_GP	3

#endif
