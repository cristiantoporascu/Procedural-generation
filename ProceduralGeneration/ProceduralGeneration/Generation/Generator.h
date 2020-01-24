#pragma once
#include "../Mathematics/Vertex.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <string>
#include <map>

struct GeneratedData {
	std::vector<Vertex> vertexData;
	std::vector<unsigned int> vertexIndices;
};

class Generator {
private:
	int m_surfaceSize;
	float m_minHeight;
	float m_textureScaler;

	std::vector<Vertex> m_terrainPos;
	float m_branchLength;

	/**
	 * Calculate the offset for each individual point in the diamond square algorithm
	 * @param surfaceCoord					The coordinates passed by referrence so that they can be changed
	 * @param row							The is the offset on x from the very top left corner of the plane
	 * @param col							The is the offset on z from the very top left corner of the plane
	 * @param size							Is the subdivision of one square size that was reached in the diamon square algorithm
	 */
	void DiamondSquare(std::vector<std::vector<float>>& surfaceCoord, int row, int col, int size);

	/**
	 * Tree generation for recursive call
	 */
	void RecGenPGTrees(std::string& sentence, std::map<std::string, std::string> rules);

	/**
	 * Read and interpret the sentence generated for the tres using the
	 * rules in vertices and indices
	 */
	void InterpretSetencePGTrees(GeneratedData& outputRef, std::string& sentence);

public:
	Generator(const std::vector<Vertex>& terrainPos, const float& branchLength) : m_terrainPos(terrainPos), m_branchLength(branchLength) {}
	Generator(const int& surfaceSize, const float& minHeight, const float& textureScaler)
		: m_surfaceSize(surfaceSize),
		m_minHeight(minHeight),
		m_textureScaler(textureScaler) {}
	~Generator() {}

	/**
	 * Build the surface by applying the diamondSquareAlgorithm
	 */
	GeneratedData DiamondSquareAlgorithmCalculation();

	/**
	 * Build the surface by applying the diamondSquareAlgorithm and 
	 * calculating the octaves for perlin noise
	 */
	std::vector<glm::vec3> PerlinNoise();

	/**
	 * Build the trees that will follow on being positioned on the surface
	 */
	GeneratedData LSystemPGTrees(const int& treeGeneration);

	/**
	 * Getters and setters
	 */
public:
	inline int GetSurfaceSize() const { return this->m_surfaceSize; }
	inline const std::vector<Vertex>& GetTreePos() const { return this->m_terrainPos; }

	inline void SetSurfaceSize(int newSize) { this->m_surfaceSize = newSize; }
};