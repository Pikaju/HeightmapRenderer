#pragma once

#include "chunk/ChunkManager.h"
#include "../graphics/shader/ShaderProgram.h"
#include "../util/Camera.h"
#include "../util/FrustumCuller.h"

namespace io {
	class Input;
}

namespace graphics {
	class Window;
}

namespace world {

	class Chunk;

	class World
	{
	public:
		World();
		~World();

		void initialize();
		void cleanup();

		void update(const io::Input& input, float delta);
		void render(const graphics::Window& window);

		inline ChunkManager& getChunkManager() { return m_chunkManager; }
		inline const math::Vec3f& getCameraPosition() const { return m_camera.position; }
		inline const util::FrustumCuller& getFrustumCuller() const { return m_frustumCuller; }

		inline float getRenderDistance() const { return 1024.0f; }
	private:
		ChunkManager m_chunkManager;
		graphics::ShaderProgram m_shaderProgram;
		util::Camera m_camera;
		util::FrustumCuller m_frustumCuller;
	};

}