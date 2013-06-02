"""
create_random_vector.py

Script to create your input file

Usage:
python3 create_random_vector.py [vector length] [destination file]

For example:
python3 create_random_vector.py 1000 v1k.txt
...will create a file called v1k.txt containing 1000 random integers
between 0 and 1000000.
"""
import random
import sys


def read_input_args(argv):
    """
    Inputs: sys.argv
    Quits unless it was given exactly 2 arguments:
        a positive integer and a string (filename)
    Returns (int, str)
    """
    usage_str = "\nUsage:\npython3 create_random_vector.py [vector length]\
[destination file]"
    if len(argv) != 3:
        sys.exit(usage_str)
    try:
        vector_length = int(argv[1])
        if vector_length < 0:
            sys.exit(usage_str)
    except ValueError:
        sys.exit(usage_str)
    return(vector_length, argv[2])


def main():
    (vector_len, filename) = read_input_args(sys.argv)
    try:
        outfile = open(filename, "wt")
    except IOError:
        sys.exit("Could not open file " + filename)
    for i in range(vector_len):
        outfile.write(str(random.randrange(0, 1000001))+"\n")
    outfile.close()


main()
