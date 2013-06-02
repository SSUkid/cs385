"""
check_correctness.py

Script to check your output file

Usage:
python3 check_correctness.py [input file] [output file]

For example:
python3 check_correctness.py in.txt out.txt
will print "CORRECT" if the following hold true:
a) in.txt and out.txt are both lists of integers
b) both lists are the same length
c) every number in out.txt is 1 more than the corresponding
   number in in.txt

Otherwise, the program will print one of the following error messages:
a) Could not open file _________
b) File ________ contains a non-integer value
c) Input list and output list are not the same length
d) Output list is incorrect (first mismatch: ____)
"""
import sys


def read_input_args(argv):
    """
    Inputs: sys.argv
    Quits unless it was given exactly 2 arguments (filenames)
    Returns (str, str)
    """
    usage_str = "\nUsage:\npython3 check_correctness.py [infile] [outfile]\n"
    if len(argv) != 3:
        sys.exit(usage_str)
    return(argv[1], argv[2])


def open_file_or_die(filename):
    """
    Input: filename
    Opens file and returns file variable...
    ...or exits with error
    """
    try:
        return(open(filename, "rt"))
    except IOError:
        sys.exit("Could not open file " + filename)


def read_vector(filevar, filename):
    """
    Input: the file variable and the filename (for error reporting)
    Returns a list of the integers in the file
    ...or dies if the file contained data other than integers
    """
    try:
        return [int(x) for x in filevar.read().split()]
    except ValueError:
        sys.exit("Found non-numeric data in file " + filename)


def main():
    (filename_in, filename_out) = read_input_args(sys.argv)
    in_vector = read_vector(open_file_or_die(filename_in), filename_in)
    out_vector = read_vector(open_file_or_die(filename_out), filename_out)
    if len(in_vector) != len(out_vector):
        sys.exit("Input list and output list are not the same length.")
    for i in range(len(in_vector)):
        if out_vector[i] != in_vector[i] + 1:
            sys.exit("Output list is incorrect. First error: line " +
                     str(i+1))
    print("CORRECT")


main()
