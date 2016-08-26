#include <iostream>
#include <string.h>
#include <math.h>
#include <opencv/cv.h>
#include <vector>
#include <list>

//#include "kernel/KHistogram/DenseTrack.h"
//#include "cluster/SelectFeatures.h"
//#include "cluster/Cluster.h"
//#include "cluster/EncodeFeatures.h"
//#include "classification/ComputeDistance.h"
//#include "classification/ComputeKernelMatrix.h"
//#include "classification/TrainAndTest.h"

//#include "countLine.h"

#include "stat/Stat.h"
int main(int argc, char** argv)
{

	// step 1. extract trajectories and compute kernel histograms.
	//DenseTrack(argc, argv);
	
	// step 2. select features to compute k-means.
	//SelectFeatures(argc, argv);
	
	// step 3. compute k-means clustering.
	//Cluster(argc, argv);

	// step 4. encode each video to <label> <index1>:<TF1> <index2>:<TF2>
	//EncodeFeatures(argc, argv);
	
	// step 5. compute the x^2 distance of video pairs.
	//ComputeDistance(argc, argv);
	
	// step 6. compute the kernel matrix of video pairs.
	//ComputeKernelMatrix(argc, argv);

	// step 7. train and test with svm.
	//TrainAndTest(argc, argv);
	
	//countLine(argc, argv);
	Stat(argc, argv);
	return 0;
}



