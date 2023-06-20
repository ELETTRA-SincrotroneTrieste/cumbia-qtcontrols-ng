# new generation cumbia-qtcontrols


## Installation

### Dependencies

Qt libraries

### Download build and install

> git clone https://github.com/ELETTRA-SincrotroneTrieste/cumbia-qtcontrols-ng.git
> cd cumbia-qtcontrols-ng
> qmake && make && make install

### Options
You can either edit cumbia-qtcontrols-ng.pri to change the *INSTALL_ROOT* installation prefix or pass the *INSTALL_ROOT=path/to/qumbia-svg* to qmake:

> qmake INSTALL_ROOT=/my/custom/dir

#### Default prefix

The default prefix is */usr/local/cumbia-libs* (includes in */usr/local/cumbia-libs/include/cumbia-qtcontrols-ng/*, libs in */usr/local/cumbia-libs/lib/* and so forth)

### Important advice

Please make sure the installation prefix is the same as that used for the *cumbia* library. This ensures proper library interoperability.

### Are you ready?

Start reading the <a href="https://elettra-sincrotronetrieste.github.io/cumbia-qtcontrols-ng/">cumbia svg library documentation</a>.

# cumbia-qtcontrols-ng
