/*
File:   Provisioner.cpp
Author: J. Ian Lindsay
Date:   2016.09.10

*/


#if defined(OC_CLIENT) && defined(MANUVR_OPENINTERCONNECT)
#include "Provisioner.h"
#include <CommonConstants.h>
#include <Platform/Platform.h>

//ManuvrOIC::INSTANCE

#include "deps/tinycbor/src/cbor.h"

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
// TODO: This is an ugly kludge for now.
PriorityQueue<Argument*> _known_devices;

Argument* locateRequest(oc_client_response_t* resp) {
  if (_known_devices.size() == 1) {
    // TODO: Wrong.
    return _known_devices.get();
  }
  return nullptr;
}


void get_doxm_callback(oc_client_response_t* data) {
  Argument* intial_req = locateRequest(data);
  if (intial_req) {
    if (OC_STATUS_OK == data->code) {
      // Now, we should set pstat to allow write operations to doxm.
      //if (oc_init_post("", &light_server, NULL, &post2_light, LOW_QOS)) {
      //  oc_rep_start_root_object();
      //  oc_rep_set_int(root, power, 55);
      //  oc_rep_end_root_object();
      //}
      //else {
      //  Kernel::log("get_doxm_callback(): Could not init POST.\n");
      //}
      Kernel::log("get_doxm_callback(): doxm returned.\n");
    }
    else {
      Kernel::log("get_doxm_callback(): Unacceptable status code.\n");
    }
    delete intial_req;
  }
  else {
    Kernel::log("get_doxm_callback(): Failed to find initial request.\n");
  }
}


void post_resrc_callback(oc_client_response_t* data) {
  StringBuilder _log;
  _log.concat("get_resrc_callback(): \n");
  oc_rep_t *rep = data->payload;
  while (rep != NULL) {
    _log.concatf("  key %s, value ", oc_string(rep->name));
    switch (rep->type) {
      case BOOL:
        _log.concatf("%s\n", rep->value_boolean?"true":"false");
        break;
      case INT:
        _log.concatf("%d\n", rep->value_int);
        break;
      case STRING:
        _log.concatf("%s\n", oc_string(rep->value_string));
        break;
      default:
        break;
    }
    rep = rep->next;
  }
  Kernel::log(&_log);
}


void get_resrc_callback(oc_client_response_t* data) {
  StringBuilder _log;
  _log.concat("get_resrc_callback(): \n");
  oc_rep_t *rep = data->payload;
  while (rep != NULL) {
    _log.concatf("key %s, value ", oc_string(rep->name));
    switch (rep->type) {
      case BOOL:
        _log.concatf("%s\n", rep->value_boolean?"true":"false");
        break;
      case INT:
        _log.concatf("%d\n", rep->value_int);
        break;
      case STRING:
        _log.concatf("%s\n", oc_string(rep->value_string));
        break;
      default:
        break;
    }
    rep = rep->next;
  }
  Kernel::log(&_log);
}



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
  while (_known_devices.size()) {
    delete _known_devices.dequeue();
  }
}


/**
* We are now responsible for the memory attached to dev_args.
*
* @param  dev_args [description]
* @return          0 on success.
*/
int8_t Provisioner::queryDeviceDoxm(Argument* dev_args) {
  oc_server_handle_t* srv_ep = nullptr;
  StringBuilder* uri         = nullptr;
  StringBuilder* di          = nullptr;
  dev_args->printDebug(&local_log);

  //if (dev_args->retrieveArgByKey("secure")) {
    Kernel::log("Found secure device. Proceeding to query its doxm...\n");
    if (0 == dev_args->getValueAs("server", &srv_ep)) {
      if (0 == dev_args->getValueAs("uri", &uri)) {
        if (0 == dev_args->getValueAs("di", &di)) {
          // Take ownership of the argument chain.
          _known_devices.insert(dev_args);
          // Build an endpoint definition for OIC and start the query.
          flushLocalLog();
          oc_do_get("/oic/sec/doxm", srv_ep, NULL, get_doxm_callback, LOW_QOS, NULL);
          return 0;
        }
        else {
          local_log.concat("queryDeviceDoxm(): Failed to get 'di'.\n");
        }
      }
      else {
        local_log.concat("queryDeviceDoxm(): Failed to get 'uri'.\n");
      }
    }
    else {
      local_log.concat("queryDeviceDoxm(): Failed to get 'server'.\n");
    }
  //}
  delete dev_args;
  flushLocalLog();
  return -1;
}


int8_t Provisioner::_get_resource_by_idx(int idx) {
  Argument* a = _known_devices.get(idx);
  if (a) {
    oc_server_handle_t* srv_ep = nullptr;
    StringBuilder* uri         = nullptr;
    if (0 == a->getValueAs("server", &srv_ep)) {
      if (0 == a->getValueAs("uri", &uri)) {
        if (oc_do_get((char*)uri->string(), srv_ep, NULL, get_resrc_callback, LOW_QOS, NULL)) {
          return 0;
        }
        else {
          local_log.concat("_get_resource_by_idx(): Failed to oc_do_get().\n");
        }
      }
      else {
        local_log.concat("_get_resource_by_idx(): No retained URI.\n");
      }
    }
    else {
      local_log.concat("_get_resource_by_idx(): No retained server endpoint.\n");\
    }
  }
  else {
    local_log.concat("_get_resource_by_idx(): No retained Argument.\n");
  }
  return -1;
}

int8_t Provisioner::_post_resource_by_idx(int idx, int val) {
  Argument* a = _known_devices.get(idx);
  if (a) {
    oc_server_handle_t* srv_ep = nullptr;
    StringBuilder* uri         = nullptr;
    if (0 == a->getValueAs("server", &srv_ep)) {
      if (0 == a->getValueAs("uri", &uri)) {
        if (oc_init_put((char*)uri->string(), srv_ep, NULL, &post_resrc_callback, LOW_QOS, NULL)) {
          cbor_encoder_create_map(&g_encoder, &root_map, CborIndefiniteLength);
          cbor_encode_text_string(&root_map, "state", strlen("state"));       \
          cbor_encode_boolean(&root_map, (val > 0));                        \
          cbor_encode_text_string(&root_map, "power", strlen("power"));       \
          cbor_encode_int(&root_map, val);                        \
          cbor_encoder_close_container(&g_encoder, &root_map);

          if (oc_do_put()) {
            return 0;
          }
          else local_log.concat("Could not send PUT request\n");
        }
        else local_log.concat("Could not init PUT request\n");
      }
    }
  }
  return -1;
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
int8_t Provisioner::attached() {
  if (EventReceiver::attached()) {
    return 1;
  }
  return 0;
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

  temp = opts->retrieveArgByKey("owner");
  if (temp) {
    char* buf;
    if (0 == temp->getValueAs(&buf)) {
      // We have an option passed for identity. Let's try to decode it...
      // Since we are decoding, the length of the result will always be smaller
      //   than the length of the input. So no guess-work here.
      int src_len = strlen(buf);
      uint8_t* cbor_buf = (uint8_t*) alloca(src_len);
      size_t result_len = 0;
      int ret = wrapped_base64_decode(
                  cbor_buf,
                  src_len,
                  &result_len,
                  (const uint8_t*) buf,
                  src_len
                );
      if (0 == ret) {
        // We have a buffer to feed to CBOR. We expect this to be an OIC1.1
        //   cred entry sufficient to establish ownership.
        printf("B64 Decode succeeded.\n");
      }
      else {
        printf("B64 Decode failed.\n");
      }
    }
    else {
      printf("Failed to get base64 data.\n");
    }
  }
  opts->printDebug(&local_log);
  printf("Provisioner::erConfigure()\n%s\n", (const char*) local_log.string());

  flushLocalLog();
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
int8_t Provisioner::callback_proc(ManuvrMsg* event) {
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


int8_t Provisioner::notify(ManuvrMsg* active_event) {
  int8_t return_value = 0;
  Argument* args = active_event->getArgs();

  switch (active_event->eventCode()) {
    case MANUVR_MSG_OIC_DISCOVERY:
      if (args) {
        // Detach the Argument from the message. We need to keep it.
        if (0 == queryDeviceDoxm(active_event->takeArgs())) {
          local_log.concat("Discovered new device:\t");
          if (relayDiscovery()) {
            // TODO: Send to connected TCP clients.
          }
        }
        return_value++;
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


#if defined(MANUVR_CONSOLE_SUPPORT)
void Provisioner::procDirectDebugInstruction(StringBuilder *input) {
  char* str = input->position(0);
  char c    = *(str);

  if (0) {
  }
  else {
    switch (c) {
      case 'g':
        // Get a resource.
        local_log.concatf("Resource get %s.\n",
          (0 == _get_resource_by_idx(input->position_as_int(1))) ?
            "succeeded" : "failed"
        );
        break;
      case 'p':
        // Get a resource.
        local_log.concatf("Resource post %s.\n",
          (0 == _post_resource_by_idx(input->position_as_int(1), input->position_as_int(2))) ?
            "succeeded" : "failed"
        );
        break;

      default:
        EventReceiver::procDirectDebugInstruction(input);
        break;
    }
  }

  flushLocalLog();
}
#endif  // MANUVR_CONSOLE_SUPPORT
#endif  // OC_CLIENT && MANUVR_OPENINTERCONNECT
