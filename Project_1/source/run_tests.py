import subprocess
import sys

def findTime(str):
    t = str.split("= ")
    return float(t[1])

###EROTIMA 1###
def cmdRun(file, n, procs, iters_for_avg):
    print("Printing results for " + file + "(n: " + n + "), "+ "(" + str(procs) + " threads)" + ": ")
    averageRuntime = 0
    cmd = ["./" + file, n, procs]
    for _ in range(0, iters_for_avg):
        p = subprocess.Popen(cmd,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT)
        for line in iter(p.stdout.readline, b''):
            #print ('>>> {}'.format(line.rstrip()))
            temp = line.decode("utf-8")
            if ("= " in temp):
                #print(temp)
                averageRuntime += findTime(temp)

    averageRuntime /= iters_for_avg

    print("Average runtime:", averageRuntime)
    return averageRuntime

def er1_tests():
    subprocess.call(["gcc", "-o", "er1_serial", "er1_serial.c"])
    subprocess.call(["gcc", "-o", "er1_pthreads", "er1_pthreads.c", "-lpthread"])
    subprocess.call(["gcc", "-fopenmp", "-o", "er1_openmp", "er1_openmp.c"])

    iters_for_avg = 4
    n = 10**5

    T1 = cmdRun("er1_serial", str(n), str(1), iters_for_avg)
    print("-------------------------------------------")

    proc_list = [2, 4, 8]
    exe_list = ["er1_pthreads", "er1_openmp"]

    for i in proc_list:
        for name in exe_list:
            Tp = cmdRun(name, str(n), str(i), iters_for_avg)
            Sp = T1/Tp
            Ep = Sp/i
            print("-Speed up:", Sp)
            print("-Efficiency:", Ep)
            print("-------------------------------------------")

###EROTIMA 2###
def cmdRun2(file, m, n, procs, iters_for_avg):
    print("Printing results for " + file + "(" + m + "x" + n + "), "+ "(" + str(procs) + " threads)" + ": ")
    averageRuntime = 0
    cmd = ["./" + file, procs, m, n]
    for _ in range(0, iters_for_avg):
        p = subprocess.Popen(cmd,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT)
        for line in iter(p.stdout.readline, b''):
            #print ('>>> {}'.format(line.rstrip()))
            temp = line.decode("utf-8")
            if ("= " in temp):
                #print(temp)
                averageRuntime += findTime(temp)

    averageRuntime /= iters_for_avg

    print("Average runtime:", averageRuntime)
    return averageRuntime

def er2_tests():
    subprocess.call(["gcc", "-o", "pth_mat_vect", "pth_mat_vect_rand_split.c", "-lpthread"])
    subprocess.call(["gcc", "-o", "er2_1", "er2_1.c", "-lpthread"])
    subprocess.call(["gcc", "-o", "er2_2", "er2_2.c", "-lpthread"])

    iters_for_avg = 4
    m = [8000000, 8000, 8]
    n = [8, 8000, 8000000]

    proc_list = [1, 2, 4, 8]
    exe_list = ["pth_mat_vect", "er2_1", "er2_2"]

    for j in range(0, len(m)):
        for name in exe_list:
            for i in proc_list:
                if (i == 1):
                    T1 = cmdRun2(name, str(m[j]), str(n[j]), str(i), iters_for_avg)
                else:
                    Tp = cmdRun2(name, str(m[j]), str(n[j]), str(i), iters_for_avg)
                    Sp = T1/Tp
                    Ep = Sp/i
                    print("-Speed up:", Sp)
                    print("-Efficiency:", Ep)
                print("-------------          -------------")
            print("-------------------------------------------")
        print("///////////////////////////////////////")

###EROTIMA 3###
def cmdRun3(file, m, n, procs, iters_for_avg):
    print("Printing results for " + file + "(" + m + "x" + n + "), "+ "(" + str(procs) + " threads)" + ": ")
    averageRuntime = 0
    cmd = ["./" + file, procs, m, n]
    for _ in range(0, iters_for_avg):
        p = subprocess.Popen(cmd,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT)
        for line in iter(p.stdout.readline, b''):
            #print ('>>> {}'.format(line.rstrip()))
            temp = line.decode("utf-8")
            if ("= " in temp):
                #print(temp)
                averageRuntime += findTime(temp)

    averageRuntime /= iters_for_avg

    print("Average runtime:", averageRuntime)
    return averageRuntime

def er3_tests():
    subprocess.call(["gcc", "-fopenmp", "-o", "omp_mat_vect", "omp_mat_vect_rand_split.c"])
    subprocess.call(["gcc", "-fopenmp", "-o", "er3_static", "er3_static.c"])
    subprocess.call(["gcc", "-fopenmp", "-o", "er3_guided", "er3_guided.c"])

    iters_for_avg = 4
    m = [8000000, 8000, 8]
    n = [8, 8000, 8000000]

    proc_list = [1, 2, 4, 8]
    exe_list = ["omp_mat_vect", "er3_static", "er3_guided"]

    for j in range(0, len(m)):
        for name in exe_list:
            for i in proc_list:
                if (i == 1):
                    T1 = cmdRun2(name, str(m[j]), str(n[j]), str(i), iters_for_avg)
                else:
                    Tp = cmdRun2(name, str(m[j]), str(n[j]), str(i), iters_for_avg)
                    Sp = T1/Tp
                    Ep = Sp/i
                    print("-Speed up:", Sp)
                    print("-Efficiency:", Ep)
                print("-------------          -------------")
            print("-------------------------------------------")
        print("///////////////////////////////////////")

###EROTIMA 4###
def cmdRun4(file, n, procs, iters_for_avg):
    print("Printing results for " + file + "(" + n + "x" + n + "), "+ "(" + str(procs) + " threads)" + ": ")
    averageRuntime = 0
    cmd = ["./" + file, procs, n]
    for _ in range(0, iters_for_avg):
        p = subprocess.Popen(cmd,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT)
        for line in iter(p.stdout.readline, b''):
            #print ('>>> {}'.format(line.rstrip()))
            temp = line.decode("utf-8")
            if ("= " in temp):
                #print(temp)
                averageRuntime += findTime(temp)

    averageRuntime /= iters_for_avg

    print("Average runtime:", averageRuntime)
    return averageRuntime

def er4_tests():
    subprocess.call(["gcc", "-fopenmp", "-o", "er4", "er4.c"])

    iters_for_avg = 4
    n = [4000, 8000]

    proc_list = [1, 2, 4, 8]
    exe_list = ["er4"]

    for j in range(0, len(n)):
        for name in exe_list:
            for i in proc_list:
                if (i == 1):
                    T1 = cmdRun4(name, str(n[j]), str(i), iters_for_avg)
                else:
                    Tp = cmdRun4(name, str(n[j]), str(i), iters_for_avg)
                    Sp = T1/Tp
                    Ep = Sp/i
                    print("-Speed up:", Sp)
                    print("-Efficiency:", Ep)
                print("-------------------------------------------")
        print("///////////////////////////////////////")

###RUN TESTS###
test_number = int(sys.argv[1])
if (test_number == 1):
    er1_tests()
elif (test_number == 2):
    er2_tests()
elif (test_number == 3):
    er3_tests()
elif (test_number == 4):
    er4_tests()
else:
    print("\n\nRunning test 1:")
    er1_tests()
    print("\n\nRunning test 2:")
    er2_tests()
    print("\n\nRunning test 3:")
    er3_tests()
    print("\n\nRunning test 4:")
    er4_tests()