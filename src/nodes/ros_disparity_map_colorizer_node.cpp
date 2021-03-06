/****************************************************************************
 *
 *   Copyright (c) 2015-2016 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/*
 * depth_map_colorizer.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: Vilhjalmur, dominiho
 *
 */

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>

#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <stereo_msgs/DisparityImage.h>

int MIN_DEPTH = 8;
int MAX_DEPTH = 255;

class DepthMapPublisher {
  public:
	DepthMapPublisher() {} 
	DepthMapPublisher(ros::NodeHandle &nh, std::string topic_name) 
		: topic_name(topic_name)
	{
		publisher = nh.advertise<sensor_msgs::Image>(topic_name + "_color", 1);
		subscriber = nh.subscribe(topic_name, 
  						 1, 
  						 &DepthMapPublisher::publishWithColor, 
  						 this);
	}

	//from opencv ros disparity nodelet
	unsigned char colormap[768] =
	  { 150, 150, 150,
	    107, 0, 12,
	    106, 0, 18,
	    105, 0, 24,
	    103, 0, 30,
	    102, 0, 36,
	    101, 0, 42,
	    99, 0, 48,
	    98, 0, 54,
	    97, 0, 60,
	    96, 0, 66,
	    94, 0, 72,
	    93, 0, 78,
	    92, 0, 84,
	    91, 0, 90,
	    89, 0, 96,
	    88, 0, 102,
	    87, 0, 108,
	    85, 0, 114,
	    84, 0, 120,
	    83, 0, 126,
	    82, 0, 131,
	    80, 0, 137,
	    79, 0, 143,
	    78, 0, 149,
	    77, 0, 155,
	    75, 0, 161,
	    74, 0, 167,
	    73, 0, 173,
	    71, 0, 179,
	    70, 0, 185,
	    69, 0, 191,
	    68, 0, 197,
	    66, 0, 203,
	    65, 0, 209,
	    64, 0, 215,
	    62, 0, 221,
	    61, 0, 227,
	    60, 0, 233,
	    59, 0, 239,
	    57, 0, 245,
	    56, 0, 251,
	    55, 0, 255,
	    54, 0, 255,
	    52, 0, 255,
	    51, 0, 255,
	    50, 0, 255,
	    48, 0, 255,
	    47, 0, 255,
	    46, 0, 255,
	    45, 0, 255,
	    43, 0, 255,
	    42, 0, 255,
	    41, 0, 255,
	    40, 0, 255,
	    38, 0, 255,
	    37, 0, 255,
	    36, 0, 255,
	    34, 0, 255,
	    33, 0, 255,
	    32, 0, 255,
	    31, 0, 255,
	    29, 0, 255,
	    28, 0, 255,
	    27, 0, 255,
	    26, 0, 255,
	    24, 0, 255,
	    23, 0, 255,
	    22, 0, 255,
	    20, 0, 255,
	    19, 0, 255,
	    18, 0, 255,
	    17, 0, 255,
	    15, 0, 255,
	    14, 0, 255,
	    13, 0, 255,
	    11, 0, 255,
	    10, 0, 255,
	    9, 0, 255,
	    8, 0, 255,
	    6, 0, 255,
	    5, 0, 255,
	    4, 0, 255,
	    3, 0, 255,
	    1, 0, 255,
	    0, 4, 255,
	    0, 10, 255,
	    0, 16, 255,
	    0, 22, 255,
	    0, 28, 255,
	    0, 34, 255,
	    0, 40, 255,
	    0, 46, 255,
	    0, 52, 255,
	    0, 58, 255,
	    0, 64, 255,
	    0, 70, 255,
	    0, 76, 255,
	    0, 82, 255,
	    0, 88, 255,
	    0, 94, 255,
	    0, 100, 255,
	    0, 106, 255,
	    0, 112, 255,
	    0, 118, 255,
	    0, 124, 255,
	    0, 129, 255,
	    0, 135, 255,
	    0, 141, 255,
	    0, 147, 255,
	    0, 153, 255,
	    0, 159, 255,
	    0, 165, 255,
	    0, 171, 255,
	    0, 177, 255,
	    0, 183, 255,
	    0, 189, 255,
	    0, 195, 255,
	    0, 201, 255,
	    0, 207, 255,
	    0, 213, 255,
	    0, 219, 255,
	    0, 225, 255,
	    0, 231, 255,
	    0, 237, 255,
	    0, 243, 255,
	    0, 249, 255,
	    0, 255, 255,
	    0, 255, 249,
	    0, 255, 243,
	    0, 255, 237,
	    0, 255, 231,
	    0, 255, 225,
	    0, 255, 219,
	    0, 255, 213,
	    0, 255, 207,
	    0, 255, 201,
	    0, 255, 195,
	    0, 255, 189,
	    0, 255, 183,
	    0, 255, 177,
	    0, 255, 171,
	    0, 255, 165,
	    0, 255, 159,
	    0, 255, 153,
	    0, 255, 147,
	    0, 255, 141,
	    0, 255, 135,
	    0, 255, 129,
	    0, 255, 124,
	    0, 255, 118,
	    0, 255, 112,
	    0, 255, 106,
	    0, 255, 100,
	    0, 255, 94,
	    0, 255, 88,
	    0, 255, 82,
	    0, 255, 76,
	    0, 255, 70,
	    0, 255, 64,
	    0, 255, 58,
	    0, 255, 52,
	    0, 255, 46,
	    0, 255, 40,
	    0, 255, 34,
	    0, 255, 28,
	    0, 255, 22,
	    0, 255, 16,
	    0, 255, 10,
	    0, 255, 4,
	    2, 255, 0,
	    8, 255, 0,
	    14, 255, 0,
	    20, 255, 0,
	    26, 255, 0,
	    32, 255, 0,
	    38, 255, 0,
	    44, 255, 0,
	    50, 255, 0,
	    56, 255, 0,
	    62, 255, 0,
	    68, 255, 0,
	    74, 255, 0,
	    80, 255, 0,
	    86, 255, 0,
	    92, 255, 0,
	    98, 255, 0,
	    104, 255, 0,
	    110, 255, 0,
	    116, 255, 0,
	    122, 255, 0,
	    128, 255, 0,
	    133, 255, 0,
	    139, 255, 0,
	    145, 255, 0,
	    151, 255, 0,
	    157, 255, 0,
	    163, 255, 0,
	    169, 255, 0,
	    175, 255, 0,
	    181, 255, 0,
	    187, 255, 0,
	    193, 255, 0,
	    199, 255, 0,
	    205, 255, 0,
	    211, 255, 0,
	    217, 255, 0,
	    223, 255, 0,
	    229, 255, 0,
	    235, 255, 0,
	    241, 255, 0,
	    247, 255, 0,
	    253, 255, 0,
	    255, 251, 0,
	    255, 245, 0,
	    255, 239, 0,
	    255, 233, 0,
	    255, 227, 0,
	    255, 221, 0,
	    255, 215, 0,
	    255, 209, 0,
	    255, 203, 0,
	    255, 197, 0,
	    255, 191, 0,
	    255, 185, 0,
	    255, 179, 0,
	    255, 173, 0,
	    255, 167, 0,
	    255, 161, 0,
	    255, 155, 0,
	    255, 149, 0,
	    255, 143, 0,
	    255, 137, 0,
	    255, 131, 0,
	    255, 126, 0,
	    255, 120, 0,
	    255, 114, 0,
	    255, 108, 0,
	    255, 102, 0,
	    255, 96, 0,
	    255, 90, 0,
	    255, 84, 0,
	    255, 78, 0,
	    255, 72, 0,
	    255, 66, 0,
	    255, 60, 0,
	    255, 54, 0,
	    255, 48, 0,
	    255, 42, 0,
	    255, 36, 0,
	    255, 30, 0,
	    255, 24, 0,
	    255, 18, 0,
	    255, 12, 0,
	    255,  6, 0,
	    255,  0, 0,
	};

	// Publish a colorized version of msg
	void publishWithColor(const stereo_msgs::DisparityImageConstPtr& msg) 
	{
		// std::cout << topic_name << std::endl;
		cv_bridge::CvImagePtr disparity = cv_bridge::toCvCopy(msg->image, sensor_msgs::image_encodings::TYPE_8UC1);
		colorizeDepth(disparity->image);
		cv_bridge::CvImage out_msg;
		out_msg.header = msg->header;
		out_msg.encoding = "rgb8";
		out_msg.image = colored_mat;
	  	publisher.publish(out_msg.toImageMsg());
	}

	// Update colored_mat
	void colorizeDepth(const cv::Mat &gray) {
		if (gray.size() != colored_mat.size()) {
	  		colored_mat.create(gray.size(), CV_8UC3);
		}
		for (int y = 0; y < gray.rows; y++) {
			for (int x = 0; x < gray.cols; x++) {
				colored_mat.at<cv::Point3_<unsigned char> >(y, x) = 
					spectralColor(gray.at<unsigned char>(y, x));
			}
		}
	}

	// Returns a spectral color between blue (hue=0) and red (hue=255)
	cv::Point3_<unsigned char> spectralColor(int hue) {
		if (hue < MIN_DEPTH) {
			// No depth information
			return cv::Point3_<unsigned char>(150,150,150); 
		}

		// opencv int index = (*d - min_disparity) * multiplier + 0.5;
		// FPGA offers 32 disparity values multipled by 8 to reach full 0-255 range
		// min disparity is set to 0
		int index = hue*8 + 0.5;
		index = std::min(255, std::max(0, index));
      		// Fill as BGR
		unsigned char r = colormap[3*index + 0];
		unsigned char g = colormap[3*index + 1];
		unsigned char b = colormap[3*index + 2];
		return cv::Point3_<unsigned char>(r,g,b);

	}

	// Fields
	ros::Publisher publisher;
	ros::Subscriber subscriber;
	std::string topic_name;
	cv::Mat colored_mat;
};


// Returns true if topic is a depth map
bool isDepthMapPublisher(std::string topic_name) {
	return topic_name.find('disparity') != std::string::npos;
}


int main(int argc, char **argv)
{
	// Create node
	ros::init(argc, argv, "ros_disparity_map_colorizer");
	ros::NodeHandle nh("~");

	// Get topic Names
	ros::master::V_TopicInfo topic_infos;
	ros::master::getTopics(topic_infos);

	// Filter depth-map topics
	std::vector<std::string> depth_topics;
	for (auto topic : topic_infos) {
		if (isDepthMapPublisher(topic.name)) {
			depth_topics.push_back(topic.name);
		}
	} 

	// Reserve space so the publishers-vector doesn't move elements in memory
	int n = depth_topics.size();
	std::vector<DepthMapPublisher> publishers;
	publishers.reserve(n);

	// Create publishers and matching subscribers
	for (int i=0; i < n; ++i) {
		std::string topic_name = depth_topics[i];
  		publishers.emplace_back(nh, topic_name);
	}

	ros::spin();
	return 0;
}
