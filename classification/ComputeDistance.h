//--------------------------------begin--------------------------------
//-----------------------------by Yikun Lin----------------------------
#ifndef COMPUTEDISTANCE_H_
#define COMPUTEDISTANCE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "libsvm/svm.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

static int nClasses = 6; // number of classes
static int k = 4000; // number of centers
static float r = 0.125 * 0; // the proportion of features to filter out ADL UCF
//static float r = 0.1875 * 1; // the proportion of features to filter out KTH 0.1875 * 4000 = 750
static char* line = NULL;
static int max_line_len;
static struct svm_problem train_prob;
static struct svm_node *train_x_space;
static struct svm_problem test_prob;
static struct svm_node *test_x_space;
char* ReadLine(FILE* input);
bool ReadTrainProblem(char** trainFiles, int nTrains);
bool ReadTestProblem(char* testFile);
//float CalculateEntropy(float no, float total);
//void Compute_IDF(float* IDF);
//void Compute_IG(float* IG);
//void FeatureSelection(float* IG);
//void Compute_TF(char* TFFile);
//void Compute_TFIDF(char* TFIDFFile, float* IDF);
bool ComputeDistance(int argc, char** argv);

#endif /*COMPUTEDISTANCE_H_*/
//---------------------------------end---------------------------------
