#include "Scheduler.h"
#include "Led.h"

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
		P6OUT = 0xb;
		i=-1; //in the next round, start from 0

	}
	P6OUT = 0xc;
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

   P4OUT = Tasks[0].Invoked;
  P4OUT = 0xff;
  
  PrintResults();
  /* ---------------------------------------------------------------- */
}
