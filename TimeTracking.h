#ifndef __TimeTracking_h
#define __TimeTracking_h

/*
 * Timetracking interface
 *
 * Boris Blokland, 8-2-2019
 * Amended by: D. Offerhaus & D. Zwart, 8-3-2019
 */

#define PIN0 0x0
#define PIN1 0x1
#define PIN2 0x2
#define PIN3 0x4
#define PIN4 0x8
#define PIN5 0x10
#define PIN6 0x20
#define PIN7 0x40

void StartTracking(uint8_t index);
void StopTracking(uint8_t index);
void AddJobExecution();

#endif
