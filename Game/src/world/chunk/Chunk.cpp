#include "Chunk.h"

#include "../World.h"
#include "../../util/Noise.h"

namespace world {

	Chunk::Chunk(World* world, int x, int z) : m_world(world), m_x(x), m_z(z), m_id(0), m_running(false), m_removed(false), m_inWorkVector(true), m_mesh(), m_model(), m_heightMapFilled(false), m_meshCreated(true)
	{
		m_mesh.addAttribute(graphics::Attribute(graphics::TYPE_FLOAT, 3, false));
		m_mesh.addAttribute(graphics::Attribute(graphics::TYPE_FLOAT, 3, false));
		m_model.initialize();

		if ((m_neighbors[0] = world->getChunkManager().getChunk(math::Vec2i(m_x + 1, m_z))) != nullptr) m_neighbors[0]->m_neighbors[2] = this;
		if ((m_neighbors[1] = world->getChunkManager().getChunk(math::Vec2i(m_x, m_z + 1))) != nullptr) m_neighbors[1]->m_neighbors[3] = this;
		if ((m_neighbors[2] = world->getChunkManager().getChunk(math::Vec2i(m_x - 1, m_z))) != nullptr) m_neighbors[2]->m_neighbors[0] = this;
		if ((m_neighbors[3] = world->getChunkManager().getChunk(math::Vec2i(m_x, m_z - 1))) != nullptr) m_neighbors[3]->m_neighbors[1] = this;
	}
	
	Chunk::~Chunk()
	{
		if (m_neighbors[0] != nullptr) m_neighbors[0]->m_neighbors[2] = nullptr;
		if (m_neighbors[1] != nullptr) m_neighbors[1]->m_neighbors[3] = nullptr;
		if (m_neighbors[2] != nullptr) m_neighbors[2]->m_neighbors[0] = nullptr;
		if (m_neighbors[3] != nullptr) m_neighbors[3]->m_neighbors[1] = nullptr;

		m_model.cleanup();
	}

	void Chunk::update()
	{
		if (!m_running) {
			const float chunkRemovalThreshold = (float)CHUNK_SIZE * 2.0f;
			if (getDistanceToCamera() - chunkRemovalThreshold > m_world->getRenderDistance()) {
				remove();
				return;
			}

			if (m_heightMapFilled) {
				if (m_neighbors[0] == nullptr) m_world->getChunkManager().generateChunk(math::Vec2i(m_x + 1, m_z));
				if (m_neighbors[1] == nullptr) m_world->getChunkManager().generateChunk(math::Vec2i(m_x, m_z + 1));
				if (m_neighbors[2] == nullptr) m_world->getChunkManager().generateChunk(math::Vec2i(m_x - 1, m_z));
				if (m_neighbors[3] == nullptr) m_world->getChunkManager().generateChunk(math::Vec2i(m_x, m_z - 1));
			}

			int appropriateLOD = getAppropriateLOD();
			if (!m_inWorkVector && m_currentLOD != appropriateLOD && m_world->getChunkManager().canChunkGenerateMesh(this)) {
				m_inWorkVector = true;
				m_currentLOD = appropriateLOD;
				m_meshCreated = false;
				m_world->getChunkManager().addToWorkVector(this);
			}
			createModel();
		}
	}

	void Chunk::render()
	{
		if (m_world->getFrustumCuller().sphereInFrustum(math::Vec3f(((float)m_x + 0.5f) * CHUNK_SIZE, 0.0f, ((float)m_z + 0.5f) * CHUNK_SIZE), 256.0f)) {
			m_model.render();
		}
	}

	void Chunk::createMesh()
	{
		HeightmapVertex* vertices = new HeightmapVertex[(CHUNK_SIZE / m_currentLOD + 3) * (CHUNK_SIZE / m_currentLOD + 3)];
		for (int x = -1; x < CHUNK_SIZE / m_currentLOD + 2; x++) {
			for (int z = -1; z < CHUNK_SIZE / m_currentLOD + 2; z++) {
				vertices[(x + 1) + (z + 1) * (CHUNK_SIZE / m_currentLOD + 3)] = HeightmapVertex(math::Vec3f((float)(x * m_currentLOD + m_x * CHUNK_SIZE), getHeightRelative(x * m_currentLOD, z * m_currentLOD), (float)(z * m_currentLOD + m_z * CHUNK_SIZE)), getNormal(x * m_currentLOD, z * m_currentLOD));
				if (x < 0 || z < 0 || x > CHUNK_SIZE / m_currentLOD || z > CHUNK_SIZE / m_currentLOD) {
					vertices[(x + 1) + (z + 1) * (CHUNK_SIZE / m_currentLOD + 3)].position.y -= 8.0f;
				}
			}
		}
		m_mesh.clear();
		for (int x = -1; x < CHUNK_SIZE / m_currentLOD + 1; x++) {
			for (int z = -1; z < CHUNK_SIZE / m_currentLOD + 1; z++) {
				m_mesh.quad(vertices[(x + 1) + (z + 1) * (CHUNK_SIZE / m_currentLOD + 3)], vertices[(x + 1) + (z + 2) * (CHUNK_SIZE / m_currentLOD + 3)], vertices[(x + 2) + (z + 2) * (CHUNK_SIZE / m_currentLOD + 3)], vertices[(x + 2) + (z + 1) * (CHUNK_SIZE / m_currentLOD + 3)]);
			}
		}
		delete[] vertices;
		m_meshCreated = true;
	}

	void Chunk::createModel()
	{
		if (!m_running && !m_mesh.isEmpty()) {
			m_mesh.createModel(m_model);
			m_mesh.clear();
		}
	}

	void Chunk::run()
	{
		if (!m_heightMapFilled) {
			util::Noise noise(0);
			const float scale = 128.0f;
			for (unsigned int xx = 0; xx < CHUNK_SIZE; xx++) {
				for (unsigned int zz = 0; zz < CHUNK_SIZE; zz++) {
					m_heightMap[xx + zz * CHUNK_SIZE] = -abs(noise.noise((float)((int)xx + m_x * CHUNK_SIZE) / scale / 2.0f, (float)((int)zz + m_z * CHUNK_SIZE) / scale / 2.0f) * scale)
						+ noise.noise((float)((int)xx + m_x * CHUNK_SIZE) / scale / 2.0f + 128.6f, (float)((int)zz + m_z * CHUNK_SIZE) / scale / 2.0f) * scale
						+ noise.noise((float)((int)xx + m_x * CHUNK_SIZE) / scale * 32.0f, (float)((int)zz + m_z * CHUNK_SIZE) / scale * 32.0f) * scale / 256.0f
						+ noise.noise((float)((int)xx + m_x * CHUNK_SIZE) / scale * 16.0f, (float)((int)zz + m_z * CHUNK_SIZE) / scale * 16.0f) * scale / 128.0f
						+ noise.noise((float)((int)xx + m_x * CHUNK_SIZE) / scale * 2.0f, (float)((int)zz + m_z * CHUNK_SIZE) / scale * 2.0f) * scale / 4.0f;
				}
			}
			m_heightMapFilled = true;
		}
		if (!m_meshCreated) {
			createMesh();
		}
		m_running = false;
	}

	float Chunk::getHeightRelative(int x, int z) const
	{
		if (x >= CHUNK_SIZE) if (m_neighbors[0] != nullptr) return m_neighbors[0]->getHeightRelative(x - CHUNK_SIZE, z); else return 0.0f;
		if (z >= CHUNK_SIZE) if (m_neighbors[1] != nullptr) return m_neighbors[1]->getHeightRelative(x, z - CHUNK_SIZE); else return 0.0f;
		if (x < 0) if (m_neighbors[2] != nullptr) return m_neighbors[2]->getHeightRelative(x + CHUNK_SIZE, z); else return 0.0f;
		if (z < 0) if (m_neighbors[3] != nullptr) return m_neighbors[3]->getHeightRelative(x, z + CHUNK_SIZE); else return 0.0f;
		return getHeight((unsigned int)x, (unsigned int)z);
	}

	math::Vec3f Chunk::getNormal(int x, int z) const
	{
		math::Vec3f e0 = math::Vec3f((float)x, getHeightRelative(x, z), (float)z) - math::Vec3f((float)(x - 1), getHeightRelative(x - 1, z - 1), (float)(z - 1));
		math::Vec3f e1 = math::Vec3f((float)x, getHeightRelative(x, z), (float)z) - math::Vec3f((float)(x + 1), getHeightRelative(x + 1, z - 1), (float)(z - 1));
		math::Vec3f e2 = math::Vec3f((float)x, getHeightRelative(x, z), (float)z) - math::Vec3f((float)(x + 1), getHeightRelative(x + 1, z + 1), (float)(z + 1));
		math::Vec3f e3 = math::Vec3f((float)x, getHeightRelative(x, z), (float)z) - math::Vec3f((float)(x - 1), getHeightRelative(x - 1, z + 1), (float)(z + 1));
		return (e0.cross(e3).normalized() + e3.cross(e2).normalized() + e2.cross(e1).normalized() + e1.cross(e0).normalized()).normalized();
	}

	int Chunk::getAppropriateLOD() const
	{
		int lod = (int)(math::Vec2f((float)m_x + 0.5f, (float)m_z + 0.5f) - math::Vec2f(m_world->getCameraPosition().x, m_world->getCameraPosition().z) / (float)CHUNK_SIZE).getLength<float>() / 2 + 1;
		if (lod < 2) lod = 1;
		else if (lod < 4) lod = 2;
		else if (lod < 16) lod = 4;
		else lod = 8;
		return lod;
	}

	void Chunk::calculateDistanceToCamera()
	{
		m_distanceToCamera = (math::Vec2f((float)m_x, (float)m_z) * (float)CHUNK_SIZE + 0.5f - math::Vec2f(m_world->getCameraPosition().x, m_world->getCameraPosition().z)).getLength<float>();
	}

}