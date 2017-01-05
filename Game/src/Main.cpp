#include "math/Vec3.h"
#include "math/Mat4.h"

#include "graphics/GraphicsAPI.h"
#include "graphics/Renderer.h"
#include "graphics/Window.h"

#include "io/Log.h"

#include "util/Timer.h"

#include "world/World.h"

int main(int agrc, char** agrv)
{
	graphics::api::initialize();

	graphics::Window window;
	window.initialize("Game  | Pre-Alpha", 1280, 720, true, false, true);

	graphics::Renderer::initialize();

	util::Camera camera;

	world::World world;
	world.initialize();

	util::Timer deltaTimer;
	float delta = 0.0;
	util::Timer fpsTimer;
	unsigned int frames = 0;

	while (!window.isCloseRequested()) {
		delta = (float)deltaTimer.getPassedSeconds();
		deltaTimer.reset();

		window.bind();
		window.clear();

		camera.debugUpdate(window.getInput(), delta);
		world.update(window.getInput(), delta);

		world.render(window);

		window.update();
		window.swap();

		frames++;
		while (fpsTimer.getPassedSeconds() >= 1.0) {
			fpsTimer.reset();
			io::log(std::to_string(frames) + " fps");
			frames = 0;
		}
	}

	world.cleanup();
	
	graphics::Renderer::cleanup();
	window.cleanup();
	graphics::api::cleanup();
	return 0;
}