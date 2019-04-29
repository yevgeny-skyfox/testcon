#include "SReader.h"
#include "SCoord.h"
#include "SSysTime.h"
#include "SThread.h"
#include <stdlib.h>

ERROR_CODE SReader::Step(double* pTime, bool bFirst) {
	SVec ecef, llh;
	struct VehicleState NewState;
	int i, err = SREADER_ERROR_UNEXPECTED_EOF;
	double t, theta;
	double ignored;
	
	if (!f_input.is_open()) return SREADER_ERROR_FILE_NOT_SET;
	
	if (bFirst) {
		/* set file pointer to the beginning of the file */
		f_input.seekg(0);
		/* if the first point cannot be read it is a critical error */
		if (!(f_input >> t)) return err;
	} else {
		/* if the next point cannot be read it is not critical, may be EOF */
		if (!(f_input >> t)) return SREADER_EOF;
	}
	
	t *= 0.001; /* time in ms -> time in sec */
	
	/* ECEF position (X,Y,Z) */
	if (!(f_input >> ecef.x0)) return err;
	if (!(f_input >> ecef.x1)) return err;
	if (!(f_input >> ecef.x2)) return err;
	
	/* Ignored */
	for (i=0; i<3; i++) if (!(f_input >> ignored)) return err;
	
	/* Velocity in m/sec */
	if (!(f_input >> NewState.Velocity)) return err;
	
	/* Ignored */
	for (i=0; i<2; i++) if (!(f_input >> ignored)) return err;
	
	/* Azimuth, convert to rad */
	if (!(f_input >> theta)) return err;
	NewState.Theta = M_PI * (theta / 180.0);
	
	/* Ignored */
	for (i=0; i<2; i++) if (!(f_input >> ignored)) return err;
	
	if (bFirst) {
		/* Set reference point to the first one */
		Reference = ecef;
		/* Set reference time to the first one */
		ReferenceTime = t;
		
		/* Convert the first ECEF position to LLH */
		if (ECEFToLLH(ecef, llh) < 0)
		    return SREADER_ERROR_NOT_CALCULATED_POSITION;
		
		/* Compute transformation matrix to NED (rotation) */
		ECEFToNEDMatrix(llh, TransformMatrix);
		
		/* Current NED position and time, zeroes */
		NewState.Time = 0;
		NewState.Position = SVec(0.0, 0.0, 0.0);
	} else {
		NewState.Position = TransformMatrix * (ecef - Reference);
		NewState.Time = t - ReferenceTime;
	}

	/* pTime pointer is used for realtime emulation to store current virtual time */
	if (pTime) *pTime = NewState.Time;
	
	/* Update common variables (state) */
	SetState(&NewState);
	return VEHICLE_OK;
}

// Open the file (file can be changed when no running)
ERROR_CODE SReader::OpenFile(const char* filename) {
	if (isRunning) {
		return SREADER_ERROR_RUNNING;
	}
	
	if (f_input.is_open()) {
		f_input.close();
	}
	
	f_input.open(filename, std::ios::in);
	
	// (re)open file (filename can be changed)
	if (!f_input.is_open()) {
		return SREADER_ERROR_FILE_NOT_FOUND;
	}
	
	return VEHICLE_OK;
}

