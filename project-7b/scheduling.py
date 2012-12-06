'''
Created on Dec 3, 2012

@author: Shaun Carlson - 10576840
'''

import copy

class Job:
    def __init__(self, pid, arrival_time, time_to_complete):
        self.pid              = pid
        self.arrival_time     = arrival_time
        self.time_to_complete = time_to_complete
        
        self.wait_time    = 0
        self.process_time = 0
        self.started      = 0
        self.idle_time    = 0
    
    def delay(self, val):
        if self.started == 0:
            self.wait_time += val
        else:
            self.idle_time += val
    
    def process(self):
        self.started = 1
        self.process_time += 1
    
    def turn_around(self):
        return self.wait_time + self.idle_time + self.process_time
    
    def time_left(self):
        return self.time_to_complete - self.process_time
    
    def finished(self):
        return (self.time_to_complete == self.process_time)
    

def displayResults(finished):
    finished.sort(key=lambda Job: Job.arrival_time)
    
    #calculate and print average turnaround and wait times 
    avgWait = 0
    avgTurnaround = 0
    for job in finished:
        print str(job.pid) + "\t" + str(job.wait_time) + "\t" + str(job.turn_around())
        avgWait += job.wait_time
        avgTurnaround += job.turn_around()
    
    print "Average Wait:       "+str(float(avgWait) / len(finished))
    print "Average Turnaround: "+str(float(avgTurnaround) / len(finished))
    
    return

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
        while len(jobs) > 0 and jobs[0].arrival_time == clock:
            queue.append(jobs.pop(0))
        
        #pop a job off the queue if one is not being processed
        if currentJob == None and len(queue) > 0: 
            currentJob = queue.pop(0)
            
        if currentJob != None:
            if not currentJob.finished():
                currentJob.process()
            
            if currentJob.finished():
                finished.append(currentJob)
                currentJob = None
    
        clock += 1
        for job in queue:
            job.delay(1)
    
    displayResults(finished)
    return

def shortestJobFirst(jobs):
    print "SHORTEST JOB FIRST"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    
    queue      = []
    finished   = []
    clock      = 0
    currentJob = None
    while len(jobs) > 0 or len(queue) > 0 or currentJob != None:
        #jobs are sorted by arrival time, keep pushing the top job into the queue if it arrives at this clock cycle
        while len(jobs) > 0 and jobs[0].arrival_time == clock:
            queue.append(jobs.pop(0))
        
        #pop a job off the queue if one is not being processed
        if currentJob == None and len(queue) > 0: 
            shortestIndex = -1
            shortestTime  = 99999
            for i, job in enumerate(queue):
                if job.time_left() < shortestTime:
                    shortestIndex = i
                    shortestTime  = job.time_left()
            
            currentJob = queue.pop(shortestIndex)
        
            
        if currentJob != None:
            if not currentJob.finished():
                currentJob.process()
            
            if currentJob.finished():
                finished.append(currentJob)
                currentJob = None
    
        clock += 1
        for job in queue:
            job.delay(1)
        
    displayResults(finished)
    return

def shortestRemainingTimeNext(jobs):
    print "SHORTEST REMAINING TIME NEXT"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    
    queue      = []
    finished   = []
    clock      = 0
    currentJob = None
    while len(jobs) > 0 or len(queue) > 0 or currentJob != None:
        #jobs are sorted by arrival time, keep pushing the top job into the queue if it arrives at this clock cycle
        while len(jobs) > 0 and jobs[0].arrival_time == clock:
            queue.append(jobs.pop(0))
        
        #seek for shortest job left, starting with what's left on the current job
        shortestIndex = -1
        shortestTime  = 99999
        if currentJob != None:
            shortestTime = currentJob.time_left()
        
        #check in the queue for a shorter job
        for i, job in enumerate(queue):
            if (job.time_left()) < shortestTime:
                shortestIndex = i
                shortestTime  = job.time_left()
        
        #if shorter job found, switch out currentJob for that job
        if (shortestIndex > -1):
            if currentJob != None:
                queue.append(currentJob)
            currentJob = queue.pop(shortestIndex)
            
        if currentJob != None:
            if not currentJob.finished():
                currentJob.process()
            
            if currentJob.finished():
                finished.append(currentJob)
                currentJob = None
    
        clock += 1
        for job in queue:
            job.delay(1)
    
    displayResults(finished)
    return

def roundRobin(jobs, contextSwitchPenalty):
    print "ROUND ROBIN"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    
    ROTATION_PERIOD = 4
    timeSlice  = ROTATION_PERIOD
    queue      = []
    finished   = []
    clock      = 0
    started    = False
    currentJob = None
    while len(jobs) > 0 or len(queue) > 0 or currentJob != None:
        #jobs are sorted by arrival time, keep pushing the top job into the queue if it arrives at this clock cycle
        while len(jobs) > 0 and jobs[0].arrival_time == clock:
            queue.append(jobs.pop(0))

        #grab a new job when the previous job completed or its time slice ran out        
        if timeSlice == 0 or (len(queue) > 0 and currentJob == None):
            if currentJob != None: 
                queue.append(currentJob)
            
            #do context switch penalty
            if started:
                for job in queue:
                    job.delay(contextSwitchPenalty)
            
            currentJob = queue.pop(0)
            started = True
        
        if currentJob != None:
            if not currentJob.finished():
                currentJob.process()
                timeSlice -= 1
            
            if currentJob.finished():
                finished.append(currentJob)
                currentJob = None
                timeSlice = ROTATION_PERIOD
    
        clock += 1
        for job in queue:
            job.delay(1)
    
    displayResults(finished)
    return

def main():
    print "Input three integers, separated by space, that represent Process ID, Arrival Time, and Needed Execution Time, followed by return."
    print "To finish input, press CTRL-D"
    jobs = []
    while True:
        try:
            s = raw_input("")
            t = s.split(" ")
            if len(t) != 3: raise IOError("Invalid input ["+s+"]")
            processId     = int(t[0])
            arrivalTime   = int(t[1])
            executionTime = int(t[2])
            jobs.append(Job(processId,arrivalTime,executionTime))
        except EOFError:
            'received end of transmission'
            break;
        except:
            'something weird happened'
            raise
    
    #sort by arrival time
    jobs.sort(key=lambda Job: Job.arrival_time)
    
#    print ""
#    firstComeFirstServed(copy.deepcopy(jobs))
#    print ""
#    shortestJobFirst(copy.deepcopy(jobs))
#    print ""
#    shortestRemainingTimeNext(copy.deepcopy(jobs))
    print ""
    roundRobin(copy.deepcopy(jobs), 0)
#    print ""
#    roundRobin(copy.deepcopy(jobs), 0.4)
    
    return

main()
