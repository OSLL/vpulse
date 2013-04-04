#include "processor.h"
using namespace std;

//temporary_functions======================================
void processor::PrintData(double* src, long len, const char* filename)
{
    //long len = this->NumberOfFrames*this->frameHeight*this->frameWidth*3;
    int currNumofFr=0;
    int curRow=0;
    int curCol=0;
    //int curCh=0;
        FILE * stream;
                if ((stream=fopen(filename, "w")) != 0)
        {

            for(long curr_id1 = 0; curr_id1 <= len-1; curr_id1++)
            {
                if (currNumofFr==NumberOfFrames+1)
                {
                    currNumofFr=0;
                    curRow++;
                    fprintf(stream, "\n \t Row = %d\n",curRow);
                    if(curRow==this->frameHeight+1)
                    {
                        curRow=0;
                        curCol++;
                        fprintf(stream, "\n \t Col = %d\n",curCol);
                        if(curCol==frameWidth+1)
                        {
                            curCol=0;
                            //curCh++;
                            //fprintf(stream, "\n \t Ch = %d\n",curCh);
                        }
                    }
                }
                fprintf(stream, "%f", src[curr_id1]);
                fputc('\n',stream);
                currNumofFr++;
            };
        }
        fclose(stream);
}

//=========================================================
void PrintDataInt(int* src, long len, const char* filename)
{
        FILE * stream;
                if ((stream=fopen(filename, "w")) != 0)
        {

            for(long curr_id1 = 0; curr_id1 <= len-1; curr_id1++)
            {

                fprintf(stream, "%ld. \t %d",curr_id1, src[curr_id1]);
                fputc('\n',stream);

            };
        }
        fclose(stream);
}
//========================================================
void PrintDataCmx(fftw_complex* src, long len, const char* filename)
{
        FILE * stream;
                if ((stream=fopen(filename, "w")) != 0)
        {

            for(long curr_id1 = 0; curr_id1 <= len-1; curr_id1++)
            {

                fprintf(stream, "%ld. \t| re = %lf, \t| \t im = %lf",curr_id1, src[curr_id1][0],src[curr_id1][0]);
                fputc('\n',stream);

            };
        }
        fclose(stream);
}
//=========================================================
void PrintDataDb(double* src, long len, const char* filename)
{
        FILE * stream;
                if ((stream=fopen(filename, "w")) != 0)
        {

            for(long curr_id1 = 0; curr_id1 <= len-1; curr_id1++)
            {

                fprintf(stream, "%ld. \t %lf",curr_id1, src[curr_id1]);
                fputc('\n',stream);

            };
        }
        fclose(stream);
}
//=========================================================

void print_frame_N(int N,double* src, int num_of_fr, const char* filename, int height_fr, int width_fr)
{
    FILE * stream;
            if ((stream=fopen(filename, "w")) != 0)
    {
    for(int row_id=0; row_id<height_fr; row_id++)
    {
        fprintf(stream,"\n \t row = %d \n\n",row_id);
        for(int col_id=0; col_id<width_fr; col_id++)
        {
                fprintf(stream, "%d. \t %lf || \t %lf || \t %lf",col_id,
                        src[N + row_id*num_of_fr + col_id*num_of_fr*height_fr],
                        src[N + row_id*num_of_fr + col_id*num_of_fr*height_fr + num_of_fr*height_fr*width_fr],
                        src[N + row_id*num_of_fr + col_id*num_of_fr*height_fr + num_of_fr*height_fr*width_fr*2]);
                fputc('\n',stream);
        }
    }
    }

fclose(stream);
}

//==============================

void PrintDataFrame(double* src, int num_of_fr, const char* filename, int height_fr, int width_fr)
{
        FILE * stream;
                if ((stream=fopen(filename, "w")) != 0)
        {
            fprintf(stream, "№\t\t Y \t\t\t I\t\t\tQ\t\t\n");
            for(int col_id=0; col_id<width_fr; col_id++)
            {
                fprintf(stream,"\n \t column = %d \n\n",col_id);
                for(int row_id=0; row_id<height_fr; row_id++)
                {
                    fprintf(stream,"\n \t row = %d \n\n",row_id);
                    for(int curr_id1 = 0; curr_id1 < num_of_fr; curr_id1++)
                    {

                        fprintf(stream, "%d. \t %lf || \t %lf || \t %lf",curr_id1,
                                src[curr_id1 + row_id*num_of_fr + col_id*num_of_fr*height_fr],
                                src[curr_id1 + row_id*num_of_fr + col_id*num_of_fr*height_fr + num_of_fr*height_fr*width_fr],
                                src[curr_id1 + row_id*num_of_fr + col_id*num_of_fr*height_fr + num_of_fr*height_fr*width_fr*2]);
                        fputc('\n',stream);

                    };
                }
            }
        }
        fclose(stream);
}
//=========================================================

processor::processor(/*, Mat** Frames*/):
    debugflag(0),
    framecounter(0),
    bufferflag(0)
{

    //this->FramesToVector(Frames,this->AllFrames, this->frameWidth, this->frameHeight, this->NumberOfFrames);
}

void processor::init(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in)
{
    NumberOfFrames=NumberOfFrames_in;
    frameHeight=frameHeight_in;
    frameWidth=frameWidth_in;
    samplingRate=sRate_in;
    long LengthAll=this->NumberOfFrames*this->frameHeight*this->frameWidth*3;
    this->AllFrames=(double*)malloc(LengthAll*sizeof(double));
    this->buffer2=(double*)malloc(LengthAll*sizeof(double));
}

processor::~processor()
{
    free(this->AllFrames);
    free(this->buffer2);
}

void processor::addFrame(Mat* src)
{
    long rowD= NumberOfFrames*frameHeight;
    long colD=rowD*frameWidth;
    int invCh =0;
    //FIXME - FIxed bug with R-B channel rotation
    if(bufferflag==0){
        for(int ch = 2; ch >= 0; ch--)
        //for(int ch = 0; ch < 3; ch++)   //FIXME
        {
            for(int col = 0; col < frameWidth; col++)
            {
                for(int row = 0; row < frameHeight; row++)
                {
                    AllFrames[(long)invCh*colD+col*rowD+(long)row*(long)NumberOfFrames+(long)framecounter]=src->at<Vec3b>(row,col).val[ch];
                }
            }
            invCh++;
        }
    }
    else
    {
        for(int ch = 2; ch >= 0; ch--)
        {
            for(int col = 0; col < frameWidth; col++)
            {
                for(int row = 0; row < frameHeight; row++)
                {
                    buffer2[(long)invCh*colD+col*rowD+(long)row*(long)NumberOfFrames+(long)framecounter]=src->at<Vec3b>(row,col).val[ch];
                }
            }
            invCh++;
        }
    }
}

void processor::IncFrCounter(void)
{
    framecounter++;
}

int processor::getFrCounter(void)
{
    return(framecounter);
}

void processor::FramesToVector(Mat** src, double* dst, int frWidth, int frHeight, int NofFrames)
{

    long rowD= NofFrames*frHeight;
    long colD=rowD*frWidth;
    int invCh =0;                       //FIXME - FIxed bug with R-B channel rotation
    for(int ch = 2; ch >= 0; ch--)
    //for(int ch = 0; ch < 3; ch++)   //FIXME
    {
        for(int col = 0; col < frWidth; col++)
        {
            for(int row = 0; row < frHeight; row++)
            {
                for(int t = 0; t < NofFrames; t++)
                {
                    dst[(long)invCh*colD+col*rowD+(long)row*(long)NofFrames+(long)t]=src[t]->at<Vec3b>(row,col).val[ch];
                }
            }
        }
        invCh++;
    }
}

void processor::VectorToFrames(double* src, Mat** dst, int frWidth, int frHeight, int NofFrames)
{
    long rowD= (long)NofFrames*(long)frHeight;
    long colD=(long)rowD*(long)frWidth;
    int invCh =0;
    for(int ch = 2; ch >= 0; ch--)
    //for(int ch = 0; ch < 3; ch++)     //FIXME
    {
        for(int col = 0; col < frWidth; col++)
        {
            for(int row = 0; row < frHeight; row++)
            {
                for(int t = 0; t < NofFrames; t++)
                {
                    dst[t]->at<Vec3b>(row,col).val[ch] = src[invCh*colD+col*rowD+row*NofFrames+t];
                }
            }
        }
        invCh++;
    }
}

void processor::rgb2yiq(double* srcDst, int frWidth, int frHeight, int NofFrames)
{
    long chD=(long)NofFrames*(long)frHeight*(long)frWidth;
    double tmp[3];                  //FIXME BUG FIXED
    for(long ch = 0; ch <chD; ch++)
    {
        /*if((debugflag==1)&&(ch<3))
        {
            printf("%ld srcDst[%ld]=%lf\n",ch,ch,srcDst[ch]);
            printf("%ld srcDst[%ld]=%lf\n",ch,ch+chD,srcDst[ch+chD]);
            printf("%ld srcDst[%ld]=%lf\n",ch,ch+chD*2,srcDst[ch+chD*2]);
        }*/
       tmp[0]=srcDst[ch];
       tmp[1]=srcDst[ch+chD];
       tmp[2]=srcDst[ch+chD*2];
       srcDst[ch] = tmp[0]*rgb2yiqCoef[0] + tmp[1]*rgb2yiqCoef[1] + tmp[2]*rgb2yiqCoef[2];
       srcDst[ch+chD] = tmp[0]*rgb2yiqCoef[3] + tmp[1]*rgb2yiqCoef[4] + tmp[2]*rgb2yiqCoef[5];
       srcDst[ch+chD*2] = tmp[0]*rgb2yiqCoef[6] + tmp[1]*rgb2yiqCoef[7] + tmp[2]*rgb2yiqCoef[8];
       /*if((debugflag==1)&&(ch<3))
       {
           printf("%ld srcDst[%ld]=%lf\n",ch,ch,srcDst[ch]);
           printf("%ld srcDst[%ld]=%lf\n",ch,ch+chD,srcDst[ch+chD]);
           printf("%ld srcDst[%ld]=%lf\n",ch,ch+chD*2,srcDst[ch+chD*2]);
           printf("\n\n");
       }*/
    }

}
void processor::yiq2rgb(double* srcDst, int frWidth, int frHeight, int NofFrames)
{
    long chD=(long)NofFrames*(long)frHeight*(long)frWidth;
    double tmp[3];                      //FIXME BUG FIXED
    for(long ch = 0; ch <chD; ch++)
    {
        tmp[0]=srcDst[ch];
        tmp[1]=srcDst[ch+chD];
        tmp[2]=srcDst[ch+chD*2];
        srcDst[ch] = tmp[0]*yiq2rgbCoef[0] + tmp[1]*yiq2rgbCoef[1] + tmp[2]*yiq2rgbCoef[2];
        srcDst[ch+chD] = tmp[0]*yiq2rgbCoef[3] + tmp[1]*yiq2rgbCoef[4] + tmp[2]*yiq2rgbCoef[5];
        srcDst[ch+chD*2] = tmp[0]*yiq2rgbCoef[6] + tmp[1]*yiq2rgbCoef[7] + tmp[2]*yiq2rgbCoef[8];
    }
}

void processor::normalize(double* src, long len, double factor)
{
    for(long i=0; i<len; i++)
    {
        src[i]=src[i]/factor;
    }
}

int* processor::createFreqMask(double fLow, double fHight)
{
    int* indexes=(int*)malloc((this->NumberOfFrames+1)*sizeof(int));
    int j=0;
    double* mask=(double*)malloc(this->NumberOfFrames*sizeof(double));
    for(int i=0; i<this->NumberOfFrames;i++)
    {
        mask[i]=(double)i/(double)NumberOfFrames*(double)samplingRate;
        //printf("mask[%d]=%f\n",i,mask[i]);
        if((mask[i]>fLow)&&(mask[i]<fHight))
        {
            //printf("mask[%d]=%f \t > \t fLow= %f && \t < \t fHight= %f \n",i,mask[i],fLow, fHight);
            indexes[j]=i;
            j++;
        }
    }
    indexes[j]=-1; //indicates end of list of frequences to pass
    for(int k=j+1; k<NumberOfFrames+1; k++)
    {
        indexes[k]=0;
    }
    return(indexes);
}

void processor::copyVector(double* src, double* dst, long len)
{
    for(long i=0; i< len; i++)
    {
        dst[i]=src[i];
    }
}
void processor::copyFFTW_cpx(fftw_complex* src, fftw_complex* dst, long len)
{
    for(long i=0; i< len; i++)
    {
        dst[i][0]=src[i][0];
        dst[i][1]=src[i][1];
    }
}

void processor::applyMask(fftw_complex*src, fftw_complex* dst, int* mask, long len)
{
    int k=0;
    for(int i=0; i<len; i++)
    {
        if(i==mask[k])
        {
            dst[i][0]=src[i][0];
            dst[i][1]=src[i][1];
            k++;
        }
        else
        {
            dst[i][0]=0.0;
            dst[i][1]=0.0;
        }
    }
    //dst[9][0]=src[9][0];
    //dst[9][1]=src[9][1];
}

void processor::sumVector(double* src1, double *src2, double* dst, long len)
{
    for(long i=0; i<len; i++)
    {
        dst[i]=src1[i]+src2[i];
    }
}



void processor::work(double fLow, double fHight, double ampFactor)
{
    //char* UGDownStack_file = "UnnormGDownStack.log";
    //PrintDataFrame(this->AllFrames, this->NumberOfFrames, UGDownStack_file, this->frameHeight, this->frameWidth);
    /*char* Fr_from_arr = "FramesBlurredTxt/frame_arr0.log";
    char* Fr_from_arr1 = "FramesBlurredTxt/frame_arr1.log";
    char* Fr_from_arr2 = "FramesBlurredTxt/frame_arr2.log";
    char* Fr_from_arr291 = "FramesBlurredTxt/frame_arr291.log";
    print_frame_N(0,AllFrames, this->NumberOfFrames, Fr_from_arr, this->frameHeight, this->frameWidth);
    print_frame_N(1,AllFrames, this->NumberOfFrames, Fr_from_arr1, this->frameHeight, this->frameWidth);
    print_frame_N(2,AllFrames, this->NumberOfFrames, Fr_from_arr2, this->frameHeight, this->frameWidth);
    print_frame_N(290,AllFrames, this->NumberOfFrames, Fr_from_arr291, this->frameHeight, this->frameWidth);*/
    this->normalize((double*)this->AllFrames,(long) this->NumberOfFrames*this->frameHeight*this->frameWidth*3,255.0);
    this->rgb2yiq(this->AllFrames,this->frameWidth,this->frameHeight, this->NumberOfFrames);
    printf("flow= %f, fHight= %f \n", fLow, fHight);
    //char* GDownStack_file = "GDownStack.log";
    //PrintDataFrame(this->AllFrames, this->NumberOfFrames, GDownStack_file, this->frameHeight, this->frameWidth);
    int* mask=createFreqMask(fLow,fHight);

    //const char* filename_mask = "fr_mask.log";
    //PrintDataInt(mask,this->NumberOfFrames+1,filename_mask);

    //fft_header
    /*fftw_complex* out_fft;
    double* in_fft;
    in_fft = (double*)malloc(sizeof(double)*this->NumberOfFrames);
    out_fft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*this->NumberOfFrames);
    fftw_plan p = fftw_plan_dft_r2c_1d(this->NumberOfFrames,in_fft,out_fft,FFTW_MEASURE);

    //ifft_header
    fftw_complex* in_ifft= (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(this->NumberOfFrames));
    double* out_ifft = (double*)malloc(sizeof(double)*this->NumberOfFrames);
    fftw_plan p_ifft = fftw_plan_dft_c2r_1d(this->NumberOfFrames,in_ifft,out_ifft,FFTW_MEASURE);
    */

    for(int i=0; i<this->frameHeight*this->frameWidth*3; i++)
    {
        //fft:compute
        this->copyVector((double*)&this->AllFrames[i*this->NumberOfFrames],in_fft,this->NumberOfFrames);
        fftw_execute(p);

        //char* f_name_fftw_test = "fftw_test.log";
        //char* f_name_fft_in = "fft_in1.log";
        //PrintDataDb(in_fft,this->NumberOfFrames,f_name_fft_in);
        //PrintDataCmx(out_fft,this->NumberOfFrames,f_name_fftw_test);

        //ifft:compute
        this->applyMask(out_fft,in_ifft,mask,this->NumberOfFrames/2+1);
        fftw_execute(p_ifft);
        this->normalize(out_ifft, (long)this->NumberOfFrames,(double)this->NumberOfFrames/ampFactor);
        //this->sumVector(&this->AllFrames[i*this->NumberOfFrames],out_ifft,&this->AllFrames[i*this->NumberOfFrames],(long)this->NumberOfFrames);

        this->copyVector(out_ifft, &this->AllFrames[i*this->NumberOfFrames],this->NumberOfFrames);
        //char* f_name_fft_in2 = "fft_in2new.log";
        //PrintDataDb(out_ifft,this->NumberOfFrames,f_name_fft_in2);


    }
    //===========
    /*fftw_destroy_plan(p);
    fftw_destroy_plan(p_ifft);
    fftw_free(out_fft);
    fftw_free(in_ifft);
    free(in_fft);
    free(out_ifft);*/
//qDebug("FFT");
    //print fft_results
    /*char frame_filename[35];
    for(int i = 0; i< this->NumberOfFrames; i++)
    {
        sprintf(frame_filename, "PulseSignal/1fft_frame%d.log",i);
        PrintDataFrame(this->AllFrames,this->NumberOfFrames,frame_filename,this->frameHeight,this->frameWidth);
    }*/

    //this->yiq2rgb(this->AllFrames,this->frameWidth,this->frameHeight, this->NumberOfFrames);
    //this->normalize((double*)this->AllFrames,(long) this->NumberOfFrames*this->frameHeight*this->frameWidth*3,(double)1.0/255.0);
}

void processor::rgbBoarder(double* src, long len)
{
    for(long i=0; i<len; i++)
    {
        if(src[i]>1)
        {src[i]=1;}
        if(src[i]<0)
        {src[i]=0;}
    }
}

void processor::YIQ2RGBnormalizeColorChannels(double* srcDst, int frWidth, int frHeight, int NofFrames)
{
    long chD=(long)NofFrames*(long)frHeight*(long)frWidth;
    double tmp[3];                      //FIXME BUG FIXED
    for(long ch = 0; ch <chD; ch++)
    {
        tmp[0]=srcDst[ch];
        tmp[1]=srcDst[ch+chD];
        tmp[2]=srcDst[ch+chD*2];
        srcDst[ch] = tmp[0]*yiq2rgbCoef[0] + tmp[1]*yiq2rgbCoef[1] + tmp[2]*yiq2rgbCoef[2];
        srcDst[ch+chD] = tmp[0]*yiq2rgbCoef[3] + tmp[1]*yiq2rgbCoef[4] + tmp[2]*yiq2rgbCoef[5];
        srcDst[ch+chD*2] = tmp[0]*yiq2rgbCoef[6] + tmp[1]*yiq2rgbCoef[7] + tmp[2]*yiq2rgbCoef[8];
        double normfactor = 1.0;
        if(srcDst[ch] > 1.0){
            normfactor=srcDst[ch];
        }
        if((srcDst[ch+chD] > 1.0)&&(srcDst[ch+chD]>normfactor)){
            normfactor=srcDst[ch+chD];
        }
        if((srcDst[ch+chD*2] > 1.0)&&(srcDst[ch+chD*2]>normfactor)){
            normfactor=srcDst[ch+chD*2];
        }
        if(normfactor > 1.0)
        {
            srcDst[ch]=srcDst[ch]/normfactor;
            srcDst[ch+chD]=srcDst[ch+chD]/normfactor;
            srcDst[ch+chD*2]=srcDst[ch+chD*2]/normfactor;
        }
    }
}

int processor::AddPulseToFrames(Mat* frames, double* pulse, int NofFrames, int frame_number)
{
    int FrHeight = frames->rows;
    int FrWidth = frames->cols;
    long LengthAll = FrWidth*FrHeight*3;
    double* fullFrames = (double*)malloc(LengthAll*sizeof(double));
    double* pulseFrames = (double*)malloc(LengthAll*sizeof(double));
    this->FramesToVector(&frames,fullFrames, FrWidth, FrHeight, 1);
    /*if(frame_number<90){
    char frame_filename[35];
        sprintf(frame_filename, "BSum/frame%d.log",frame_number);
        PrintDataFrame(fullFrames,1,frame_filename,FrHeight,FrWidth);
    }*/
    //qDebug("curr_frame=%d",frame_number);
    this->normalize(fullFrames,LengthAll,255.0);
    this->rgb2yiq(fullFrames,FrWidth, FrHeight, 1);
    NearInterpolation(pulse,pulseFrames,frameWidth,frameHeight,FrWidth,FrHeight,NofFrames,frame_number);
    this->sumVector(fullFrames,pulseFrames,fullFrames,LengthAll);     //FIXME tmp above
    this->YIQ2RGBnormalizeColorChannels(fullFrames,FrWidth, FrHeight, 1);
    this->rgbBoarder(fullFrames,LengthAll);
    this->normalize(fullFrames,LengthAll,1.0/255.0);
    this->VectorToFrames(fullFrames,&frames,FrWidth,FrHeight,1);
    /*if(frame_number<90){
    char frame_filename[35];
        sprintf(frame_filename, "Sum/frame%d.log",frame_number);
        PrintDataFrame(fullFrames,1,frame_filename,FrHeight,FrWidth);
    }*/
    free(fullFrames);
    free(pulseFrames);

}


double* processor::getAllFrames(void)
{
    return(this->AllFrames);
}

int processor::getFrH(void)
{
    return(this->frameHeight);
}

int processor::getFrW(void)
{
    return(this->frameWidth);
}

int processor::getNFr(void)
{
    return(this->NumberOfFrames);
}

void processor::NearInterpolation(double* src, double* dst, int oldwidth, int oldheight, int newwidth, int newheight, int nofFr, int frameInd)
{

    //if(src == NULL) return false;
    //
    // Get a new buuffer to interpolate into
    //unsigned char* newData = new unsigned char [newWidth * newHeight * 3];

    double scalewidth =  (double)newwidth / (double)oldwidth;
    double scaleheight = (double)newheight / (double)oldheight;

    for(int cx = 0; cx < newwidth; cx++)
    {
        for(int cy = 0; cy < newheight; cy++)
        {
            int pixel = (cx * (newheight/**nofFr*/)) + (cy/**nofFr*/)/*+frameInd*/;
            long nearestMatch =  (((long)(cx / scalewidth) * (oldheight*nofFr)) + ((long)(cy / scaleheight)*nofFr) )+frameInd;
            //printf("pixel = %d\n",pixel);
            //printf("near = %d\n",nearestMatch);
            dst[pixel] =  src[nearestMatch];
            dst[pixel + newheight*newwidth/**nofFr*/] =  src[nearestMatch + oldheight*oldwidth*nofFr];
            dst[pixel + newheight*newwidth*2/**nofFr*/] =  src[nearestMatch + oldheight*oldwidth*nofFr*2];
        }
    }

    //
    //delete[] _data;
    //_data = newData;
    //_width = newWidth;
    //_height = newHeight;
}

void processor::initFFT_IFFT(void)
{
    in_fft = (double*)malloc(sizeof(double)*this->NumberOfFrames);
    out_fft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*this->NumberOfFrames);
    p = fftw_plan_dft_r2c_1d(this->NumberOfFrames,in_fft,out_fft,FFTW_MEASURE);

    //ifft_header
    in_ifft= (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(this->NumberOfFrames));
    out_ifft = (double*)malloc(sizeof(double)*this->NumberOfFrames);
    p_ifft = fftw_plan_dft_c2r_1d(this->NumberOfFrames,in_ifft,out_ifft,FFTW_MEASURE);
}

void processor::clearFFT_IFFT(void)
{
    fftw_destroy_plan(p);
    fftw_destroy_plan(p_ifft);
    fftw_free(out_fft);
    fftw_free(in_ifft);
    free(in_fft);
    free(out_ifft);
}

void processor::changeBufferFlag(void)
{
    if(bufferflag==0){bufferflag=1;}
    else{bufferflag=0;}
}
