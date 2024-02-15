#ifndef GENERAL_H
#define GENERAL_H

#include <mutex>
#include <sys/time.h>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>

using namespace cv;
using namespace ml;
using namespace std;

#define DEBUG_MODE       // 模式选择
#define RELEASE_MODE

#define USE_SERIAL       // 使用串口
#define USE_VIDEO 0      // 0迈德威视 1摄像头/视频
#define IF_SAVE_VP 0     // 0普通模式 1图像存储模式
//#define USE_WIN
#define SHOW_WIDTH 752
#define SHOW_HEIGHT 480
#define SHOW_RADIO 1     //显示比例
//#define DATA_BUFF_ROW_SIZE_MAX 5
//#define DATA_BUFF_COL_SIZE_MAX 1200


//-----------------------------------【宏定义-键盘指令】--------------------------------------------
// brief：
// 按v  开始、结束录视频
// 按p  截图
// 按s  保存参数
//------------------------------------------------------------------------------------------------
#define KEY_SAVE_VIDEO  76  //开始、结束录视频
#define KEY_SAVE_RESULT 70  //截图
#define KEY_SAVE_PARAM  115 //保存参数


#define PARAM_OTHER_PATH "/home/free/Qtcreater/NJIT_robo_vision/General/setting_file/Param_Other_XS.yml"            // 全局配置文件路径
#define PARAM_CALIBRATION_752 "/home/free/Qtcreater/NJIT_robo_vision/General/setting_file/Camera752-hero1.xml"   // 相机参数
#define SAVE_VIDEO_DIR   "/home/free/Qtcreater/NJIT_robo_vision/save_video/"
#define SAVE_PIC_DIR   "/home/free/Qtcreater/NJIT_robo_vision/save_pic/"
#define JL_CAMERA_SET  "/home/free/Qtcreater/NJIT_robo_vision/General/setting_file/Camera_Params_JL.xml"
#define JL_Modle_Run   "/home/free/Qtcreater/NJIT_robo_vision/General/setting_file/123svm.xml"

//-----------------------------------【重定义数据结构类型】-------------------------------------
// brief：便于与电控进行协议沟通
//------------------------------------------------------------------------------------------
typedef signed int	       s32;
typedef unsigned int	   u32;
typedef signed short	   s16;
typedef unsigned short	   u16;
typedef unsigned char	   u8;

typedef signed int	       int32_t;
typedef unsigned int	   uint32_t;
typedef signed short	   int16_t;
typedef unsigned short	   uint16_t;
typedef unsigned char	   uint8_t;


// extern variables
extern pthread_mutex_t Globalmutex; // threads conflict due to image-updating //由于图形更新导致的线程冲突
extern pthread_cond_t GlobalCondCV; // threads conflict due to image-updating
extern bool imageReadable;          // threads conflict due to image-updating
extern cv::Mat src;                     // Transfering buffer

/**
* @brief: imageUpdating thread
*/
void* imageUpdatingThread(void* PARAM);

/**
* @brief: armorDetecting thread
*/
void* armorDetectingThread(void* PARAM);

/**
 *@brief: the types of armor BIG SMALL 大装甲板 小装甲板
 */
enum ArmorType
{
    SMALL_ARMOR = 0,
    BIG_ARMOR = 1
};

/**
* @brief: colors in order B G R 颜色B蓝 G绿 R红
*/
enum Color
{
    BLUE = 0,
    GREEN = 1,
    RED = 2
};


//-----------------------------------【调式参数】---------------------------------------------
// brief：用于调试参数，创建滑动条
//------------------------------------------------------------------------------------------
struct Debug
{
    int b_show_src;                        // 原图
    int b_show_bin;                        // 二值图
    int b_show_target;                     // 目标-灯条
    int b_show_armor;                      // 装甲板
    int b_show_result;                     // 结果
    int b_show_fps;                        // 帧率
    int b_save_result;                     // 识别结果图
    int n_save_result;                     // 存储序号
    char n_key_order;                      // 键盘键位
    int b_save_pic;                        // 存储图像
    int f_save_pic_inter;                  // 存储图像参数
    int expore_time;                       // 曝光时间
    int b_show_dc;                         // 波形图
    int b_show_ex_armor;                   // 识别最终图像
    int b_show_shoot;                      // 由算法控制是否发射

    Debug()
    {
        b_show_src = false;                // 原图
        b_show_bin = false;                // 二值图
        b_show_target = false;             // 目标-灯条
        b_show_armor = false;              // 装甲板
        b_show_result = false;             // 结果
        b_show_fps = false;                // 帧率
        b_save_result = false;             // 识别结果图
        n_save_result = 0;                 // 存储序号
        n_key_order = -1;                  // 键盘键位
        b_save_pic = false;                // 存储图像
        f_save_pic_inter = 1000;           // 存储图像参数
        expore_time = 7000;                // 曝光时间
        b_show_dc = false;                 // 波形图
        b_show_ex_armor = false;            // 识别最终图像
        b_show_shoot = false;              // 由算法控制是否发射
    }
};




//-----------------------------------【类-设置】-------------------------------------------
// brief：
//---------------------------------------------------------------------------------------
class MainSettings
{
public:
    MainSettings(const char *param);

    /**
     * @brief  读取其他参数
     * @param  param_path
     * @return void
     */
    void readOtherParam(const char *param_path);

    /**
     * @brief  保存配置
     * @param  第一个参数
     * @return 返回值
     */
    void writeOtherParam(const char *param_path);

    /**
     * @brief  获取照片
     */
    bool grabImg(cv::Mat &img, char order, long interval);

public:
    int main_mode_flag = 0;                // 模式切换标志位
    Debug debug;                           // 调试参数
};

/**
* @brief: get distance between two points
*/
float getPointsDistance(const Point2f& a, const Point2f& b);

#endif // GENERAL_H
