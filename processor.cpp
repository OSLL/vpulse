#include "processor.h"
using namespace std;

//temporary_functions======================================
void processor::PrintData(float* src, long len, const char* filename)
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
//=========================================================


processor::processor(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in, Mat** Frames):
    NumberOfFrames(NumberOfFrames_in),
    frameHeight(frameHeight_in),
    frameWidth(frameWidth_in),
    samplingRate(sRate_in)
{
    long LengthAll=this->NumberOfFrames*this->frameHeight*this->frameWidth*3;
    this->AllFrames=(float*)malloc(LengthAll*sizeof(float));
    this->FramesToVector(Frames,this->AllFrames);
}

processor::~processor()
{
    free(this->AllFrames);
}

void processor::FramesToVector(Mat** src, float* dst)
{

    int rowD= NumberOfFrames*frameHeight;
    int colD=rowD*frameWidth;

    for(int ch = 2; ch >= 0; ch--)
    {
        for(int col = 0; col < this->frameWidth; col++)
        {
            for(int row = 0; row < this->frameHeight; row++)
            {
                for(int t = 0; t < this->NumberOfFrames; t++)
                {
                    dst[ch*colD+col*rowD+row*NumberOfFrames+t]=src[t]->at<Vec3b>(row,col).val[ch];
                }
            }
        }
    }
}

void processor::rgb2yiq(void)
{
    long chD=this->NumberOfFrames*this->frameHeight*this->frameWidth;
    for(long ch = 0; ch <chD; ch++)
    {
        AllFrames[ch] = AllFrames[ch]*rgb2yiqCoef[0] + AllFrames[ch+chD]*rgb2yiqCoef[1] + AllFrames[ch+chD*2]*rgb2yiqCoef[2];
        AllFrames[ch+chD] = AllFrames[ch]*rgb2yiqCoef[3] + AllFrames[ch+chD]*rgb2yiqCoef[4] + AllFrames[ch+chD*2]*rgb2yiqCoef[5];
        AllFrames[ch+chD*2] = AllFrames[ch]*rgb2yiqCoef[6] + AllFrames[ch+chD]*rgb2yiqCoef[7] + AllFrames[ch+chD*2]*rgb2yiqCoef[8];
    }

}

void processor::normalize(void)
{
    for(int i=0; i<this->NumberOfFrames*this->frameHeight*this->frameWidth*3; i++)
    {
        this->AllFrames[i]=this->AllFrames[i]/255.0;
    }
}

int* processor::createFreqMask(float fLow, float fHight)
{
    int* indexes=(int*)malloc((this->NumberOfFrames+1)*sizeof(int));
    int j=0;
    float* mask=(float*)malloc(this->NumberOfFrames*sizeof(float));
    for(int i=0; i<this->NumberOfFrames;i++)
    {
        mask[i]=(float)i/(float)NumberOfFrames*samplingRate;
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

void processor::copyVector(double* src, double* dst, int len)
{
    for(int i=0; i< len; i++)
    {
        dst[i]=src[i];
    }
}

void processor::work(float fLow, float fHight)
{
    this->normalize();
    this->rgb2yiq();
    printf("flow= %f, fHight= %f \n", fLow, fHight);
    int* mask=createFreqMask(fLow,fHight);

    //const char* filename_mask = "fr_mask.log";
    //PrintDataInt(mask,this->NumberOfFrames+1,filename_mask);

    fftw_complex* out;
    double* in;

    //fftw_plan p;
    in = (double*)malloc(sizeof(double)*this->NumberOfFrames);
    out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*this->NumberOfFrames);
    fftw_plan p = fftw_plan_dft_r2c_1d(this->NumberOfFrames,in,out,FFTW_MEASURE);

    this->copyVector((double*)&this->AllFrames[0],in,this->NumberOfFrames);
    fftw_execute(p);
    fftw_destroy_plan(p);
    fftw_free(out);
    free(in);
}

float* processor::getAllFrames(void)
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
