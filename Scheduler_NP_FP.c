#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"

static void ExecuteTask (Taskp t)
{
  t->Invoked++; // increment invoked counter
  t->Flags |= BUSY_EXEC; // set this task to busy executing
  
  StopTracking(0);
  SetLeds(BROWN, 0);

  t->Taskf(t->ExecutionTime); // execute task
StartTracking(0);
  AddJobExecution();
  
  SetLeds(BROWN, 1);
  
  t->Flags &= ~BUSY_EXEC; // this task is done busy executing
}

void Scheduler_NP_FP (Task Tasks[])
{ 
  uint8_t i; // loop increment variable
  int8_t NextTaskIndex; // index of task which is going to be scheduled next
  uint8_t ScheduleAgain;

  do 
  {
    ScheduleAgain = 0; // reset ScheduleAgain flag
    NextTaskIndex = -1; // reset NextTaskIndex

    for (i = 0; i < NUMTASKS; i++) // loop through all task to see which need to be scheduled
    {
      Taskp t = &Tasks[i];

      if (t->Flags & TRIGGERED) // check if task is available
      {
        if (t->Flags & BUSY_EXEC)
        {
          // this task was already executing, no need to invoke it again, exit scheduler
          break;
        }  
        else if (t->Activated != t->Invoked) // check if task needs to be executed
        {   
          NextTaskIndex = i; // set next task index to this task
          break; // because task are stored in priority order, this will be the highest priority task       
        }   
      }
    }

    if (NextTaskIndex >= 0)
    {
      ExecuteTask(&Tasks[NextTaskIndex]);
      ScheduleAgain = 1; // because we executed a task, we should call scheduler again
    }
	
		//	check whether there is pending interrupt
	if (TACCTL0 & CCIFG){ // Check if interrupt is pending		
		TACCTL0 &= ~CCIFG; // Clear interrupt pending flag
		return;
	}
	
  } while (ScheduleAgain); // check whether scheduler should be executed again
}
