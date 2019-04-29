import pdb
import datetime
import subprocess
import os
import sys
import numpy as np
import multiprocessing
import concurrent.futures
ITER = 10
test_names = []
benchmark = []


def multithreading(func, args,
                   workers):
    with concurrent.futures.ThreadPoolExecutor(workers) as ex:
        res = ex.map(func, args)
    res=list(res)
    return res


def multiprocessing(func, args,
                    workers):
    with concurrent.futures.ProcessPoolExecutor(workers) as ex:
        res = ex.map(func, args)
    return list(res)


def runminion(n):
    subprocess.check_call(['./minion', test_names[n] + '.minion', '-noprintsols', '-findallsols', '-tableout', test_names[n] + ".xls", '-nodelimit', '10000000'])
    return 1


def parproc(n):
    poolstart = multithreading(runminion, [n]*ITER, 8)
    x =summary(n)
    return x


def setbench(n, val):
    benchmark[n] = val


def summary(n):
    f = open(test_names[n] + '.xls', 'r')
    data = f.readlines()
    f.close()
    for i in range(len(data)):
        data[i] = data[i].replace(' \n', '').split(' ')
    data = np.array(data)
    data = data[1:-1, [4, 12, 13]].astype('double')
    print('\n' + str(test_names[n]) + '\nnodes\t\tsolve time')
    print(str(data[0, 0]) + '\t\t ' + str(np.mean(data[:, 2])))
    bench = str(np.mean(data[:, 2]))
    if os.path.isfile(test_names[n] + ".txt"):
        f = open(test_names[n] + ".txt", 'a')
    else:
        f = open(test_names[n] + ".txt", 'w+')
        f.write("timestamp\t\t\t\tnodes\t\t\tsolutions found\t\tsolve time\n")
    f.write(datetime.datetime.now().isoformat() + '\t\t' + str(data[0, 0]) + '\t\t ' + str(data[0, 1]) + '\t\t ' + str(np.mean(data[:, 2])) + '\n')
    f.close()
    return bench


def get_benchmark():
    categorized_benchmark = {}
    for i in range(len(test_names)):
        bcat = test_names[i].split('/')[0:-1]
        bcat = '/'.join(bcat)
        if bcat in categorized_benchmark.keys():
            categorized_benchmark[bcat].append(i)
        else:
            categorized_benchmark[bcat] = [i]
    for cat in categorized_benchmark.keys():
        print(cat)
        if os.path.isfile(cat+"_benchmarks.txt"):
            f = open(cat+"_benchmarks.txt", 'a')
        else:
            f = open(cat+"_benchmarks.txt", 'w+')
            s = ''
            for i in categorized_benchmark[cat]:
                s = s + test_names[i].split('/')[-1] + ' ' * max(0, (16-len(test_names[i].split('/')[-1]))) + '\t\t'
            f.write(s+'\n')
        s = ''
        for i in categorized_benchmark[cat]:
            s = s + benchmark[i] + ' ' * max(0, (16-len(benchmark[i]))) + '\t\t'
        f.write(s+'\n')
    return categorized_benchmark


def clear_results(test_names, bm):
    for i in test_names:
        try:
            subprocess.check_call(['rm', i + '.xls'])
        except:
            print('xls missing')
            continue
        try:
            subprocess.check_call(['rm', i + '.txt'])
        except:
            print('txt missing')
            continue
    for i in bm.keys():
        subprocess.check_call(['rm', './'+i+'_benchmarks.txt'])


def test_name_add(subfolder):
    test = subprocess.check_output(['ls', subfolder])
    test = test.split('\n')
    for i in test:
        if '.minion' in i:
            try:
                test_names.append(subfolder + '/' + i.replace('.minion', ''))
                subprocess.check_call(['rm', test_names[-1] + '.xls'])  # , shell=True)
            except:
                continue


if __name__ == '__main__':
    for i in range(10):
        test_names = []
        benchmark = []
        try:
            subdirs = [x[0] for x in os.walk('./testcases/')]
            #subdirs = subdirs[0:]
            for i in subdirs:
                test_name_add(i)
        except:
            subdirs = []
        if len(sys.argv) > 2:
            if sys.argv[2] == 'c':
                clear_results(test_names)
        else:
            benchmark = ['x'] * len(test_names)
            benchmark = multiprocessing(parproc, range(len(test_names)), min(len(test_names), 4))
            bm = get_benchmark()
     
    print('done')
