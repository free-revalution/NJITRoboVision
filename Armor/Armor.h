#ifndef ARMOR
#define ARMOR

#include "General/General.h"


enum DetectorState
{
	LIGHTS_NOT_FOUND = 0,
	LIGHTS_FOUND = 1,
	ARMOR_NOT_FOUND = 2,
	ARMOR_FOUND = 3
};


/**
* @brief:  装甲板识别中用到的各种参数
*/
struct ArmorParam{

	int color_threshold;   //通道相减的colorImg使用的二值化阈值
	int bright_threshold;  //亮度图二值化阈值

	float min_area;		//灯条允许的最小面积
	float max_angle;	//灯条允许的最大偏角

	float max_angle_diff; //两个灯条之间允许的最大角度差
	float max_lengthDiff_ratio; //两个灯条之间允许的最大长度差比值
	float max_deviation_angle; //两灯条最大错位角

	float max_y_diff_ratio;  //max y 
	float max_x_diff_ratio;  //max x


	//default values  给各参数设定默认值
	ArmorParam(){
	color_threshold = 100-20;
	bright_threshold = 60;

	min_area = 50;
	max_angle = 45;

	max_angle_diff = 6;
	max_lengthDiff_ratio = 0.5;
	max_deviation_angle = 50;

	max_y_diff_ratio = 0.5;
	max_x_diff_ratio = 4.5;
	}
};
extern ArmorParam armorParam;

/**
 * @brief: 装甲板两侧灯条的相关信息
 */
class LightBar
{
public:

	LightBar();
/**
 *@brief: 灯条有参构造函数
 *@param: 拟合椭圆获得的旋转矩形来构造灯条
 */
	LightBar(const RotatedRect& light);
	~LightBar();

public:
	RotatedRect lightRect; //灯条的旋转矩形（椭圆拟合获得）
	float length;  //灯条长度
	Point2f center; //灯条中心
	float angle;  //(between length direction and vertical, left 0~90 right 0~-90) 灯条长度方向与竖直方向的夹角，左偏为0~90,右偏为0~-90
};

/**
 * @brief: 装甲板相关数据信息
 */
class ArmorBox
{
public:
	ArmorBox();
/**
 *@brief: 装甲板有参构造函数
 *@param: 左右两个灯条
 */
	ArmorBox(const LightBar& l_light, const LightBar& r_light);
	~ArmorBox();

	// 装甲板左右灯条角度差
	float getAngleDiff() const;

	// 灯条错位度角(两灯条中心连线与水平线夹角) 
	float getDeviationAngle() const;
	
	// 灯条位置差距 两灯条中心x方向差距比值
	float getDislocationX() const;
	
	// 灯条位置差距 两灯条中心Y方向差距比值
	float getDislocationY() const;

	// 左右灯条长度差比值
	float getLengthRation() const;
	
	//一个综合的功能（函数）来判断这个装甲板是否合适
	bool isSuitableArmor() const;

public:
	LightBar l_light, r_light; //装甲板的左右灯条
	int l_index, r_index; //左右灯条的下标(默认为-1，仅作为ArmorDetector类成员时生效) 
	int armorNum;  //装甲板上的数字（用SVM识别得到）
	vector<Point2f> armorVertices;  //左下 左上 右上 右下
	ArmorType type; //the type of armor //装甲板的种类
	Point2f center;	//装甲板中心
	Rect armorRect;  //装甲板的矩形获取roi用
	float armorAngle;//装甲板角度(灯条角度的平均值)
	Mat armorImg;	//装甲板的图片（透射变换获得）
};

/**
 * @brief: 利用透射变换截取装甲板图片（SVM模型大小），并利用SVM来识别装甲板数字
 */
class ArmorNumClassifier
{
public:
	ArmorNumClassifier();
	~ArmorNumClassifier();

	/**
	 * @brief: 载入SVM模型（用于识别装甲板数字）
	 * @param: 待载入SVM模型的路径 模型的图片尺寸
	 */
	void loadSvmModel(const char *model_path, Size armorImgSize = Size(40, 40));

	/**
	 * @brief: 载入roiImage（剪切出装甲板）
	 * @param: 待载入SVM模型的路径
	 */
    void loadImg(Mat & srcImg);

	/**
	 * @brief: 利用透视变换获得装甲板图片
	 * @param: 待载入SVM模型的路径
	 */
	void getArmorImg(ArmorBox& armor);

	/**
	 * @brief: 利用SVM实现装甲板数字识别
	 */
	void setArmorNum(ArmorBox& armor);

private:
	Ptr<SVM>svm;  //svm模型
	Mat p;		//载入到SVM中识别的矩阵
	Size armorImgSize; //SVM模型的识别图片大小（训练集的图片大小）

	Mat warpPerspective_src; //透射变换的原图
	Mat warpPerspective_dst; //透射变换生成的目标图
	Mat warpPerspective_mat; //透射变换的变换矩阵
	Point2f srcPoints[4];   //透射变换的原图上的目标点 tl->tr->br->bl  左上 右上 右下 左下
	Point2f dstPoints[4];	//透射变换的目标图中的点   tl->tr->br->bl  左上 右上 右下 左下
};

/**
* @brief: 装甲板识别类，实现装甲板两侧灯条的检测，
*		  装甲板的灯条匹配，装甲板的筛选，装甲板数字识别，选择目标等功能
*/
class ArmorDetector
{
public:
	ArmorDetector();
	~ArmorDetector();
	/**
	 * @brief: load svm model for client
	 * @param: the model file path of svm
	 */
	void loadSVM(const char * model_path, Size armorImgSize = Size(40, 40));

	/**
	 * @brief: set enemyColor  设置敌方颜色
	 */
	void setEnemyColor(Color enemyColor);

	/**
	 *@brief: for client, set the target armor number 操作手用，设置目标装甲板数字
	 */
	void setTargetNum(const int & targetNum);

	/**
	 *@brief: reset the ArmorDetector(delete the priviois lights and armors) to start next frame detection 重设检测器（删除原有的灯条和装甲板s）和装甲板状态，以便进行下一帧的检测
	 */
	void resetDetector();

	/**
	 * @brief: load source image and set roi if roiMode is open and found target in last frame 载入源图像并进行图像预处理
	 * @param: const Mat& src     源图像的引用
	 */
    void setImg(Mat & src);

	/**
	 * @brief: find all the possible lights of armor (get lights) 检测所有可能的灯条
	 */
	void findLights();

	/**
	* @brief: match lights into armors (get armors) 将识别到的灯条拟合为装甲板
	*/
	void matchArmors();

	/**
	 *@brief: set the privious targetArmor as lastArmor and then choose the most valuable armor from current armors as targetArmor (set targetArmor and lastArmor)
	 *			将上一帧的目标装甲板作为lastArmor选择本帧图像中所有装甲板里面价值最大的装甲板作为目标装甲板
	 */
	void setTargetArmor();

	/**
	 *@brief: an integrative function to run the Detector 集成的装甲板检测识别函数
	 */
    void run(Mat & src);

    /**
     *@brief: return the Detector status 识别程序是否识别到装甲版
     *@return: FOUND(1) NOT_FOUND(0)
     */
    bool isFoundArmor();

	/**
	 *@brief: show all the informations of this frame detection  显示所有信息
	 */
	void showDebugInfo(bool showSrcImg_ON, bool showSrcBinary_ON, bool showLights_ON, bool showArmors_ON, bool textLights_ON, bool textArmors_ON, bool textScores_ON);


	/**
	 *@brief: 将detector的结果输出
	 */
    void getTargetInfo(vector<Point2f> &armorVertices, Point2f &centerPoint, ArmorType &type);

	

private:
	Mat srcImg;  //(current frame acquired from camera) 从相机采集的当前的图像帧
	Mat srcImg_binary; //源图像的二值图
	Color enemyColor;  //敌方颜色
	int targetNum; //操作手设定的目标装甲板数字
	vector<LightBar> lights; //找到的灯条
	vector<ArmorBox> armors; //识别到的所有装甲板
	ArmorBox targetArmor; //当前图像帧对应的目标装甲板
	ArmorBox lastArmor;  //上一帧图像的目标装甲板
	ArmorNumClassifier classifier; //获取装甲板图像及识别装甲板数字的类
	DetectorState state; //装甲板检测器的状态，随着装甲板进程的执行而不断更新 
};


#endif // !ARMOR

