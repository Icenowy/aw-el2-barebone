sudo sunxi-fel -v -p \
	spl uboot/spl32_h6_lpddr3.bin \
	write 0x4a000000 uboot/u-boot_h6.bin \
	write 0x104000 uboot/bl31_h6_40010000.bin \
	write 0x4fc00000 uboot/uEnv_h6.txt \
	write 0x40080000 uboot/kernel \
	write 0x4fa00000 uboot/sun50i-h6-pine-h64-model-b.dtb \
	write 0x40010000 el2-bb.bin \
	reset64 0x104000
