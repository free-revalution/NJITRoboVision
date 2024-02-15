#ifndef MVVIDEOCAPTURE_H
#define MVVIDEOCAPTURE_H

#include "General/General.h"

#if (USE_VIDEO == 0)

#include <thread>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include "./include/CameraApi.h"
#include "./include/CameraDefine.h"
#include "./include/CameraStatus.h"

#define ACQ_BUFFER_NUM          5               ///< Acquisition Buffer Qty.
#define ACQ_TRANSFER_SIZE       (64 * 1024)     ///< Size of data transfer block
#define ACQ_TRANSFER_NUMBER_URB 64              ///< Qty. of data transfer block

void *ProcGetImage(void* pAcquisitionThread);

///Acquisition thread param
typedef struct AcquisitionThread
{
    cv::Mat* m_pImage;
    int m_hDevice;
    uint64_t* m_pTimeStamp;
    bool* g_AcquisitionFlag;
    AcquisitionThread()
    {
        m_pImage = NULL;
        m_hDevice = NULL;
        m_pTimeStamp = NULL;
        g_AcquisitionFlag = NULL;
    }
}AcquisitionThread;

//定义相机类
class MVVideoCapture
{
//访问权限
//公共权限
public:
    //定义静态成员函数
    static int    Init(int id = 0);  //相机初始化
    static int    Play();
    static int    Read();
    static int    GetFrame(cv::Mat& frame);
    static int    acquisitionStart(cv::Mat * targetMatImg);
    static int    Stop();
    static int    Uninit();          //相机反初始化
    static int    SetExposureTime(bool auto_exp, double exp_time = 10000); //相机自动曝光设置
    static double GetExposureTime();
    static int    SetLargeResolution(bool if_large_resolution);
    static cv::Size   GetResolution();
    static int    SetGain(double gain);
    static double GetGain();
    static int    SetWBMode(bool auto_wb = true);
    static int    GetWBMode(bool & auto_wb);
    static int    SetOnceWB();


    static int  iCameraCounts;
    static int  iStatus;
    static int  hCamera;
    static int  channel;
    static tSdkCameraCapbility     tCapability;      //设备描述信息
    static tSdkFrameHead           sFrameInfo;
    static unsigned char           *	pbyBuffer;
    static unsigned char           * g_pRgbBuffer[2];     //处理后数据缓存区
    static int                     ready_buffer_id;
    static bool                    stopped;
    static bool                    updated;
    static std::mutex              mutex1;
    static pthread_t g_nAcquisitonThreadID;            ///< Thread ID of Acquisition thread
    static bool g_bAcquisitionFlag;                    ///< Thread running flag
    static AcquisitionThread threadParam;


//私有权限
//类外不可以访问
private:
    MVVideoCapture();
    ~MVVideoCapture();




};



#endif  // USE_VIDEO

#endif // MVVIDEOCAPTURE_H
