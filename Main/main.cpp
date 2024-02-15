#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <unistd.h>
#include <ctime>

#include"AngleSolver/AngleSolver.h"
#include"Armor/Armor.h"
#include"General/General.h"


pthread_t thread1;
pthread_t thread2;

// muti-threads control variables

//定义互斥锁变量
pthread_mutex_t Globalmutex;             // threads conflict due to image-updating 
//定义同步条件变量
pthread_cond_t GlobalCondCV;             // threads conflict due to image-updating

bool imageReadable = false;              // threads conflict due to image-updating
Mat src = Mat::zeros(600,800,CV_8UC3);   // Transfering buffer
//Mat src = Mat::zeros(1024,1280,CV_8UC3);   // Transfering buffer


int main(int argc, char** argv)
{
    std::cout << "开始初始化..." << std::endl;
    //For MutiTHread
    XInitThreads();

    //Init mutex
    pthread_mutex_init(&Globalmutex,NULL);

    //Init cond
    pthread_cond_init(&GlobalCondCV,NULL); //传入值为NULL时，默认

    //Create thread 1 -- image acquisition thread //
    pthread_create(&thread1,NULL,imageUpdatingThread,NULL);

    //Create thread 2 -- armor Detection thread // 
    //传入参数为空，装甲板识别部分独立工作
    pthread_create(&thread2,NULL,armorDetectingThread,NULL);

    //Wait for children thread //
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_mutex_destroy(&Globalmutex);

    return 0;
}
//pthread_create:
//参数1：指向线程标识符的指针，也就是线程对象的指针
//参数2：用来设置线程属性
//参数3：线程运行函数的地址，通俗理解线程要执行函数（线程做的事情的）指针
//参数4：线程要运行函数的参数
