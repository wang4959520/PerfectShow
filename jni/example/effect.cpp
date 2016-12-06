#include <opencv2/highgui.hpp>

#include "example/effect.h"
#include "example/UserData.h"
#include "example/utility.h"

#include "platform/jni_bridge.h"
#include "venus/scalar.h"

#include "venus/Effect.h"

using namespace cv;
using namespace venus;

static const std::string TAG("Effect");


void posterize(const cv::Mat& image)
{
	const std::string title("Posterize");
	const int max = 1024;

	Mat processed = image.clone();
	UserData user_data(title, max, {}, image, processed);

	auto onProgressChanged = [](int level, void* user_data)
	{
		UserData& data = *reinterpret_cast<UserData*>(user_data);
		float x = level / static_cast<float>(data.max);
		x *= 256;  // [0, 1] => [0, 256]
		x = clamp(x, 1.0F, 256.0F);
		
		Effect::posterize(data.processed, data.original, x);
		cv::imshow(data.title, data.processed);
	};

	int level = 256;
	cv::namedWindow(title);
	cv::setMouseCallback(title, UserData::onClick, &user_data);
	cv::createTrackbar("amount", title, &level, max, onProgressChanged, &user_data);

	onProgressChanged(level, &user_data);
}