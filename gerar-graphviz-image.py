import pydot
import sys
import os

input_file = sys.argv[1]

file_name = input_file.split('/')[-1].split('.')[0]

teste_name = sys.argv[2]

graph, = pydot.graph_from_dot_data(open(input_file).read())

if not os.path.exists("./out/" + teste_name + "/images"):
    os.makedirs("./out/" + teste_name + "/images")

graph.write_png("./out/" + teste_name + "/images/" + file_name + '.png')