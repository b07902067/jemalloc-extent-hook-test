
import matplotlib.pyplot as plt

'''
VmPeak:	   48036 kB
VmSize:	   48000 kB
VmHWM:	   11216 kB
VmRSS:	   11216 kB
VmPTE:	      68 kB
'''

def parse_test_output(fileName):
    cycle = 100
    peak = []
    size = []
    hwm = []
    rss = []
    pte = []
    cycles = []
    with open(fileName, "r") as f:
        for line in f.readlines():
            # print(line)
            line = line.split() 
            if len(line) == 0:
                continue
            if line[0][0] != "V":
                cycles.append(int(line[0].strip()))
                continue
            elif line[0] == "VmPeak:":
                peak.append(int(line[1]))
            elif line[0] == "VmSize:":
                size.append(int(line[1]))
            elif line[0] == "VmHWM:":
                hwm.append(int(line[1]))
            elif line[0] == "VmRSS:":
                rss.append(int(line[1]))
            elif line[0] == "VmPTE:":
                pte.append(int(line[1]))
    # print(peak)
    return peak, size, hwm, rss, pte, cycles

def mmap_to_vm(file):
    peak, size, hwm, rss, pte, cycles = parse_test_output("./mmap_test_out")
    print("cycle : ", cycles)
    plt.figure(figsize=(14, 14), dpi=100)

    # plt.subplot(2,2,1)
    plt.xlabel('mmap size(MB)', fontsize=20)
    plt.ylabel('VmPeak(KB)', fontsize=20)
    plt.tick_params(labelsize=23)
    plt.plot(cycles, peak)

    # plt.subplot(2,2,2)
    # plt.xlabel('mmap size(MB)')
    # plt.ylabel('VmSize(KB)')
    # plt.plot(cycles, size)

    # plt.subplot(2,2,3)
    # plt.xlabel('mmap size(MB)')
    # plt.ylabel('VmHWM(KB)')
    # plt.plot(cycles, hwm)

    # plt.subplot(2,2,4)
    # plt.xlabel('mmap size(MB)')
    # plt.ylabel('VmRSS(KB)')
    # plt.plot(cycles, rss)

    plt.savefig(file)
    plt.show()

def mallocx_to_vm(file):
    peak, size, hwm, rss, pte, cycles = parse_test_output(file)
    plt.figure(figsize=(14, 14), dpi=100)

    # plt.subplot(2,2,1)
    plt.xlabel('malloc request size(MB)', fontsize=20)
    plt.ylabel('VmPeak(KB)', fontsize=20)
    plt.tick_params(labelsize=23)
    plt.plot(cycles, peak)

    # plt.subplot(2,2,2)
    # plt.xlabel('malloc request size(MB)')
    # plt.ylabel('VmSize(KB)')
    # plt.plot(cycles, size)

    # plt.subplot(2,2,3)
    # plt.xlabel('malloc request size(MB)')
    # plt.ylabel('VmHWM(KB)')
    # plt.plot(cycles, hwm)

    # plt.subplot(2,2,4)
    # plt.xlabel('malloc request size(MB)')
    # plt.ylabel('VmRSS(KB)')
    # plt.plot(cycles, rss)

    if "no_mmap" in file:
        plt.savefig('mallocx_to_vm_no_mmap.png')
    else:
        plt.savefig('mallocx_to_vm.png')
    plt.show()
            

if __name__ == "__main__":
    mmap_to_vm('mmap_to_vm.png')
    mallocx_to_vm("./malloc_test_out")
    mallocx_to_vm("./malloc_test_no_mmap_out")
    