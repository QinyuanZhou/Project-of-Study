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
    void processA();            // ִ��AͼƬ����
    void processB();            // ִ��BͼƬ����
    void imageDataToMat();      // ��ImageData��������ת��ΪMat��������
    void matToImageData();      // ��Mat��������ת��ΪImageData��������
    void imageDataToHImage();   // ��ImageData��������ת��ΪHImage��������
    void HImageToImageData();   // ��HImage��������ת��ΪImageData��������
    // demo                       �������ӡ�������Ծ
    void hist(Mat matInput);
    //void hist_halcon(Hobject& hobjectInout);
    // Preprocess-Image Calulator ͼ�����������������Զ
//    void rotateImage(Hobject& himage);
//    void invertImage(Hobject& himage);
    void calculator(structImageCalculatorProcessor calculatorProcessor,Hobject& himage);

    // Preprocess-Image Binning   ͼ�����(resize)���������ճɴ�

    // Preprocess-Image Fliter    ����������������ǧ

    // Preprocess-Image Generator

    // Preprocess-Image Shift     ͼ����λ�������������
private:
    ImageData* imgAInput;
    ImageData* imgBInput;
    bool imageGetted;                       // ���������������
    Mat matAInput;
    Mat matBInput;
    Hobject himageInputA;
    Hobject himageInputB;
//    Hobject himageOutputA;
//    Hobject himageOutputB;
    bool matGetted;                         // Ԥ������������
    structProcessorGroup processAGroup;     // AͼƬ�������
    structProcessorGroup processBGroup;     // BͼƬ�������
    bool parameterGetted;
};

#endif // PREPROCESSFUNCS

