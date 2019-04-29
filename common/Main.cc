#include <stdio.h>
#include <stdlib.h>
#include "SReader.h"
#include "SSimulator.h"
#include "SSysTime.h"
#include "SCoord.h"
#include <iostream>

static void AssertReturnCode(int ret) {
	switch (ret) {
		case VEHICLE_OK: break;
		case VEHICLE_UNKNOWN_CPP_EXCEPTION: throw "Unclassified cpp exception in vehicle thread";
		case SREADER_ERROR_UNEXPECTED_EOF: throw "unexpected end of input file";
		case SREADER_ERROR_FILE_NOT_FOUND: throw "input file not found";
		case SREADER_ERROR_FILE_NOT_SET: throw "file name should be set";
		case SREADER_ERROR_NOT_CALCULATED_POSITION: throw "cannot calculate NED position";
		case SREADER_ERROR_RUNNING: throw "cannot change source when running";
	}
}

int main (int argc, char** argv) {

	try	{
		/* allocate common structure for vehicle parameters */
		struct VehicleState* pState = new VehicleState;
		SVehicle* pVehicle;
		SReader* pReader = NULL;
		SSimulator* pSimulator = NULL;
		int ret = VEHICLE_OK;
		
		if (pState == NULL) {
			throw "cannot allocate memory for struct VehicleState";
		}
		SGuard* pGuard = new SGuard;
		if (pGuard == NULL) {
			throw "cannot allocate memory for SGuard";
		}
		pState->pGuard = pGuard;
		
		if ((argc >= 2) && (argv[1] != NULL)) {
			/* Reader mode */
			pReader = new SReader(pState);
			pVehicle = dynamic_cast<SVehicle*>(pReader);
		
			if (pVehicle == NULL) {
				throw "cannot allocate memory for SReader";
			}
			ret = pReader->OpenFile(argv[1]);
			AssertReturnCode(ret);
		} else {
			/* Simulation mode */
			pSimulator = new SSimulator(pState);
			pVehicle = dynamic_cast<SVehicle*>(pSimulator);
			
			if (pVehicle == NULL) {
				throw "cannot allocate memory for SSimulator";
			}
		}
		
		pVehicle->Start();
		
		/* info Window (currently use terminal) */
		
		struct VehicleState* p = new VehicleState;
		while (pVehicle->isRunning) {
		
			pVehicle->GetState(p);
			printf("Time=%.3f sec, x=%.3f m, y=%.3f m, V=%.3f m/sec Theta=%.3f (deg), Stearing=%.2f (deg)\n",
			p->Time, p->Position.x0, p->Position.x1, p->Velocity, Degrees(p->Theta), Degrees(p->Delta));
			
			SysTimeSleep(100);
		}
		
		ret = pVehicle->Stop();
		AssertReturnCode(ret);
		
		if (pReader) delete pReader;
		if (pSimulator) delete pSimulator;
		
		delete pGuard;
		delete p;
		delete pState;
	
	} catch(const char* str) {
		std::cout << "Exception: " << str << std::endl;
		return 1;
	} catch(...) {
		std::cout << "Unclassified exception" << std::endl;
		return 1;
	}
	
	return VEHICLE_OK;
	
}
