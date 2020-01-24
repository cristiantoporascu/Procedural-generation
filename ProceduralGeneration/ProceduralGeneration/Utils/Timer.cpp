#include "Timer.h"
#include <iostream>

Timer::Timer()
{
	this->m_frameTime = 0;
	this->m_deltaTime = 0;
	this->m_frameCount = 0;
	this->m_framesPerSecond = 0;
	this->m_passedTime = 0;
	this->m_oldFrameTime = 0;
	this->m_newFrameTime = glfwGetTime();
}

void Timer::CalculateFrameRate()
{
	// Save the last frametime for the next fpstick
	this->m_oldFrameTime = this->m_newFrameTime;

	// Store the current time
	this->m_newFrameTime = glfwGetTime();

	this->m_deltaTime = this->m_newFrameTime - this->m_oldFrameTime;

	this->m_passedTime += this->m_deltaTime;

	this->m_frameCount++;

	// ------------------FRAME PER SECONDS--------------------
	if ((this->m_newFrameTime - this->m_frameTime) > 0.5)
	{
		this->m_framesPerSecond = (this->m_frameCount / (this->m_newFrameTime - this->m_frameTime));
		this->m_frameTime = this->m_newFrameTime;
		this->m_frameCount = 0;
	}
	// ------------------FRAME PER SECONDS--------------------

	// If the last frame was rendered less than 1 ms ago, 
	// the detalaTime will be 0 ms. This causes problems in calculations, so sleep for 1ms to adjust.
	if (this->m_deltaTime == 0)
	{
		glfwSwapInterval(0.001);
		this->m_newFrameTime = glfwGetTime();
		this->m_deltaTime = this->m_newFrameTime - this->m_oldFrameTime;
	}
}