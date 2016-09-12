/*
File:   Provisioner.cpp
Author: J. Ian Lindsay
Date:   2016.09.10

*/


#if defined(OC_CLIENT) && defined(MANUVR_OPENINTERCONNECT)
#include "Provisioner.h"
#include <Platform/Platform.h>

//ManuvrOIC::INSTANCE

extern "C" {
  //#include "oc_api.h"
  //#include "port/oc_signal_main_loop.h"
  //#include "port/oc_connectivity.h"
} // extern "C"


/*******************************************************************************
*      _______.___________.    ___   .___________. __    ______     _______.
*     /       |           |   /   \  |           ||  |  /      |   /       |
*    |   (----`---|  |----`  /  ^  \ `---|  |----`|  | |  ,----'  |   (----`
*     \   \       |  |      /  /_\  \    |  |     |  | |  |        \   \
* .----)   |      |  |     /  _____  \   |  |     |  | |  `----.----)   |
* |_______/       |__|    /__/     \__\  |__|     |__|  \______|_______/
*
* Static members and initializers should be located here.
*******************************************************************************/


/*******************************************************************************
*   ___ _              ___      _ _              _      _
*  / __| |__ _ ______ | _ ) ___(_) |___ _ _ _ __| |__ _| |_ ___
* | (__| / _` (_-<_-< | _ \/ _ \ | / -_) '_| '_ \ / _` |  _/ -_)
*  \___|_\__,_/__/__/ |___/\___/_|_\___|_| | .__/_\__,_|\__\___|
*                                          |_|
* Constructors/destructors, class initialization functions and so-forth...
*******************************************************************************/
/**
* Vanilla constructor.
*/
Provisioner::Provisioner() : EventReceiver() {
  setReceiverName("Provisioner");
}


/**
* Constructor. Takes arguments.
*
* @param   Argument* root_config
*/
Provisioner::Provisioner(Argument* root_config) : Provisioner() {
  erConfigure(root_config);
}


/**
* Unlike many of the other EventReceivers, THIS one needs to be able to be torn down.
*/
Provisioner::~Provisioner() {
}



/****************************************************************************************************
*  ▄▄▄▄▄▄▄▄▄▄▄  ▄               ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄
* ▐░░░░░░░░░░░▌▐░▌             ▐░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌
* ▐░█▀▀▀▀▀▀▀▀▀  ▐░▌           ▐░▌ ▐░█▀▀▀▀▀▀▀▀▀ ▐░▌░▌     ▐░▌ ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀▀▀
* ▐░▌            ▐░▌         ▐░▌  ▐░▌          ▐░▌▐░▌    ▐░▌     ▐░▌     ▐░▌
* ▐░█▄▄▄▄▄▄▄▄▄    ▐░▌       ▐░▌   ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌ ▐░▌   ▐░▌     ▐░▌     ▐░█▄▄▄▄▄▄▄▄▄
* ▐░░░░░░░░░░░▌    ▐░▌     ▐░▌    ▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌     ▐░▌     ▐░░░░░░░░░░░▌
* ▐░█▀▀▀▀▀▀▀▀▀      ▐░▌   ▐░▌     ▐░█▀▀▀▀▀▀▀▀▀ ▐░▌   ▐░▌ ▐░▌     ▐░▌      ▀▀▀▀▀▀▀▀▀█░▌
* ▐░▌                ▐░▌ ▐░▌      ▐░▌          ▐░▌    ▐░▌▐░▌     ▐░▌               ▐░▌
* ▐░█▄▄▄▄▄▄▄▄▄        ▐░▐░▌       ▐░█▄▄▄▄▄▄▄▄▄ ▐░▌     ▐░▐░▌     ▐░▌      ▄▄▄▄▄▄▄▄▄█░▌
* ▐░░░░░░░░░░░▌        ▐░▌        ▐░░░░░░░░░░░▌▐░▌      ▐░░▌     ▐░▌     ▐░░░░░░░░░░░▌
*  ▀▀▀▀▀▀▀▀▀▀▀          ▀          ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀       ▀       ▀▀▀▀▀▀▀▀▀▀▀
*
* These are overrides from EventReceiver interface...
****************************************************************************************************/
/**
* The Kernel is booting, or has-booted prior to our being instanced.
*
* @return 0 on no action, 1 on action, -1 on failure.
*/
int8_t Provisioner::bootComplete() {
  EventReceiver::bootComplete();
  return 1;
}


/**
* We are being configured.
*
* @return 0 on no action, 1 on action, -1 on failure.
*/
int8_t Provisioner::erConfigure(Argument* opts) {
  int8_t return_value = 0;
  Argument* temp = opts->retrieveArgByKey("relay_devs");
  if (temp) {
    // TODO: Need a better way to do this.
    uint8_t val = 0;
    if ((0 == temp->getValueAs(&val)) && (0 != val)) {
      if (!relayDiscovery()) {
        relayDiscovery(true);
        return_value++;
      }
    }
  }
  return return_value;
}


/**
* If we find ourselves in this fxn, it means an event that this class built (the argument)
*   has been serviced and we are now getting the chance to see the results. The argument
*   to this fxn will never be NULL.
*
* Depending on class implementations, we might choose to handle the completed Event differently. We
*   might add values to event's Argument chain and return RECYCLE. We may also free() the event
*   ourselves and return DROP. By default, we will return REAP to instruct the Kernel
*   to either free() the event or return it to it's preallocate queue, as appropriate. If the event
*   was crafted to not be in the heap in its own allocation, we will return DROP instead.
*
* @param  event  The event for which service has been completed.
* @return A callback return code.
*/
int8_t Provisioner::callback_proc(ManuvrRunnable *event) {
  /* Setup the default return code. If the event was marked as mem_managed, we return a DROP code.
     Otherwise, we will return a REAP code. Downstream of this assignment, we might choose differently. */
  int8_t return_value = event->kernelShouldReap() ? EVENT_CALLBACK_RETURN_REAP : EVENT_CALLBACK_RETURN_DROP;

  /* Some class-specific set of conditionals below this line. */
  switch (event->eventCode()) {
    default:
      break;
  }

  return return_value;
}


int8_t Provisioner::notify(ManuvrRunnable *active_event) {
  int8_t return_value = 0;
  Argument* args = active_event->getArgs();

  switch (active_event->eventCode()) {
    case MANUVR_MSG_OIC_DISCOVERY:
      if (args) {
        StringBuilder* str;
        if (0 == args->getValueAs(&str)) {
          local_log.concat("Discovered new device:\t");
          local_log.concatHandoff(str);
          return_value++;
          if (relayDiscovery()) {
            // TODO: Send to connected TCP clients.
          }
        }
      }
      break;
    case MANUVR_MSG_OIC_READY:
      oc_init_platform(IDENTITY_STRING, NULL, NULL);
      oc_add_device("/oic/d", "oic.d.phone", "Client", "1.0", "1.0", NULL, NULL);
      return_value++;
      break;
    default:
      return_value += EventReceiver::notify(active_event);
      break;
  }

  flushLocalLog();
  return return_value;
}


/**
* Debug support method. This fxn is only present in debug builds.
*
* @param   StringBuilder* The buffer into which this fxn should write its output.
*/
void Provisioner::printDebug(StringBuilder *output) {
  EventReceiver::printDebug(output);
  if (_desired_owner) {
    output->concat("-- Owner info:\n");
    _desired_owner->toString(output);
  }
  else {
    output->concat("-- No owner.\n");
  }
  output->concatf("-- Provisioning:        %s\n", (isProvisioning() ? "yes" : "no"));
  output->concatf("-- Relay new devices:   %s\n", (relayDiscovery() ? "yes" : "no"));
}


#if defined(__MANUVR_CONSOLE_SUPPORT)
void Provisioner::procDirectDebugInstruction(StringBuilder *input) {
  char* str = input->position(0);
  char c    = *(str);

  switch (c) {
    default:
      EventReceiver::procDirectDebugInstruction(input);
      break;
  }

  flushLocalLog();
}
#endif  // __MANUVR_CONSOLE_SUPPORT
#endif  // OC_CLIENT && MANUVR_OPENINTERCONNECT
