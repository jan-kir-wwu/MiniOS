#include <stdio.h>
#include <stdlib.h>
#include "mini_os.h"
#include <signal.h>
#include <sys/time.h>

void signal_routine(int signal)
{
	sigset_t old_mask;

	// Lock signals
	sigfillset(&old_mask);
	sigprocmask(SIG_BLOCK, &old_mask, NULL);

	// Perform tasks here
	// ...
	sigprocmask(SIG_UNBLOCK, &old_mask, NULL);
}

int main(void)
{
	struct itimerval timer;
	struct sigaction sigact;

	// Set up the signal handler
	sigact.sa_handler = signal_routine;
	sigemptyset(&sa.sa_mask);
	sigact.sa_flags = 0;
	sigaction(SIGPROF, &sigact, NULL);
	// Set up the timer
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 500000; // 500000 microseconds = 0.5 seconds
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 500000;

	setitimer(ITIMER_VIRTUAL, &timer, NULL);

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
