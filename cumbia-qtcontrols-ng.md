# new generation cumbia-qtcontrols

The new generation of *cumbia controls* focuses on the design of *high level software*, offering 
components for custom-tailored data visualization as well as for synoptic applications



Circular plots draw curves along a circular x axis and a radial y axis.
A QuCircularPlotEngine is used to store plot data and draw its contents.
The engine is common to both QWidget based and QGraphicsItem based implementations, namely
QuCircularPlotWidget and QuCircularPlotItem.
According to the nature of your application and the preferred technology, both provide
identical behaviour through QuCircularPlotEngine.
QuCircularPlotEngine is itself a QObject rich in methods, signals and slots.
QuCircularPlotEngine stores the options of the plots (axis autoscale, bounds, colors)
through a dedicated structure named qucircularplot_attributes.
QuCircularPlotCurve represents a curve in the plot. There can be several curves in a single
plot. Each curve is identified by a name.
