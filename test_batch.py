import os
import sys

TEST_ROUNDS = 10

def test(rounds, para):
    total = 0.0
    for i in range(rounds):
        result = os.popen("./main --" + para).read().strip().split()
        print result[0] + ' ' + result[1]
        total += float(result[0])
    average = total / rounds;
    print "Average : " + str(average)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        para = 'test'
    else:
        para = sys.argv[1]
    if para != 'test' and para != 'train':
        print 'invalid option(using "test" or "train")'
    else:
        print 'Testing using ' + para + 'file for ' + str(TEST_ROUNDS) + ' times'
        test(TEST_ROUNDS, para)
