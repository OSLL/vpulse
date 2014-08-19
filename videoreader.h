#ifndef VIDEOREADER_H
#define VIDEOREADER_H

#include <conf.h>
#include <mat.h>

using namespace std;

/*!
  Videoreader class is used to store read frames from video and blurred frames
*/
class VideoReader
{
private:

    /*! Vector, storing the frames */
    vector<unique_ptr<Mat>> frames;
    /*! Vector, storing the blurred frames */
    vector<unique_ptr<Mat>> blured_frames;
    /*! Number of frames */
    unsigned int NumberOfFrames;
    /*! Frame height */
    unsigned int frameHeight;
    /*! Frame width */
    unsigned int frameWidth;
    /*! Blurred frame height */
    unsigned int blurred_frameHeight;
    /*! Blurred frame width */
    unsigned int blurred_frameWidth;
public:

    /*!
        Function reads frames from input file using ffempg codec
        \param pyramid_level the Gaussian pyramide level used for blurred image
        \param framesLimit maxmimum frames to be read
    */
    int ReadFrames(const string& videofilename_in, int pyramid_level, int framesLimit);

    /*!
        \return Frame height
    */
    unsigned int getFrameHeight(void) const;

    /*!
        \return Frame width
    */
    unsigned int getFrameWidth(void) const;

    /*!
        \return Blurred frame height
    */
    unsigned int getBlurredFrameHeight(void) const;

    /*!
        \return Blurred frame width
    */
    unsigned int getBlurredFrameWidth(void) const;

    /*!
        \return Number of frames
    */
    unsigned int getNumberOfFrames(void) const;

    /*!
        \return Frames vector
    */
    vector<unique_ptr<Mat>>& getFrames(void);

    /*!
        \return Blurred frames vector
    */
    vector<unique_ptr<Mat>>& getBluredFrames(void);
};

#endif // VIDEOREADER_H
