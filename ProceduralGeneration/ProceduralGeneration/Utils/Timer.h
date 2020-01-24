#pragma once
#include <GLFW/glfw3.h>
#include <string>

class Timer
{
private:
	// Last calculated SDL_GetTicks
	float m_oldFrameTime;
	float m_newFrameTime;

	// Total frames rendered
	float m_frameCount;

	// The value of FPS
	float m_framesPerSecond;

	// Time between the frames that are rendered
	float m_frameTime;

	// Time between 2 frames
	float m_deltaTime;

	// Passed time
	float m_passedTime;

public:
	Timer();
	~Timer() {}
	void CalculateFrameRate();

	/**
	 * Getters and setters
	 */
public:
	inline float GetFrameRate() const { return this->m_framesPerSecond; }
	inline float GetDeltaTime() const { return this->m_deltaTime; }
	inline float GetPassedTime() const { return this->m_passedTime; }
};