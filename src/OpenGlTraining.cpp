#include "OpenGlTraining.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>

void OpenGlTraining::init(GLFWwindow* window)
{
	this->window = window;
	loadWorld();
	playerController.setWindow(window);
}

void OpenGlTraining::update(std::chrono::milliseconds deltaTime)
{
    playerController.update(deltaTime);
}

void OpenGlTraining::render()
{
	glm::mat4 m, v, p;
	m = glm::mat4(1.0);
	v = glm::mat4(1.0);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	p = glm::ortho<float>(0, 16.f * width / height, 0, 16);

	renderer.setMvp(p * v * m);

	drawWorld();
	drawPlayer();

    renderer.flush();
}

void OpenGlTraining::loadWorld() {
    using namespace std;
    ifstream myFile ("res/sample.wld");

    if (myFile.is_open())
    {
        vector<string> lines;
        string line;

        while (getline(myFile, line))
            lines.push_back(line);

        reverse(lines.begin(), lines.end());

        for(const auto& l : lines) {
            istringstream stream(l);
            int singleTile;

            while (stream >> singleTile) {
                assert(!stream.fail());
                world.push_back(static_cast<unsigned char>(singleTile));
            }
        }
    }

    else cout << "Unable to open file";
}

void OpenGlTraining::drawWorld() {
    using namespace glm;

    auto tileCount = sqrt(world.size());

    for (int i = 0; i < tileCount; ++i) {
        for (int j = 0; j < tileCount; ++j) {
            auto index = static_cast<std::size_t>(j + (i * tileCount));
            renderer.drawTile(vec2(j, i), vec2(1, 1), world.at(index));
        }
    }
}

void OpenGlTraining::drawPlayer() {
    using namespace glm;
    auto direction = playerController.direction();
    playerPosition += direction * 0.1f;
    renderer.drawPlayer(playerPosition, vec2(1,1), 0);
}


