/*
File:   LostPuppy.h
Author: J. Ian Lindsay
Date:   2016.09.10

*/


#ifndef __NEUSTAR_IOT_LOST_PUPPY_H__
#define __NEUSTAR_IOT_LOST_PUPPY_H__

#include <Platform/Platform.h>
#include <Frameworks/OIC/ManuvrOIC.h>


class LostPuppy : public EventReceiver {
  public:
    LostPuppy();
    LostPuppy(Argument*);
    ~LostPuppy();

    /* Overrides from EventReceiver */
    void procDirectDebugInstruction(StringBuilder*);
    void printDebug(StringBuilder*);
    int8_t notify(ManuvrMsg*);
    int8_t callback_proc(ManuvrMsg*);
    int8_t erConfigure(Argument*);

    inline bool ownerSet() {  return (nullptr != _owner);   };


  protected:
    int8_t attached();


  private:
    Identity* _owner   = nullptr;
    oc_resource_t *res = nullptr;
};

#endif   // __NEUSTAR_IOT_LOST_PUPPY_H__
