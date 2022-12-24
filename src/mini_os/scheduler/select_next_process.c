#include "../mini_os.h"
#include <stdio.h>
struct process *select_next_process(void)
{
	struct process *sel;

	sel = ptable.run_queue.head;

	printf("-------- run_queue HEAD is \"%s\" (PID %i)!\n",
		   sel->name, sel->process_id);

	for (struct process *cur = sel->next; cur; cur = cur->next)
	{
		sel = cur;
		printf("-------- Process \"%s\" (PID %i) has been selected for "
			   "the run_queue!\n",
			   cur->name, cur->process_id);
		break;
	}

	queue_remove(sel);
	return sel;
}
