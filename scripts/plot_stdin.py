import sys
import matplotlib.pyplot as plt

def main():
    numbers = []
    for line in sys.stdin:
        try:
            number = float(line.strip())
            numbers.append(number)
        except ValueError:
            print(f"Invalid number: {line.strip()}", file=sys.stderr)
            continue

    if not numbers:
        print("No valid numbers provided.", file=sys.stderr)
        return

    plt.plot(numbers)
    plt.xlabel('Index')
    plt.ylabel('Value')
    plt.title('Plot of Input Numbers')
    plt.show()

if __name__ == "__main__":
    main()