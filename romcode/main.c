int gboot_main()
{
	int i;
	unsigned char buf[1024*4];
#ifdef MMU_ON
	mmu_init();
#endif

	uart_init();
	led_init();
	printf("======== led_init done ==========\n");

	button_init();

	init_irq();

	led_off();


	NF_Erase(0x20000000);
	buf[0] = 100;
	NF_WritePage(0x20000000,buf);

	buf[0] = 10;
	NF_PageRead(0x20000000,buf);

	if ( buf[0] == 100 ) {
		printf("read right data........\n");
		printf("buf[0] = %d\n", buf[0]);
		led_on();
	}

	nand_readid();

	while(1);


	return 0;    
}


