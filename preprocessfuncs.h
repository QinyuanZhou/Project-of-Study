#ifndef PREPROCESSFUNCS
#define PREPROCESSFUNCS
#include "rtapivdata/imagedata.h"
#include "preprocessordialog.h"
#include <opencv2/opencv.hpp>
#include "HalconCpp.h"
using namespace cv;
using namespace Halcon;
class PreProcesssFuncs
{
public:
    PreProcesssFuncs();
    ~PreProcesssFuncs();
    void setImageData(ImageData* imgAPass,ImageData* imgBPass);
    void setParameters(structPreProcessor &PreProcessorParameter);
    ImageData* getImageA();
    ImageData* getImageB();
    void exectePreProcess();
protected:
    void processA();            // 执行A图片处理
    void processB();            // 执行B图片处理
    void imageDataToMat();      // 将ImageData数据类型转换为Mat数据类型
    void matToImageData();      // 将Mat数据类型转换为ImageData数据类型
    void imageDataToHImage();   // 将ImageData数据类型转换为HImage数据类型
    void HImageToImageData();   // 将HImage数据类型转换为ImageData数据类型
    // demo                       流程例子——王飞跃
    void hist(Mat matInput);
    //void hist_halcon(Hobject& hobjectInout);
    // Preprocess-Image Calulator 图像运算操作——周勤远
//    void rotateImage(Hobject& himage);
//    void invertImage(Hobject& himage);
    void calculator(structImageCalculatorProcessor calculatorProcessor,Hobject& himage);

    // Preprocess-Image Binning   图像分类(resize)操作——陶成川

    // Preprocess-Image Fliter    过滤器操作——万千

    // Preprocess-Image Generator

    // Preprocess-Image Shift     图像移位操作——龚宸婷
private:
    ImageData* imgAInput;
    ImageData* imgBInput;
    bool imageGetted;                       // 输入输出数据类型
    Mat matAInput;
    Mat matBInput;
    Hobject himageInputA;
    Hobject himageInputB;
//    Hobject himageOutputA;
//    Hobject himageOutputB;
    bool matGetted;                         // 预处理数据类型
    structProcessorGroup processAGroup;     // A图片处理参数
    structProcessorGroup processBGroup;     // B图片处理参数
    bool parameterGetted;
};

#endif // PREPROCESSFUNCS

