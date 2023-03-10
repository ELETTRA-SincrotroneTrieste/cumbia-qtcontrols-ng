# cumbia svg library documentation

### Use Qt SVG module to show and animate SVG drawings
The <a href="https://doc.qt.io/qt-5/qtsvg-module.html">Qt SVG C++ module</a> provides functionality for handling SVG images. The *cumbia* engines and
infrastructures can be used to animate any SVG element, grouped in user defined QGraphicsSVGItem objects.

### Connect with the Tango and Epics (and more...) control system software
SVG elements in the drawing can be *connected* to values obtained from the available *cumbia* engines and their properties changed accordingly.
In several cases, the connections defined and the type of attributes in the SVG elements allow for automatic changes in the representation of
the object within the drawing. In more complex ones, the programmer will map values from the engines to values of the *attributes* in the SVG
DOM document.

### Helper application support
Helper applications can be defined by the *helper* attribute in any item. As an alternative, they can be deduced from the source connected to the element, if
a single one is defined (and if the engine in use supports this feature, e.g. Tango does).

### Item event handling
Events on an item are notified by Qt *signals*. This includes clicks (left button and contextual menu events). Clicks can target a *write operation*, while contextual
menus will by default make available *helper applications* (if defined) and pop up dialogs to perform slightly more complex writings (for example, write a scalar number
or change a text value on the engine)



## Installation

### Dependencies

The library relies on *PKGCONFIG* to find the needed dependencies, that are
- cumbia
- cumbia-qtcontrols
Please make sure that the PKG_CONFIG_PATH variable include the cumbia installation prefix, for instance:

> echo $PKG_CONFIG_PATH
> /usr/local/epics/base-7.0.2/lib/pkgconfig:/usr/local/tango-9.3.3/lib64/pkgconfig:/usr/local/omniorb-4.2.3/lib/pkgconfig:/usr/local/zeromq-4.1.7/lib/pkgconfig:/usr/local/qwt-6.1.4/lib/pkgconfig:*/usr/local/cumbia-libs/lib/pkgconfig*


On the Qt side, these are the required modules:
- widgets
- xml
- svg
- opengl

### Download build and install

> git clone https://github.com/ELETTRA-SincrotroneTrieste/qumbia-svg.git
> cd qumbia-svg
> qmake && make && make install

### Options
You can either edit qumbia-svg.pri to change the *INSTALL_ROOT* installation prefix or pass the *INSTALL_ROOT=path/to/qumbia-svg* to qmake:

> qmake INSTALL_ROOT=/usr/local/qumbia-svg

#### Default prefix

The default prefix is */usr/local/cumbia-libs* (includes in */usr/local/cumbia-libs/include/qumbia-svg/*, libs in */usr/local/cumbia-libs/lib/* and so forth)

### Important advice

Please make sure the installation prefix is the same as that used for the *cumbia* library. This ensures proper library interoperability.

### Are you ready?

Start reading the <a href="https://elettra-sincrotronetrieste.github.io/qumbia-svg/">cumbia svg library documentation</a>.

# cumbia-qtcontrols-ng
