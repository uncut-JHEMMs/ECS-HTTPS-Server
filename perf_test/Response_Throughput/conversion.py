import sys

def main():
    if len(sys.argv) < 2:
        print("No value passed in. Exitting")
        return 1
    
    try:
        BPS = float(sys.argv[1])
    except ValueError:
        print("Non-Integer/Float value read. Exitting.")
        return -1
    
    MB = pow(2, 20)
    print("\n\n" + str(BPS / MB) + " Mbps")
    
if __name__ == "__main__":
    main()

