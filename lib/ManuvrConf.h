/*
File:   FirmwareDefs.h
Author: J. Ian Lindsay
Date:   2015.03.01


This is one of the files that the application author is required to provide.
This is where definition of (application or device)-specific parameters ought to go.

This is an example file for building firmware on linux. Anticipated target is a Raspi.
  but that shouldn't matter too much. These settings are only typically relevant for
  reasons of memory-constraint, threading model (if any), or specific features that
  this hardware can support.
*/

#ifndef __FIRMWARE_DEFS_H
#define __FIRMWARE_DEFS_H

/****************************************************************************************************
* Required fields...                                                                                *
****************************************************************************************************/

/*
* Particulars of this platform.
*/
#define PLATFORM_RNG_CARRY_CAPACITY      128    // How many random numbers should be cached? Must be > 0.
#define PROTOCOL_MTU                   65536    // See MTU notes above....

/*
* Particulars of this Manuvrable.
*/
// This is the string that identifies this Manuvrable to other Manuvrables. In MHB's case, this
//   will select the mEngine.
#define IDENTITY_STRING          "LostPuppy"    // The name of this firmware.

// This would be the version of the Manuvrable's firmware (this program).
#define VERSION_STRING               "0.0.1"

// Hardware is versioned. Manuvrables that are strictly-software should comment this.
//#define HW_VERSION_STRING               "rev4"

// The version of Manuvr's protocol we are using.
#define PROTOCOL_VERSION             "0.0.1"


/*
* Kernel options.
*/
#define EVENT_MANAGER_PREALLOC_COUNT      32    // How large a preallocation buffer should we keep?
#define MANUVR_PLATFORM_TIMER_PERIOD_MS   10    // What is the granularity of our scheduler?

#define MAXIMUM_SEQUENTIAL_SKIPS          20    // How many scheduler skips are we willing to tolerate?


#define EXTENDED_DETAIL_STRING    "RasPiBuild"  // Optional. User-defined.

#endif
