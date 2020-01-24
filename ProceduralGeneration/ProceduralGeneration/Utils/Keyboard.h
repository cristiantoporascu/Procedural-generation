#pragma once
#include <GLFW/glfw3.h>

// Forward declarations
class WindowDisplay;

class Keyboard {
private:
	bool currentKeyState[350] = { false };
	bool previousKeyState[350] = { false };

	WindowDisplay* m_windowEntity;

public:
	Keyboard(WindowDisplay* graphicEngine);
	~Keyboard() {}

	/**
	 * Update the previous state of the keys
	 */
	void UpdatePreviousState();

	/**
	 * Check the state of the certain keyboard inputs every update
	 * and sets the previous key based on release and pressed
	 */
	void CheckKeyboardInput();

	/**
	 * Based on the keyboard inputs run a certain action
	 * and push a certain state to the event queue
	 * @param generatorEntity				Specific scene that the actions are running from
	 */
	void KeyboardAction(/*Generator* generatorEntity*/);

	/**
	 * Getters and setters
	 */
public:
	inline bool GetKeyDown(GLenum keyCode) const { return this->currentKeyState[keyCode]; }
	inline bool GetKeyPressed(GLenum keyCode) const { return this->currentKeyState[keyCode] && !this->previousKeyState[keyCode]; }
	inline bool GetKeyRelease(GLenum keyCode) const { return !this->currentKeyState[keyCode] && this->previousKeyState[keyCode]; }

	inline bool GetCurrentKeyState(GLenum keyCode) { return this->currentKeyState[keyCode]; }
	inline bool GetPreviousKeyState(GLenum keyCode) { return this->previousKeyState[keyCode]; }
};