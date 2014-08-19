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
    size_t NumberOfFrames;
    /*! Frame height */
    size_t frameHeight;
    /*! Frame width */
    size_t frameWidth;
    /*! Blurred frame height */
    size_t blurred_frameHeight;
    /*! Blurred frame width */
    size_t blurred_frameWidth;
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
    size_t getFrameHeight(void) const;

    /*!
        \return Frame width
    */
    size_t getFrameWidth(void) const;

    /*!
        \return Blurred frame height
    */
    size_t getBlurredFrameHeight(void) const;

    /*!
        \return Blurred frame width
    */
    size_t getBlurredFrameWidth(void) const;

    /*!
        \return Number of frames
    */
    size_t getNumberOfFrames(void) const;

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
