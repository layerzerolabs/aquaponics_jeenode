aquaponics_jeenode
==================

Jeenode code for wireless aquaponics sensors.

code structure
--------------
The entire repo is intended to be uploaded to each jeenode in the system. This has a number of benefits over a more traditional approach of separate ino files for each sensor. It makes it very easy to mix and match sensors - if you want to put two or three in a box, fine, just use the jeenode_configurator to tell it what is on each port and some settings, and you're away. Then there are development benefits. One is that only on ino file needs to be maintained. Another is that it forces each sensor class to conform to a standard interface. As a result, this code is actually a primitive framework. You can add classes for additional sensors very easily.

The disadvantage, of course, is upload size - but we haven't found that to be a problem.

configuration
-------------

The current system is to have a configurator sketch that sets parameters such as the sensors connected, the ports/pins used and the node id.

These parameters don't change during the lifetime of the sensors.

After setting these parameters, the main sketch is loaded, and this runs forever.

Some pH configuration (calibration) is now done in mode-red - see http://l0l.org.uk/2014/10/ph-calibration-node-red/

This should be regarded as beta code under active development.

We welcome feature request, bug reports, feedback of all sorts etc.
