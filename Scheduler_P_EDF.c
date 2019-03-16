#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"

volatile uint16_t BusyPrio_EDF = 0xffff;      /* Current priority being served             */

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

void Scheduler_P_EDF (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */
  
  uint8_t i;
  uint16_t oldPrio;
	uint16_t earlestDDL = BusyPrio_EDF; //store the earlest deadline
  uint8_t selectedTask = 0;
  
  				P4DIR = 0xff;
				P4OUT = 0xf;
  
  oldPrio = BusyPrio_EDF;
  while(selectedTask != 0xff){
	earlestDDL = oldPrio;
	selectedTask = 0xff;
	P4OUT = P4OUT + 1;
	//find the task with earlest deadline
	for(i = 0; i < NUMTASKS; i++)
	{
		Taskp t = &Tasks[i];
	
		if (t->Activated != t->Invoked)
		{
			if(t->NextRelease < earlestDDL)
			{	
				earlestDDL = t->NextRelease;
				selectedTask = i;
				P3OUT = earlestDDL & 0xff;
				P2OUT = i;
			}
		}
	}
	
	if(selectedTask != 0xff){
		BusyPrio_EDF =  Tasks[selectedTask].NextRelease;
		ExecuteTask(&Tasks[selectedTask]);
	}

	
  }
  
  BusyPrio_EDF = oldPrio;//restore the BusyPrio
  
  PrintResults();
  /* ---------------------------------------------------------------- */
}
