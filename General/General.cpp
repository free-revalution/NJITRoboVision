#include "General/General.h"

MainSettings::MainSettings(const char *param)
{
    this->readOtherParam(param);
}

void MainSettings::readOtherParam(const char *param_path)
{
    cv::FileStorage fs(param_path, cv::FileStorage::READ);
    fs["debug-b_show_src"] >> this->debug.b_show_src;
    fs["debug-b_show_bin"] >> this->debug.b_show_bin;
    fs["debug-b_show_target"] >> this->debug.b_show_target;
    fs["debug-b_show_armor"] >> this->debug.b_show_armor;
    fs["debug-b_show_result"] >> this->debug.b_show_result;
    fs["debug-b_show_fps"] >> this->debug.b_show_fps;
    fs["debug-b_save_result"] >> this->debug.b_save_result;
    fs["debug-n_save_result"] >> this->debug.n_save_result;
    fs["debug-b_save_pic"] >> this->debug.b_save_pic;
    fs["debug-f_save_pic_inter"] >> this->debug.f_save_pic_inter;
    fs["debug-expore_time"] >> this->debug.expore_time;
    fs["debug-b_show_dc"] >> this->debug.b_show_dc;
#ifdef DEBUG_MODE
    std::cout << "Read other param finished!" << std::endl;
#endif
    fs.release();
}


void MainSettings::writeOtherParam(const char *param_path)
{
    cv::FileStorage fs(param_path, cv::FileStorage::WRITE);
    fs << "debug-b_show_src" << this->debug.b_show_src;
    fs << "debug-b_show_bin" << this->debug.b_show_bin;
    fs << "debug-b_show_target" << this->debug.b_show_target;
    fs << "debug-b_show_armor" << this->debug.b_show_armor;
    fs << "debug-b_show_result" << this->debug.b_show_result;
    fs << "debug-b_show_fps" << this->debug.b_show_fps;
    fs << "debug-b_save_result" << this->debug.b_save_result;
    fs << "debug-n_save_result" << this->debug.n_save_result;
    fs << "debug-b_save_pic" << this->debug.b_save_pic;
    fs << "debug-f_save_pic_inter" << this->debug.f_save_pic_inter;
    fs << "debug-expore_time" << this->debug.expore_time;
    fs << "debug-b_show_dc" << this->debug.b_show_dc;

    std::cout << "Sava other param finished!" << std::endl;
    fs.release();
}


bool MainSettings::grabImg(cv::Mat &img, char order, long interval)
{
    cv::Mat img_show = img.clone();
    //1 s
    //=1000       ms = 10e3 ms
    //=1000000    us = 10e6 us
    //=1000000000 ns = 10e9 ns
    static long start;
    static int img_filename = 0;

    //判断语句没有进入
    if(cv::getTickCount() - start >= interval)
    {
        start=cv::getTickCount();
        std::ostringstream s;
        s << SAVE_PIC_DIR << ++img_filename << ".png" ;
        const string s_2 = s.str();
        cv::imwrite(s_2, img);
        cout << "图片未写入" << endl;
        img_show = -img_show;
    }
    return true;
}
