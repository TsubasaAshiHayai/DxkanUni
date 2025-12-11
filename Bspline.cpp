#include "Bspline.h"
#include <math.h>    


static void InitKnots(double* knots) {
    for (int i = 0; i < NUM_KNOTS; i++) {
        if (i <= K) knots[i] = 0.0;
        else if (i >= N) knots[i] = 1.0;
        else knots[i] = (double)(i - K) / (N - K);
    }
}


void InitKanEdge(KanEdge* edge) {
    InitKnots(edge->knots);

    for (int i = 0; i < N; i++) {
        edge->controlY[i] = 0.3 * sin(i * 0.5 * 3.14159);
    }
}

double BasisFunction(double* knots, int i, int k, double t) {
    if (k == 0) {
        if (knots[i] <= t && t < knots[i + 1]) return 1.0;
        if (t == 1.0 && knots[i + 1] == 1.0 && knots[i] < 1.0) return 1.0;
        return 0.0;
    }

    double w1 = 0.0, w2 = 0.0;

    if (knots[i + k] - knots[i] != 0)
        w1 = ((t - knots[i]) / (knots[i + k] - knots[i])) * BasisFunction(knots, i, k - 1, t);

    if (knots[i + k + 1] - knots[i + 1] != 0)
        w2 = ((knots[i + k + 1] - t) / (knots[i + k + 1] - knots[i + 1])) * BasisFunction(knots, i + 1, k - 1, t);

    return w1 + w2;
}

double Forward(KanEdge* edge, double x) {
    double y = 0.0;
    for (int i = 0; i < N; i++) {
        double b = BasisFunction(edge->knots, i, K, x);
        y += edge->controlY[i] * b;
    }
    return y;
}