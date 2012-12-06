'''
Created on Dec 3, 2012

@author: Shaun Carlson - 10576840
'''

PROCESS_ID   = 0
ARRIVAL_TIME = 1
TTC          = 2

JOB          = 0
WAIT_TIME    = 1
PROCESS_TIME = 2
jobs = []

def firstComeFirstServed(jobs):
    print "FIRST COME FIRST SERVED"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    
    queue      = []
    finished   = []
    clock      = 0
    currentJob = None
    while len(jobs) > 0 or len(queue) > 0 or currentJob != None:
        #jobs are sorted by arrival time, keep pushing the top job into the queue if it arrives at this clock cycle
        while len(jobs) > 0 and jobs[0][ARRIVAL_TIME] == clock:
            queue.append([jobs.pop(0), 0, 0])
        
        #pop a job off the queue if one is not being processed
        if currentJob == None and len(queue) > 0: 
            currentJob = queue.pop(0)
            
        if currentJob != None:
            if currentJob[PROCESS_TIME] < currentJob[JOB][TTC]:
                currentJob[PROCESS_TIME] += 1
            
            if currentJob[JOB][TTC] == currentJob[PROCESS_TIME]:
                print str(currentJob[JOB][PROCESS_ID])+"\t"+str(currentJob[WAIT_TIME])+"\t"+str(currentJob[PROCESS_TIME] + currentJob[WAIT_TIME])
                finished.append(currentJob)
                currentJob = None
    
        clock += 1
        for job in queue:
            job[WAIT_TIME] += 1
        
    avgWait = 0
    avgTurnaround = 0
    for job in finished:
        avgWait += job[WAIT_TIME]
        avgTurnaround += job[WAIT_TIME] + job[PROCESS_TIME]
    
    print "Average Wait:       "+str(float(avgWait) / len(finished))
    print "Average Turnaround: "+str(float(avgTurnaround) / len(finished))
    
    return

def shortestJobFirst(jobs):
    print "SHORTEST JOB FIRST"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    return

def shortestRemainingTimeNext(jobs):
    print "SHORTEST REMAINING TIME NEXT"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    return

def roundRobin(jobs):
    print "ROUND ROBIN"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    return

print "Input three integers, separated by space, that represent Process ID, Arrival Time, and Needed Execution Time, followed by return."
print "To finish input, press CTRL-D"
while True:
    try:
        s = raw_input("")
        t = s.split(" ")
        if len(t) != 3: raise IOError("Invalid input ["+s+"]")
        processId     = int(t[0])
        arrivalTime   = int(t[1])
        executionTime = int(t[2])
        jobs.append((processId,arrivalTime,executionTime))
    except EOFError as e:
        'received end of transmission'
        break;
    except:
        'something weird happened'
        raise

print ""

jobs.sort(key=lambda tup: tup[1])

firstComeFirstServed(list(jobs))
#shortestJobFirst(list(jobs))
#shortestRemainingTimeNext(list(jobs))
#roundRobin(list(jobs))