#include "utility.h"
/*
CSCI 580 Algorithm Design, Analysis, and Implementation
The cloest points problem (duplicate x values)
Author: Chu-An Tsai
*/

using namespace std;

struct Points
{
	double x;
	double y;
};

// Compute the Euclidean distance between two points
double ComputeDistance(Points point1, Points point2)
{
	double distance;
	distance = sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2));

	return distance;
}

// Sort the points by their x values
bool sortByXcoord(Points point1, Points point2)
{
	if (point1.x < point2.x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Sort the points by their y values
bool sortByYcoord(Points point1, Points point2)
{
	if (point1.y < point2.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Find two cloest points and their distance
double ClosestPoint(Points points[], int start, int end, Points index[])
{
	Points XL[16], XR[16], Middle[16];
	double distanceL, distanceR, distanceM, MinDistance;
	// For points = 2
	if (end - start == 1)
	{
		MinDistance = ComputeDistance(points[start], points[end]);
		index[0] = points[start];
		index[1] = points[end];

		return MinDistance;
	}

	// For points = 3
	if (end - start == 2)
	{
		distanceL = ComputeDistance(points[start], points[start + 1]);
		distanceR = ComputeDistance(points[start + 1], points[end]);
		distanceM = ComputeDistance(points[start], points[end]);

		if (min(distanceL, min(distanceR, distanceM)) == distanceL)
		{
			MinDistance = distanceL;
			index[0] = points[start];
			index[1] = points[start + 1];

			return MinDistance;
		}
		else if (min(distanceL, min(distanceR, distanceM)) == distanceR)
		{
			MinDistance = distanceR;
			index[0] = points[start + 1];
			index[1] = points[end];

			return MinDistance;
		}
		else
		{
			MinDistance = distanceM;
			index[0] = points[start];
			index[1] = points[end];

			return MinDistance;
		}

	}

	// For points > 3
	int middle = (start + end) / 2;
	//cout << "Split point index: " << middle << endl;											//test
	//cout << "Split point: (" << points[middle].x << "," << points[middle].y << ")" << endl;	//test
	//cout << "L line: X = " << points[middle].x << endl;										//test
	//cout << endl;																				//test

	distanceL = ClosestPoint(points, start, middle, index);
	XL[0] = index[0];
	XL[1] = index[1];
	//cout << "XL closest pair:" << endl;														//test
	//cout << "(" << XL[0].x << "," << XL[0].y << ")" << endl;									//test
	//cout << "(" << XL[1].x << "," << XL[1].y << ")" << endl;									//test
	//cout << "DistanceL: " << distanceL << endl;												//test	
	//cout << endl;																				//test

	distanceR = ClosestPoint(points, middle + 1, end, index);
	XR[0] = index[0];
	XR[1] = index[1];
	//cout << "XR closest pair:" << endl;														//test
	//cout << "(" << XR[0].x << "," << XR[0].y << ")" << endl;									//test
	//cout << "(" << XR[1].x << "," << XR[1].y << ")" << endl;									//test
	//cout << "DistanceR: " << distanceR << endl;												//test
	//cout << endl;																				//test

	if (min(distanceL, distanceR) == distanceL)
	{
		MinDistance = distanceL;
		index[0] = XL[0];
		index[1] = XL[1];
	}
	else
	{
		MinDistance = distanceR;
		index[0] = XR[0];
		index[1] = XR[1];
	}

	int k = 0;
	// Find the points within 2d-wide range (Y')
	// From L line, find the points between L + d
	for (int i = middle + 1; (i <= end) && ((points[i].x - points[middle].x) < MinDistance); i++)
	{
		Middle[k++] = points[i];
	}

	// From L line, find the points between L - d
	for (int i = middle; (i >= start) && ((points[middle].x - points[i].x) < MinDistance); i--)
	{
		Middle[k++] = points[i];
	}

	// Sort by y-coordinate
	sort(Middle, Middle + k, sortByYcoord);

	// 7 points in Y' to consider with
	for (int i = 0; i < k; i++)
	{
		for (int j = i + 1; j < i + 7 && j < k; j++)
		{
			// Ignore the point that has a distance greater than the MinDistance
			if ((Middle[j].y - Middle[i].y) >= MinDistance)
			{
				break;
			}
			else
			{
				distanceM = ComputeDistance(Middle[i], Middle[j]);
				if (distanceM < MinDistance)
				{
					MinDistance = distanceM;
					index[0] = Middle[i];
					index[1] = Middle[j];
				}
			}
		}
	}
	return MinDistance;
}

int main()
{
	// Read data from file
	ifstream dataFile;
	dataFile.open("points.txt");
	Points points[16], X[16], Y[16];
	double minDistance;

	// Give a warning and exit if fail to read the file
	if (dataFile.fail())
	{
		cout << "Unable to find input file!";
		exit(1);
	}

	double tempX, tempY;
	int i = 0;
	//cout << "The file contains the following points:" << endl; 		// test
	while (dataFile >> tempX >> tempY)
	{
		points[i].x = tempX;
		points[i].y = tempY;
		//cout << points[i].x << "," << points[i].y << endl;			// test
		i++;
	}

	int length = i;

	// Sort the points by x-coordinate
	sort(points, points + length, sortByXcoord);
	for (int i = 0; i < length; i++)
	{
		X[i] = points[i];
		//cout << points[i].x << "," << points[i].y << endl;			// test
	}
	//cout << endl;														// test
	// Sort the points by y-coordinate
	sort(points, points + length, sortByYcoord);
	for (int i = 0; i < length; i++)
	{
		Y[i] = points[i];
		//cout << points[i].x << "," << points[i].y << endl;			// test
	}

	// A variable for the index of two cloest points
	Points index[2];

	// Check the number of points
	if (length == 1)
	{
		cout << "There is only 1 point!!" << endl;
	}
	else
	{
		// Call the function to find cloest points
		minDistance = ClosestPoint(Y, 0, length - 1, index);

		cout << "The cloest two points are:" << endl;
		cout << "(" << index[0].x << "," << index[0].y << ")" << endl;
		cout << "(" << index[1].x << "," << index[1].y << ")" << endl;
		cout << "The distance between them is:" << endl;
		cout << minDistance << endl;
	}

	cout << "\nPress any key to continue.";
	cin.get();

	return 0;
}
