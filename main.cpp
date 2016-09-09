/*
File:   main.cpp
Author: J. Ian Lindsay
Date:   2016.08.29

neudev application.
*/

#include <XenoSession/Console/ManuvrConsole.h>
#include <Transports/ManuvrSocket/ManuvrTCP.h>
#include <Transports/StandardIO/StandardIO.h>

#include <Platform/Platform.h>
#include <Kernel.h>

#define VERSION_STRING "0.0.1"

/* This global makes this source file read better. */
Kernel* kernel = nullptr;

char* program_name = nullptr;

/*******************************************************************************
* Functions that just print things.                                            *
*******************************************************************************/

/*******************************************************************************
* The main function.                                                           *
*******************************************************************************/
int main(int argc, char *argv[]) {
  int   main_pid     = getpid();  // Our PID.
  program_name = argv[0];   // Name of running binary.

  platform.platformPreInit();
  kernel = platform.kernel();

  // Parse through all the command line arguments and flags...
  // Please note that the order matters. Put all the most-general matches at the bottom of the loop.
  for (int i = 1; i < argc; i++) {
    if ((strcasestr(argv[i], "--version")) || ((argv[i][0] == '-') && (argv[i][1] == 'v'))) {
      // Print the version and quit.
      printf("%s v%s\n\n", argv[0], VERSION_STRING);
      exit(0);
    }
    if ((strcasestr(argv[i], "--console")) || ((argv[i][0] == '-') && (argv[i][1] == 'c'))) {
      // The user wants a local stdio "Shell".
      #if defined(__MANUVR_CONSOLE_SUPPORT)
        // TODO: Until smarter idea is finished, manually patch the transport
        //         into a console session.
        StandardIO* _console_xport = new StandardIO();
        ManuvrConsole* _console = new ManuvrConsole((BufferPipe*) _console_xport);
        kernel->subscribe((EventReceiver*) _console);
        kernel->subscribe((EventReceiver*) _console_xport);
      #else
        printf("%s was compiled without any console support. Ignoring directive...\n", argv[0]);
      #endif
    }
  }

  platform.bootstrap();

  /*
    Workflow:
    TODO: The tool will be handed an IP/port, and a set of acceptable credentials to set for ownership.
    -----------------------
    TODO: The OBT queries to see if the new device is not yet owned.
    TODO: The new device returns the /oic/sec/doxm resource containing ownership, supported owner transfer methods and supported credential types.
    TODO: The OBT selects the ‘Just Works’ method.
    TODO: The OBT also queries to determine if the device is operationally ready to transfer device ownership.
    TODO: The OBT asserts that it will follow the Client-directed provisioning convention.
    TODO: A DTLS session is established using anonymous Diffie-Hellman. Note: This method assumes the operator is aware of the potential for man-in-the-middle attack and has taken precautions to perform the method in a clean-room network.
    TODO: The OBT asserts itself as the owner of the new device and requests device owned status to be changed to TRUE.
    TODO: If symmetric credential type is selected: The OBT uses a pseudo-random-function (PRF) and other information to generate a symmetric key credential - OwnerPSK.
    TODO: If symmetric credential type is selected: The OBT generates credential resource based on OwnerPSK and then sends the resource to the new device with empty “PrivateData” property value.
    TODO: If symmetric credential type is selected: The new device locally generates the OwnerPSK and put it to the “PrivateData” property of the credential resource created at step 19.
    TODO: If asymmetric credential type is selected: The OBT posts an asymmetric type credential resource with its pubic key (OC) to the new device. It may be used subsequently to authenticate the OBT. The new device creates an credential resource based on the public key generated on step 24 - OC.
    TODO: If asymmetric credential type is selected: The OBT reads the new device’s asymmetric type credential resource generated at step 25. It may be used subsequently to authenticate the new device.
    TODO: If certificate credential type is selected: The OBT posts a certificate type credential resource with its certificate to the new device. It may be used subsequently to authenticate the OBT.
    TODO: If certificate credential type is selected: The new device creates an asymmetric key type credential resource based on the public key generated at step 30.
    TODO: If certificate credential type is selected: The OBT reads the new device’s asymmetric type credential resource to issue a certificate.
    TODO: If certificate credential type is selected: The OBT generates a new device’s certificate and signs the certificate with a CA key. The OBT creates a certificate type credential resource with the signed certificate and posts it to the new device.
    TODO: If OC is determined, the temporal secure session is closed and a new DTLS session using the OC is re-established. If it fails to establish the secure session with OC, it treats as an authentication error.
    TODO: The OBT creates an entry in the new device’s /oic/sec/svc resource that identifies the OBT service.
    TODO: The new device changes the /oic/sec/doxm. Owned status to TRUE and refuses to accept
    TODO: requests to perform ownership transfer methods. The OBT accepts the new device into its database of ‘owned’ devices.
    TODO: The new device is now on Ready-for-Provisioning state.
    TODO: The new device is now on Ready-for-Normal-Operation state.
    TODO: Close the DTLS session.
  */


  printf("%s (PID %u): Starting...\n", program_name, main_pid);
  #if defined(RASPI) || defined(RASPI2)
    gpioDefine(14, OUTPUT);
  #endif

  while (1) {
    kernel->procIdleFlags();
  }
}
