#include "preprocessfuncs.h"
PreProcesssFuncs::PreProcesssFuncs()
{
    imgAInput = new ImageData();
    imgBInput = new ImageData();
    imageGetted = false;
    matGetted = false;
    parameterGetted = false;
}
PreProcesssFuncs::~PreProcesssFuncs()
{
    if(imageGetted)
    {
        delete imgAInput;
        delete imgBInput;
    }
}
void PreProcesssFuncs::imageDataToMat()
{
    matAInput = Mat(imgAInput->height(),imgAInput->width(),CV_64FC1,imgAInput->buffer()).clone();
    matBInput = Mat(imgAInput->height(),imgAInput->width(),CV_64FC1,imgBInput->buffer()).clone();
#if 1
    // CV64FC1的数据类型可能OpenCV的很多算法并不支持，但是可以很方便地转成CV_8UC1类型
    matAInput.convertTo(matAInput,CV_8UC1);
    matBInput.convertTo(matBInput,CV_8UC1);
#endif

    matGetted = true;
}
void PreProcesssFuncs::imageDataToHImage()
{
    // 获取长宽，创建字符缓冲区
    int widthA = imgAInput->width();
    int heightA = imgAInput->height();
    uchar* dataArrayA = new uchar[widthA*heightA];
    for(int i=0;i<heightA*widthA;i++)
    {
        dataArrayA[i] = imgAInput->buffer()[i];
    }
    int widthB = imgBInput->width();
    int heightB = imgBInput->height();
    uchar* dataArrayB = new uchar[widthB*heightB];
    for(int i=0;i<heightB*widthB;i++)
    {
        dataArrayB[i] = imgBInput->buffer()[i];
    }
    // 构造HImage图像
    gen_image1(&himageInputA,"byte",widthA,heightA,(Hlong)(dataArrayA));
    gen_image1(&himageInputB,"byte",widthB,heightB,(Hlong)(dataArrayB));

    // 释放New的内存
    delete dataArrayA;
    delete dataArrayB;
//    double value1 = 0,value2 = 0,value3 = 0,value4 = 0;
//    get_grayval(himageInputA,20,30,&value1);
//    get_grayval(himageInputA,50,80,&value2);
//    value3 = imgAInput->buffer()[20*imgAInput->width()+30];
//    value4 = imgAInput->buffer()[50*imgAInput->width()+80];
//    int a = 1;
}

void PreProcesssFuncs::matToImageData()
{
    if(!imageGetted)
    {
        imgAInput = new ImageData();
        imgBInput = new ImageData();
        imageGetted = true;
    }
    // 保证数据区的类型一致
    if(matAInput.type()==CV_8UC1)
    {
        matAInput.convertTo(matAInput,CV_64FC1);
        matBInput.convertTo(matBInput,CV_64FC1);
    }

    imgAInput->setWidth(matAInput.cols);
    imgAInput->setHeight(matAInput.rows);
    imgAInput->createBuf();
    for(int i=0;i<matAInput.rows;i++)
    {
        for(int j=0;j<matAInput.cols;j++)
        {
            imgAInput->buffer()[i*matAInput.cols+j] = matAInput.at<double>(i,j);
        }
    }

    imgBInput->setHeight(matBInput.rows);
    imgBInput->setWidth(matBInput.cols);
    imgBInput->createBuf();
    for(int i=0;i<matBInput.rows;i++)
    {
        for(int j=0;j<matBInput.cols;j++)
        {
            imgBInput->buffer()[i*matBInput.cols+j] = matBInput.at<double>(i,j);
        }
    }
}
void PreProcesssFuncs::HImageToImageData()
{
    if(!imageGetted)
    {
        imgAInput = new ImageData();
        imgBInput = new ImageData();
        imageGetted = true;
    }
    uchar* ptr;
    char cType[MAX_STRING];
    Hlong width,height;
    // imageA convert
    convert_image_type(himageInputA,&himageInputA,"byte");
    get_image_pointer1(himageInputA,(Hlong*)&ptr,cType,&width,&height);
    imgAInput->setWidth(width);
    imgAInput->setHeight(height);
    imgAInput->createBuf();
    for(int i=0;i<width*height;i++)
    {
        imgAInput->buffer()[i] = ptr[i];
    }
    // imageB convert
    convert_image_type(himageInputB,&himageInputB,"byte");
    get_image_pointer1(himageInputB,(Hlong*)&ptr,cType,&width,&height);
    imgBInput->setWidth(width);
    imgBInput->setHeight(height);
    imgBInput->createBuf();
    for(int i=0;i<height*width;i++)
    {
        imgBInput->buffer()[i] = ptr[i];
    }
//    double value1 = 0,value2 = 0,value3 = 0,value4 = 0;
//    get_grayval(himageInputA,20,30,&value1);
//    get_grayval(himageInputA,50,80,&value2);
//    value3 = imgAInput->buffer()[20*imgAInput->width()+30];
//    value4 = imgAInput->buffer()[50*imgAInput->width()+80];
//    int a = 1;
}

void PreProcesssFuncs::setImageData(ImageData* imgAPass,ImageData* imgBPass)
{
    imgAPass->copyTo(imgAInput);
    imgBPass->copyTo(imgBInput);
    imageGetted = true;
}
void PreProcesssFuncs::setParameters(structPreProcessor &PreProcessorParameter)
{
    if(PreProcessorParameter.ProcessorGroup.count()==0)
    {
        parameterGetted = false;
    }
    else if(PreProcessorParameter.ProcessorGroup.count()==1)
    {
        processAGroup = PreProcessorParameter.ProcessorGroup[0];
        processBGroup = PreProcessorParameter.ProcessorGroup[0];
        parameterGetted = true;
    }
    else
    {
        processAGroup = PreProcessorParameter.ProcessorGroup[0];
        processBGroup = PreProcessorParameter.ProcessorGroup[1];
        parameterGetted = true;
    }
}
void PreProcesssFuncs::processA()
{
    //hist(matAInput);
    //hist_halcon(himageInputA);
//    rotateImage(himageInputA);
//    invertImage(himageInputA);
    if(parameterGetted)
    {
        for (int i = 0; i < processAGroup.ProcessorType.length(); i++)
        {
            if (processAGroup.ProcessorType[i] == "Calculator")
            {
                structImageCalculatorProcessor CalculatorProcessor = processAGroup.Processor[i].value<structImageCalculatorProcessor>();

                // 这里是图像计算函数
                calculator(CalculatorProcessor,himageInputA);
            }
            else if(processAGroup.ProcessorType[i] == "Binning")
            {
                structImageBinningProcessor BinningProcessor = processAGroup.Processor[i].value<structImageBinningProcessor>();
                // 这里是图像分类函数
            }
            else if(processAGroup.ProcessorType[i] == "Filter")
            {
                structImageFilterProcessor FilterProcessor = processAGroup.Processor[i].value<structImageFilterProcessor>();
                // 这里是图像滤波函数
            }
            else if(processAGroup.ProcessorType[i] == "Generator")
            {
                structImageGeneratorProcessor GeneratorProcessor = processAGroup.Processor[i].value<structImageGeneratorProcessor>();
            }
            else if(processAGroup.ProcessorType[i] ==  "Deformation")
            {
                 structImageDeformationProcessor DeformationProcessor = processAGroup.Processor[i].value<structImageDeformationProcessor>();
                 // 这里是图像移位函数
            }
        }
    }
}
void PreProcesssFuncs::processB()
{
    if(matGetted&&parameterGetted)
    {
        for (int i = 0; i < processBGroup.ProcessorType.length(); i++)
        {
            if (processBGroup.ProcessorType[i] == "Calculator")
            {
                structImageCalculatorProcessor CalculatorProcessor = processBGroup.Processor[i].value<structImageCalculatorProcessor>();
                // 这里是图像计算函数
            }
            else if(processBGroup.ProcessorType[i] == "Binning")
            {
                structImageBinningProcessor BinningProcessor = processBGroup.Processor[i].value<structImageBinningProcessor>();
                // 这里是图像分类函数
            }
            else if(processBGroup.ProcessorType[i] == "Filter")
            {
                structImageFilterProcessor FilterProcessor = processBGroup.Processor[i].value<structImageFilterProcessor>();
                // 这里是图像滤波函数
            }
            else if(processBGroup.ProcessorType[i] == "Generator")
            {
                structImageGeneratorProcessor GeneratorProcessor = processBGroup.Processor[i].value<structImageGeneratorProcessor>();
            }
            else if(processBGroup.ProcessorType[i] ==  "Deformation")
            {
                 structImageDeformationProcessor DeformationProcessor = processBGroup.Processor[i].value<structImageDeformationProcessor>();
                 // 这里是图像移位函数
            }
        }
    }
}

void PreProcesssFuncs::exectePreProcess()
{
    //imageDataToMat();
    imageDataToHImage();
    processA();         // 自己做测试的时候可以只处理一张图片，选择性注释即可
    processB();
    HImageToImageData();
    //matToImageData();
}

ImageData* PreProcesssFuncs::getImageA()
{
    return imgAInput;
}

ImageData* PreProcesssFuncs::getImageB()
{
    return imgBInput;
}
void PreProcesssFuncs::hist(Mat matInput)
{
    equalizeHist(matInput,matInput);
}

//void PreProcesssFuncs::invertImage(Hobject &himage)
//{
//    invert_image(himage,&himage);
//}

void PreProcesssFuncs::calculator(structImageCalculatorProcessor calculatorProcessor, Hobject &himage)
{   Hobject operateImage;
//    Hlong process;
    if(calculatorProcessor.OperatorType == 0 || calculatorProcessor.OperatorType == 1 || calculatorProcessor.OperatorType == 2 ||calculatorProcessor.OperatorType == 3)
    {
        if(calculatorProcessor.UseImageOperand)
        {   if(!calculatorProcessor.ImageSource)
            {   QString WorkPath = "F:/PIV_pro/RTA_PIV_FINAL20180124/RTA_PIV_FINAL/Image";
                WorkPath=WorkPath+calculatorProcessor.OperandImageFilePath;
                QByteArray m = WorkPath.toLatin1();
                char* imageSourse = m.data();//convert QString to char*
                read_image(&operateImage,imageSourse);
            }
        }
        else
        {Hlong rows , columns,row,col;
         get_image_size(himage,&rows,&columns);
         gen_image_const(&operateImage,"byte",columns,rows);
         unsigned char* pointer=NULL;
         char cType[MAX_STRING];
         get_image_pointer1(operateImage,(Hlong*)&pointer,cType,&columns,&rows);
         for (row=0; row<rows-1; row++)
         {
           for (col=0; col<columns-1; col++)
           {
             pointer[row*columns+col] = calculatorProcessor.OperandConstant;
           }
         }
        }
    }
    switch(calculatorProcessor.OperatorType)
    {case 0:
        add_image(himage,operateImage,&himage,1.0,0.0);
        break;
     case 1:
        if (!calculatorProcessor.ReverseOrder)
            sub_image(himage,operateImage,&himage,1.0,0.0);
        else
            sub_image(operateImage,himage,&himage,1.0,0.0);
        break;
     case 2:
        mult_image(himage,operateImage,&himage,1.0,0.0);
        break;
     case 3:
        if (!calculatorProcessor.ReverseOrder)
            div_image(himage,operateImage,&himage,1.0,0.0);
        else
            div_image(operateImage,himage,&himage,1.0,0.0);
        break;
     case 4:
        break;
     case 5:
        invert_image(himage,&himage);//invert the grayscale
        break;
     case 6:
        rotate_image(himage,&himage,270,"constant");//rotate the image by positive 90 degrees
        break;
     case 7:
        rotate_image(himage,&himage,90,"constant");//rotate the image by negative 90 degrees
        break;
     case 8:
        mirror_image(himage,&himage,"row");//filp the image horizontally
        break;
     case 9:
        mirror_image(himage,&himage,"column");//flip the image vertically
        break;
     default:
        break;
    }
}
//void PreProcesssFuncs::hist_halcon(Hobject& hobjectInout)
//{
//    equ_histo_image(hobjectInout,&hobjectInout);
// //    equ_histo_image(himageInputB,&himageInputB);
//}
//void PreProcesssFuncs::rotateImage(Hobject& himage)
//{
//    rotate_image(himage,&himage,90,"constant");
//}
