#include <stdio.h>
#include <stdlib.h>
#include "mini_os.h"
#include <signal.h>
#include <sys/time.h>

void signal_routine(int signal)
{
	if (signal == 27 || signal == SIGPROF)
	{
		sigset_t sigset;
		sigset_t old_set;
		sigemptyset(&sigset);
		sigaddset(&sigset, SIGINT);
		sigprocmask(SIG_BLOCK, &sigset, &old_set);
		// Hier ist SIGINT gesperrt

		schedule();

		// Sperre aufheben :
		sigprocmask(SIG_SETMASK, &old_set, NULL);
	}
}

int main(void)
{

	// Set up the signal handler
	struct sigaction sigact;
	sigact.sa_handler = signal_routine;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigaction(SIGPROF, &sigact, NULL);

	// Set up the timer
	struct itimerval timer;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 500000; // 500000 microseconds = 0.5 seconds
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 500000;

	// delivers SIGPROF at the end
	setitimer(ITIMER_PROF, &timer, NULL);

	if (create_process("init", init, NULL) == -1)
	{
		perror("Creating init process failed");
		return EXIT_FAILURE;
	}

	if (start_scheduler())
	{
		perror("Scheduling failed");
		return EXIT_FAILURE;
	}

	puts("Scheduler returned, all processes done");
	return EXIT_SUCCESS;
}
