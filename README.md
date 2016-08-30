# oic-provisioning-tool
A program to provision OIC devices

----------------------

#### What is in this repository:
**./doc**:  Location for generated documentation.

**./Docker**:  Docker files to generate optional build environments.

**./lib**:  Third-party libraries required for a project.

**./tests**:  Automated tests.

**./examples**:  Example projects.

**./buildEnvironments.sh**   A script to build the optional environments.

**./downloadDeps.sh**   A script to download build dependencies.

----------------------

## Building (everything)

    make

##### Or with debug symbols:

    make debug

##### Automated tests:

    make tests

##### OCF devices:
To test the various device provisioning schemes, I've built some proto-devices
that have trivial functionality, but full security.

  * lost-puppy       // "Just-works"

    make devices


----------------------

## Usage
The provisioning tool is named neudev.

    ./neudev --help

----------------------
## License
Original code is covered by LICENSE.

Code adapted from others' work inherits their license terms, which were preserved in the commentary where it applies.
