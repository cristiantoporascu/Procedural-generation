#include "Generator.h"
#include <time.h>
#include <iostream>

GeneratedData Generator::DiamondSquareAlgorithmCalculation() {
	// Initialise rand seed
	srand(time(0));

	// Initialise the output data
	GeneratedData output;

	// Initialise terrain - set values in the height map to 0
	std::vector<std::vector<float>> surface 
		= std::vector<std::vector<float>>(this->m_surfaceSize, std::vector<float>(this->m_surfaceSize));

	// Calculate the diamond-square step
	int squareSize = this->m_surfaceSize - 1;
	int stepNumber = log2(this->m_surfaceSize);
	int numberSquares = 1;

	// Set the corners of the terrain as being random on height
	surface[0][0] = -this->m_minHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surface[squareSize][0] = -this->m_minHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surface[0][squareSize] = -this->m_minHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surface[squareSize][squareSize] = -this->m_minHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));

	for (int i = 0; i < stepNumber; i++)
	{
		int row = 0;
		for (int j = 0; j < numberSquares; j++)
		{
			int col = 0;
			for (int k = 0; k < numberSquares; k++)
			{
				this->DiamondSquare(surface, row, col, squareSize);
				col += squareSize;
			}
			row += squareSize;
		}
		numberSquares *= 2;
		squareSize /= 2;
		this->m_minHeight *= 0.5f;
	}

	// Initilise normals data for vertices
	std::vector<std::vector<glm::vec3>> vertexNormals 
		= std::vector<std::vector<glm::vec3>>(this->m_surfaceSize, std::vector<glm::vec3>(this->m_surfaceSize));
	std::vector< std::vector<glm::vec3> > tempNormals[2];
	for (auto i = 0; i < 2; i++) {
		tempNormals[i] = std::vector<std::vector<glm::vec3>>(this->m_surfaceSize - 1, std::vector<glm::vec3>(this->m_surfaceSize - 1));
	}

	// Build up the normals for each quad starting from the first top left down to last - 1
	for (int x = 0; x < this->m_surfaceSize - 1; x++)
	{
		for (int z = 0; z < this->m_surfaceSize - 1; z++)
		{
			glm::vec3 vertexA = glm::vec3((float)x, surface[x][z], (float)z);
			glm::vec3 vertexB = glm::vec3((float)x, surface[x][z + 1], (float)(z + 1));
			glm::vec3 vertexC = glm::vec3((float)(x + 1), surface[x + 1][z + 1], (float)(z + 1));
			glm::vec3 vertexD = glm::vec3((float)(x + 1), surface[x + 1][z], (float)z);

			glm::vec3 triangleNormalA = glm::cross(vertexB - vertexA, vertexA - vertexD);
			glm::vec3 triangleNormalB = glm::cross(vertexD - vertexC, vertexC - vertexB);

			tempNormals[0][x][z] = glm::normalize(triangleNormalA);
			tempNormals[1][x][z] = glm::normalize(triangleNormalB);
		}
	}

	// Generate the normals for each vertex
	for (int x = 0; x < this->m_surfaceSize; x++)
	{
		for (int z = 0; z < this->m_surfaceSize; z++)
		{
			bool isFirstRow = x == 0;
			bool isFirstColumn = z == 0;
			bool isLastRow = x == this->m_surfaceSize - 1;
			bool isLastColumn = z == this->m_surfaceSize - 1;

			glm::vec3 finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for triangle to the upper-left
			if (!isFirstRow && !isFirstColumn) {
				finalVertexNormal += tempNormals[1][x - 1][z - 1];
			}

			// Look for triangles to the upper-right
			if (!isFirstRow && !isLastColumn) {
				for (int k = 0; k < 2; k++) {
					finalVertexNormal += tempNormals[k][x - 1][z];
				}
			}

			// Look for triangle to the bottom-right
			if (!isLastRow && !isLastColumn) {
				finalVertexNormal += tempNormals[0][x][z];
			}

			// Look for triangles to the bottom-left
			if (!isLastRow && !isFirstColumn) {
				for (int k = 0; k < 2; k++) {
					finalVertexNormal += tempNormals[k][x][z - 1];
				}
			}

			// Store final normal of j-th vertex in i-th ro
			vertexNormals[x][z] = glm::normalize(finalVertexNormal);
		}
	}

	// Generate texture co-ordinates
	float fScaleR = ( this->m_textureScaler > 0.0f ? float(this->m_surfaceSize) * this->m_textureScaler : 1.0f );
	float fScaleC = ( this->m_textureScaler > 0.0f ? float(this->m_surfaceSize) * this->m_textureScaler : 1.0f );

	// Intialise vertex array
	for (int x = 0; x < this->m_surfaceSize; x++)
	{
		for (int z = 0; z < this->m_surfaceSize; z++)
		{
			float fTextureR = float(x) / float(this->m_surfaceSize - 1);
			float fTextureC = float(z) / float(this->m_surfaceSize - 1);

			output.vertexData.push_back(
				Vertex(glm::vec3((float)x, surface[x][z], (float)z), glm::vec2(fScaleR * fTextureR, fScaleC * fTextureC), -vertexNormals[x][z], glm::vec3(1.0f, 1.0f, 1.0f)));
		}
	}

	// Now build the index data 
	for (int x = 0; x < this->m_surfaceSize - 1; x++)
	{
		int spacer = (x + 1) * this->m_surfaceSize;
		for (int z = this->m_surfaceSize * x; z < this->m_surfaceSize * (x + 1); z++)
		{
			if (z % this->m_surfaceSize == 0 && x > 0)
				output.vertexIndices.push_back(z);

			output.vertexIndices.push_back(z);
			output.vertexIndices.push_back(spacer + z % this->m_surfaceSize);
			if (z % this->m_surfaceSize >= this->m_surfaceSize - 1)
				output.vertexIndices.push_back(spacer + z % this->m_surfaceSize);
		}
	}

	return output;
}

std::vector<glm::vec3> Generator::PerlinNoise() {
	// Initialise rand seed
	srand(time(0));

	// Initialise the output data
	std::vector<glm::vec3> output;

	// Initialise terrain - set values in the height map to 0
	std::vector<std::vector<float>> surface
		= std::vector<std::vector<float>>(this->m_surfaceSize, std::vector<float>(this->m_surfaceSize));

	// Calculate the diamond-square step
	int squareSize = this->m_surfaceSize - 1;
	int stepNumber = log2(this->m_surfaceSize);
	int numberSquares = 1;

	// Set the corners of the terrain as being random on height
	surface[0][0] = -this->m_minHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surface[squareSize][0] = -this->m_minHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surface[0][squareSize] = -this->m_minHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surface[squareSize][squareSize] = -this->m_minHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));

	for (int i = 0; i < stepNumber; i++)
	{
		int row = 0;
		for (int j = 0; j < numberSquares; j++)
		{
			int col = 0;
			for (int k = 0; k < numberSquares; k++)
			{
				this->DiamondSquare(surface, row, col, squareSize);
				col += squareSize;
			}
			row += squareSize;
		}
		numberSquares *= 2;
		squareSize /= 2;
		this->m_minHeight *= 0.5f;
	}

	// Initialise the elements for the perlin noise
	for (int x = 0; x < this->m_surfaceSize; x++)
	{
		for (int z = 0; z < this->m_surfaceSize; z++)
		{
			output.push_back(glm::vec3((float)x, surface[x][z], (float)z));
		}
	}

	return output;
}

GeneratedData Generator::LSystemPGTrees(const int& treeGeneration)
{
	// Initialise rand seed
	srand(time(0));

	// Initialise the output data
	GeneratedData output;

	// Alphabet of this specific L-system will be F,+,-,[,]
	// Define the axiom which is the starting point of our program
	std::string axiom = "F";

	// Define the sentance for the generation of the rule set
	std::string sentence = axiom;

	// Declare the set of rules that will build our L-system
	std::map<std::string, std::string> rules;
	rules["F"] = "FF+[+F-F-F]-[-F+F+F]";

	for (unsigned int i = 0; i < treeGeneration; i++) {
		this->RecGenPGTrees(sentence, rules);
	}

	//std::cout << sentence << std::endl;

	this->InterpretSetencePGTrees(output, sentence);

	return output;
}

void Generator::RecGenPGTrees(std::string& sentence, std::map<std::string, std::string> rules) {
	std::string outputSentence = "";
	for (unsigned int i = 0; i < sentence.size(); i++) {
		std::string currentChar = std::string(1, sentence[i]);
		if (rules.find(currentChar) == rules.end()) {
			outputSentence += currentChar;
		}
		else {
			outputSentence += rules[currentChar];
		}
	}
	sentence = outputSentence;
}

void Generator::InterpretSetencePGTrees(GeneratedData& outputRef, std::string& sentence) {
	float angle = glm::radians(25.0f);
	glm::vec3 branchDirection(0.0f, 1.0f, 0.0f);

	glm::vec3 colour(0.55f, 0.27f, 0.075f);
	glm::vec3 normals(0.0f, 0.0f, 1.0f);
	glm::vec2 texCoord(0.0f, 0.0f);

	outputRef.vertexData.push_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f), texCoord, normals, colour));
	outputRef.vertexIndices.push_back(0);

	int branchIndex = 0;
	std::vector<int> holdIndex;
	std::vector<glm::vec3> holdRotation;
	int previousIndex = 0;

	for (unsigned int i = 0; i < sentence.size(); i++) {
		std::string currentChar = std::string(1, sentence[i]);
		if (currentChar == "F") {
			outputRef.vertexData.push_back(
				Vertex(outputRef.vertexData[previousIndex].pos + this->m_branchLength * branchDirection, texCoord, normals, colour)
			);
			branchIndex++;

			outputRef.vertexIndices.push_back(branchIndex);
			outputRef.vertexIndices.push_back(branchIndex);

			previousIndex = branchIndex;
		}
		else if (currentChar == "+") {
			branchDirection = glm::rotateZ(branchDirection, angle);
		}
		else if (currentChar == "-") {
			branchDirection = glm::rotateZ(branchDirection, -angle);
		}
		else if (currentChar == "[") {
			holdIndex.push_back(previousIndex);
			holdRotation.push_back(branchDirection);
		}
		else if (currentChar == "]") {
			outputRef.vertexIndices.push_back(previousIndex);
			previousIndex = holdIndex[holdIndex.size() - 1];
			outputRef.vertexIndices.push_back(holdIndex[holdIndex.size() - 1]);
			holdIndex.pop_back();

			branchDirection = holdRotation[holdRotation.size() - 1];
			holdRotation.pop_back();
		}
	}
	this->m_branchLength *= 0.5;
}

void Generator::DiamondSquare(std::vector<std::vector<float>>& surfaceCoord, int row, int col, int size) {
	int halfSize = size * 0.5;
	int midR = row + halfSize;
	int midC = col + halfSize;

	surfaceCoord[midR][midC] = (surfaceCoord[row][col] + surfaceCoord[row + size][col] + surfaceCoord[row][col + size] + surfaceCoord[row + size][col + size]) * 0.25 + (-this->m_minHeight + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	
	surfaceCoord[row + halfSize][col] = (surfaceCoord[row][col] + surfaceCoord[row + size][col] + surfaceCoord[midR][midC]) / 3 + (-this->m_minHeight + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surfaceCoord[row][col + halfSize] = (surfaceCoord[row][col] + surfaceCoord[row][col + size] + surfaceCoord[midR][midC]) / 3 + (-this->m_minHeight + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surfaceCoord[midR + halfSize][midC] = (surfaceCoord[row + size][col + size] + surfaceCoord[row][col + size] + surfaceCoord[midR][midC]) / 3 + (-this->m_minHeight + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
	surfaceCoord[midR][midC + halfSize] = (surfaceCoord[row + size][col + size] + surfaceCoord[row + size][col] + surfaceCoord[midR][midC]) / 3 + (-this->m_minHeight + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (this->m_minHeight + this->m_minHeight)));
}