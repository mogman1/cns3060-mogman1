'''
Created on Dec 3, 2012

@author: Shaun Carlson - 10576840

NOTE ON CLOCK:
    Due to the context switch penalty for the round robin scheduler being 0.4ms, and due to
    the risk of floating point errors, each integer in the clock cycle represents 0.1ms.
'''

import copy
import sys

CLOCK_CYCLE     = 1  #Represents 0.1 milliseconds
ROTATION_PERIOD = 40 #Represents 4.0 milliseconds

class Job:
    """
    Keeps track of job time metrics.  All times are repesented in 10 * ms, so
    one millisecond would equal 10
    """
    def __init__(self, pid, arrival_time, time_to_complete):
        self.pid              = pid
        self.arrival_time     = arrival_time * 10
        self.time_to_complete = time_to_complete * 10
        
        self.wait_time    = 0
        self.process_time = 0
        self.started      = False
        self.idle_time    = 0
    
    def wait(self):
        """
        Returns the wait second in milliseconds
        """
        return float(self.wait_time) / 10
    
    def delay(self, val):
        """
        If the job HAS NOT started, delay will cause the job wait_time counter to increment
        If the job HAS started, delay will cause the idle_time counter to increment
        """
        if self.started == False:
            self.wait_time += val
        else:
            self.idle_time += val
    
    def process(self, amount):
        """
        Causes the job to process some amount of time towards job completion
        """
        self.started = True
        self.process_time += amount
    
    def turn_around(self):
        """
        Returns the total turnaround time in milliseconds for a job, which includes process time, wait time, and idle time
        """
        return float(self.wait_time + self.idle_time + self.process_time) / 10
    
    def time_left(self):
        """
        Returns the time left for the job to complete in milliseconds
        """
        return float(self.time_to_complete - self.process_time) / 10
    
    def finished(self):
        """
        Returns True if the job has completed
        """
        return (self.time_to_complete == self.process_time)
    

def displayResults(finished):
    """
    Generic function for taking the finished jobs and printing out the average wait and turnaround time.
    """
    finished.sort(key=lambda Job: Job.arrival_time)
    
    #calculate and print average turnaround and wait times 
    avgWait = 0
    avgTurnaround = 0
    for job in finished:
        print str(job.pid) + "\t" + str(job.wait()) + "\t" + str(job.turn_around())
        avgWait += job.wait()
        avgTurnaround += job.turn_around()
    
    print "Average Wait:       "+str(float(avgWait) / len(finished))
    print "Average Turnaround: "+str(float(avgTurnaround) / len(finished))
    
    return

def firstComeFirstServed(jobs):
    """
    Processes list of jobs in a first-come-first-served schedule
    """
    print "FIRST COME FIRST SERVED"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    
    queue      = []
    finished   = []
    clock      = 0
    currentJob = None
    while len(jobs) > 0 or len(queue) > 0 or currentJob != None:
        #Jobs are sorted by arrival time, keep pushing the top job into the queue if it arrives at this clock cycle.
        while len(jobs) > 0 and jobs[0].arrival_time == clock:
            queue.append(jobs.pop(0))
        
        #pop a job off the queue if one is not being processed
        if currentJob == None and len(queue) > 0: 
            currentJob = queue.pop(0)
            
        if currentJob != None:
            if not currentJob.finished():
                currentJob.process(CLOCK_CYCLE)
            
            if currentJob.finished():
                finished.append(currentJob)
                currentJob = None
    
        clock += CLOCK_CYCLE
        for job in queue: job.delay(CLOCK_CYCLE)
    
    displayResults(finished)
    return

def shortestJobFirst(jobs):
    """
    Processes list of jobs by shortest job in list first
    """
    print "SHORTEST JOB FIRST"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    
    queue      = []
    finished   = []
    clock      = 0
    currentJob = None
    while len(jobs) > 0 or len(queue) > 0 or currentJob != None:
        #Jobs are sorted by arrival time, keep pushing the top job into the queue if it arrives at this clock cycle.
        while len(jobs) > 0 and jobs[0].arrival_time == clock:
            queue.append(jobs.pop(0))
        
        #pop a job off the queue if one is not being processed
        if currentJob == None and len(queue) > 0: 
            shortestIndex = -1
            shortestTime  = sys.maxint
            for i, job in enumerate(queue):
                if job.time_left() < shortestTime:
                    shortestIndex = i
                    shortestTime  = job.time_left()
            
            currentJob = queue.pop(shortestIndex)
        
            
        if currentJob != None:
            if not currentJob.finished():
                currentJob.process(CLOCK_CYCLE)
            
            if currentJob.finished():
                finished.append(currentJob)
                currentJob = None
    
        clock += CLOCK_CYCLE
        for job in queue: job.delay(CLOCK_CYCLE)
        
    displayResults(finished)
    return

def shortestRemainingTimeNext(jobs):
    """
    Processes list of jobs by whichever job has the shortest remaining time left in the list.  This
    algorithm checks the list every millisecond to look for jobs shorter than the one being processed.
    """
    print "SHORTEST REMAINING TIME NEXT"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    
    queue      = []
    finished   = []
    clock      = 0
    currentJob = None
    while len(jobs) > 0 or len(queue) > 0 or currentJob != None:
        #Jobs are sorted by arrival time, keep pushing the top job into the queue if it arrives at this clock cycle.
        while len(jobs) > 0 and jobs[0].arrival_time == clock:
            queue.append(jobs.pop(0))
        
        if clock % 10 == 0:
            #Check every millisecond for the shortest job left, starting with what's left on the current job.
            shortestIndex = -1
            shortestTime  = sys.maxint
            if currentJob != None:
                shortestTime = currentJob.time_left()
            
            #check in the queue for a shorter job
            for i, job in enumerate(queue):
                if job.time_left() < shortestTime:
                    shortestIndex = i
                    shortestTime  = job.time_left()
            
            #if shorter job found, switch out currentJob for that job
            if (shortestIndex > -1):
                if currentJob != None:
                    queue.append(currentJob)
                currentJob = queue.pop(shortestIndex)
            
        if currentJob != None:
            if not currentJob.finished():
                currentJob.process(CLOCK_CYCLE)
            
            if currentJob.finished():
                finished.append(currentJob)
                currentJob = None
    
        clock += CLOCK_CYCLE
        for job in queue: job.delay(CLOCK_CYCLE)
    
    displayResults(finished)
    return

def roundRobin(jobs, contextSwitchPenalty):
    """
    Processes list of jobs by round robin with a contextSwitchPenalty.  This penalty is represented as 10 * ms
    to avoid any decimal values, and thus avoid any floating point arithmetic errors.  E.g. 0.4ms context switch
    penalty would be passed in as 4
    Each job is alloted a time slice of ROTATION_PERIOD to complete before being pushed to the back of the queue
    to be finished later.
    """
    print "ROUND ROBIN"
    print "PID\tWait\tTurnaround"
    print "---\t----\t----------"
    
    timeSlice  = ROTATION_PERIOD
    queue      = []
    finished   = []
    clock      = 0
    currentJob = None
    contextSwitch = 0
    while len(jobs) > 0 or len(queue) > 0 or currentJob != None:
        #Jobs are sorted by arrival time, keep pushing the top job into the queue if it arrives at this clock cycle.
        while len(jobs) > 0 and jobs[0].arrival_time == clock:
            queue.append(jobs.pop(0))
        
        #If the job finished or the time slice ended, complete the job or push it back onto the queue as appropriate and
        #initiate the context switch penalty.
        if timeSlice == 0 or (currentJob != None and currentJob.finished()):
            if currentJob.finished():
                finished.append(currentJob)
            else:
                queue.append(currentJob)
            
            currentJob    = None
            timeSlice     = ROTATION_PERIOD
            contextSwitch = contextSwitchPenalty
        
        #When in a context switch, skip all other actions to ensure that every job in the queue receives the penalty
        if contextSwitch > 0:
            contextSwitch -= CLOCK_CYCLE
            clock         += CLOCK_CYCLE
            for job in queue: job.delay(CLOCK_CYCLE)
            continue
        
        if len(queue) > 0 and currentJob == None:
            currentJob = queue.pop(0)
        
        if currentJob != None:
            currentJob.process(CLOCK_CYCLE)
            timeSlice -= CLOCK_CYCLE
        
        clock += CLOCK_CYCLE
        for job in queue: job.delay(CLOCK_CYCLE)
    
    displayResults(finished)
    return

def main():
    print "Input three integers, separated by one or more spaces, that represent Process ID, Arrival Time, and Needed Execution Time, followed by return."
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
    
    print ""
    firstComeFirstServed(copy.deepcopy(jobs))
    print ""
    shortestJobFirst(copy.deepcopy(jobs))
    print ""
    shortestRemainingTimeNext(copy.deepcopy(jobs))
    print ""
    roundRobin(copy.deepcopy(jobs), 0)
    print ""
    roundRobin(copy.deepcopy(jobs), 4)
    
    return

main()
