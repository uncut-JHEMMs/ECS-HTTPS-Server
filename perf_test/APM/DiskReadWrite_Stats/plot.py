import sys
import matplotlib.pyplot as plt

def main():

    if(len(sys.argv) < 2):
        print("No input file provided.")

    totalWrite = []
    totalRead = []
    currentRead = []
    currentWrite = []

    ifile = open(sys.argv[1], "r")

    alt = 0
    count = 0
    for line in ifile:
        #If total data read
        if alt == 0:
            string = line.split()
            totalRead.append(string[0])
            totalWrite.append(string[1])
            alt = 1
        else:
            count += 1
            string = line.split()
            currentRead.append(string[0])
            currentWrite.append(string[1])
            alt = 0
    ifile.close

    plt.plot(currentRead, label = "Current Read")
    plt.plot(currentWrite, label = "Current Write")
    plt.legend()
    plt.show()

    plt.plot(totalRead, label = "Total Read")
    plt.plot(totalWrite, label = "Total write")
    plt.legend()
    plt.show()
    
    return

if __name__ == "__main__":
    main()
