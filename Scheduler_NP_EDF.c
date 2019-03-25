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

void Scheduler_NP_EDF (Task Tasks[])
{ 
  uint8_t i, j; // loop increment variable
  int8_t NextTaskIndex; // index of task which is going to be scheduled next
  uint8_t ScheduleAgain;
 uint8_t selectedTask;
 uint16_t earlestDDL = 0xffff; //store the earlest deadline
 Taskp t;
  do 
  {
    ScheduleAgain = 0; // reset ScheduleAgain flag
    NextTaskIndex = -1; // reset NextTaskIndex
	earlestDDL = 0xffff; //reset the earlest Deadline
	
	for(i = 0; i < NUMTASKS; i++)
	{
		t = &Tasks[i];
		
		if (t->Flags & TRIGGERED)
		{
			if (t->Activated != t->Invoked)
			{			
				if(t->NextRelease < earlestDDL)
				{	
					earlestDDL = t->NextRelease;
					NextTaskIndex = i;
				}
			}
		}
	}

	if (Tasks[NextTaskIndex].Flags & BUSY_EXEC)
	{
    // this task was already executing, no need to invoke it again, exit scheduler
		break;
	} 

	P4OUT = NextTaskIndex;
	P4OUT = 0x0f;
	
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

