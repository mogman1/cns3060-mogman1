/*
 * signals.c
 *
 *  Created on: Dec 8, 2012
 *      Author: Shaun Carlson
 *  Student ID: 10576840
 */

#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void parent_sigusr1_handler(int signal) {
	printf("**** Parent SIGUSR1 handler - Received a 'task started' signal [%d] from child ****\n", signal);
}

void parent_sigusr2_handler(int signal) {
	printf("**** Parent SIGUSR2 handler - Received a 'task completed' signal [%d] from the child ****\n", signal);
}

void parent_process(pid_t childPID) {
	//4
	printf("PARENT: Spawned child.  PID: %d\n", childPID);

	//5
	sleep(3); //allow child to register its signal handlers

	//8
	printf("PARENT: *snore* Huh wuh?  Oh, time to tell my child to work.\n");
	sigset_t signalMask;
	sigfillset(&signalMask);
	sigdelset(&signalMask, SIGUSR1);
	kill(childPID, SIGUSR1);
	sigsuspend(&signalMask);

	//11
	sleep(3);
	printf("PARENT: Such attitude!  Tell me when you're done.  Kids these days...\n");
	sigfillset(&signalMask);
	sigdelset(&signalMask, SIGUSR2);
	kill(childPID, SIGUSR2);
	sigsuspend(&signalMask);

	//13
	printf("PARENT: Yeah, yeah, thank you.  Dinner's at six.\n");
}

void child_sigusr1_handler(int signal) {
	printf("**** Child SIGUSR1 handler - Received a 'task start' signal [%d] from the parent process ****\n", signal);
}

void child_sigusr2_handler(int signal) {
	printf("**** Child SIGUSR2 handler - Received a 'task complete verification' signal [%d] from parent ****\n", signal);
}

void child_process(pid_t parentPID) {
	//6
	struct sigaction sig1_new_action, sig1_old_action;
	sig1_new_action.sa_handler = child_sigusr1_handler;
	sig1_new_action.sa_flags   = 0;
	sigemptyset(&sig1_new_action.sa_mask);
	sigaction(SIGUSR1, &sig1_new_action, &sig1_old_action);

	struct sigaction sig2_new_action, sig2_old_action;
	sig2_new_action.sa_handler = child_sigusr2_handler;
	sig2_new_action.sa_flags   = 0;
	sigemptyset(&sig2_new_action.sa_mask);
	sigaction(SIGUSR2, &sig2_new_action, &sig2_old_action);


	//7
	printf("CHILD:  Now that I'm spawned, it's time for Internet and hope Dad [%d] doesn't make me do work.\n", parentPID);
	sigset_t signalMask;
	sigfillset(&signalMask);
	sigdelset(&signalMask, SIGUSR1);
	sigsuspend(&signalMask);

	//9
	printf("CHILD:  FINE, DAD!  I'LL WORK!\n");
	sleep(3);
	kill(parentPID, SIGUSR1);
	printf("CHILD:  Whistle while you work...\n");

	//10
	sigfillset(&signalMask);
	sigdelset(&signalMask, SIGUSR2);
	sigsuspend(&signalMask);

	//12
	printf("CHILD:  I'M DONE, DAD!  GOSH!\n");
	kill(parentPID, SIGUSR2);
	printf("CHILD:  YOU'RE SUCH A SLAVE DRIVER!  I HATE THIS FAMILY!\n");

	//not needed, but for completion's sake we'll restore the old signal action before ending
	sigaction(SIGUSR1, &sig1_old_action, &sig1_new_action);
	sigaction(SIGUSR2, &sig2_old_action, &sig2_new_action);
}

int main() {
	//1
	printf("Name:       Shaun Carlson\n");
	printf("Student ID: 10576840\n");
	printf("Class:      CS 3060\n");
	printf("Section:	601\n");

	//2
	struct sigaction sig1_new_action, sig1_old_action;
	sig1_new_action.sa_handler = parent_sigusr1_handler;
	sig1_new_action.sa_flags   = 0;
	sigemptyset(&sig1_new_action.sa_mask);
	sigaction(SIGUSR1, &sig1_new_action, &sig1_old_action);

	struct sigaction sig2_new_action, sig2_old_action;
	sig2_new_action.sa_handler = parent_sigusr2_handler;
	sig2_new_action.sa_flags   = 0;
	sigemptyset(&sig2_new_action.sa_mask);
	sigaction(SIGUSR2, &sig2_new_action, &sig2_old_action);

	//3
	pid_t mainId = getpid();
	pid_t childId= fork();
	if (childId == -1) {
		perror("Unable to fork new process");
		return 1;
	} else if (childId == 0) {
		//inside child
		child_process(mainId);
	} else {
		//inside parent
		parent_process(childId);

		//not needed, but for completion's sake we'll restore the old signal action before ending
		sigaction(SIGUSR1, &sig1_old_action, &sig1_new_action);
		sigaction(SIGUSR2, &sig2_old_action, &sig2_new_action);
	}

	return 0;
}
