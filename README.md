# Neudev and LostPuppy
A pair of programs for OIC provisioning workflows.

LostPuppy is an OIC device that is discoverable and unprovisioned. It exhibits the "just works" provisioning work-flow.

Neudev is a tool for onboarding devices, and debugging low-level IoT issues.

----------------------

#### What is in this repository:
**./lib**:  Third-party libraries required for a project.

**./src**:  The source for Neudev and LostPuppy.

**./downloadDeps.sh**   A script to download build dependencies.

**./Dockerfile**   Docker build of LostPuppy.



----------------------

## Building

#### Neudev
Debugging is presently a necessary build option to do anything useful with Neudev.

    ./downloadDeps.sh
    make OIC_CLIENT=1 DEBUG=1

#### LostPuppy
Debugging is presently a necessary build option.

    ./downloadDeps.sh
    make lostpuppy OIC_SERVER=1 DEBUG=1

LostPuppy is also the purpose of the Dockerfile, which will build a ready-to-run instance of LostPuppy, along with all the UDP/TCP port breakouts required to talk to it.

    docker build .


----------------------

## Usage
The provisioning tool is named neudev. It can be used in interactive mode this way.

    ./neudev --console

Alternatively, it can be wrapped in an init script and used as a system service...

    ./neudev &2 > 1

In either case, it will open a TCP listening socket on 0.0.0.0:5685. Connecting to this socket will allow an external program to get updates about changes in the IoT environment.

The interactive shell is due for rework, and so documentation on it will not be included here.

----------------------
## License
Original code is covered by LICENSE.

Code adapted from others' work inherits their license terms, which were preserved in the commentary where it applies.

This program
