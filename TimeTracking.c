#include <io.h>
#include <iomacros.h>

#include "TimeTracking.h"

/*
 * Timetracking interface, used to track the amount of time spent on certain intervals in code. 
 * Integrated with TCL signal script 'signals.tcl'. These functions drive external pins port_out[0:7] on the msp430 signal group,
 * 'signals.tcl' is used to sum the total time the pins are high for hi-res overhead measurement.
 * Only use defined index PINx as input for tracking functions
 *
 * Boris Blokland, 8-2-2019
 * Amended by: D. Zwart & D. Offerhaus, 8-3-2019
 */


static uint8_t JobsExecuted;

// Trigger ping high (detected as START by edge-detect TCL procedure loopSignal)
void StartTracking(uint8_t index)
{
    P4OUT |= (0x1 << index);
}

// Trigger ping high (detected as STOP by edge-detect TCL procedure loopSignal)
void StopTracking(uint8_t index)
{
    P4OUT &= ~(0x1 << index);
}

void AddJobExecution()
{
    JobsExecuted++;
}