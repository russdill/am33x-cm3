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

#ifndef __LOW_POWER_H__
#define __LOW_POWER_H__

#include <stdint.h>

#define CM3_VERSION		0x24

#define MOSC_OFF		0x0
#define MOSC_ON			0x1

#define DS_COUNT_DEFAULT	0x6A75
#define DS_COUNT_SHIFT		0
#define DS_COUNT_MASK		(0xffff << DS_COUNT_SHIFT)
#define DS_ENABLE_SHIFT		17
#define DS_ENABLE_MASK		(1 << DS_ENABLE_SHIFT)


#define PD_ON			0x3
#define PD_RET			0x1
#define PD_OFF			0x0

#define MEM_BANK_RET_ST_RET	0x1
#define MEM_BANK_RET_ST_OFF	0x0

#define MEM_BANK_ON_ST_ON	0x3
#define MEM_BANK_ON_ST_RET	0x1
#define MEM_BANK_ON_ST_OFF	0x0

#define WAKE_ALL		0x17ff	/* all except MPU_WAKE in DS modes */
#define MPU_WAKE		0x800

#define RTC_TIMEOUT_DEFAULT	0x2
#define RTC_TIMEOUT_MAX		0xf

struct rtc_data {
	int rtc_timeout_val :4;	/* Delay for RTC alarm timeout. Default = 2secs */
};

struct deep_sleep_data {
	int mosc_state :1;			/* MOSC to be kept on (1) or off (0) */
	int deepsleep_count :16;		/* Count of how many OSC clocks needs to be seen \
						before exiting deep sleep mode */

	int vdd_mpu_val :15;			/* If vdd_mpu is to be lowered, vdd_mpu in mV */

	int pd_mpu_state :2;			/* Powerstate of PD_MPU */
	int pd_mpu_ram_ret_state :1;		/* Sabertooth RAM in retention state */
	int pd_mpu_l1_ret_state :1;		/* L1 memory in retention state */
	int pd_mpu_l2_ret_state :1;		/* L2 memory in retention state */
	int res1 :2;

	int pd_per_state :2;	 		/* Powerstate of PD_PER */
	int pd_per_icss_mem_ret_state :1;	/* ICSS memory in retention state */
	int pd_per_mem_ret_state :1; 		/* Other memories in retention state */
	int pd_per_ocmc_ret_state :1; 		/* OCMC memory in retention state */
	int pd_per_ocmc2_ret_state :1;		/* OCMC bank 2 in retention state */
	int res2 :5;

	int wake_sources :13;			/* Wake sources */
						/* USB, I2C0, RTC_ALARM, TIMER1 \
						   UART0, GPIO0_WAKE0, GPIO0_WAKE1, \
						   WDT1, ADTSC, RTC_TIMER, USBWOUT0, \
						   MPU, USBWOUT1 */
	int reserved :1;			/* Internal use */
};

struct pd_mpu_bits {
	int	ram_retst_mask;
	int	ram_retst_shift;
	int	l2_retst_mask;
	int	l2_retst_shift;
	int	l1_retst_mask;
	int	l1_retst_shift;
	int	lpstchg_mask;
	int	lpstchg_shift;
	int	logicretst_mask;
	int	logicretst_shift;
	int	pwrst_mask;
	int	pwrst_shift;
};

struct pd_per_bits {
	int	per_retst_mask;
	int	per_retst_shift;
	int	ram1_retst_mask;
	int	ram1_retst_shift;
	int	ram2_retst_mask;
	int	ram2_retst_shift;
	int	icss_retst_mask;
	int	icss_retst_shift;
	int	lpstchg_mask;
	int	lpstchg_shift;
	int	logicretst_mask;
	int	logicretst_shift;
	int	pwrst_mask;
	int	pwrst_shift;
};

int reg_mod(int, int, int);

int module_state_change(int, int);
int clkdm_state_change(int state, int reg);
int interconnect_modules_enable(void);
int interconnect_modules_disable(void);
int essential_modules_disable(void);
int essential_modules_enable(void);
void mpu_disable(void);
void mpu_enable(void);
void clkdm_sleep(void);
void clkdm_wake(void);
void mpu_clkdm_sleep(void);
void mpu_clkdm_wake(void);
void wkup_clkdm_sleep(void);
void wkup_clkdm_wake(void);
int pd_state_change(int, int);
void pd_state_restore(int);

int mpu_ram_ret_state_change(int, int);
int mpu_l1_ret_state_change(int, int);
int mpu_l2_ret_state_change(int, int);
int icss_mem_ret_state_change(int, int);
int per_mem_ret_state_change(int, int);
int ocmc_mem_ret_state_change(int, int);

int per_powerst_change(int, int);
int mpu_powerst_change(int, int);

int get_pd_per_stctrl_val(int);
int get_pd_mpu_stctrl_val(int);

int verify_pd_transitions(void);

int disable_master_oscillator(void);
int enable_master_oscillator(void);

void configure_deepsleep_count(int ds_count);
void configure_wake_sources(int wake_sources);
void clear_wake_sources(void);

void dpll_power_down(unsigned int dpll);
void dpll_power_up(unsigned int dpll);

void am33xx_power_down_plls(void);
void am33xx_power_up_plls(void);

void core_ldo_power_down(void);
void core_ldo_power_up(void);

void sram_ldo_ret_mode(int state);

void ddr_io_suspend(void);
void ddr_io_resume(void);

void vtp_disable(void);
void vtp_enable(void);

void set_ddr_reset(void);
void clear_ddr_reset(void);

void vtt_low(void);
void vtt_high(void);

void pll_bypass(unsigned int dpll);
void pll_lock(unsigned int dpll);

void ds_save(void);
void ds_restore(void);

#endif
