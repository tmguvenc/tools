#include <imagesaver.h>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

ImageSaver::ImageSaver(ImageSaver::Queue *queue)
    : m_queue(queue)
{

}

ImageSaver::~ImageSaver()
{

}

void ImageSaver::run()
{
    while(true){
        spImage image;

        m_queue->pop(image);

        if(!image) break;

        auto folder_name = "/home/turanmurat/Desktop/outputs/" + image->videoFileName;

        if(!fs::exists(folder_name))
            fs::create_directory(folder_name);

        auto name = folder_name + "/image_" + std::to_string(image->index) + ".jpg";

        cv::imwrite(name, *image->image);
        image.reset();
    }
}
