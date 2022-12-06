from prettytable import PrettyTable
import os 

try:
    for filename in os.listdir('solutions_executionTime'):
        f = open("solutions_executionTime/"+filename,'r')
        line = f.readline()
        x = PrettyTable()
        titleTable = filename[0 : len(filename) - 4]
        x.title = titleTable
        x.field_names = ["n","Solution","Effort","Execution time"]

        while line:
            values = line.strip("\n").split("\t")
            while("" in values):
                values.remove("")
            
            x.add_row([values[0],values[1],values[2],values[3]])
            line = f.readline()

        print(x)
        print("\n")

finally:
    f.close()
