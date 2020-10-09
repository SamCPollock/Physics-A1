#include "SimulationStats.h"

SimulationStats::SimulationStats()
{
	ResetToDefaults();
}

void SimulationStats::ResetToDefaults()
{
	primaryVariable = variable::range;
	secondaryVariable = variable::time;
	timescale = 1.0f;
	unitscale = 1.0f;
	gravityEnabled = true;
	showTrajectory = true;
	showAltTrajectory = true;
	showStats = true;

	range = 485.0f;
	angle = 15.8896f;
	speed = 95.0f;
	gravity = 9.8f;
	time = calcTime(speed, angle, gravity);
	velocity = calcVelocity(speed, angle);
	
	altAngle = 90 - angle;
	altRange = calcRange(speed, altAngle, gravity);
	altTime = calcTime(speed, altAngle, gravity);
	altVelocity = calcVelocity(speed, altAngle);
}

// Set all other variables, keeping the specified one fixed
void SimulationStats::setVariable(variable fixed_variable)
{
	switch(fixed_variable == primaryVariable ? secondaryVariable : primaryVariable)
	{
	case SimulationStats::variable::range:
		range = calcRange(speed, angle, gravity);
		altRange = calcRange(speed, altAngle, gravity);
		break;
	case SimulationStats::variable::angle:
		angle = calcAngle(range, speed, gravity);
		altAngle = 90 - angle;
		break;
	case SimulationStats::variable::speed:
		speed = calcSpeed(range, angle, gravity);
		break;
	case SimulationStats::variable::gravity:
		gravity = calcGravity(range, speed, angle);
		break;
	case SimulationStats::variable::time:
		time = calcTime(speed, angle, gravity);
		altTime = calcTime(speed, altAngle, gravity);
		break;
	default:
		throw;
	}

	time = calcTime(speed, angle, gravity);
	altTime = calcTime(speed, altAngle, gravity);
}

// Calling this function will set the given value, but will also set the predetermined variable value (either primary or secondary) in order to ensure all other variables remain constant.
void SimulationStats::setRange(float value)
{
	range = value;
	setVariable(variable::range);
}

// Calling this function will set the given value, but will also set the predetermined variable value (either primary or secondary) in order to ensure all other variables remain constant.
void SimulationStats::setAngle(float value)
{
	angle = value;
	altAngle = calcAltAngle(value);
	setVariable(variable::angle);
	velocity = calcVelocity(speed, angle);
	altVelocity = calcVelocity(speed, altAngle);
}

// Calling this function will set the given value, but will also set the predetermined variable value (either primary or secondary) in order to ensure all other variables remain constant.
void SimulationStats::setSpeed(float value)
{
	speed = value;
	setVariable(variable::speed);
	velocity = calcVelocity(speed, angle);
	altVelocity = calcVelocity(speed, altAngle);
}

// Calling this function will set the given value, but will also set the predetermined variable value (either primary or secondary) in order to ensure all other variables remain constant.
void SimulationStats::setGravity(float value)
{
	gravity = value;
	setVariable(variable::gravity);
}

// Calling this function will set the given value, but will also set the predetermined variable value (either primary or secondary) in order to ensure all other variables remain constant.
void SimulationStats::setTime(float value)
{
	time = value;
	setVariable(variable::time);
	altTime = calcTime(speed, altAngle, gravity);
}
