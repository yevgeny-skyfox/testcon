#ifndef _SVEHICLE_H
#define _SVEHICLE_H

#include "SMat.h"
#include "SGuard.h"
#include "SThread.h"
#include <stdio.h>
#include <atomic>

/* common error codes returned by Vehicle::Step */
/* its' ok */
#define VEHICLE_OK  0l

/* there are many things in c++ that may raise exceptions... */
#define VEHICLE_UNKNOWN_CPP_EXCEPTION 1l

//! \brief current state of the vehicle (reader, simulation, hw sensors, etc...) \n
struct VehicleState {
	double Time;		// current relative time in sec
	double Theta;		// azimuth, rad
	double Delta;		// steering angle in body frame (wheel), rad
	double Velocity;	// velocity m/sec
	SVec Position;		// relative position (NED)
	SGuard* pGuard;		// pointer to common syncronization object
	VehicleState() {
		Time = Theta = Delta = Velocity = 0.0;
		Position = SVec(0.0, 0.0, 0.0);
		pGuard = NULL;
	}
};

//! \brief An abstract of Vehicle (Reader and Simulator are derived from this)
class SVehicle {
protected:
	VehicleState *m_p_state;
	SThread* m_pThread;
public:
	std::atomic<bool> isRunning;
	bool bStartFromBegin;

//! What to do in every virtual time step. Use bFirst = true for initial step (from beginning of the file or initial model state)
	virtual ERROR_CODE Step(double* pTime, bool bFirst) = 0;

	SVehicle(VehicleState * p_state) {
		m_p_state = p_state;
		m_pThread = new SThread();
		if (!m_pThread) throw "m_pThread assertion error";
		isRunning = false;
		bStartFromBegin = true;
	}
	virtual ~SVehicle() {
		if (m_pThread) {
			delete m_pThread;
			m_pThread = NULL;
		}
	}
	void SetState(VehicleState * p_state) {
		m_p_state->pGuard->Lock();
		m_p_state->Time = p_state->Time;
		m_p_state->Theta = p_state->Theta;
		m_p_state->Delta = p_state->Delta;
		m_p_state->Velocity = p_state->Velocity;
		m_p_state->Position = p_state->Position;
		m_p_state->pGuard->Unlock();
	}
	
	void GetState(VehicleState * p_state) {
		m_p_state->pGuard->Lock();
		p_state->Time = m_p_state->Time;
		p_state->Theta = m_p_state->Theta;
		p_state->Delta = m_p_state->Delta;
		p_state->Velocity = m_p_state->Velocity;
		p_state->Position = m_p_state->Position;
		m_p_state->pGuard->Unlock();
	}

	void Start(bool bBegin = true);
	void Resume();
	ERROR_CODE Stop();
};

#endif
