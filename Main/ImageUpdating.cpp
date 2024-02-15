#include <opencv2/opencv.hpp>
#include <thread>
#include "Camera/MVVideoCapture.h"
#include"CameraDefine.h"
#include"CameraStatus.h"
#include "Camera/include/CameraApi.h"
#include"General/General.h"

using namespace cv;
using namespace std;

extern cv::Mat src;          // Transfering buffer

void* imageUpdatingThread(void* PARAM)
{
    //---------------------------------【工业相机/视频初始化】-------------------------------------
    // brief：初始化主函数类
    //-----------------------------------------------------------------------------------------
#if (USE_VIDEO == 0)
    std::cout << "MVVideoCapture Init" << std::endl;
    if(-1 == MVVideoCapture::Init(0))
    {
        std::cout << "MVVideoCapture ERROR!!!" << std::endl;
        //return return_num;
    }
    MVVideoCapture::Play();
    MVVideoCapture::SetExposureTime(false, 6522);
    MVVideoCapture::SetLargeResolution(true);
    //MVVideoCapture::SetWBMode(true);

    std::cout << "MVVideoCapture Finished!" << std::endl;
#endif  // USE_VIDEO
    while(1)
    {
        //cv::Mat src;
        MVVideoCapture::acquisitionStart(&src);
        cout << "Width : " << src.cols << endl;
        cout << "Height: " << src.rows << endl;
        if(src.empty())
        {
            std::cout << "Image empty!" << std::endl;
        }

    }


#if (USE_VIDEO == 0)
    MVVideoCapture::Stop();
    MVVideoCapture::Uninit();
#endif  // USE_VIDEO
}














