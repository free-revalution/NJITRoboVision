#ifndef ANGLE_SOLVER
#define ANGLE_SOLVER

#include "General/General.h"

using namespace cv;
using namespace std;


class AngleSolver
{
public:
	AngleSolver();
	~AngleSolver();

	//设置相机参数
	/**
	* @brief Set camera params
	* @param camera_matrix: camera IntrinsicMatrix
	* @param distortion_coeff: camera DistortionCoefficients
	*/
    void setCameraParam_JL(const cv::Mat & camera_matrix, const cv::Mat & distortion_coeff);
    //overload function. Params set by xml file
    int setCameraParam_JL(const char* filePath, int camId);


	//装甲板数据设置
	/**
	* @brief Set armor size
	* @param type: input target type small/big
	* @param width: the width of armor (mm)
	* @param height: the height of armor (mm)
	*/
	void setArmorSize(ArmorType type, double width, double height);


	//设置子弹的速度
	/**
	* @brief Set bullet speed
	* @param bulletSpeed: the speed of bullet(mm/s)
	*/
	void setBulletSpeed(int bulletSpeed);


	//设置目标装甲轮廓点和中心点
	/**
	* @brief set the target armor contour points and centerPoint
	* @param points2d image points set with following order : left_up, right_up, left_down, right_down
	* @param type target armor type
	*/
	void setTarget(vector<Point2f> contoursPoints, Point2f centerPoint, ArmorType type);


	//按照距离使用角度结算
	/**
	* @brief solve the angles using P4P or PinHole according to the distance
	*/
	void solveAngles();


	//使用P4P方案解算
	/**
	* @brief solve the angles using P4P method
	*/
	void P4P_solver();


	//使用Pinhole方案解算
	/**
	* @brief solve the angles using PinHole method
	*/
	void PinHole_solver();

	/**
	* @brief compensation of pitch
	*/
	void compensateAngle();

	/**
	* @brief compensation of pitch for y_offset between barrel and camera
	*/
	void compensateOffset();

	/**
	* @brief compensation of pitch for gravity
	*/
	void compensateGravity();

	/**
	* @brief according to the target2D points to get the yaw and pitch and distance towards the certain type target using solvePnP
	* @param inputArrayOfPoints contourPoints, the vertices of target armor
	* @param inputPoint centerPoint the center point of target armor
	* @param input type the type of armor BIG_ARMOR or SMALL_ARMOR
	* @param output y_yaw angle     the angle that yaw revolve     '<-' left is minus-       '->' right is positive+
	* @param output x_pitch angle   the angle that pitch revolve   '下' down is minus-       '上' up is positive+ 
	* @param output distance  unit is mm
	*/
	void getAngle(vector<Point2f> & contourPoints, Point2f centerPoint, ArmorType type, double & yaw, double & pitch, double & evaluateDistance);

    /**
    * @brief show debug information
    */
    void showDebugInfo(bool showCurrentResult, bool showTVec, bool showP4P, bool showPinHole, bool showCompensation, bool showCameraParams);

private:

	//Camera params
	Mat CAMERA_MATRIX;    //IntrinsicMatrix		  fx,fy,cx,cy
	Mat DISTORTION_COEFF; //DistortionCoefficients k1,k2,p1,p2

	//Object points in world coordinate
	vector<Point3f> SMALL_ARMOR_POINTS_3D;
	vector<Point3f> BIG_ARMOR_POINTS_3D;

	//speed of bullet (compensation for gravity and air fru)
	double BULLET_SPEED;

	//distance between camera and barrel in y axis(positive when camera is under barrel)  barrel_y = camera_y + barrel_camera_y
	double GUN_CAM_DISTANCE_Y;

	//Targets
	vector<Point2f> targetContour;
	Point2f targetCenter;
	ArmorType targetType;

	// calculated by solvePnP
	//s[R|t]=s'  s->world coordinate;s`->camera coordinate
	Mat rVec;    //rot rotation between camera and target center
	Mat tVec;  //trans tanslation between camera and target center

	//Results
	float y_yaw;
	float x_pitch;
	double distance;
};


#endif // !ANGLE_SOLVER

