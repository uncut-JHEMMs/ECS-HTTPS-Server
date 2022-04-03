import sys
import matplotlib.pyplot as plt

def main():

    if(len(sys.argv) < 2):
        print("Input file not provided")

    ifile = open(sys.argv[1], "r")
    if ifile.closed:
        print("File failed to open.")
        return

    file_data = ""
    
    for line in ifile:
        file_data += line

    file_data = file_data[file_data.find("avg-cpu"):len(file_data)]

    split_string = file_data.split()
    percentages = []
    for i in range(0, file_data.count("avg-cpu")):
        percentages.append(split_string[12 * (i + 1) + i])

    plt.plot(len(percentages))
    plt.ylabel("CPU utilization (%)")
    plt.xlabel("Seconds(s)")
    plt.show()
    return

if __name__ == "__main__":
    main()
