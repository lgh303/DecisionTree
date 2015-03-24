import os

def test(rounds):
    total = 0.0
    for i in range(rounds):
        result = os.popen("./main").read().strip()
        print result
        total += float(result)
    average = total / rounds;
    print "Average : " + str(average)

if __name__ == '__main__':
    test(5)
