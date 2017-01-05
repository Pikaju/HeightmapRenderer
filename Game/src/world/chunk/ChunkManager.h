#pragma once

#include <vector>
#include <unordered_map>

#include "../../math/Vec2.h"
#include "../../util/ThreadPool.h"

namespace world {

	class World;
	class Chunk;

	class ChunkManager
	{
	public:
		ChunkManager(World* world);
		~ChunkManager();

		void initialize(unsigned int numThreads);
		void cleanup();

		void update();
		void render();

		void addChunk(Chunk* chunk);
		void generateChunk(const math::Vec2i& position);
		Chunk* getChunk(const math::Vec2i& position);
		void removeChunk(Chunk* chunk);
		bool canChunkGenerateMesh(Chunk* chunk);

		inline void addToWorkVector(Chunk* chunk) { m_chunkWorkVector.push_back(chunk); }
	private:
		static void chunkThread(void* chunk);
		static bool compareChunks(Chunk* c0, Chunk* c1);

		World* m_world;
		std::vector<Chunk*> m_chunkVector;
		std::vector<Chunk*> m_chunkWorkVector;
		std::unordered_map<math::Vec2i, Chunk*> m_chunkMap;
		util::ThreadPool m_threadPool;
	};

}