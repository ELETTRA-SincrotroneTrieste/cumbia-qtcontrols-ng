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

#### quscalarplot and quarrayplot

Two implementations of *QwtPlot*, one tailored for scalar variables over time, the other for spectrum
data, offering substantially only two shortcuts, one for adding curves, one either for appending (scalar) 
or setting (array) data and a few signals on data updated, on error and on curve added.
Less than one hundred lines the array version and about 150 the scalar version, the implementations
are minimalistic in order to guarantee no overhead with respect to base QwtPlot and at the same time
ensure immediate debugging and benchmark comparison with QwtPlot.

The QuScalarPlot uses a *circular buffer* to store data. This means that the user must decide in 
advance how much data needs to be displayed on the chart. Convenience duration values, defined in
QuCircularBuf::Duration enum can be used to initialize the *scalar plots*.

The QuArrayPlot uses a QuArrayBuf buffer, that offers one method to *set* (copying) y data and one
to *move* y data, according to the needs of the client.
QuArrayPlot::setData uses the *move* semantics.

Additional functionality (i.e. curve search and selection, zooming and chart configuration) is added
by means of dedicated objects that can be *installed* on the plot. A list of relevant classes follows:

1. QuCurves, manage and search curves attached to the plot
2. QuCurveSelector, select a curve on the plot with the mouse
3. QuPlotOptions, configure plot options (X and Y bounds, axes autoscale, openGL mode, ...)
4. QuPlotZoomer, zoom a plot area with the mouse
5. QuXTimeScale, install a time (with optional date) scale draw on the X axis



#### Former qgraphicsplot

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
