sudo sunxi-fel -v -p \
	spl uboot/spl32_a64_lpddr3.bin \
	write 0x4a000000 uboot/u-boot_a64.bin \
	write 0x44000 uboot/bl31_a64_40010000.bin \
	write 0x4fc00000 uboot/uEnv_a64.txt \
	write 0x40080000 uboot/kernel \
	write 0x4fa00000 uboot/sun50i-a64-pinephone-1.1.dtb \
	write 0x40010000 el2-bb.bin \
	reset64 0x44000
