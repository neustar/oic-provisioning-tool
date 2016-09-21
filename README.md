
[![CircleCI](https://circleci.com/gh/datasnap-io/oic-provisioning-tool.svg?style=shield&circle-token=a77e536ac8c03e8b93b7a4c72e2d2d5480add75f)](https://circleci.com/gh/datasnap-io/oic-provisioning-tool)


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

Regardless of tool, you can pass the DEBUG build flag to make...

    make DEBUG=1

...to enable more console output and other less-common runtime features.    

#### Neudev

    ./downloadDeps.sh
    make OIC_CLIENT=1

#### LostPuppy

    ./downloadDeps.sh
    make lostpuppy OIC_SERVER=1

Or, to build for Raspberry Pi (LED should be connected to GPIO pin 14):

    ./downloadDeps.sh
    make lostpuppy OIC_SERVER=1 BOARD=RASPI


LostPuppy is also the purpose of the Dockerfile, which will build a ready-to-run instance of LostPuppy, along with all the UDP/TCP port breakouts required to talk to it.

    docker build .

----------------------

## Usage
The provisioning tool is named neudev. The device is named ./LostPuppy. Their invocation rules are the same. They can be used in interactive mode this way.

    ./neudev --console

Alternatively, they can be wrapped in an init script and used as a system service...

    ./neudev > /path/to/logfile

In either case, they will open a TCP listening socket on 0.0.0.0:5685. Connecting to this socket will allow an external program to get updates about changes in the IoT environment.

Specific usage information [is given here](doc/HOWTO.md).



----------------------
## Bugs, contributions, and point-of-contact

This program was written by J. Ian Lindsay for Neustar's IoT department. Please direct non-problem inquiries to ian.lindsay@neustar.biz. For bugs, please use github's issue-tracker.

If you wish to publish a fix, improvement, or extension. Please fork, modify, and submit a PR.

When you contribute code, you affirm that the contribution is your original work and that you license the work to the project under the project's open source license. Whether or not you state this explicitly, by submitting any copyrighted material via pull request, email, or other means you agree to license the material under the project's open source license and warrant that you have the legal authority to do so.


----------------------
## License
Original code is covered by LICENSE.

Code adapted from others' work inherits their attributions and license terms (if they differ from LICENSE), which were preserved in the commentary where it applies.
