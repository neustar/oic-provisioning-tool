/*
File:   LostPuppy.cpp
Author: J. Ian Lindsay
Date:   2016.09.10

*/


#if defined(OC_SERVER) && defined(MANUVR_OPENINTERCONNECT)
#include "LostPuppy.h"
#include <Platform/Platform.h>

//ManuvrOIC::INSTANCE


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

extern "C" {
  static bool light_state = false;
  static uint8_t light_gpio_pin = 14;

  static void set_device_custom_property(void *data) {
    // TODO: There *has* to be a better way.
    light_state = !light_state;
    setPin(light_gpio_pin, light_state);
  }


  static void put_light(oc_request_t *request, oc_interface_mask_t interface, void* data) {
    Kernel::log("PUT_light:\n");
    bool state = false;
    oc_rep_t *rep = request->request_payload;
    while(rep != NULL) {
      PRINT("key: %s ", oc_string(rep->name));
      switch(rep->type) {
        case BOOL:
          state = rep->value_boolean;
          break;
        default:
          oc_send_response(request, OC_STATUS_BAD_REQUEST);
          return;
          break;
      }
      rep = rep->next;
    }
    light_state = state;
    Kernel::log(light_state ? "value: ON\n" : "value: OFF\n");
    setPin(light_gpio_pin, light_state);
    oc_send_response(request, OC_STATUS_CHANGED);
  }


  static void get_light(oc_request_t *request, oc_interface_mask_t interface, void* data) {
    Kernel::log("GET_light:\n");
    // TODO: Strip and re-write all use of macros to executable code.
    //       Debugging preprocessor macros is a waste of life.
    CborEncoder g_encoder, _rmap;
    CborError g_err;
    cbor_encoder_create_map(&g_encoder, &_rmap, CborIndefiniteLength);
    switch (interface) {
      case OC_IF_BASELINE:
        oc_process_baseline_interface(request->resource);
      case OC_IF_RW:
        cbor_encode_text_string(&_rmap, "state", strlen("state"));
        cbor_encode_boolean(&_rmap, light_state);
        break;
      default:
        break;
    }
    oc_send_response(request, OC_STATUS_OK);
    Kernel::log(light_state ? "Light: ON\n" : "Light: OFF\n");
  }
} // extern "C"


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
LostPuppy::LostPuppy() : EventReceiver("LostPuppy") {
}


/**
* Constructor. Takes arguments.
*
* @param   Argument* root_config
*/
LostPuppy::LostPuppy(Argument* root_config) : LostPuppy() {
  erConfigure(root_config);
}


/**
* Unlike many of the other EventReceivers, THIS one needs to be able to be torn down.
*/
LostPuppy::~LostPuppy() {
}



/*******************************************************************************
* ######## ##     ## ######## ##    ## ########  ######
* ##       ##     ## ##       ###   ##    ##    ##    ##
* ##       ##     ## ##       ####  ##    ##    ##
* ######   ##     ## ######   ## ## ##    ##     ######
* ##        ##   ##  ##       ##  ####    ##          ##
* ##         ## ##   ##       ##   ###    ##    ##    ##
* ########    ###    ######## ##    ##    ##     ######
*
* These are overrides from EventReceiver interface...
*******************************************************************************/

/**
* This is called when the kernel attaches the module.
* This is the first time the class can be expected to have kernel access.
*
* @return 0 on no action, 1 on action, -1 on failure.
*/

int8_t LostPuppy::attached() {
  if (EventReceiver::attached()) {
    gpioDefine(light_gpio_pin, OUTPUT);
    light_state = false;
    setPin(light_gpio_pin, light_state);  // We will assume there is no inversion in hardware.
    return 1;
  }
  return 0;
}


/**
* We are being configured.
*
* @return 0 on no action, 1 on action, -1 on failure.
*/
int8_t LostPuppy::erConfigure(Argument* opts) {
  int8_t return_value = 0;
  Argument* temp = opts->retrieveArgByKey("gpio_pin");
  if (temp) {
    // TODO: Need a better way to do this.
    const char* val = nullptr;
    if ((0 == temp->getValueAs(&val)) && (nullptr != val)) {
      light_gpio_pin = atoi(val);
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
int8_t LostPuppy::callback_proc(ManuvrMsg* event) {
  /* Setup the default return code. If the event was marked as mem_managed, we return a DROP code.
     Otherwise, we will return a REAP code. Downstream of this assignment, we might choose differently. */
  int8_t return_value = (0 == event->refCount()) ? EVENT_CALLBACK_RETURN_REAP : EVENT_CALLBACK_RETURN_DROP;

  /* Some class-specific set of conditionals below this line. */
  switch (event->eventCode()) {
    default:
      break;
  }

  return return_value;
}


int8_t LostPuppy::notify(ManuvrMsg* active_event) {
  int8_t return_value = 0;
  Argument* args = active_event->getArgs();

  switch (active_event->eventCode()) {
    case MANUVR_MSG_OIC_REG_RESOURCES:
      res = oc_new_resource("/light/1", 1, 0);
      oc_resource_bind_resource_type(res, "oic.r.light");
      oc_resource_bind_resource_interface(res, OC_IF_RW);
      oc_resource_set_default_interface(res, OC_IF_RW);
      oc_resource_make_secure(res);
      oc_resource_set_discoverable(res, true);
      oc_resource_set_periodic_observable(res, 1);
      oc_resource_set_request_handler(res, OC_GET, get_light, nullptr);
      oc_resource_set_request_handler(res, OC_PUT, put_light, nullptr);
      oc_add_resource(res);
      return_value++;
      break;
    case MANUVR_MSG_OIC_READY:
      oc_init_platform(IDENTITY_STRING, NULL, NULL);
      oc_add_device("/oic/d", "oic.d.light", "Light", "1.0", "1.0", set_device_custom_property, NULL);
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
void LostPuppy::printDebug(StringBuilder *output) {
  EventReceiver::printDebug(output);
  if (_owner) {
    output->concat("-- Owner info:\n");
    _owner->toString(output);
  }
  else {
    output->concat("-- No owner (*arf*).\n");
  }
  output->concatf("-- GPIO pin:            %u\n", light_gpio_pin);
}


#if defined(MANUVR_CONSOLE_SUPPORT)
void LostPuppy::procDirectDebugInstruction(StringBuilder *input) {
  char* str = input->position(0);
  char c    = *(str);

  switch (c) {
    default:
      EventReceiver::procDirectDebugInstruction(input);
      break;
  }

  flushLocalLog();
}
#endif  // MANUVR_CONSOLE_SUPPORT
#endif  // OC_CLIENT && MANUVR_OPENINTERCONNECT
