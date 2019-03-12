#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"



volatile uint8_t BusyPrio = NUMTASKS;      /* Current priority being served             */

static void ExecuteTask (Taskp t)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */
  t->Invoked++;
  if (t->Flags & TRIGGERED) {
    SetLeds (BROWN, 1);
	_EINT();
	t->Taskf(t->ExecutionTime); // execute task
	_DINT();
	SetLeds (BROWN, 0);
  } else {
    t->Activated = t->Invoked;
  }
  /* ---------------------------------------------------------------- */

}

void Scheduler_P_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */
  StartTracking(TT_SCHEDULER);
  
  uint8_t oldBP = BusyPrio; // Set BusyPrio as the priority of the currently running task (the one that was executing just before the scheduler is called)

  for(BusyPrio = 0; BusyPrio < oldBP; BusyPrio++)
  {
    Taskp t = &Tasks[BusyPrio];

	while (t->Activated != t->Invoked)
	{
		StopTracking(TT_SCHEDULER);
		ExecuteTask(t);
		StartTracking(TT_SCHEDULER);
	}  
  }
  
  PrintResults();
  /* ---------------------------------------------------------------- */
}
