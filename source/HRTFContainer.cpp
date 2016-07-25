#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(pop)

#include "HRTFContainer.h"



HRTFContainer::HRTFContainer()
	:
	hrir_()
{
}

HRTFContainer::~HRTFContainer()
{
}

void HRTFContainer::updateHRIR(double azimuth, double elevation)
{

    // Iterate through all the faces of the triangulation
   for(int i=0;i<quickHull.getTriangleCount();i+=3)
    {
        Vector3<float> a=quickHull.getTrianglePoint(i);
        Vector3<float> b=quickHull.getTrianglePoint(i+1);
        Vector3<float> c=quickHull.getTrianglePoint(i+2);
        
        const double T[] = {a.x - c.x, a.y - c.y,
            b.x - c.x, b.y - c.y};
        
        double invT[] = {T[3], -T[1], -T[2], T[0]};
        const auto det = 1 / (T[0] * T[3] - T[1] * T[2]);
        
        for (auto i = 0; i < 4; ++i)
            invT[i] *= det;
        
        
        const double X[] = {azimuth - c.x, elevation - c.y};
        
        // Barycentric coordinates of point X
        const auto g1 = static_cast<float>(invT[0] * X[0] + invT[2] * X[1]);
        const auto g2 = static_cast<float>(invT[1] * X[0] + invT[3] * X[1]);
        const auto g3 = 1 - g1 - g2;
        
        // If any of the barycentric coordinate is negative, the point
        // does not lay inside the triangle, so continue the loop.
        if (g1 < 0 || g2 < 0 || g3 < 0)
            continue;
        else
        {
            auto& irA = hrirDict_[(int)a.x][getElvIndex(std::lround(a.y))];
            auto& irB = hrirDict_[(int)b.x][getElvIndex(std::lround(b.y))];
            auto& irC = hrirDict_[(int)c.x][getElvIndex(std::lround(c.y))];
            // Fill HRIR array and return
            for (size_t i = 0; i < hrir_[0].size(); ++i)
            {
                hrir_[0][i] = g1 * irA[0][i] + g2 * irB[0][i] + g3 * irC[0][i];
                hrir_[1][i] = g1 * irA[1][i] + g2 * irB[1][i] + g3 * irC[1][i];
            }
            return;
        }
    }
	
	// If the query point was not found , return zero-ed impulse response
	return;
}

const HrirBuffer& HRTFContainer::hrir() const
{
	return hrir_;
}

void HRTFContainer::loadHrir(String filename)
{
	FileInputStream istream(filename);
	if (istream.openedOk())
	{
		std::vector<Point2D> points;
		int azimuths[] = {-90, -80, -65, -55, -45, -40, -35, -30, -25, -20,
			-15, -10, -5, 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 55, 65, 80, 90};
		for (auto azm : azimuths)
		{

			hrirDict_.insert(std::make_pair(azm, std::array<HrirBuffer, 52>()));
			// -90 deg
			istream.read(hrirDict_[azm][0][0].data(), 200 * sizeof(float));
			istream.read(hrirDict_[azm][0][1].data(), 200 * sizeof(float));
			// 50 elevations
			for (int i = 1; i < 51; ++i)
			{
				istream.read(hrirDict_[azm][i][0].data(), 200 * sizeof(float));
				istream.read(hrirDict_[azm][i][1].data(), 200 * sizeof(float));
    
			}
			// 270 deg
			istream.read(hrirDict_[azm][51][0].data(), 200 * sizeof(float));
			istream.read(hrirDict_[azm][51][1].data(), 200 * sizeof(float));
			
		}
        
        //Quick fix not the best way to do it
        //Quick hull reduces drastically the amount of triagles in the triangulation
        //the original code used delaunay (Triangle++) but didn't worked on OSX
        //and other delaunay libraries were too heavy for realtime.
        
        //Ading the points again to a list
        for (int i=0;i<27;i++)
        {
            // -90 deg
            points.push_back(Point2D(azimuths[i], -90));
           
            // 50 elevations
            for (int j = 1; j < 51; ++j)
                points.push_back(Point2D(azimuths[i], -45 + 5.625 * (j - 1)));
            // 270 deg
            points.push_back(Point2D(azimuths[i], 270));
        }
        
        //Casting point to the ones quick hull uses
        points2.push_back(Vector3<float>(0,0,0));
        
        for (int i=0;i<points.size();i++)
            points2.push_back(Vector3<float>(points[i].x,points[i].y,0));

       
        
        quickHull.triangulate(points2);
	}
	else
		throw std::ios_base::failure("Failed to open HRIR file");
}

int HRTFContainer::getElvIndex(int elv)
{
	if (elv == -90)
		return 0;
	else if (elv == 270)
		return 51;
	else
		return std::lroundf((elv + 45) / 5.625f);
}
