#include "Scheduler.h"
#include "Led.h"

uint8_t edfRunningFlag = 0;

static void ExecuteTask (Taskp t)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */
  t->Invoked++;
  if (t->Flags & TRIGGERED) {
    SetLeds (BROWN, 1);
	t->Taskf(t->ExecutionTime); // execute task
	SetLeds (BROWN, 0);
  } else {
    t->Activated = t->Invoked;
  }
  /* ---------------------------------------------------------------- */
}

void Scheduler_NP_EDF (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */
  
  uint8_t i;
	uint16_t earlestDDL = 0xffff; //store the earlest deadline
  uint8_t selectedTask = 0;
  
  if(edfRunningFlag == 0)
  while(selectedTask != 0xff){
	earlestDDL = 0xffff;
	selectedTask = 0xff;
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
			}
		}
	}
	
	if(selectedTask != 0xff){
		i = -1;
		edfRunningFlag = 1;
		ExecuteTask(&Tasks[selectedTask]);
		edfRunningFlag = 0;
	}
	
	//	check whether there is pending interrupt
	if (TACCTL0 & CCIFG){ // Check if interrupt is pending
		int j;
		uint16_t MinTime = 0xffff; //set to max number
		for(j = 0;j < NUMTASKS; j++){
			Taskp t = &Tasks[j];
			if(TACCR0 == t->NextRelease){
				t->Activated++;
				t->NextRelease += t->Period; // set next release time
			}		
			if(MinTime > t->NextRelease)
				MinTime = t->NextRelease;
		}
		P6OUT = 0x3a;
		TACCR0 = MinTime;
		
		TACCTL0 &= ~CCIFG; // Clear interrupt pending flag
	}

  }
  
  PrintResults();
  /* ---------------------------------------------------------------- */
}
