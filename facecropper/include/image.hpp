#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <string>
#include <stdlib.h>

namespace cv {
    class Mat;
}

class CroppedImage{
public:
    std::shared_ptr<cv::Mat> image;
    std::string fileName, videoFileName;
    uint32_t index;

    CroppedImage(std::shared_ptr<cv::Mat>&& img, const std::string& file_name, const std::string& video_file_name, uint32_t idx)
        :image(std::forward<std::shared_ptr<cv::Mat>>(img)),
          fileName(std::move(file_name)),
          videoFileName(std::move(video_file_name)),
          index(idx)
    {

    }
};

using spImage = std::shared_ptr<CroppedImage>;

#endif
