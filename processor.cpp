#include "processor.h"
using namespace std;

#include<functional>
#include<cmath>

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

processor::processor(int NumberOfFrames_in, int frameHeight_in, int frameWidth_in, int sRate_in, Mat** Frames):
    NumberOfFrames(NumberOfFrames_in),
    frameHeight(frameHeight_in),
    frameWidth(frameWidth_in),
    samplingRate(sRate_in),
    debugflag(0)
{
    long LengthAll=NumberOfFrames*frameHeight*frameWidth*channels;
    AllFrames = new double[LengthAll];
    FramesToVector(Frames, this->AllFrames, this->frameWidth, this->frameHeight, this->NumberOfFrames);
}

processor::~processor()
{
    delete[] AllFrames;
}

/*void processor::FramesToVector(Mat** src, double* dst, int frWidth, int frHeight, int NofFrames)
{
    long rowD = NofFrames * frHeight;
    long colD = rowD * frWidth;
    int invCh = 0;
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
}*/


void processor::FramesToVector(Mat** src, double* dst, int frWidth, int frHeight, int NofFrames)
{
    //cout << src[0]->rows << " " << src[0]->cols << endl;
    //cout << frWidth << " " << frHeight << " " << NofFrames << endl;

    for(int k = 0; k < NofFrames; k++)
        for(int row = 0; row < frHeight; row++)
            for(int col = 0; col < frWidth; col++)
            {
                Vec3b source = src[k]->at<Vec3b>(row,col);
                int destination_index {channels*(frWidth*frHeight*k+frWidth*row+col)};
                dst[destination_index + 0] = source[0]; //temporary solution. Works slowly
                dst[destination_index + 1] = source[1];
                dst[destination_index + 2] = source[2];
            }
}

void processor::VectorToFrames(double* src, Mat** dst, int frWidth, int frHeight, int NofFrames)
{
    for(int k = 0; k < NofFrames; k++)
        for(int row = 0; row < frHeight; row++)
            for(int col = 0; col < frWidth; col++)
            {
                Vec3b* destination = &(dst[k]->at<Vec3b>(row,col));
                int source_index {channels*(frWidth*frHeight*k+frWidth*row+col)};
                (*destination)[0] = src[source_index + 0];
                (*destination)[1] = src[source_index + 1];
                (*destination)[2] = src[source_index + 2];
            }
}


void processor::rgb2yiq(double* srcDst, int frWidth, int frHeight, int NofFrames, bool rev)
{
    int pixels = NofFrames*frHeight*frWidth*channels;
    const double* Coefs = rev ? &yiq2rgbCoef[0] : &rgb2yiqCoef[0];
    for(int i = 0; i < pixels; i+=3)
    {
       double tmp[3] {srcDst[i], srcDst[i + 1], srcDst[i + 2]};  //ugly code, could be done in one line using linear algebra
       srcDst[i + 0] = tmp[0]*Coefs[0] + tmp[1]*Coefs[1] + tmp[2]*Coefs[2];
       srcDst[i + 1] = tmp[0]*Coefs[3] + tmp[1]*Coefs[4] + tmp[2]*Coefs[5];
       srcDst[i + 2] = tmp[0]*Coefs[6] + tmp[1]*Coefs[7] + tmp[2]*Coefs[8];
    }
}

void normalize(vector<double>& src, double factor)
{
    for(auto& e : src)
        e/=factor;
}

vector<int> processor::createFreqMask(double fLow, double fHigh)
{
    int frames_count = NumberOfFrames;

    vector<int> indices;

    for(int i = 0; i < frames_count; i++)
    {
        double mask = (double)i/NumberOfFrames*samplingRate;
        if (mask > fLow && mask < fHigh)
            indices.push_back(i);
    }
    return indices;
}

void processor::copyFFTW_cpx(fftw_complex* src, fftw_complex* dst, long len)
{
    for(long i=0; i< len; i++)
    {
        dst[i][0]=src[i][0];
        dst[i][1]=src[i][1];
    }
}

void processor::applyMask(const complex_vector& src, complex_vector& dst, const vector<int>& mask)
{
    dst.assign(dst.size(),complex_number(0,0)); //set all elements to 0

    for(auto i : mask) //copy src data in elements with indices from mask
    {
        //cout << i << " ";
        dst[i] = src[i];
    }
    //cout << endl;
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
    //std::transform(AllFrames,AllFrames+NumberOfFrames*frameHeight*frameWidth*channels,AllFrames,[](double a){return a/255;}); //normalizing array using range transformation and lambda function
    for(int i = 0; i < NumberOfFrames*frameHeight*frameWidth*channels; i++)
        AllFrames[i]/=255;

    rgb2yiq(AllFrames, frameWidth, frameHeight, NumberOfFrames, false);

    vector<int> mask = createFreqMask(fLow,fHight);

    complex_vector out_fft(NumberOfFrames);
    //fftw_complex* out_fft = new fftw_complex[NumberOfFrames];

    vector<double> in_fft(NumberOfFrames);

    fftw_plan p = fftw_plan_dft_r2c_1d((int)in_fft.size(), in_fft.data(), reinterpret_cast<fftw_complex*>(out_fft.data()), FFTW_MEASURE);

    //ifft_header

    complex_vector in_ifft(NumberOfFrames);
    //fftw_complex* in_ifft = new fftw_complex[NumberOfFrames];

    vector<double> out_ifft(NumberOfFrames);

    fftw_plan p_ifft = fftw_plan_dft_c2r_1d(NumberOfFrames, reinterpret_cast<fftw_complex*>(in_ifft.data()), out_ifft.data(), FFTW_MEASURE);

    for(int row = 0; row < frameHeight; row++)
        for(int col = 0; col < frameWidth; col++)
            for(int channel = 0; channel < channels; channel++)
            {
                vector<double> tmp = receive_pixel_values(row,col,channel);
                in_fft.assign(tmp.begin(),tmp.end());
                //copy(tmp.begin(),tmp.end(),in_fft.data());

                fftw_execute(p);

                //in_ifft = out_fft;
                /*for(int i = 0; i < NumberOfFrames; i++)
                {
                    in_ifft[i][0] = out_fft[i][0];
                    in_ifft[i][1] = out_fft[i][1];

                }*/
                applyMask(out_fft,in_ifft,mask);
                fftw_execute(p_ifft);

                normalize(out_ifft,(double)NumberOfFrames/ampFactor);
                insert_pixel_values(out_ifft,row,col,channel);

            }
    fftw_destroy_plan(p);
    fftw_destroy_plan(p_ifft);

    //rgb2yiq(AllFrames,frameWidth,frameHeight,NumberOfFrames,true);
    //std::transform(AllFrames,AllFrames+NumberOfFrames*frameHeight*frameWidth*channels,AllFrames,[](double a){return a*255;}); //normalizing array using range transformation and lambda function
}

void processor::rgbBoarder(double* src, long len)
{
    for(long i=0; i<len; i++)
    {
        if(src[i] > 1)
            src[i] = 1;
        if(src[i] < 0)
            src[i] = 0;
    }
}

void processor::YIQ2RGBnormalizeColorChannels(double* srcDst, int frWidth, int frHeight, int NofFrames)
{
    rgb2yiq(srcDst,frWidth,frHeight,NofFrames,true);

    int pixels = NofFrames*frHeight*frWidth*channels;
    for(int i = 0; i < pixels; i+=3)
    {
        double normfactor {max({srcDst[i + 0], srcDst[i + 1], srcDst[i + 2], 1.0})};

       if (normfactor > 1.0)
       {
            srcDst[i + 0]/=normfactor;
            srcDst[i + 1]/=normfactor;
            srcDst[i + 2]/=normfactor;
       }
    }
}

int processor::AddPulseToFrames(Mat** frames, int NofFrames)
{
    const int FrWidth {frames[0]->cols};
    const int FrHeight {frames[0]->rows};
    const int FrChannels {frames[0]->channels()};
    const int frame_size {FrHeight*FrWidth*FrChannels};
    //const int frame_size {frameHeight*frameWidth*channels};
    vector<double> fullFrame(frame_size);
    vector<double> pulseFrame(frame_size);

    //cout << frameWidth << " " << frameHeight << endl << FrWidth << " " << FrHeight << " " << NofFrames << endl;
    for(int frame_number = 0; frame_number < NofFrames; frame_number++)
    {
        FramesToVector(&frames[frame_number], fullFrame.data(), FrWidth, FrHeight, 1);
        normalize(fullFrame,255.0);
        rgb2yiq(fullFrame.data(), FrHeight, FrWidth, 1, false);
        NearInterpolation(AllFrames,pulseFrame.data(),frameWidth,frameHeight,FrWidth,FrHeight,
                          frame_number);
        std::transform(fullFrame.begin(), fullFrame.end(), pulseFrame.begin(), fullFrame.begin(),
                       std::plus<double>()); //summing the vectors
        YIQ2RGBnormalizeColorChannels(fullFrame.data(), FrWidth, FrHeight, 1);
        std::transform(fullFrame.begin(),fullFrame.end(),fullFrame.begin(),
                       [](double a){if (a > 1) return 1.0; if (a < 0) return 0.0; return a;});
        normalize(fullFrame,1.0/255);
        VectorToFrames(fullFrame.data(),&frames[frame_number], FrWidth, FrHeight, 1);
    }
    return 0;
}

double* processor::getAllFrames(void)
{
    return(this->AllFrames);
}

int processor::getFrH(void) const
{
    return(this->frameHeight);
}

int processor::getFrW(void) const
{
    return(this->frameWidth);
}

int processor::getNFr(void) const
{
    return(this->NumberOfFrames);
}

void processor::NearInterpolation(double* src, double* dst, int oldwidth, int oldheight, int newwidth, int newheight, int frameInd)
{
   for(int cx = 0; cx < newwidth; cx++)
        for(int cy = 0; cy < newheight; cy++)
        {
            int oldcx = static_cast<int>(floor((double)cx / newwidth * oldwidth));
            int oldcy = static_cast<int>(floor((double)cy / newheight * oldheight));
            int nearest_match {channels*(oldwidth*oldheight*frameInd+oldwidth*oldcy+oldcx)};
            int pixel_index{channels * (cy * newwidth + cx)};
            dst[pixel_index + 0] = src[nearest_match + 0];
            dst[pixel_index + 1] = src[nearest_match + 1];
            dst[pixel_index + 2] = src[nearest_match + 2];
        }
}

vector<double> processor::receive_pixel_values(int row, int col, int channel) const
{
    vector<double> res(NumberOfFrames);

    for(int k = 0; k < (int)res.size(); k++)
        res[k]=AllFrames[calc_pixel_coor(k,row,col,channel)];

    return res;
}

void processor::insert_pixel_values(const vector<double>& values, int row, int col, int channel)
{
    //check that values.size == NofFrames
    for(int k = 0; k < (int)values.size(); k++)
        AllFrames[calc_pixel_coor(k,row,col,channel)]=values[k];

}


int processor::calc_pixel_coor(int k, int row, int col, int channel) const
{
    int width = this->frameWidth;
    int height = this->frameHeight;
    return channels*(width*height*k+width*row+col)+channel;
}


double processor::at(int k, int row, int col, int channel) const
{
    return this->AllFrames[calc_pixel_coor(k,row,col,channel)];
}
