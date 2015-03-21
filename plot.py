import re
from itertools import *
from collections import defaultdict
import pylab

class TestData:
    def __init__(self, name, bodies, iterations, threads, times):
        self.name = name
        self.bodies = bodies
        self.iterations = iterations
        self.threads = threads
        self.times = times
    
    def __repr__(self):
        return (str(self.name)
            + " " + str(self.bodies) 
            + " " + str(self.iterations) 
            + " " + str(self.threads))

testdata = defaultdict(list)
with open("measurements") as f:
    while True:
        header = "\n"
        while header == "\n":
            header = f.readline()
        data = "\n"
        while data == "\n":
            data = f.readline()
        if not data and not header:
            break
        header_items = header.split(" ")
        name = re.match("./bin/([\w_]+)", header_items[0]).group(1)
        bodies = int(re.search("\d+", header_items[1]).group(0))
        iterations = int(header_items[2])
        threads = 1
        has_threads = name.find("parallel") > 0
        if has_threads:
            threads = int(header_items[len(header_items) - 1])
        times = [float(x) for x in data.strip().split("\t")]
        testdata[bodies].append(TestData(name, bodies, iterations, threads, times))


for bodies, datas in testdata.iteritems():
    pylab.clf()
    print "Plot"
    for name, g in groupby(sorted(datas, key=lambda d: d.name), lambda d: d.name):
        print "Group ", name
        times = []
        threads = []
        for data in sorted(g, key=lambda data: data.threads):
            times.append(sum(data.times) / len(data.times))
            threads.append(data.threads)
        if len(times) == 1:
            times = times*3
            threads = [1,2,4]
        print times, threads
        pylab.plot(threads, times, label=name)
    pylab.ylabel("Time [s]")
    pylab.xlabel("Threads [N]")
    pylab.xticks([1,2, 3,4])
    pylab.legend(loc="lower left")
    pylab.title(str(bodies) + " bodies")
    pylab.savefig("Plot" + str(bodies))