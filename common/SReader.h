#ifndef _SREADER_H
#define _SREADER_H

#include "SMat.h"
#include "SGuard.h"
#include "SThread.h"
#include "SVehicle.h"

#include <fstream>
#include <string.h>

/* error codes returned by Reader::Step and Reader::OpenFile */

/* EOF is reached, no error */
#define SREADER_EOF 2ul

/* EOF is not expected */
#define SREADER_ERROR_UNEXPECTED_EOF 3ul

/* File not found */
#define SREADER_ERROR_FILE_NOT_FOUND 4ul

/* Filename not set */
#define SREADER_ERROR_FILE_NOT_SET 5ul

/* NED position cannot be calculated */
#define SREADER_ERROR_NOT_CALCULATED_POSITION 6ul

/* Cannot change the file when running */
#define SREADER_ERROR_RUNNING 7ul

//! Reader
class SReader : public SVehicle {
private:
	std::ifstream f_input;
//! Reference position for ECEF (the first point)
	SVec Reference;
//! Reference time (the first point time), sec
	double ReferenceTime;
//! Transformation matrix ECEF->NED, calculated only for the reference position
	SMat TransformMatrix;
public:
	ERROR_CODE Step(double* pTime, bool bFirst);
	SReader(VehicleState * p_state) : SVehicle(p_state) {
		Reference = SVec(0.0, 0.0, 0.0);
		TransformMatrix = SMat(0.0, 0.0, 0.0,
					0.0, 0.0, 0.0,
					0.0, 0.0, 0.0);
	}
	~SReader() {
		if (f_input.is_open()) f_input.close();
	}

	ERROR_CODE OpenFile(const char* filename);
};

#endif
