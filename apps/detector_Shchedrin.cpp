#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

const char* params =
     "{ h | help     | false | print usage                                   }"
     "{   | detector |       | XML file with a cascade detector              }"
     "{   | image    |       | image to detect objects on                    }"
     "{   | video    |       | video file to detect on                       }"
     "{   | camera   | false | whether to detect on video stream from camera }";


void drawDetections(const vector<Rect>& detections,
                    const Scalar& color,
                    Mat& image)
{
    for (size_t i = 0; i < detections.size(); ++i)
    {
        rectangle(image, detections[i], color, 2);
    }
}

const Scalar red(0, 0, 255);
const Scalar green(0, 255, 0);
const Scalar blue(255, 0, 0);
const Scalar colors[] = {red, green, blue};

void detectOnImage(CascadeClassifier &classif, const Mat &image, Mat &res){
    res = image.clone();
    vector<Rect> found;
    classif.detectMultiScale(image, found);    
    for(int i = 0; i < found.size(); i++){
        rectangle(res, found[i],Scalar(100,200,0),2);
    }
}

int main(int argc, char** argv)
{
    // Parse command line arguments.
    CommandLineParser parser(argc, argv, params);
    // If help flag is present, print help message and exit.
    if (parser.get<bool>("help"))
    {
        parser.printParams();
        return 0;
    }

    string detector_file = parser.get<string>("detector");
    CV_Assert(!detector_file.empty());
    string image_file = parser.get<string>("image");
    string video_file = parser.get<string>("video");
    bool use_camera = parser.get<bool>("camera");

    // TODO: Load detector.
    CascadeClassifier classif;
    
    if(!classif.load(detector_file)){
        cerr<<"Can not load detector"<<endl;
    }

    if (!image_file.empty())
    {   
        Mat image = imread(image_file);
        Mat res;
        detectOnImage(classif, image, res);
        imshow("detect", res);
        int key = waitKey();
    }
    else if (!video_file.empty())
    {
        
    }
    else if (use_camera)
    {
        // TODO: Detect objects on a live video stream from camera.
        int key = 0;
        VideoCapture cap(0);
        Mat image, res;
        do{
            cap >> image;
            if(image.empty()){
                continue;
            }
            detectOnImage(classif, image, res);
            imshow("detect", res);
            key = waitKey(1);
        }while(key != 27);
    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

    return 0;
}



