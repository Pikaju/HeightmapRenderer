#pragma once

#include "../../math/Vec2.h"
#include "../../math/Vec3.h"
#include "../../graphics/model/Mesh.h"

namespace world {

	const int CHUNK_SIZE = 64;

	struct HeightmapVertex
	{
		math::Vec3f position;
		math::Vec3f normal;

		HeightmapVertex() : position(), normal() {  }
		HeightmapVertex(const math::Vec3f& position, const math::Vec3f& normal) : position(position), normal(normal) {  }
	};

	class World;

	class Chunk
	{
	public:
		Chunk(World* world, int x, int z);
		~Chunk();

		void update();
		void render();

		void createMesh();
		void createModel();

		inline math::Vec2i getPosition() const { return math::Vec2i(m_x, m_z); }
		inline unsigned int getID() const { return m_id; }
		inline void setID(unsigned int id) { m_id = id; }

		inline bool isRunning() const { return m_running; }
		inline void setRunning(bool running) { m_running = running; }
		void run();

		inline bool isRemoved() const { return m_removed; }
		inline void remove() { m_removed = true; }
		inline bool isInWorkVector() const { return m_inWorkVector; }
		inline void removeFromWorkVector() { m_inWorkVector = false; }

		inline float getHeight(unsigned int x, unsigned int z) const { return m_heightMap[x + z * CHUNK_SIZE]; }
		float getHeightRelative(int x, int z) const;
		math::Vec3f getNormal(int x, int z) const;
		void calculateDistanceToCamera();
		inline float getDistanceToCamera() const { return m_distanceToCamera; }

		inline bool isHeightMapFilled() const { return m_heightMapFilled; }
	private:
		int getAppropriateLOD() const;

		World* m_world;
		int m_x;
		int m_z;
		unsigned int m_id;
		Chunk* m_neighbors[4];

		bool m_removed;
		bool m_running;
		bool m_inWorkVector;

		float m_heightMap[CHUNK_SIZE * CHUNK_SIZE];
		graphics::Mesh<HeightmapVertex, unsigned int> m_mesh;
		graphics::Model m_model;
		float m_distanceToCamera;
		int m_currentLOD;

		bool m_heightMapFilled;
		bool m_meshCreated;
	};

}