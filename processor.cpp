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


processor::processor(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in, Mat** Frames):
    NumberOfFrames(NumberOfFrames_in),
    frameHeight(frameHeight_in),
    frameWidth(frameWidth_in),
    samplingRate(sRate_in)
{
    long LengthAll=this->NumberOfFrames*this->frameHeight*this->frameWidth*3;
    this->AllFrames=(double*)malloc(LengthAll*sizeof(double));
    this->FramesToVector(Frames,this->AllFrames, this->frameWidth, this->frameHeight, this->NumberOfFrames);
}

processor::~processor()
{
    free(this->AllFrames);
}

void processor::FramesToVector(Mat** src, double* dst, int frWidth, int frHeight, int NofFrames)
{

    long rowD= NofFrames*frHeight;
    long colD=rowD*frWidth;

    for(int ch = 2; ch >= 0; ch--)
    {
        for(int col = 0; col < frWidth; col++)
        {
            for(int row = 0; row < frHeight; row++)
            {
                for(int t = 0; t < NofFrames; t++)
                {
                    dst[ch*colD+col*rowD+row*NofFrames+t]=src[t]->at<Vec3b>(row,col).val[ch];
                }
            }
        }
    }
}

void processor::VectorToFrames(double* src, Mat** dst, int frWidth, int frHeight, int NofFrames)
{
    long rowD= (long)NofFrames*(long)frHeight;
    long colD=(long)rowD*(long)frWidth;

    for(int ch = 2; ch >= 0; ch--)
    {
        for(int col = 0; col < frWidth; col++)
        {
            for(int row = 0; row < frHeight; row++)
            {
                for(int t = 0; t < NofFrames; t++)
                {
                    dst[t]->at<Vec3b>(row,col).val[ch] = src[ch*colD+col*rowD+row*NofFrames+t];
                }
            }
        }
    }
}

void processor::rgb2yiq(double* srcDst, int frWidth, int frHeight, int NofFrames)
{
    long chD=(long)NofFrames*(long)frHeight*(long)frWidth;
    for(long ch = 0; ch <chD; ch++)
    {
       srcDst[ch] = srcDst[ch]*rgb2yiqCoef[0] + srcDst[ch+chD]*rgb2yiqCoef[1] + srcDst[ch+chD*2]*rgb2yiqCoef[2];
       srcDst[ch+chD] = srcDst[ch]*rgb2yiqCoef[3] + srcDst[ch+chD]*rgb2yiqCoef[4] + srcDst[ch+chD*2]*rgb2yiqCoef[5];
       srcDst[ch+chD*2] = srcDst[ch]*rgb2yiqCoef[6] + srcDst[ch+chD]*rgb2yiqCoef[7] + srcDst[ch+chD*2]*rgb2yiqCoef[8];
    }

}
void processor::yiq2rgb(double* srcDst, int frWidth, int frHeight, int NofFrames)
{
    long chD=(long)NofFrames*(long)frHeight*(long)frWidth;
    for(long ch = 0; ch <chD; ch++)
    {
        srcDst[ch] = srcDst[ch]*yiq2rgbCoef[0] + srcDst[ch+chD]*yiq2rgbCoef[1] + srcDst[ch+chD*2]*yiq2rgbCoef[2];
        srcDst[ch+chD] = srcDst[ch]*yiq2rgbCoef[3] + srcDst[ch+chD]*yiq2rgbCoef[4] + srcDst[ch+chD*2]*yiq2rgbCoef[5];
        srcDst[ch+chD*2] = srcDst[ch]*yiq2rgbCoef[6] + srcDst[ch+chD]*yiq2rgbCoef[7] + srcDst[ch+chD*2]*yiq2rgbCoef[8];
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
    for(int i=0; i<len; i++)
    {
        dst[i]=src1[i]+src2[i];
    }
}

void processor::work(double fLow, double fHight, double ampFactor)
{
    this->normalize((double*)this->AllFrames,(long) this->NumberOfFrames*this->frameHeight*this->frameWidth*3,255.0);
    this->rgb2yiq(this->AllFrames,this->frameWidth,this->frameHeight, this->NumberOfFrames);
    printf("flow= %f, fHight= %f \n", fLow, fHight);
    int* mask=createFreqMask(fLow,fHight);

    //const char* filename_mask = "fr_mask.log";
    //PrintDataInt(mask,this->NumberOfFrames+1,filename_mask);

    //fft_header
    fftw_complex* out_fft;
    double* in_fft;
    in_fft = (double*)malloc(sizeof(double)*this->NumberOfFrames);
    out_fft = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*this->NumberOfFrames);
    fftw_plan p = fftw_plan_dft_r2c_1d(this->NumberOfFrames,in_fft,out_fft,FFTW_MEASURE);

    //ifft_header
    fftw_complex* in_ifft= (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*(this->NumberOfFrames));
    double* out_ifft = (double*)malloc(sizeof(double)*this->NumberOfFrames);
    fftw_plan p_ifft = fftw_plan_dft_c2r_1d(this->NumberOfFrames,in_ifft,out_ifft,FFTW_MEASURE);

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
        //char* f_name_fft_in2 = "fft_in2.log";
        //PrintDataDb(out_ifft,this->NumberOfFrames,f_name_fft_in2);
    }
    //===========
    fftw_destroy_plan(p);
    fftw_destroy_plan(p_ifft);
    fftw_free(out_fft);
    fftw_free(in_ifft);
    free(in_fft);
    free(out_ifft);


    this->yiq2rgb(this->AllFrames,this->frameWidth,this->frameHeight, this->NumberOfFrames);
    this->normalize((double*)this->AllFrames,(long) this->NumberOfFrames*this->frameHeight*this->frameWidth*3,(double)1.0/255.0);
}


int AddPulseToFrames(Mat** frames, Mat** pulse)
{

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
