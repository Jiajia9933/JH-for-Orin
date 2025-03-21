# To compile the kernel

### 1. Add jailhouse enabling patches to the kernel

    cd Linux_for_Tegra/source/public/
    git clone https://github.com/Minervasys/jailhouse-documentation
    git clone https://github.com/Minervasys/jailhouse.git
    git am --directory=kernel/kernel-5.10 jailhouse-documentation/jailhouse-enabling-patches/5.10.y/*.patch

!!  The patch 0007 could be removed, because the kernel source code has the symbol **ioremap_page_range** already exported.

### 2. Reserving memory for jailhouse in dts:
Linux_for_Tegra/source/public/hardware/nvidia/platform/t23x/concord/kernel-dts/tegra234-p3701-0000-p3737-0000.dts

    reserved-memory {
		#address-cells = <2>;
		#size-cells = <2>;
		ranges;

		jailhouse@c0000000  {
			no-map;
			reg = <0x0 0xc0000000 0x0 0x08000000>;
		};
	};


    // set uart0 status = "okay"

    serial@3100000 {
		status = "okay";
	};

### 3. Download and set the toolchain as discribed in [orin_kernel.txt](orin_kernel.txt)

    -> for compiling the kernel just do following commands:

    cd Linux_for_Tegra/source/public/
    mkdir kernel_out
    mkdir modules_out

    export CROSS_COMPILE=$HOME/35.6.0/aarch64--glibc--stable-final/bin/aarch64-buildroot-linux-gnu-
    export ARCH=arm64
    export TEGRA_KERNEL_OUT=Linux_for_Tegra/source/public/kernel_out/
    export MODULES_OUT_DIR=Linux_for_Tegra/source/public/modules_out/

    make -C kernel/kernel-5.10/ LOCALVERSION="-YOUR_VERSION" O=$PWD/kernel_out/ tegra_defconfig
    make -C kernel/kernel-5.10/ LOCALVERSION="-YOUR_VERSION" O=$PWD/kernel_out/ menuconfig


-> set configs as discribed in [orin_kernel.txt](orin_kernel.txt)
    
    
    make -C kernel/kernel-5.10/ LOCALVERSION="-YOUR_VERSION" O=$PWD/kernel_out/ -j12 --output-sync=target Image dtbs modules

    make -C kernel/kernel-5.10/ LOCALVERSION="-YOUR_VERSION" O=$PWD/kernel_out/ -j12 INSTALL_MOD_PATH=$MODULES_OUT_DIR modules_install


### These you should bring to the Orin board for the kernel:
Image:      
Linux_for_Tegra/source/public/kernel_out/arch/arm64/boot/Image

dtb:\
Linux_for_Tegra/source/public/kernel_out/arch/arm64/boot/dts/nvidia/tegra234-p3701-0000-p3737-0000.dtb

modules:\
Linux_for_Tegra/source/public/modules_out/lib/modules/5.10.216-YOUR_VERSION/



# To compile jailhouse

### files to modify:
1. Copy the [jailhouse_config.h](jailhouse_config.h) file to Linux_for_Tegra/source/public/jailhouse/include/jailhouse/ as "config.h" 

2. Modify jailhouse/include/jailhouse/mem-bomb.h to add Jailhouse configs for the AGX Orin board:

    #ifdef CONFIG_MACH_AGXORIN
    #define NUM_CPU			12
    #define MAIN_PHYS_BASE		0xc0300000
    #define COMM_PHYS_BASE		0xc5300000
    #else
    #define NUM_CPU			8
    #endif

### Following steps to compile:

    cd Linux_for_Tegra/source/public/jailhouse
    export KDIR=Linux_for_Tegra/source/public/kernel/kernel-5.10/
    export DESTDIR=Linux_for_Tegra/source/public/modules_out/

    make LOCALVERSION="-YOUR_VERSION" O=$PWD/../kernel_out/ install

    rm -rf ../modules_out/usr/local/lib/python3.11/dist-packages/pyjailhouse/__pycache__/

### compile another kernel Image for jailhouse guest cell

    // Download source code:
    
    wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.104.tar.xz
    tar xf linux-6.1.104.tar.xz
    mv linux-6.1.104 linux-guest

    // Add jailhouse enabling patches:

    cd linux-guest
    git init
    git add .??* *
    git commit -m "import"
    git am jailhouse-documentation/jailhouse-enabling-patches/6.1.y/*.patch

-> copy [.config_guest](.config_guest) to linux-guest/ and compile the kernel

    cd linux-guest
    export ARCH=arm64
    export CROSS_COMPILE=aarch64-linux-gnu-
    make oldconfig
    make -j$(nproc)
    cp arch/arm64/boot/Image Image_guest  // -> this Image_guest is used for jailhouse guest cell 

    




### These you should bring to the Orin board for jailhouse:

For root cell:

Linux_for_Tegra/source/public/modules_out/usr/\
Linux_for_Tegra/source/public/jailhouse/configs/arm64/orin.cell\
Linux_for_Tegra/source/public/jailhouse/driver/jailhouse.ko\
Linux_for_Tegra/source/public/jailhouse/hypervisor/jailhouse.bin

For guest cell:

Linux_for_Tegra/source/public/jailhouse/configs/arm64/dts/inmate-orin.dtb\
Linux_for_Tegra/source/public/jailhouse/configs/arm64/dts/inmate-orin2.dtb\
Linux_for_Tegra/source/public/jailhouse/configs/arm64/orin-linux-demo.cell\
Linux_for_Tegra/source/public/jailhouse/configs/arm64/orin-linux-demo2.cell\
linux_guest/arch/arm64/boot/Image_guest\
[initrd](initrd) 



# Start the jailhouse root cell on the Orin board:
with "sudo nvpmodle -m 0" you can set the nvpmodel to get all 12 CPUs online with max frequencies , see details here:
https://docs.nvidia.com/jetson/archives/r35.5.0/DeveloperGuide/SD/PlatformPowerAndPerformance/JetsonOrinNanoSeriesJetsonOrinNxSeriesAndJetsonAgxOrinSeries.html#cpu-power-management 

    sudo nvpmodle -m 0

    sudo cp orin.cell /lib/firmware/
    sudo cp jailhouse.bin /lib/firmware/
    sudo cp jailhouse.ko /lib/modules/5.10.216-jiajia1/kernel/drivers/
    sudo rsync -av --ignore-existing --update /home/nvidia/usr/ /usr/

    sudo depmode -a
    sudo modprobe jailhouse
    sudo jailhouse enable /lib/firmware/orin.cell

# Start the FIRST guest cell:

    sudo cp orin-linux-demo.cell /lib/firmware/
    sudo cp inmate-orin.dtb /lib/firmware/
    sudo stty -F /dev/ttyTHS0 115200
    sudo jailhouse cell linux /lib/firmware/orin-linux-demo.cell /home/nvidia/Image_guest -d /lib/firmware/inmate-orin.dtb -i /home/nvidia/initrd -c "console=ttyS1,115200 cma=4M"
    (check guest cell here: picocom -b 115200 /dev/ttyUSB.orin.4) 

# Start the SECOND guest cell:

    sudo cp orin-linux-demo2.cell /lib/firmware/
    sudo cp inmate-orin2.dtb /lib/firmware/
    sudo jailhouse cell linux /lib/firmware/orin-linux-demo2.cell /home/nvidia/Image_guest -d /lib/firmware/inmate-orin2.dtb -i /home/nvidia/initrd -c "console=ttyAMA1,115200 cma=4M"

