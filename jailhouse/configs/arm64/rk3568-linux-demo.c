/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Configuration for RK3566 and RK3568 SoCs
 *
 * Copyright (c) 2024 Minerva Systems
 *
 * Authors:
 *  Alex Zuepke <alex.zuepke@minervasys.tech>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.
 * See the COPYING file in the top-level directory.
 *
 * Linux guest cell on cores 2 and 3, 62 MB RAM.
 *
 * We'll use UART8 as output, so you have to enable it in the root cell's DTB:
 *   &uart8 {
 *     status = "okay";
 *   };
 *
 * Change here and the root cell DTB accordingly to activate the other serials.
 */

#include <jailhouse/types.h>
#include <jailhouse/cell-config.h>

struct {
	struct jailhouse_cell_desc cell;
	__u64 cpus[1];
	struct jailhouse_memory mem_regions[4 + 4 + 2 + 2];
	struct jailhouse_irqchip irqchips[3];
	struct jailhouse_pci_device pci_devices[2];
} __attribute__((packed)) config = {
	.cell = {
		.signature = JAILHOUSE_CELL_DESC_SIGNATURE,
		.architecture = JAILHOUSE_ARM64,
		.revision = JAILHOUSE_CONFIG_REVISION,
		.name = "rk3568-linux-demo",
		.flags = JAILHOUSE_CELL_PASSIVE_COMMREG,

		.cpu_set_size = sizeof(config.cpus),
		.num_memory_regions = ARRAY_SIZE(config.mem_regions),
		.num_irqchips = ARRAY_SIZE(config.irqchips),
		.num_pci_devices = ARRAY_SIZE(config.pci_devices),

		.vpci_irq_base = 288 - 32,

		.console = {
			/* uart2, interrupt 156 (SPI 124) */
			.address = 0xfe6c0000,
			.size = 0x00010000,
			.type = JAILHOUSE_CON_TYPE_8250,
			.flags = JAILHOUSE_CON_ACCESS_MMIO |
				 JAILHOUSE_CON_REGDIST_4,
		},
	},

	.cpus = {
		0b1100,
	},

	.mem_regions = {
		/* 6 MB memory region from 0x0aa00000 to 0x0b000000 for communication */

		/* IVSHMEM shared memory regions for 00:00.0 (demo) */
		/* 4 regions for 2 peers */
		/* state table, read-only for all */ {
			.phys_start = 0x0aa00000,
			.virt_start = 0x0aa00000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_ROOTSHARED,
		},
		/* shared region, read-write for all */ {
			.phys_start = 0x0aa10000,
			.virt_start = 0x0aa10000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_ROOTSHARED,
		},
		/* peer 0 output region */ {
			.phys_start = 0x0aa20000,
			.virt_start = 0x0aa20000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_ROOTSHARED,
		},
		/* peer 1 output region */ {
			.phys_start = 0x0aa30000,
			.virt_start = 0x0aa30000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_ROOTSHARED,
		},

		/* IVSHMEM shared memory regions for 00:01.0 (networking) */
		JAILHOUSE_SHMEM_NET_REGIONS(0x0ab00000, 1), /* four regions, size 1MB */

		/* 80 MB memory region from 0x0b000000 to 0x10000000 for cells */

		/* RAM for loader */ {
			.phys_start = 0x0fff0000,
			.virt_start = 0,
			.size = 0x00010000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_LOADABLE,
		},
		/* RAM for kernel */ {
			.phys_start = 0x0b000000,
			.virt_start = 0x0b000000,
			.size = 0x04ff0000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
			         JAILHOUSE_MEM_EXECUTE | JAILHOUSE_MEM_DMA |
			         JAILHOUSE_MEM_LOADABLE,
		},

		/* uart8 */ {
			.phys_start = 0xfe6c0000,
			.virt_start = 0xfe6c0000,
			.size = 0x10000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_IO | JAILHOUSE_MEM_ROOTSHARED,
		},
		/* communication region */ {
			.virt_start = 0x80000000,
			.size = 0x00001000,
			.flags = JAILHOUSE_MEM_READ | JAILHOUSE_MEM_WRITE |
				JAILHOUSE_MEM_COMM_REGION,
		},
	},

	.irqchips = {
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 32,
			.pin_bitmap = {
				0, 0, 0, (1u << (156 - 128)),
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 160,
			.pin_bitmap = {
				0, 0, 0, 0,
			},
		},
		/* GIC */ {
			.address = 0xfd400000,
			.pin_base = 288,
			.pin_bitmap = {
				(0xf << (288 - 288)), 0, 0, 0,
			},
		},
	},

	.pci_devices = {
		/* 00:00.0 (demo) */ {
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 0,
			.bdf = 0 << 3,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
			.shmem_regions_start = 0,
			.shmem_dev_id = 1,
			.shmem_peers = 2,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_UNDEFINED,
		},
		/* 00:01.0 (networking) */ {
			.type = JAILHOUSE_PCI_TYPE_IVSHMEM,
			.domain = 0,
			.bdf = 1 << 3,
			.bar_mask = JAILHOUSE_IVSHMEM_BAR_MASK_INTX,
			.shmem_regions_start = 4,
			.shmem_dev_id = 1,
			.shmem_peers = 2,
			.shmem_protocol = JAILHOUSE_SHMEM_PROTO_VETH,
		},
	},
};
