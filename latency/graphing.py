import matplotlib.pyplot as plt
import sys

def main():

    if len(sys.argv) < 2:
        return

    f = open(sys.argv[1], "r")
    if f.closed:
        return
    
    i = 0
    times = []
    x_axis = []
    for line in f:
        times.append(float(line))
        x_axis.append(i)
        i = i + 1

    f.close()
    
    sum = 0
    for val in times:
        sum = sum + val

    print("Average: ", sum / (i + 1))
    
    plt.plot(x_axis, times)
    plt.title("Time took vs Test Number")
    plt.xlabel("Test Number")
    plt.ylabel("Time Took (s)")
    plt.show()
    return

if __name__ == "__main__":
    main()
