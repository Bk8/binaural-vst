#pragma once
#include <array>
#include <vector>
#include <memory>
#include <map>
#include "../JuceLibraryCode/JuceHeader.h"
#include "QuickHullWrapper.h"



#define HRIR_LENGTH 200
using HrirBuffer = std::array < std::array<float, HRIR_LENGTH>, 2 >;

class HRTFContainer
{
public:
	HRTFContainer();
	~HRTFContainer();

	void updateHRIR(double azimuth, double elevation);
	const HrirBuffer& hrir() const;

	void loadHrir(String filename);
private:
	int getElvIndex(int elv);

	std::map<int, std::array<HrirBuffer, 52>> hrirDict_;
    QuickHullTriangulation quickHull;
	HrirBuffer hrir_;
    std::vector<Point2D> points;
    std::vector<Vector3<float>> points2;
};
