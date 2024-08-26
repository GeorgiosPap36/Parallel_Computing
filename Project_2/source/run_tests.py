import subprocess
import sys

def findTime(str):
    t = str.split("= ")
    return float(t[1])

###EROTIMA 1###
def cmdRun1(file, procs, iters_for_avg):
    print("Printing results for " + file + "(" + str(procs) + " threads)" + ": ")
    averageRuntime = 0
    cmd = ["./" + file, procs]
    if ("mpi" in file):
        cmd = ["mpiexec", "-host", "linux01,linux10,linux11,linux12,linux13,linux14,linux15,linux16,linux17,linux18,linux19,linux20,linux21,linux22,linux22,linux23,linux24", "-np", procs, "./" + file,]
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
    subprocess.call(["gcc", "-o", "monte_carlo_serial", "monte_carlo_serial.c"])
    subprocess.call(["mpicc", "-o", "monte_carlo_mpi", "monte_carlo_mpi.c"])

    iters_for_avg = 4

    T1 = cmdRun1("monte_carlo_serial", str(1), iters_for_avg)
    print("-------------------------------------------")

    proc_list = [2, 4, 8, 16]
    exe_list = ["monte_carlo_mpi"]

    for proc in proc_list:
        Tp = cmdRun1("monte_carlo_mpi", str(proc), iters_for_avg)
        Sp = T1/Tp
        Ep = Sp/proc
        print("-Speed up:", Sp)
        print("-Efficiency:", Ep)
        print("-------------------------------------------")

###EROTIMA 2###
def cmdRun2(file, procs, iters_for_avg):
    print("Printing results for " + file + "(" + str(procs) + " threads)" + ": ")
    averageRuntime = 0
    cmd = ["./" + file, procs]
    if ("mpi" in file):
        cmd = ["mpiexec", "-host", "linux01,linux10,linux11,linux12,linux13,linux14,linux15,linux16,linux17,linux18,linux19,linux20,linux21,linux22,linux22,linux23,linux24", "-np", procs, "./" + file,]
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
    subprocess.call(["gcc", "-o", "mat_vec_serial", "mat_vec_serial.c"])
    subprocess.call(["mpicc", "-o", "mat_vec_mpi", "mat_vec_mpi.c"])

    iters_for_avg = 4

    T1 = cmdRun2("mat_vec_serial", str(1), iters_for_avg)
    print("-------------------------------------------")

    proc_list = [2, 4, 8, 16]

    for proc in proc_list:
        Tp = cmdRun2("mat_vec_mpi", str(proc), iters_for_avg)
        Sp = T1/Tp
        Ep = Sp/proc
        print("-Speed up:", Sp)
        print("-Efficiency:", Ep)
        print("-------------------------------------------")

###EROTIMA 3###
def cmdRun3(f, procs, loops, option, iters_for_avg):
    print("Printing results for " + f + "(" + "counter max: " + loops+ ", option: " + str(option) + ", " + str(procs) + " threads)" + ": ")
    averageRuntime = 0
    cmd = ["./" + f, procs, loops, option]
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

            if ("Shared counter:" in temp):
                #print(temp)
                continue

    averageRuntime /= iters_for_avg

    print("Average runtime:", averageRuntime)
    return averageRuntime

def er3_tests():
    subprocess.call(["gcc", "-o", "counter_sequential", "counter_sequential.c"])
    subprocess.call(["gcc", "-o", "counter_pthreads_locks", "counter_pthreads_locks.c", "-lpthread"])
    subprocess.call(["gcc", "-o", "counter_pthreads_atomics", "counter_pthreads_atomics.c", "-lpthread"])

    iters_for_avg = 4

    proc_list = [1, 2, 4, 8]
    loops_list = [1000000, 10000000, 100000000]
    option_list = [0, 1]
    file_list = ["counter_pthreads_locks", "counter_pthreads_atomics"]

    for loops in loops_list:
        print("---------------[loops: " + str(loops) +"]---------------")
        T1 = cmdRun3("counter_sequential", str(1), str(loops), str(1), iters_for_avg)
        for f in file_list:
            print("---------------[file: " + f +"]---------------")
            for option in option_list:
                print("---------------[option: " + str(option) +"]---------------")
                for proc in proc_list:
                    Tp = cmdRun3(f, str(proc), str(loops), str(option), iters_for_avg)
                    Sp = T1/Tp
                    Ep = Sp/proc
                    print("-Speed up:", Sp)
                    print("-Efficiency:", Ep)
                    print("-------------------------------------------")

###EROTIMA 4###
def cmdRun4(f, rows, cols, threads, blocks, iters_for_avg):
    print("Printing results for " + f + "(" + "threads per block: " + str(threads) + ", blocks: " + str(blocks) + " - (" + str(rows) + ", " + str(cols) + ")" + ")" + ": ")
    averageRuntime = 0
    cmd = [f, rows, cols, threads, blocks]
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

            if ("Shared counter:" in temp):
                #print(temp)
                continue

    averageRuntime /= iters_for_avg

    print("Average runtime:", averageRuntime)
    return averageRuntime

def er4_tests():
    subprocess.call(["nvcc", "-o", "mat_mult", "mat_mat_mult.cu"])
    subprocess.call(["nvcc", "-o", "mat_mult_2", "mat_mat_mult_2.cu"])
    subprocess.call(["gcc", "-o", "mat_mult_serial", "mat_mat_mult_serial.c"])

    iters_for_avg = 4

    threadList = [128, 256, 512]

    rows = 1024
    cols = 1024
    
    file_list = ["mat_mult", "mat_mult_2"]

    print("\n---------------[(" + str(rows) + ", " + str(cols) + ")]---------------")
    T1 = cmdRun4("mat_mult_serial", str(rows), str(cols), str(1), str(1), iters_for_avg)
    print("-------------------------------------------")
    
    # mat_mult
    for threads in threadList:
        Tp = cmdRun4("mat_mult", str(rows), str(cols), str(threads), str(1), iters_for_avg)
        Sp = T1/Tp
        Ep = Sp/(threads * blocks)
        print("-Speed up:", Sp)
        print("-Efficiency:", Ep)
        print("-------------------------------------------")

    threads = [160, 6554, 320, 3277, 1024]
    blocks = [6554, 160, 3277, 320, 1024]

    # mat_mult_2
    for i in range(len(threads)):
        Tp = cmdRun4("mat_mult", str(rows), str(cols), str(threads[i]), str(blocks[i]), iters_for_avg)
        Sp = T1/Tp
        Ep = Sp/(threads[i] * blocks[i])
        print("-Speed up:", Sp)
        print("-Efficiency:", Ep)
        print("-------------------------------------------")





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