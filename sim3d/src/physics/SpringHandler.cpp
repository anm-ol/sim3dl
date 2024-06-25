#include "SpringHandler.h"

using namespace glm;

// width and height refers to num particles in x and y axes respectively
SpringHandler::SpringHandler(std::vector<particle> *particles, int w, int h, float s, float m)
	: targetVector(particles), width(w), height(h), size(s), mass(m) {

	initVertices();
	initSprings();
};


// adding particles as vertices
void SpringHandler::initVertices() {
	particleIDs.reserve(width * height);
    targetVector->reserve(width * height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			vec3 posn = vec3(x * size, y * size, 0.0f);
			targetVector->emplace_back(posn, size, mass);
            particleIDs.push_back(targetVector->size() - 1);
		}
	}
}

void SpringHandler::addSpring(particle& p1, particle& p2)
{
    
}

void SpringHandler::initSprings() {
    springs.reserve(width * height * 6);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            unsigned int ID = particleIDs[index];
            if (x < width - 1) {
                // Horizontal structural edge
                springs.emplace_back((*targetVector)[ID], (*targetVector)[ID + 1], structCoeff);
            }
            if (y < height - 1) {
                // Vertical structural edge
                springs.emplace_back((*targetVector)[ID], (*targetVector)[ID + width], structCoeff);
            }
            if (x < width - 1 && y < height - 1) {
                // Shear edges
                springs.emplace_back((*targetVector)[ID], (*targetVector)[ID + width + 1], shearCoeff);
                springs.emplace_back((*targetVector)[ID + 1], (*targetVector)[ID + width], shearCoeff);
            }
            if (x < width - 2) {
                // Horizontal bending edge
                springs.emplace_back((*targetVector)[ID], (*targetVector)[ID + 2], bendingCoeff);
            }
            if (y < height - 2) {
                // Vertical bending edge
                springs.emplace_back((*targetVector)[ID], (*targetVector)[ID + 2], bendingCoeff);
            }
        }
    }
}

void SpringHandler::updateForce()
{
    //reset force to zero 
    for (auto ID : particleIDs )
    {
        (*targetVector)[ID].force = vec3(0);
    }
    //re-calculate force with updated vertex positions
    for (spring& spr : springs)
    {
        spr.setForce();
    }
}