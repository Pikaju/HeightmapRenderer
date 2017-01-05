#include "ChunkManager.h"

#include "../World.h"
#include "Chunk.h"

#include <algorithm>

namespace world {

	ChunkManager::ChunkManager(World* world) : m_world(world), m_chunkVector()
	{
	}
	
	ChunkManager::~ChunkManager()
	{
	}

	void ChunkManager::initialize(unsigned int numThreads)
	{
		m_threadPool.start(numThreads);
	}
	
	void ChunkManager::cleanup()
	{
		m_threadPool.stop();
		for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
			delete m_chunkVector[i];
		}
		m_chunkVector.clear();
		m_chunkWorkVector.clear();
		m_chunkMap.clear();
	}

	void ChunkManager::update()
	{
		const math::Vec3f& cameraPosition = m_world->getCameraPosition();
		math::Vec2i currentChunk((int)(cameraPosition.x / (float)CHUNK_SIZE), (int)(cameraPosition.z / (float)CHUNK_SIZE));
		if (getChunk(currentChunk) == nullptr) {
			generateChunk(currentChunk);
		}

		for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
			m_chunkVector[i]->calculateDistanceToCamera();
			m_chunkVector[i]->update();
		}

		std::sort(m_chunkWorkVector.begin(), m_chunkWorkVector.end(), compareChunks);
		for (unsigned int i = 0; i < m_chunkWorkVector.size(); i++) {
			if (m_chunkWorkVector[i]->isRemoved()) continue;
			if (m_chunkWorkVector[i]->isRunning()) continue;
			if (m_threadPool.isSaturated()) continue;
			
			m_chunkWorkVector[i]->setRunning(true);
			m_threadPool.addTask(util::ThreadTask(chunkThread, m_chunkWorkVector[i]));
			m_chunkWorkVector[i]->removeFromWorkVector();
		}
		for (unsigned int i = 0; i < m_chunkWorkVector.size(); i++) {
			if (!m_chunkWorkVector[i]->isInWorkVector() || m_chunkWorkVector[i]->isRemoved()) {
				m_chunkWorkVector[i] = m_chunkWorkVector.back();
				m_chunkWorkVector.pop_back();
				i--;
			}
		}

		for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
			if (m_chunkVector[i]->isRemoved()) {
				removeChunk(m_chunkVector[i]);
				i--;
			}
		}
	}
	
	void ChunkManager::render()
	{
		for (unsigned int i = 0; i < m_chunkVector.size(); i++) {
			m_chunkVector[i]->render();
		}
	}

	void ChunkManager::addChunk(Chunk* chunk)
	{
		chunk->setID(m_chunkVector.size());
		m_chunkVector.push_back(chunk);
		m_chunkWorkVector.push_back(chunk);
		m_chunkMap[chunk->getPosition()] = chunk;
	}

	void ChunkManager::generateChunk(const math::Vec2i& position)
	{
		const math::Vec3f& cameraPosition = m_world->getCameraPosition();
		if ((math::Vec2f(cameraPosition.x, cameraPosition.z) - ((math::Vec2f)position * (float)CHUNK_SIZE + (float)CHUNK_SIZE / 2.0f)).getLength<float>() < m_world->getRenderDistance()) {
			if (getChunk(position) == nullptr) {
				addChunk(new Chunk(m_world, position.x, position.y));
			}
		}
	}

	Chunk* ChunkManager::getChunk(const math::Vec2i& position)
	{
		auto& it = m_chunkMap.find(position);
		if (it == m_chunkMap.end()) return nullptr;
		return it->second;
	}
	
	void ChunkManager::removeChunk(Chunk* chunk)
	{
		m_chunkMap.erase(m_chunkMap.find(chunk->getPosition()));
		m_chunkVector[chunk->getID()] = m_chunkVector.back();
		m_chunkVector[chunk->getID()]->setID(chunk->getID());
		m_chunkVector.pop_back();
		delete chunk;
	}

	bool ChunkManager::canChunkGenerateMesh(Chunk* chunk)
	{
		Chunk* c = nullptr;
		if ((c = getChunk(chunk->getPosition() + math::Vec2i(-1, 0))) == nullptr || !c->isHeightMapFilled()) return false;
		if ((c = getChunk(chunk->getPosition() + math::Vec2i(1, 0))) == nullptr || !c->isHeightMapFilled()) return false;
		if ((c = getChunk(chunk->getPosition() + math::Vec2i(0, -1))) == nullptr || !c->isHeightMapFilled()) return false;
		if ((c = getChunk(chunk->getPosition() + math::Vec2i(0, 1))) == nullptr || !c->isHeightMapFilled()) return false;
		if ((c = getChunk(chunk->getPosition() + math::Vec2i(-1, -1))) == nullptr || !c->isHeightMapFilled()) return false;
		if ((c = getChunk(chunk->getPosition() + math::Vec2i(-1, 1))) == nullptr || !c->isHeightMapFilled()) return false;
		if ((c = getChunk(chunk->getPosition() + math::Vec2i(1, -1))) == nullptr || !c->isHeightMapFilled()) return false;
		if ((c = getChunk(chunk->getPosition() + math::Vec2i(1, 1))) == nullptr || !c->isHeightMapFilled()) return false;
		return true;
	}

	void ChunkManager::chunkThread(void* chunk)
	{
		Chunk* c = (Chunk*)chunk;
		c->run();
	}

	bool ChunkManager::compareChunks(Chunk* c0, Chunk* c1)
	{
		return c0->getDistanceToCamera() < c1->getDistanceToCamera();
	}

}