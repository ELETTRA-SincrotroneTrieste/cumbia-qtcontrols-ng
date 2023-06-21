# new generation cumbia-qtcontrols

The new generation of *cumbia controls* focuses on the design of *high level software*, offering 
components for custom-tailored data visualization as well as for synoptic applications


## architecture

*cumbia-qtcontrols-ng* components shall be designed to be:

- scalable 
- integrated into SVG (see https://gitlab.com/ELETTRA-SincrotroneTrieste/qumbia-svg)
- highly customizable

QGraphicsItem components in a QGraphicsScene perfectly fit these requirements. Notwithstanding,
classical widget technology is supported. 
A common paint and configuration engine is shared between QWidget and QGraphicsItem implementations
of each component:

![architecture diagram for cumbia-qtcontrols-ng components.](artwork/class-diagram.png)


## components

### cartesian plots

The *qgraphicsplot*, formerly implemented in the namesake library, has become part of *cumbia-qtcontrols-ng*.
It offers a cartesian QGraphicsItem plot with its own zoomer along with the natural scaling of the items in 
a scene. See QGraphicsPlotItem.

![QGraphicsPlotItem](artwork/qgraphicsplot.png)


### apply numeric

The QuNumericI is a QGraphicsObject that can be used to set a scalar value on a target
The QWidget counterpart looks and does exactly the same thanks to a common engine

![QuNumericW on the left and QuNumericI, the latter in a QGraphicsScene](artwork/qunumeric-1.png)


### circular plots

Circular plots draw curves along a circular x axis and a radial y axis.
A QuCircularPlotEngine is used to store plot data and draw its contents.
The engine is common to both QWidget based and QGraphicsItem based implementations, namely
QuCircularPlotW and QuCircularPlotI.
According to the nature of your application and the preferred technology, both provide
identical behaviour through QuCircularPlotEngine.

QuCircularPlotEngine is itself a QObject rich in methods, signals and slots.
QuCircularPlotEngine stores the options of the plots (axis autoscale, bounds, colors)
through a dedicated structure named qucircularplot_attributes.
QuCircularPlotCurve represents a curve in the plot. There can be several curves in a single
plot. Each curve is identified by a name.


![Four QuCircularPlotItem instances in a QGraphicsScene. One is zoomed.](artwork/circularplots.png)


At the [ELETTRA synchrotron radiation facility](https://www.elettra.eu) circular plots are used to represent
values along the storage ring circumference. In the picture below, an additional *drawable* layer divides the
ring into the 12 sections that make up the ring.
