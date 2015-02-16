#ifndef VRPN_TRACKER_COLORS_H_
#define VRPN_TRACKER_COLORS_H_

#include <string>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vrpn_Configure.h>
#include <vrpn_Button.h>
#include <vrpn_Tracker.h>
#include <quat.h>

class VRPN_API vrpnTrackerColors: public vrpn_Tracker
{
 public:
  vrpnTrackerColors(std::string nameTracker, vrpn_Connection * trackercon, cv::Mat CameraMatrix, cv::Mat DistoMatrix);
  ~vrpnTrackerColors();

  void mainloop();


  void trackColor(cv::Mat& frame, cv::Scalar range_min, cv::Scalar range_max, int sensor_id);
		

 protected:

  struct timeval _timestamp;

  cv::VideoCapture m_cap;

  cv::Mat cameraMatrix, distCoeffs;

  float taillePostIt;

};

#endif 
