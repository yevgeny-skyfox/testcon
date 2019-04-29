#include "SSimulator.h"
#include "SCoord.h"
#include "SSysTime.h"
#include "SThread.h"

#include <stdlib.h>
#include <math.h>


ERROR_CODE SSimulator::Step(double* pTime, bool bFirst) {
	if (bFirst) {
		/* initialize state of the model to some default values */
		
		/* Virtual time step */
		VirtualStepTime = 0.005;
		/* step of integration (dt) should be set too,
		   we use the number of iterations per virtual step.
		*/
		nIterationsOfIntegrationPerStep = 100;
		Length = 2.5;
		ModelState.Position = SVec(0.0, 0.0, 0.0);
		ModelState.Time = 0.0;
		ModelState.Theta = M_PI / 4.0;
		ModelState.Delta = 0.0;
		ModelState.Velocity = 1.0;
	} else {
		double IntegrationStepTime = VirtualStepTime / nIterationsOfIntegrationPerStep;
		
		for (int i = 0; i<nIterationsOfIntegrationPerStep; i++) {
			double dx = IntegrationStepTime * ModelState.Velocity * cos(ModelState.Theta);
			double dy = IntegrationStepTime * ModelState.Velocity * sin(ModelState.Theta);
			double d_theta = IntegrationStepTime * ModelState.Velocity * tan(ModelState.Delta) / Length;
			ModelState.Position += SVec(dx, dy, 0);
			ModelState.Theta += d_theta;
		}
		ModelState.Time += VirtualStepTime;
		
	}
	if (pTime) *pTime = ModelState.Time;
	
	/* update the state (common parameters) */
	SetState(&ModelState);
	return VEHICLE_OK;
}

