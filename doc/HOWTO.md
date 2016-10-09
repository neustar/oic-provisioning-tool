## Command-line parameters

##### Both programs accept...

    --version                // Print version and exit.
    --console                // Start a console session on standardIO.
    --store_path <path>      // Where is the persisted data located? No default.
    --discovery_period  <ms> // How often will will ping the world? Default: 120s
    --discovery_timeout <ms> // How long is our discovery window open? Default: 30s
    --write_conf_and_exit    // Writes the given config as the new default.

##### Neudev-specific parameters

    --relay_devs             // Relay news of new devices, claimed or not.
    --owner                  // Owner info as (Base64 (CBOR (OIC cred))).


## Console Usage

These instructions apply to both programs. neudev will be used as example.


### Tutorial
    you@YOURBOX ~/oic-provisioning-tool $ ./neudev --console
    ./neudev> i
...will issue a list of firmware modules that are loaded and active. This list
is a general reflection of the capabilities of the running firmware. My build looks like this (I have annotated with commentary):

    ==< Kernel >========================  // The module's header.
    -- Booted               yes       // The module is initialized.
    -- Queue depth:              0
    -- Preallocation depth:      30
    -- Prealloc starves:         0
    -- events_destroyed:         0    // These fields are load-metrics.
    -- burden_of_being_specific  0
    -- idempotent_blocks         0
    -- Subscribers: (7 total):
             0: Kernel           // The root of the program.
             1: OIC              // The module that wraps iotivity-constrained.
             2: LinuxStorage     // The storage driver.
             3: Console          // The console session you are engaged in.
             4: StandardIO       // The transport that moves data to/from your session.
             5: Provisioner      // The module that handles provisioning logic.
             6: ManuvrTCP        // The TCP server for reporting events.

This constellation of modules is what makes this program what it is. The module at index 5 is what differentiates it from LostPuppy. LostPuppy will have a different module, with different options, that reacts to events in the other modules differently.

The console gives commands to modules by index, with index 0 being the default. IE, these two commands have the same effect.

    ./neudev> i
    ./neudev> 0 i

You can then...

    ./neudev> 6 i

    ==< ManuvrTCP >===================================
    -- Booted               yes
    -- Pipe: ManuvrTCP [ TERMINUS ]
    --
    -- stream-oriented transport
    --
    -- _xport_flags    0x98200000
    -- bytes sent      0
    -- bytes received  0
    --
    -- initialized     yes
    -- connected       no
    -- listening       yes
    -- _addr           0.0.0.0:5685
    -- _options        0x00000000
    -- _sock           0x00000006

And in this manner, carry out a dialog with the firmware. I know. It's awful. And it's due for some re-work. But since it has to live in small spaces, it isn't likely to ever be plush.

Flash is far-less valuable than RAM. So these commands are typically hard-coded in functions with signatures like this (should anyone decide to go read from the scrolls before the re-work is completed):

    void Provisioner::procDirectDebugInstruction(StringBuilder *input);

-------------

### Some quick commands...

*Note: Some of these will only be available if DEBUG=1 was passed at compile-time.*

All modules will have these targets (where *x* is the module's index)...

    ./neudev> <x> i      // Info
    ./neudev> <x> v      // Print verbosity. 0 is silent. 7 is maximum chatty.
    ./neudev> <x> v 3    // Set verbosity to 3.


##### The Kernel

    ./neudev> i3       // Platform information.
    ./neudev> i4       // Cryptographic support details.
    ./neudev> i5       // Scheduled event information.
    ./neudev> b 128    // Reboot (exit process on linux)
    ./neudev> B 128    // Firmware flash (exit process on linux)
    ./neudev> quit     // Exit process on linux.

##### OIC module

    ./neudev> <x> D    // Begin discovery (client) / Make discoverable (server).
    ./neudev> <x> d    // Stop discovery (client) / Make undiscoverable (server).
