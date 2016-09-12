/*
File:   Provisioner.h
Author: J. Ian Lindsay
Date:   2016.09.10

This is the asynchronous, automated tool to provision OIC devices.

*/


#ifndef __NEUSTAR_IOT_PROVISIONER_H__
#define __NEUSTAR_IOT_PROVISIONER_H__

#include <Platform/Platform.h>
#include <Frameworks/OIC/ManuvrOIC.h>

/*
* These state flags are hosted by the EventReceiver. This may change in the future.
* Might be too much convention surrounding their assignment across inherritence.
*/
#define OIC_PROV_FLAG_PROVISIONING     0x01    // Are we claiming devices?
#define OIC_PROV_FLAG_RELAY_DISCOVERY  0x02    // Should we relay new devices?
#define OIC_PROV_FLAG_RESERVED_5       0x04    //
#define OIC_PROV_FLAG_RESERVED_4       0x08    //
#define OIC_PROV_FLAG_RESERVED_3       0x10    //
#define OIC_PROV_FLAG_RESERVED_2       0x20    //
#define OIC_PROV_FLAG_RESERVED_1       0x40    //
#define OIC_PROV_FLAG_RESERVED_0       0x80    //


typedef struct {
  StringBuilder uri;
  bool  owned;
  bool  secured;
} KnownLocalDev;

class Provisioner : public EventReceiver {
  public:
    Provisioner();
    Provisioner(Argument*);
    ~Provisioner();

    /* Overrides from EventReceiver */
    void procDirectDebugInstruction(StringBuilder*);
    void printDebug(StringBuilder*);
    int8_t notify(ManuvrRunnable*);
    int8_t callback_proc(ManuvrRunnable *);
    int8_t erConfigure(Argument*);

    inline bool isProvisioning() {  return _er_flag(OIC_PROV_FLAG_PROVISIONING);    };
    inline bool relayDiscovery() {  return _er_flag(OIC_PROV_FLAG_RELAY_DISCOVERY); };

    inline bool ownerSet() {  return (nullptr != _desired_owner);   };


  protected:
    int8_t bootComplete();


  private:
    Identity* _desired_owner = nullptr;

    /* A map of things we've found nearby... */
    // TODO: Format for now is UUID/URI
    std::map<UUID*, KnownLocalDev*> _noticed_devs;


    inline void isProvisioning(bool nu) {  _er_set_flag(OIC_PROV_FLAG_PROVISIONING, nu);    };
    inline void relayDiscovery(bool nu) {  _er_set_flag(OIC_PROV_FLAG_RELAY_DISCOVERY, nu); };
};

#endif   // __NEUSTAR_IOT_PROVISIONER_H__
