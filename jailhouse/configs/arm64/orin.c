
#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

struct {
	struct jailhouse_system header;
	__u64 cpus[1];
	struct jailhouse_memory mem_regions[4 + 4 + 1 + 5];
	struct jailhouse_irqchip irqchips[8];
	struct jailhouse_pci_device pci_devices[2];
} __attribute__((packed)) config = {
	.header = {
		.signature = JAILHOUSE_SYSTEM_SIGNATURE,
		.architecture = JAILHOUSE_ARM64,
		.revision = JAILHOUSE_CONFIG_REVISION,
		.flags = JAILHOUSE_SYS_VIRTUAL_DEBUG_CONSOLE,
		.hypervisor_memory = {
			.phys_start = 0xc0000000, 
			.size = 0x00200000, 
		},
		.debug_console = {
			/* uarti */
			.address = 0x031d0000, 
			.size = 0x00010000,
			.type = JAILHOUSE_CON_TYPE_PL011,
			.flags = JAILHOUSE_CON_ACCESS_MMIO |
				 JAILHOUSE_CON_REGDIST_4,
		},
		.platform_info = {
			.pci_mmconfig_base = 0x0f800000, 
			.pci_mmconfig_end_bus = 0,
			.pci_is_virtual = 1,
			.pci_domain = 3,
			.arm = {
				.maintenance_irq = 25,	
				.gic_version = 3,
				.gicd_base = 0x0f400000, 
				.gicr_base = 0x0f440000, 
			},
			.memguard = {
				.num_irqs = 32 + 960,
				.hv_timer = 26,
				.num_pmu_irq = 12,
				/* One PMU irq per CPU */
				.pmu_cpu_irq = {
					23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 
				},
			},
		},
		.root_cell = {
			.name = "Jetson-AGX-Orin-Root", 
			.num_pci_devices = ARRAY_SIZE(config.pci_devices),
			.cpu_set_size = sizeof(config.cpus),
			.num_memory_regions = ARRAY_SIZE(config.mem_regions),
			.num_irqchips = ARRAY_SIZE(config.irqchips),
			.vpci_irq_base = 588 - 32,	
		},
	},

	.cpus = {
		0b111111111111,
	},

	.mem_regions = {
		/* 6 MB memory region from 0xc0200000 to 0xc80000000 for communication */

		/* IVSHMEM shared memory regions for 00:00.0 (demo) */
		/* 4 regions for 2 peers */
		/* state table, read-only for all */ {
			.phys_start = 0xc0200000,
			.virt_start = 0xc0200000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ,
		},
		/* shared region, read-write for all */ {
			.phys_start = 0xc0210000,
			.virt_start = 0xc0210000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE,
		},
		/* peer 0 output region */ {
			.phys_start = 0xc0220000,
			.virt_start = 0xc0220000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE,
		},
		/* peer 1 output region */ {
			.phys_start = 0xc0230000,
			.virt_start = 0xc0230000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ,
		},

		/* IVSHMEM shared memory regions for 00:01.0 (networking) */
		JAILHOUSE_SHMEM_NET_REGIONS(0xc0240000, 0), /* four regions, size 1MB */

		/* 80 MB memory region from 0xc0300000 to 0xc5300000 for cells */ {
			.phys_start = 0xc0300000,
			.virt_start = 0xc0300000,
			.size = 0x05000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE,
		},
















		/* I/O memory region, first part */ {
			.phys_start = 0x00000000,
			.virt_start = 0x00000000,
			.size =       0x0f800000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_IO | JAILHOUSE_MEM_DMA,
		},
		
		/* 0f80 0000 -- 1000 0000 :: vpci */


		/* I/O memory region, second part */ {
			.phys_start = 0x10000000,
			.virt_start = 0x10000000,
			.size =       0x70000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_IO | JAILHOUSE_MEM_DMA,
		},

		/* normal memory before the Jailhouse reserved memory region */ {
			.phys_start = 0x80000000,
			.virt_start = 0x80000000,
			.size =       0x40000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_DMA,
		},

		/* c000 0000 -- c800 0000 :: jailhouse */

		/* normal memory after the Jailhouse reserved memory region */ {
			.phys_start = 0xc8000000,
			.virt_start = 0xc8000000,
			.size = 0x778000000, /* ！！！ from c800'0000 to 8'4000'0000 ！！！ */
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_DMA,
		},

		/* I/O memory region, PCI regions */ {
			.phys_start = 0x2080000000,
			.virt_start = 0x2080000000,
			.size =       0x0030000000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_IO | JAILHOUSE_MEM_DMA,
		},


	},

	.irqchips = {
		/* GIC */ {
			.address = 0x0f400000,
			.pin_base = 32,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 160,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 288,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 416,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 544,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 672,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 800,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 928,
			.pin_bitmap = {
				0xffffffff, 0xffffffff, 0, 0,
			},
		},
	},

	.pci_devices = {
		/* 00:00.0 (demo) */ {
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 3,
			.bdf = 0 << 3,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
			.shmem_regions_start = 0,
			.shmem_dev_id = 0,
			.shmem_peers = 2,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_UNDEFINED,
		},
		/* 00:01.0 (networking) */ {
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 3,
			.bdf = 1 << 3,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
			.shmem_regions_start = 4,
			.shmem_dev_id = 0,
			.shmem_peers = 2,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_VETH,
		},
	},
};
