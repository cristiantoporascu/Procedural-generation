#include "../Utils/WindowDisplay.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 720

int main(int argc, char** argv)
{
	Camera::s_camera = new Camera(glm::vec3(0, 130, -1), glm::vec3(0, 0, 0), 70, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.01f, 1000.0f);

	WindowDisplay GameWindow((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, "Procedural Generation");

	GameWindow.DrawWindow();

	return 0;
}