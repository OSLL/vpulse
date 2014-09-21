#include <utility>
#include <videoreader.h>
#include <processor.h>
#include <calculator.h>

using namespace std;
using namespace Calculator;

int main()
{
    const string filename_in {"test1.mp4"};
    cout << filename_in << endl;
    av_register_all();
    const unsigned int frames_max {500};
    VideoReader Curr_video;
    if (Curr_video.ReadFrames(filename_in, 4, frames_max)<0)
    {
        cout << "TEST FAILED: Couldn't read file" << endl;
        return -1;
    }

    const double fr1 {50.0/60.0};
    const double fr2 {78.0/60.0};
    const double ampFactor {70.0};
    const unsigned int avg_parameter {3};
    const double area_radius {3.0};

    vector<Point> points{find_points_of_interest(Curr_video)};

    const vector<double> exp(10,65.0);
    const vector<double> res {calculate_pulse(Curr_video,points,1.0,fr1,fr2,ampFactor,avg_parameter,area_radius)};

    cout << "Expected: " << endl;
    for(auto x : exp)
        cout << x << " ";
    cout << endl;
    cout << "Result: " << endl;
    for(auto x : res)
        cout << x << " ";
    cout << endl;

    bool flag{true};
    for(size_t i = 0; i < exp.size(); i++)
    if (abs((exp[i]-res[i])/exp[i])>0.1)
        flag = false;

    cout << (flag ? "TEST PASSED" : "TEST FAILED") << endl;

    return 0;
}
