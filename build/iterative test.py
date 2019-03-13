import pdb
import datetime
import subprocess
import numpy as np
import multiprocessing
import concurrent.futures
ITER = 10
test_names = []


def multithreading(func, args,
                   workers):
    with concurrent.futures.ThreadPoolExecutor(workers) as ex:
        res = ex.map(func, args)
    return list(res)


def multiprocessing(func, args,
                    workers):
    with concurrent.futures.ProcessPoolExecutor(workers) as ex:
        res = ex.map(func, args)
    return list(res)


def runminion(n):
    subprocess.check_call(['./minion', test_names[n] + '.minion', '-noprintsols', '-findallsols', '-tableout', test_names[n] + ".xls"])
    return 1


def parproc(n):
    poolstart = multithreading(runminion, [n]*ITER, 4)
    summary(n)
    return 1


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
    try:
        f=open(test_names[n] + ".txt", 'a')
    except:
        f=open(test_names[n] + ".txt", 'w+')
        f.write('timestamp\t\tnodes\t\tsolve time')
    f.write(datetime.datetime.now().isoformat() + '\t\t' + str(data[0, 0]) + '\t\t ' + str(np.mean(data[:, 2])) + '\n')


if __name__ == '__main__':
    test = subprocess.check_output(['ls', 'testcases'])
    test = test.split('\n')
    for i in test:
        if '.minion' in i:
            try:
                test_names.append('./testcases/' + i.replace('.minion', ''))
                subprocess.check_call(['rm', test_names[-1] + '.xls'])  # , shell=True)
            except:
                continue
    pool = multiprocessing(parproc, range(len(test_names)), len(test_names))
print('done')