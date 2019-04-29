#ifndef _SCOORD_H
#define _SCOORD_H

#include "SMat.h"

int ECEFToLLH(const SVec& ecef, SVec & llh);
void ECEFToNEDMatrix(const SVec & llh, SMat & RT);
double Degrees(double rad);

#endif
