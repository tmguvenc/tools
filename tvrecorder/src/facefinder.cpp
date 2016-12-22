#include <facefinder.h>
#include <common.h>
#include <boost/filesystem.hpp>
#include <opencv2/highgui.hpp>

namespace fs = boost::filesystem;

cv::Rect roi(-1, -1, -1, -1);
cv::Mat frame;

bool selected = false;

void onMouse(int event, int x, int y, int flags, void* userdata){

    if(event == cv::EVENT_LBUTTONDOWN){
        roi.x = x;
        roi.y = y;
        selected = false;
    }else if(event == cv::EVENT_LBUTTONUP){
        roi.width = x - roi.x;
        roi.height = y - roi.y;
        selected = true;
    }

    else if(event == cv::EVENT_MOUSEMOVE){

        if(!selected){
            if(roi.x != -1 || roi.width != -1){
                cv::Mat temp;
                frame.copyTo(temp);

                cv::rectangle(temp, cv::Rect(roi.x, roi.y, x - roi.x, y - roi.y), cv::Scalar(0, 0, 255), 2);

                cv::imshow("region", temp);
            }
        }
    }
}

FaceFinder::FaceFinder(FaceFinder::Queue *queue) :
    m_queue(queue),
    m_image_index(0)
{

}

FaceFinder::~FaceFinder()
{
}

void FaceFinder::run(const std::string &camera_url)
{
    cv::VideoCapture capture(camera_url);

    if(!capture.isOpened()){
        std::cerr << "cannot open camera" << std::endl;
    }

    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);

    std::cout << "processing " << camera_url << " ... " << std::flush;

    capture >> frame;

    cv::namedWindow("region");

    cv::imshow("region", frame);

    cv::setMouseCallback("region", onMouse);

    auto ret = cv::waitKey(0);

    //if(ret == 's'){

        cv::destroyWindow("region");

        while(true){
            capture >> frame;

            if(frame.empty()) break;

            cv::waitKey(1000);

            m_queue->push(std::make_shared<CroppedImage>(std::make_shared<cv::Mat>(frame(roi)), "tv_record_image", ++m_image_index));
        }

        std::cout << "done" << std::endl;
        m_queue->push(nullptr);
        std::cout << "processing done!\n";
//   }else{
//        std::cout << "press 's' to resume" << std::endl;
//    }
}
