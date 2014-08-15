#include<utility>
#include "Conf.h"
#include "Videoreader.h"
#include "Processor.h"
#include "Calculator.h"

using namespace std;
using namespace Calculator;

int main()
{
    const string filename_in {"face.mp4"};
    av_register_all();
    const unsigned int frames_max {1000};
    VideoReader Curr_video;
    if (Curr_video.ReadFrames(filename_in,4,frames_max)<0)
    {
        cout << "Failed to read file" << endl;
        return -1;
    }

    const double fr1 {50.0/60.0};
    const double fr2 {78.0/60.0};
    const double ampFactor {70.0};
    const unsigned int avg_parameter {3};
    const double area_radius {3.0};

    vector<Point> points{find_points_of_interest(Curr_video)};

    cout << calculate_pulse(Curr_video,points,fr1,fr2,ampFactor,avg_parameter,area_radius) << endl;
    return 0;
}
