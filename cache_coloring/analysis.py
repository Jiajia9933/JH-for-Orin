import matplotlib.pyplot as plt
import os
import numpy as np

types = ['none', 'read', 'write', 'modify', 'prefetch_l3']

	
def plot_array(data, label=None):
    sizes = []
    for i in range (8, 64, 8):
        sizes.append((i))
    for i in range (64, 512, 32):
        sizes.append((i))
    for i in range(512, 8192 + 512, 512):
        sizes.append((i))

    plt.semilogx( sizes, data[1 : ], label=label, marker='x', linestyle='solid')
	
    if label:
	    plt.legend()
		

def per_vision_data(folder, dataset):
    sizes = []
    for i in range (8, 64, 8):
        sizes.append((i))
    for i in range (64, 512, 32):
        sizes.append((i))
    for i in range(512, 8192 + 512, 512):
        sizes.append((i))
    # Initialize dictionary
    disparity = {}
    mser = {}
    tracking = {}
    types = ['none', 'read', 'write', 'modify', 'prefetch_l3']
	
    for t in types:
        disparity[t] = []
        mser[t] = []
        tracking[t] = []

        for s in sizes:
            if t == 'none' and s > 8:
                continue
            filename = folder + '/' + t + '_' + str(s) + '.txt'
            collect_data = 0
            with open(filename, 'r') as file:
                while True:
                    line1 = file.readline().strip()
                    #if not line1:  # 检查文件结束
                        #print("t= ",t, "s= ",s, "file= ",filename)
                        #break
                    line2 = file.readline().strip()

                    if collect_data == 1:
                        if line1 == 'disparity':
                            disparity[t].append(float(line2))
                        elif line1 == 'mser':
                            mser[t].append(float(line2))
                        elif line1 == 'tracking':
                            tracking[t].append(float(line2))

                    
                    if collect_data == 1 and (not line1 or not line2):
                        break
                    if collect_data == 1 and (line1[0] == '-' or line2[0] == '-'):
                        break
                    if line2 == dataset:
                       line1 = line2
                       line2 = file.readline().strip()
                       collect_data = 1
                    if line1 == dataset:
                        collect_data = 1

    for t in types[1: ]:
        disparity[t].insert(0, disparity['none'][0])
        mser[t].insert(0, disparity['none'][0])
        tracking[t].insert(0, tracking['none'][0])
    
    return disparity, mser, tracking

	
dataset = {}
dataset['cif'] = '--------------------------1--------------------------------'
dataset['vga'] = '--------------------------2--------------------------------'

data_types = ['cif', 'vga']
bench_types = ['read', 'write', 'modify', 'prefetch_l3']

folder = {}
folder['none'] = '/home/jiajia/benchOrin_none'
folder['1by3'] = '/home/jiajia/benchOrin_1by3'
folder['2by2'] = '/home/jiajia/benchOrin_2by2'
folder['3by1'] = '/home/jiajia/benchOrin_3by1'


def cosmetics_percent(name):
	savename = "orin_agx_set_" + name + '_' + t+ '_' + d
	plt.axvline(64, label='L1 Cache')
	plt.axvline(256,  label='L2 Cache')
	plt.axvline(2048,  label='L3 Cache')
	plt.axvline(4096,  label='L4 Cache')
	plt.title(savename)
	plt.xlabel('Size of Interference (KiB)')
	plt.ylabel('Execution time / Execution Time with no Interference')
	plt.savefig(os.path.join(d, savename), format="eps")
	plt.close()
	
def cosmetics(name):
	savename = name + '_' + t+ '_' + d
	plt.axvline(64, label='L1 Cache')
	plt.axvline(256,  label='L2 Cache')
	plt.axvline(2048,  label='L3 Cache')
	plt.axvline(4096,  label='L4 Cache')
	plt.title(savename)
	plt.xlabel('Size of Interference (KiB)')
	plt.ylabel('Execution time')
	plt.savefig(os.path.join(d, savename))
	plt.close()

def percent_change(arr):
	if arr[0] > arr[1]:
		arr[0] = arr[1]
	for i in range(1, len(arr)):
		arr[i] = (arr[i]) / arr[0]
	arr[0] = 1
	return arr

for d in data_types:
    os.makedirs(d, exist_ok=True)

    for t in bench_types:
        for key, values in folder.items():
            disparity, mser, tracking = per_vision_data(values, dataset[d])
            plot_array(disparity[t], key)
        cosmetics('disparity')

        for key, values in folder.items():
            disparity, mser, tracking = per_vision_data(values, dataset[d])
            plot_array(mser[t], key)
        cosmetics('mser')

        for key, values in folder.items():
            disparity, mser, tracking = per_vision_data(values, dataset[d])
            plot_array(tracking[t], key)
        cosmetics('tracking')


for d in data_types:
    os.makedirs(d, exist_ok=True)

    for t in bench_types:
        maxi = 1
        for key, values in folder.items():
            disparity, mser, tracking = per_vision_data(values, dataset[d])
            disparity[t] = percent_change(disparity[t])
            plot_array(disparity[t], key)
            maxi = max(np.max(disparity[t]), maxi)
        plt.ylim(0.99, maxi)
        cosmetics_percent('_percent_disparity')

        maxi = 1
        for key, values in folder.items():
            disparity, mser, tracking = per_vision_data(values, dataset[d])
            mser[t] = percent_change(mser[t])
            plot_array(mser[t], key)
            maxi = max(np.max(mser[t]), maxi)
        plt.ylim(0.99, maxi)
        cosmetics_percent('_percent_mser')

        maxi = 1
        for key, values in folder.items():
            disparity, mser, tracking = per_vision_data(values, dataset[d])
            tracking[t] = percent_change(tracking[t])
            plot_array(tracking[t], key)
            maxi = max(np.max(tracking[t]), maxi)
        plt.ylim(0.99, maxi)
        cosmetics_percent('_percent_tracking')
