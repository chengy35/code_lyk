#include "DenseTrack.h"
#include "Descriptors.h"
#include "Initialize.h"
#include "Saliency.h"
IplImageWrapper image, prev_image, grey, prev_grey;
IplImagePyramid grey_pyramid, prev_grey_pyramid, eig_pyramid;

float* fscales = 0; // float scale values
int show_track = 0; // set show_track = 1, if you want to visualize the trajectories

bool DenseTrack(int argc, char** argv)
{
	int frameNum = 0;
	TrackerInfo tracker;
	DescInfo hogInfo;
	DescInfo hofInfo;
	DescInfo mbhInfo;
	Saliency sal;
	float staticRatio = 0.5;
	float dynamicRatio = 0.5;
	float threshold = 1.5;
	// kernel matrix of densely sampled angles
	CvMat* kernelMatrix = cvCreateMat(3600, 8, CV_32FC1); 
	
	char* video = argv[1];
	arg_parse(argc, argv);
	Video capture(video);

	InitTrackerInfo(&tracker, track_length, init_gap);
	InitDescInfo(&hogInfo, 8, 0, 1, patch_size, nxy_cell, nt_cell);
	InitDescInfo(&hofInfo, 9, 1, 1, patch_size, nxy_cell, nt_cell);
	InitDescInfo(&mbhInfo, 8, 0, 1, patch_size, nxy_cell, nt_cell);
	InitKernelMatrix(kernelMatrix, 5);

	if( show_track == 1 )
		cvNamedWindow( "DenseTrack", 0 );

	std::vector<std::list<Track> > xyScaleTracks;
	int init_counter = 0; // indicate when to detect new feature points
	while( true ) {
		IplImageWrapper frame = 0;
		int i, j, c;

		// get a new frame
		frame = capture.getFrame();
		frameNum = capture.getFrameIndex();
		if( !frame ) {
			printf("break");
			break;
		}
		if( frameNum >= start_frame && frameNum <= end_frame )
		{
			

		// build the image pyramid for the current frame
		cvCopy( frame, image, 0 );
		cvCvtColor( image, grey, CV_BGR2GRAY );
		grey_pyramid.rebuild(grey);

		if( frameNum > 0 ) 
		{
			init_counter++;
			for( int ixyScale = 0; ixyScale < scale_num; ++ixyScale )
			{
				
				

				i = 0;
				for (std::list<Track>::iterator iTrack = tracks.begin(); iTrack != tracks.end(); ++i)
				{
					if( status[i] == 1 ) { // if the feature point is successfully tracked
						PointDesc& pointDesc = iTrack->pointDescs.back();
						CvPoint2D32f prev_point = points_in[i];
						// get the descriptors for the feature point
						CvScalar rect = getRect(prev_point, cvSize(width, height), hogInfo);
						pointDesc.hog = getDesc(hogMat, rect, hogInfo);
						pointDesc.hof = getDesc(hofMat, rect, hofInfo);
						pointDesc.mbhX = getDesc(mbhMatX, rect, mbhInfo);
						pointDesc.mbhY = getDesc(mbhMatY, rect, mbhInfo);

						PointDesc point(hogInfo, hofInfo, mbhInfo, points_out[i], saliency[i], averageSaliency);
						iTrack->addPointDesc(point);	
						++iTrack;
					}
					else // remove the track, if we lose feature point
					iTrack = tracks.erase(iTrack);
				}
				ReleDescMat(hogMat);
				ReleDescMat(hofMat);
				ReleDescMat(mbhMatX);
				ReleDescMat(mbhMatY);
				cvReleaseImage( &prev_grey_temp );
				cvReleaseImage( &grey_temp );
				cvReleaseImage( &flow );
			}

		for( int ixyScale = 0; ixyScale < scale_num; ++ixyScale ) {
		std::list<Track>& tracks = xyScaleTracks[ixyScale]; // output the features for each scale
		for( std::list<Track>::iterator iTrack = tracks.begin(); iTrack != tracks.end(); ) {
			if( iTrack->pointDescs.size() >= tracker.trackLength+1 ) { // if the trajectory achieves the length we want
				std::vector<CvPoint2D32f> trajectory(tracker.trackLength+1);
				std::vector<float> saliency(tracker.trackLength+1);
				std::vector<float> averageSaliency(tracker.trackLength+1);
				std::list<PointDesc>& descs = iTrack->pointDescs;
				std::list<PointDesc>::iterator iDesc = descs.begin();

				for (int count = 0; count <= tracker.trackLength; ++iDesc, ++count) {
					trajectory[count].x = iDesc->point.x*fscales[ixyScale];
					trajectory[count].y = iDesc->point.y*fscales[ixyScale];
					saliency[count] = iDesc->saliency;
					averageSaliency[count] = iDesc->averageSaliency;
				}
				
				if( isValid(trajectory, saliency, averageSaliency, threshold) == 1 ) {
					
					iDesc = descs.begin();
					int t_stride = cvFloor(tracker.trackLength/hogInfo.ntCells);
					for( int n = 0; n < hogInfo.ntCells; n++ ) {
						std::vector<float> vec(hogInfo.dim);
						for( int t = 0; t < t_stride; t++, iDesc++ )
							for( int m = 0; m < hogInfo.dim; m++ )
								vec[m] += iDesc->hog[m];
						for( int m = 0; m < hogInfo.dim; m++ )
							printf("%f\t", vec[m]/float(t_stride));
					}

					iDesc = descs.begin();
					t_stride = cvFloor(tracker.trackLength/hofInfo.ntCells);
					for( int n = 0; n < hofInfo.ntCells; n++ ) {
						std::vector<float> vec(hofInfo.dim);
						for( int t = 0; t < t_stride; t++, iDesc++ )
							for( int m = 0; m < hofInfo.dim; m++ )
								vec[m] += iDesc->hof[m];
						for( int m = 0; m < hofInfo.dim; m++ )
							printf("%f\t", vec[m]/float(t_stride));
					}

					iDesc = descs.begin();
					t_stride = cvFloor(tracker.trackLength/mbhInfo.ntCells);
					for( int n = 0; n < mbhInfo.ntCells; n++ ) {
						std::vector<float> vec(mbhInfo.dim);
						for( int t = 0; t < t_stride; t++, iDesc++ )
							for( int m = 0; m < mbhInfo.dim; m++ )
								vec[m] += iDesc->mbhX[m];
						for( int m = 0; m < mbhInfo.dim; m++ )
							printf("%f\t", vec[m]/float(t_stride));
					}

					iDesc = descs.begin();
					t_stride = cvFloor(tracker.trackLength/mbhInfo.ntCells);
					for( int n = 0; n < mbhInfo.ntCells; n++ ) {
						std::vector<float> vec(mbhInfo.dim);
						for( int t = 0; t < t_stride; t++, iDesc++ )
							for( int m = 0; m < mbhInfo.dim; m++ )
								vec[m] += iDesc->mbhY[m];
						for( int m = 0; m < mbhInfo.dim; m++ )
							printf("%f\t", vec[m]/float(t_stride));
					}

					printf("\n");
					if( show_track == 1 ) {
						std::list<PointDesc>& descs = iTrack->pointDescs;
						std::list<PointDesc>::iterator iDesc = descs.begin();
						float length = descs.size();
						CvPoint2D32f point0 = iDesc->point;
						point0.x *= fscales[ixyScale]; // map the point to first scale
						point0.y *= fscales[ixyScale];

						float j = 0;
						for (iDesc++; iDesc != descs.end(); ++iDesc, ++j) {
							CvPoint2D32f point1 = iDesc->point;
							point1.x *= fscales[ixyScale];
							point1.y *= fscales[ixyScale];

							cvLine(image, cvPointFrom32f(point0), cvPointFrom32f(point1),
							   	CV_RGB(0,cvFloor(255.0*(j+1.0)/length),0), 2, 8,0);
							point0 = point1;
						}
						cvCircle(image, cvPointFrom32f(point0), 2, CV_RGB(255,0,0), -1, 8,0);
					}
				}
				iTrack = tracks.erase(iTrack);
			}
			else
				iTrack++;
		}
		}

		if( init_counter == tracker.initGap ) { // detect new feature points every initGap frames
		init_counter = 0;
		for (int ixyScale = 0; ixyScale < scale_num; ++ixyScale) {
			std::list<Track>& tracks = xyScaleTracks[ixyScale];
			std::vector<CvPoint2D32f> points_in(0);
			std::vector<CvPoint2D32f> points_out(0);
			for(std::list<Track>::iterator iTrack = tracks.begin(); iTrack != tracks.end(); iTrack++, i++) {
				std::list<PointDesc>& descs = iTrack->pointDescs;
				CvPoint2D32f point = descs.back().point; // the last point in the track
				points_in.push_back(point);
			}

			IplImage *grey_temp = 0, *eig_temp = 0;
			std::size_t temp_level = (std::size_t)ixyScale;
			grey_temp = cvCloneImage(grey_pyramid.getImage(temp_level));
			eig_temp = cvCloneImage(eig_pyramid.getImage(temp_level));
			cvDenseSample(grey_temp, eig_temp, points_in, points_out, quality, min_distance);

			// save the new feature points
			for( i = 0; i < points_out.size(); i++) {
				Track track(tracker.trackLength);
				PointDesc point(hogInfo, hofInfo, mbhInfo, points_out[i], 0, 0);
				track.addPointDesc(point);
				tracks.push_back(track);
			}
			cvReleaseImage( &grey_temp );
			cvReleaseImage( &eig_temp );
		}
		}
		}

		cvCopy( frame, prev_image, 0 );
		cvCvtColor( prev_image, prev_grey, CV_BGR2GRAY );
		prev_grey_pyramid.rebuild(prev_grey);
		}

		if( show_track == 1 ) {
			char str[20];
			sprintf(str, "traj/%d.jpg", frameNum);
			cvSaveImage(str, image);
			
			cvShowImage( "DenseTrack", image);
			c = cvWaitKey(3);
			if((char)c == 27) break;
		}

		// get the next frame
		if (!capture.nextFrame())
			break;
	}

	if( show_track == 1 )
		cvDestroyWindow("DenseTrack");

	cvReleaseMat(&kernelMatrix);
	return true;
}
