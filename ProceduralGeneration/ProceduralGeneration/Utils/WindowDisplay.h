#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "Camera.h"
#include "Timer.h"
#include "Keyboard.h"
#include "../Objects/GeneratedObject.h"
#include "../Generation/Generator.h"
#include "../Objects/SkyBox.h"

class WindowDisplay
{
private:
	GLFWwindow* m_window;

	Timer* m_timer;
	Keyboard* m_keyboard;

	int m_widthScreen;
	int m_heightScreen;

	bool m_windowFocusedState = true;

	std::vector<GeneratedObjects*> generatedObjects;
	SkyBox* windowSkyBox;

public:
	WindowDisplay(int width, int height, const std::string& title);
	~WindowDisplay();

	/**
	 * Is used to declare the cover of all the systems for the event poll and
	 * the drawing of all the gameobjects declared inside the scene
	 */
	void UpdateWindow();

	/**
	 * Main function which is called at the start of the game
	 * executes all the other classes
	 */
	void DrawWindow();

	/**
	 * In case if the window has been resized it recreates the projection matrix
	 * for the camera and resets the sizes of the window
	 */
	void ResizeWindow();

	/**
	 * Checks every frame whether the window is focused or not
	 * and stops drawing anything on the window if it isn't
	 */
	void FocusedWindow();

	/**
	 * Clears the colour of the window
	 */
	void ClearWindow();

	/**
	 * Getters and setters
	 */
public:
	inline int GetScreenWidth() const { return this->m_widthScreen; }
	inline int GetScreenHeight() const { return this->m_heightScreen; }
	inline GLFWwindow* GetWindow() const { return this->m_window; }
	inline Timer* GetTimer() const { return this->m_timer; }
	inline std::vector<GeneratedObjects*> GetListObject() const { return this->generatedObjects; }

	inline void SetScreenWidth(int newWidth) { this->m_widthScreen = newWidth; }
	inline void SetScreenHeight(int newHeight) { this->m_heightScreen = newHeight; }
	inline void SetNewObjects(std::vector<GeneratedObjects*> newObjects) { this->generatedObjects = newObjects; }
};