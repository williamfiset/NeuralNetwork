#!/usr/bin/env python
'''
William Fiset

'''

import sys, random, re
from pprint import pprint as pp

reverse_mapping  = lambda _tuple: list(reversed(_tuple))
strip_whitespace = lambda s: s.strip()

def outputData(col_names, rows, input_cutoff):
    
    num_rows = len(rows)
    num_cols = len(rows[0])
    
    print num_rows, num_cols, input_cutoff
    print(' '.join(col_names))
    
    for row in rows:
        str_row = ""
        for elem in row:
            str_row += str(elem) + " "
        print(str_row)

def jackknife( filename, num_jackknife_sets, col_names, rows, input_cutoff ):

    # randomize rows instead of doing a round robin like procedure
    random.shuffle(rows)

    col_names_str = ' '.join(col_names) + "\n"

    num_rows = len(rows)
    num_cols = len(col_names)
    step = num_rows // num_jackknife_sets
    if step == 0: step = 1

    # create the files
    test_files = []
    training_files = []
    for i in xrange(num_jackknife_sets):

        test_file = open(filename + "." + str(i) + ".test", "w+")
        training_file = open(filename + "." + str(i) + ".train", "w+")
        
        test_file.write( str(step) +" "+ str(num_cols) +" "+ str(input_cutoff) + "\n" )
        training_file.write( str(num_rows-step) +" "+ str(num_cols) +" "+ str(input_cutoff) + "\n" )

        test_file.write(col_names_str)
        training_file.write(col_names_str)

        test_files.append(test_file)
        training_files.append(training_file)

    # create interval bounds
    bounds = [ ]
    lo, hi = 0, step - 1
    for i in xrange(0, num_rows, step ):
        bounds.append( (lo, hi) )
        lo += step
        hi += step

    # Create jackknife files
    for row_number, row in enumerate(rows):
        
        row_str = ' '.join(map(str,row))
        for i in xrange(num_jackknife_sets):
            
            lo, hi = bounds[i]
            
            # test row in ith file
            if lo <= row_number <= hi:
                test_file = test_files[i]
                test_file.write(row_str + "\n")
            
            # training row in ith file
            else:
                train_file = training_files[i]
                train_file.write(row_str + "\n")


def isNominal(s):
    try:
        x = float(s)
        return False
    except:
        return True
    
def getColumnData(column):
    
    if len(column) == 0:return column
    first_element = column[0]

    if isNominal(first_element):
        
        output = []
        
        names = set(column)
        num_new_cols = len(names)
        mapping = dict(list(enumerate( names )))
        column_name_map = dict(map(reverse_mapping, list(enumerate( names ))))

        for col_item in column:
            
            index = column_name_map[col_item]
            
            values = [0.0 for _ in xrange(num_new_cols)]
            values[index] = 1.0
            
            output.append(' '.join(map(str,values)))

        new_col_names = []
        for i in xrange(num_new_cols):
            new_col_names.append(mapping[i])

        return new_col_names, output
        
    else:

        column = list(map(float, column))
        max_value = max(column)
        min_value = min(column)
        _range = max_value - min_value

        # Normalize all the values to be in a range of [0, 1]
        for i in xrange(len(column)):
            if _range != 0:
                if min_value < 0:
                    column[i] = (column[i] + abs(min_value)) / _range
                else:
                    column[i] = (column[i] - min_value) / _range

        return None, column

def process(filename, num_jackknife_sets, input_cutoff):
    
    with open(filename, 'r') as file_handle:
        
        lineCount = 0
        labels = None
        
        column_data = []
        
        for line in file_handle:
            
            # Strip the newline character
            line = line.rstrip()

            # Splits lines, trims each string and removes empty lines
            line_split = list(filter(None, map(strip_whitespace, re.split("[, ]+", line))))

            # The first line contains labels
            if lineCount == 0:
                labels = line_split
                for i in range(len(labels)):
                    column_data.append([])

            # Data on the other lines
            else:
                for i in range(len(line_split)):
                    column_element = line_split[i]
                    column_data[i].append(column_element)
            
            lineCount += 1
        
        normalized_columns = []
        col_names = []

        for i, column in enumerate(column_data):
            
            new_col_names, cols = getColumnData(column)
            normalized_columns.append(cols)
            
            if new_col_names == None:
                col_names.append(labels[i])
            else:
                col_names.extend(new_col_names)

        # merge columns. When there is nominal data we need more dimensions and hence more columns.
        if len(normalized_columns) > 0:
            
            rows = []
            col_length = len(normalized_columns[0])
            row_length = len(normalized_columns)

            for i in xrange(col_length):
                
                row = []
                for j in xrange(row_length):
                    
                    elem = normalized_columns[j][i]

                    # handle nominal data
                    if type(elem) is str:
                        reals = list(map(float, elem.split(' ')))
                        for e in reals:
                            row.append(e)
                    else:
                        row.append(elem)

                rows.append(row)

            # outputData(col_names, rows, input_cutoff)
            jackknife(filename, num_jackknife_sets, col_names, rows, input_cutoff )

if __name__ == '__main__':
    
    if len(sys.argv) == 4:
        
        input_cutoff = None
        num_jackknife_sets = None
        filename = None

        try:

            filename = sys.argv[1]
            num_jackknife_sets = int(sys.argv[2])
            input_cutoff = int(sys.argv[3])

            if num_jackknife_sets <= 0:
                print("Please supply a positive number for the number of jack-knife sets")

        except Exception as e:
            raise e
        
        process(filename, num_jackknife_sets, input_cutoff)
        
    else:
        print("\nPlease supply the correct number of arguments")
        print("'python GenerateJackknifeSets.py inputFile num_jackknife_sets input_cutoff'\n")

