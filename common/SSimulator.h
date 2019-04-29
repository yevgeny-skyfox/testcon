#ifndef _SSIMULATOR_H
#define _SSIMULATOR_H

#include "SMat.h"
#include "SGuard.h"
#include "SThread.h"
#include "SVehicle.h"
#include <math.h>

//! \brief Tractor simulation model based of kinematic bicycle model.\n
class SSimulator : public SVehicle {
private:
//! Working copy of the model parameters (assumed that SSimulator and SReader cannot work at the same time)
	struct VehicleState ModelState;

//! Virtual time step in sec (in realtime simulation will be syncronized to system time)
	double VirtualStepTime;

//! DFE Integration step in terms of number of iterations per virtual time step
	int nIterationsOfIntegrationPerStep;

//! The distance between front and rear wheel creners (L), meters
	double Length;
public:
//! Step of the model.
	ERROR_CODE Step(double* pTime, bool bFirst);

//! Constructor
	SSimulator(VehicleState * p_state) : SVehicle(p_state) {
		/* initialize model */
		(void) Step(NULL, true);
	}
};

#endif
