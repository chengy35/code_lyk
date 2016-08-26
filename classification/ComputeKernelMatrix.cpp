//--------------------------------begin--------------------------------
//-----------------------------by Yikun Lin----------------------------
#include "ComputeKernelMatrix.h"
bool ComputeKernelMatrix(int argc, char** argv)
{
	assert(argc == 9);
	char* grInFile = argv[1];
	char* ofInFile = argv[2];
	char* mbInFile = argv[3];
	char* grOutFile = argv[4];
	char* ofOutFile = argv[5];
	char* mbOutFile = argv[6];
	char* allOutFile = argv[7];
	char* weightOutFile = argv[8];
	FILE* grIn = fopen(grInFile, "r");
	FILE* ofIn = fopen(ofInFile, "r");
	FILE* mbIn = fopen(mbInFile, "r");
	FILE* grOut = fopen(grOutFile, "w");
	FILE* ofOut = fopen(ofOutFile, "w");
	FILE* mbOut = fopen(mbOutFile, "w");
	FILE* allOut = fopen(allOutFile, "w");
	FILE* weightOut = fopen(weightOutFile, "w");
	
	float averageGr;
	float averageOf;
	float averageMb;
	int nTrains;
	int nTests;
	
	// read the average distance of three descriptors
	fscanf(grIn, "%f", &averageGr);
	fscanf(ofIn, "%f", &averageOf);
	fscanf(mbIn, "%f", &averageMb);
	
	// read the number of training and testing videos
	fscanf(grIn, "%d", &nTrains);
	fscanf(grIn, "%d", &nTests);
	fprintf(grOut, "%d %d\n", nTrains, nTests);
	fscanf(ofIn, "%d", &nTrains);
	fscanf(ofIn, "%d", &nTests);
	fprintf(ofOut, "%d %d\n", nTrains, nTests);
	fscanf(mbIn, "%d", &nTrains);
	fscanf(mbIn, "%d", &nTests);
	fprintf(mbOut, "%d %d\n", nTrains, nTests);
	fprintf(allOut, "%d %d\n", nTrains, nTests);
	fprintf(weightOut, "%d %d\n", nTrains, nTests);
	
	for (int iRow = 0; iRow < nTrains + nTests; iRow++)
	{
		int label;
		int id;
		// read and write the class label and id of video
		fscanf(grIn, "%d", &label);
		fscanf(grIn, "%d", &id);
		fprintf(grOut, "%d 0:%d ", label, id);
		
		fscanf(ofIn, "%d", &label);
		fscanf(ofIn, "%d", &id);
		fprintf(ofOut, "%d 0:%d ", label, id);
		
		fscanf(mbIn, "%d", &label);
		fscanf(mbIn, "%d", &id);
		fprintf(mbOut, "%d 0:%d ", label, id);
		
		fprintf(allOut, "%d 0:%d ", label, id);
		fprintf(weightOut, "%d 0:%d ", label, id);
		
		for (int iCol = 0; iCol < nTrains; iCol++)
		{
			float grValue;
			float ofValue;
			float mbValue;
			float allValue;
			float weightValue;
			fscanf(grIn, "%f", &grValue);
			fscanf(ofIn, "%f", &ofValue);
			fscanf(mbIn, "%f", &mbValue);
			grValue /= averageGr;
			ofValue /= averageOf;
			mbValue /= averageMb;
			allValue = grValue + ofValue + mbValue;
			
			grValue = exp(-grValue);
			ofValue = exp(-ofValue);
			mbValue = exp(-mbValue);
			//allValue = grValue + ofValue + mbValue;
			allValue = exp(-allValue);
			weightValue = grValue + ofValue + mbValue;
			fprintf(grOut, "%d:%f ", iCol + 1, grValue);
			fprintf(ofOut, "%d:%f ", iCol + 1, ofValue);
			fprintf(mbOut, "%d:%f ", iCol + 1, mbValue);
			fprintf(allOut, "%d:%f ", iCol + 1, allValue);
			fprintf(weightOut, "%d:%f ", iCol + 1, weightValue);
		}
		fprintf(grOut, "\n");
		fprintf(ofOut, "\n");
		fprintf(mbOut, "\n");
		fprintf(allOut, "\n");
		fprintf(weightOut, "\n");
	}
	
	fclose(grIn);
	fclose(ofIn);
	fclose(mbIn);
	fclose(grOut);
	fclose(ofOut);
	fclose(mbOut);
	fclose(allOut);
	fclose(weightOut);
}
//---------------------------------end---------------------------------
