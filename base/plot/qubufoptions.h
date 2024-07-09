#ifndef QUBUFOPTIONS_H
#define QUBUFOPTIONS_H

class QuBuf_O {
public:
    QuBuf_O() : xb_auto(true), yb_auto(false),
        xlb{0.0}, xub{1000.0}, ylb(0.0), yub{1000.0} {}

    bool xb_auto, yb_auto;
    double xlb, xub, ylb, yub;

    void xauto(bool a) { xb_auto = a; }
    void yauto(bool a) { yb_auto = a; }

    void xlow(double l) { xlb = l; xb_auto = false; }
    void xupp(double u) { xub = u; xb_auto = false; }

    void ylow(double l) { ylb = l; yb_auto = false; }
    void yupp(double u) { yub = u; yb_auto = false; }
};

#endif // QUBUFOPTIONS_H
