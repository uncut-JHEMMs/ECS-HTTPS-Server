import sys
import math
import matplotlib.pyplot as plt

def main():
    if len(sys.argv) < 2:
        return -1

    ifile = open(sys.argv[1], "r")
    if ifile.closed:
        return -1

    totalSwapMemAvail = 0
    swapMemUsed = 0
    swapFree = 0
    
    for line in ifile:
        strings = line.split()
        totalSwapMemAvail = int(strings[0])
        swapMemUsed += int(strings[1])
        swapFree += int(strings[2])
    averageUsed = swapMemUsed / 3
    averageFree = swapFree / 3

    ifile.close()

    print("Swap memory statistics over 3 seconds.")
    print("Total Swap Mem Available: " + convert_size(totalSwapMemAvail))
    print("Average Swap Mem Used: " + convert_size(averageUsed))
    print("Average Free Swap Mem: " + convert_size(averageFree))

    
#Gotten from https://stackoverflow.com/questions/5194057/better-way-to-convert-file-sizes-in-python
def convert_size(size_bytes):
    if size_bytes == 0:
        return "0B"
    size_name = ("B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB")
    i = int(math.floor(math.log(size_bytes, 1024)))
    p = math.pow(1024, i)
    s = round(size_bytes / p, 2)

    return "%s %s" % (s, size_name[i])

if __name__ == "__main__":
    main()
