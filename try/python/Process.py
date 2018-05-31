import multiprocessing
import os
import subprocess

def run_proc(name):
    print("PID: %s" % os.getpid())

print("PID: %s" % os.getpid())
p = multiprocessing.Process(target=run_proc, args=('test',))
p.start()
p.join()

p = subprocess.Popen(['cat'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
p.wait()


print(p.stdout.read())


