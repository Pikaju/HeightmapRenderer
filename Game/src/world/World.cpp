#include "World.h"

#include "../graphics/Window.h"

namespace world {

	World::World() : m_chunkManager(this), m_camera(math::Vec3f(0.0f, 0.0f, 0.0f)), m_frustumCuller()
	{
	}

	World::~World()
	{
	}

	void World::initialize()
	{
		m_chunkManager.initialize(7);

		m_shaderProgram.initialize();
		m_shaderProgram.loadShader("res/World.vert", graphics::SHADERTYPE_VERTEX);
		m_shaderProgram.loadShader("res/World.frag", graphics::SHADERTYPE_FRAGMENT);
		m_shaderProgram.link();
	}

	void World::cleanup()
	{
		m_shaderProgram.cleanup();
		m_chunkManager.cleanup();
	}

	void World::update(const io::Input& input, float delta)
	{
		m_camera.debugUpdate(input, delta);
		m_chunkManager.update();
	}
	
	void World::render(const graphics::Window& window)
	{
		m_shaderProgram.enable();
		m_shaderProgram.setUniform("modelMatrix", math::Mat4f::identity());
		math::Mat4f viewMatrix = m_camera.getViewMatrix();
		m_shaderProgram.setUniform("viewMatrix", viewMatrix);
		math::Mat4f projectionMatrix = math::Mat4f::perspective(window.getAspectRatio(), 90.0f, 0.125f, 128.0f);
		m_shaderProgram.setUniform("projectionMatrix", projectionMatrix);
		m_shaderProgram.setUniform("cameraPosition", m_camera.position);

		m_frustumCuller.extract(viewMatrix * projectionMatrix);

		m_chunkManager.render();

		m_shaderProgram.disable();
	}

}