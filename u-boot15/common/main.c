/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* #define	DEBUG	*/

#include <common.h>
#include <autoboot.h>
#include <cli.h>
#include <version.h>
#include <cli_hush.h>

DECLARE_GLOBAL_DATA_PTR;

static void ExecuteCmd(char *cmd)
{
	parse_string_outer(cmd, FLAG_PARSE_SEMICOLON | FLAG_EXIT_FROM_LOOP);
}

extern int dn_filesize;
void arm_USBfuse(void)
{
	unsigned char select;
	while(1)
	{
		printf("\n##### Select the fuction #####\n");
		printf("[1] Flash u-boot\n");
		printf("[2] Flash kernel\n");	
		printf("[3] Flash system\n");		
		printf("[4] Exit\n");
		printf("Enter your Selection:");
	
		select = getc();
		printf("%c\n", select >= ' ' && select <= 127 ? select : ' ');	
		char cmd[256];

		switch(select) 
		{
			case '1':
				ExecuteCmd("nand erase 0 200000");
				ExecuteCmd("dnw 50008000");
				ExecuteCmd("nand write.uboot 50008000 0 200000");
				break;
			
			case '2':
				ExecuteCmd("nand erase 200000 500000");
				ExecuteCmd("dnw 50008000");
				ExecuteCmd("nand write.e 50008000 200000 500000");
				break;
					
			case '3':
				
				ExecuteCmd("nand erase 700000");
				ExecuteCmd("dnw 50008000");
				sprintf(cmd, "nand write.yaffs2 50008000 700000 %x", dn_filesize);
				ExecuteCmd(cmd);
				break;
			
			case '4':
				return;
			
			default:
				break;
		}
	}
}
void arm_sdfuse(void)
{
	unsigned char select;
	while(1)
	{
		printf("\n##### flash from SDcard  #####\n");
		printf("\n##### Select the fuction #####\n");
		printf("[1] Flash all image\n");
		printf("[2] Flash u-boot\n");
		printf("[3] Flash kernel\n");
		printf("[4] Flash system\n");			
		printf("[5] Exit\n");
		printf("Enter your Selection:");
	
		select = getc();
		printf("%c\n", select >= ' ' && select <= 127 ? select : ' ');	
	
		switch(select) 
		{
			case '1':
				ExecuteCmd("nand erase");
				ExecuteCmd("fatload mmc 0:1 50008000 u-boot.bin");
				ExecuteCmd("nand write.uboot 50008000 0 200000");
				ExecuteCmd("fatload mmc 0:1 50008000 zImage");
				ExecuteCmd("nand write.e 50008000 200000 500000");
				ExecuteCmd("fatload mmc 0:1 50008000 rootfs.yaffs2");
				ExecuteCmd("nand write.yaffs2 50008000 700000 $filesize");
				break;
			
			case '2':
				ExecuteCmd("nand erase 0 200000");
				ExecuteCmd("fatload mmc 0:1 50008000 u-boot.bin");
				ExecuteCmd("nand write.uboot 50008000 0 200000");
				break;
					
			case '3':
				ExecuteCmd("nand erase 200000 500000");
				ExecuteCmd("fatload mmc 0:1 50008000 zImage");
				ExecuteCmd("nand write.e 50008000 200000 500000");
				break;

			case '4':
				ExecuteCmd("nand erase 700000");
				ExecuteCmd("fatload mmc 0:1 50008000 rootfs.yaffs2");
				ExecuteCmd("nand write.yaffs2 50008000 700000 $filesize");
				break;
			case '5':
				return;
			default:
				break;
		}
	}
}

void lcd_size_config(void)
{
	unsigned char select;
	char *ppp_dev[6]={"lcdsize=35","lcdsize=43","lcdsize=56","lcdsize=70","lcdsize=VGA800","lcdsize=XGA1024"};
	char cmdline[256]="setenv bootargs root=/dev/mtdblock2 rootfstype=yaffs2 init=/linuxrc nconsole=tty1 console=ttySAC0,115200 android.ril=s3c2410_serial1 ";
	
lcd_size_config:
	printf("\n##### Select the lcdsize #####\n");
	printf("[1] 3.5\n");
	printf("[2] 4.3\n");
	printf("[3] 5.6\n");
	printf("[4] 7.0\n");
	printf("[5] VGA||8.0||10.4\n");
	printf("[6] hdmi\n");
	printf("[7] Exit\n");
	printf("Enter your Selection:");
	
	select = getc();
	printf("%c\n", select >= ' ' && select <= 127 ? select : ' ');
	
	switch(select) 
	{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
			sprintf(cmdline + strlen(cmdline), "%s", ppp_dev[select-'1']);
			ExecuteCmd(cmdline);
			ExecuteCmd("saveenv");
			break;

		case '7':
			break;
			
		default:
			goto lcd_size_config;
	}
}

#ifdef CONFIG_BOOT_NAND
void NAND_ARMMenu(void)
{
	unsigned char select;
	char *command;	
	char cramfs_cmdline[256]="setenv bootargs ";

	while(1) {
		printf("\n");
		printf("###################### User Menu for OK6410#####################\n");	
		
		printf("[1] Format the nand flash\n");		
//		printf("[2] Burn image from SD card\n");
		printf("[2] Burn image from USB\n");	
		printf("[3] configure the lcd size\n");							
		printf("[4] Boot the system\n");
		printf("[5] Reboot the u-boot\n");
		printf("[6] Exit to command line\n");								

		printf("-----------------------------Select---------------------------------\n");
		printf("Enter your Selection:");

		select = getc();
		printf("%c\n", select >= ' ' && select <= 127 ? select : ' ');

		switch(select) 
		{	
			//[1] Format the nand flash						
			case '1':
				ExecuteCmd("nand scrub");
				break;	
			//[2] Burn image from SD card
//			case '2':
//				arm_sdfuse();
//				break;
			//[3] Use USBsdfuse
			case '2':
				arm_USBfuse();
				break;	
			//[4] configure lcdsize
			case '3':
			lcd_size_config();
			break;	
			//[5] Boot the system		
			case '4':
				ExecuteCmd(getenv ("bootcmd"));
				break;
			//[6] Reboot the u-boot
			case '5':
				ExecuteCmd("reset");
				break;
			case '6':
				return;
			default:
				break;
		}
	}
}
#endif
/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
__weak void show_boot_progress(int val) {}

static void modem_init(void)
{
#ifdef CONFIG_MODEM_SUPPORT
	debug("DEBUG: main_loop:   gd->do_mdm_init=%lu\n", gd->do_mdm_init);
	if (gd->do_mdm_init) {
		char *str = getenv("mdm_cmd");

		setenv("preboot", str);  /* set or delete definition */
		mdm_init(); /* wait for modem connection */
	}
#endif  /* CONFIG_MODEM_SUPPORT */
}

static void run_preboot_environment_command(void)
{
#ifdef CONFIG_PREBOOT
	char *p;

	p = getenv("preboot");
	if (p != NULL) {
# ifdef CONFIG_AUTOBOOT_KEYED
		int prev = disable_ctrlc(1);	/* disable Control C checking */
# endif

		run_command_list(p, -1, 0);

# ifdef CONFIG_AUTOBOOT_KEYED
		disable_ctrlc(prev);	/* restore Control C checking */
# endif
	}
#endif /* CONFIG_PREBOOT */
}

/* We come here after U-Boot is initialised and ready to process commands */
void main_loop(void)
{
	const char *s;

	bootstage_mark_name(BOOTSTAGE_ID_MAIN_LOOP, "main_loop");

#ifndef CONFIG_SYS_GENERIC_BOARD
	puts("Warning: Your board does not use generic board. Please read\n");
	puts("doc/README.generic-board and take action. Boards not\n");
	puts("upgraded by the late 2014 may break or be removed.\n");
#endif

	modem_init();
#ifdef CONFIG_VERSION_VARIABLE
	setenv("ver", version_string);  /* set version variable */
#endif /* CONFIG_VERSION_VARIABLE */

	cli_init();

	run_preboot_environment_command();

#if defined(CONFIG_UPDATE_TFTP)
	update_tftp(0UL, NULL, NULL);
#endif /* CONFIG_UPDATE_TFTP */

	s = bootdelay_process();
	if (cli_process_fdt(&s))
		cli_secure_boot_cmd(s);

	autoboot_command(s);
#ifdef CONFIG_BOOT_NAND
    NAND_ARMMenu();
#endif

	cli_loop();
}
