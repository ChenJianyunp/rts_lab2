#include "Scheduler.h"
#include "Led.h"

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

void Scheduler_NP_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */
 
	int i;
  for(i = 0; i < NUMTASKS; i++)
  {
    Taskp t = &Tasks[i];

	if (t->Activated != t->Invoked)
	{
		ExecuteTask(t);
		i = 0;
	}  
  }
  
  PrintResults();
  /* ---------------------------------------------------------------- */
}
