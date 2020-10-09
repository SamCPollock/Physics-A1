#pragma once
#ifndef __SIMULATION_STATS__
#define __SIMULATION_STATS__
#include <glm/glm.hpp>

/** This class contains all the variables and stats necessary to run a simulation for range of a projectile across a flat, non-angled surface with uniform downward gravity and no friction.
* 
*/
class SimulationStats
{
public:
	enum class variable { range, speed, angle, gravity, time };

public:
	// ---------- Utility Functions ----------
	SimulationStats();
	void ResetToDefaults();
	inline glm::vec2 calcVelocity(float speed, float angle) { return glm::vec2(glm::cos(glm::radians(angle)) * speed, glm::sin(glm::radians(angle)) * speed); }	// Obtain both velocity component vectors from the given speed and angle
	inline float calcAltAngle(float angle) { return (angle <= 90 ? 90 - angle : 180 + (90 - angle)); }

	// ---------- Physics Calculations ----------
	inline float calcRange(float speed, float angle, float gravity) { return ((glm::pow(speed, 2) * glm::sin(2 * glm::radians(angle))) / gravity); }	// R = (v^2 * sin(2a)) / g
	
	inline float calcAngle(float range, float speed, float gravity) { return glm::degrees(0.5 * (glm::asin((gravity * range) / glm::pow(speed, 2)))); }				// a = 1/2 * arcsin((gR) / v^2)
	inline float calcSpeed(float range, float angle, float gravity) { return (glm::sqrt(range * gravity / glm::sin(2 * glm::radians(angle)))); }		// v = sqrt(R * g / sin(2a))
	inline float calcGravity(float range, float speed, float angle) { return ((glm::pow(speed, 2) * glm::sin(2 * glm::radians(angle))) / range); }		// g = (v^2 * sin(2a)) / R
	inline float calcTime(float speed, float angle, float gravity)  { return ((2 * speed * glm::sin(glm::radians(angle))) / gravity); }					// t = (2v * sin(a)) / g

	// Mutator Functions
	void setVariable(variable variable);
	void setRange(float range);
	void setAngle(float angle);
	void setSpeed(float speed);
	void setGravity(float gravity);
	void setTime(float time);

	// Accessor Functions
	float getTimescale() { return timescale; }
	float getUnitScale() { return unitscale; }
	float getRange() { return range; }
	float getAngle() { return angle; }
	float getSpeed() { return speed; }
	float getGravity() { return gravity; }
	float getTime() { return time; }
	glm::vec2 getVelocity() { return velocity; }
	glm::vec2 getDisplacementAtTime(float time) { return glm::vec2((velocity.x * time), (velocity.y * time + 0.5f * -gravity * glm::pow(time, 2))); }
	
	float getAltTime() { return altTime; }
	glm::vec2 getAltVelocity() { return altVelocity; }
	glm::vec2 getAltDisplacementAtTime(float time) { return glm::vec2((altVelocity.x * time), (altVelocity.y * time + 0.5f * -gravity * glm::pow(time, 2))); }

public:
	bool		gravityEnabled;
	bool		showTrajectory;
	bool		showAltTrajectory;
	bool		showStats;
	variable	primaryVariable;
	variable	secondaryVariable;

private:
	// Scale Variables
	float		timescale; // simulationSeconds per realSsecond
	float		unitscale; // meters per pixel

	// Simulation Variables
	float		range;
	float		speed;
	float		angle;
	float		time;
	float		gravity;
	glm::vec2	velocity;

	float		altRange;
	float		altAngle;
	float		altTime;
	glm::vec2	altVelocity;

};

#endif // ! __SIMULATION_STATS__