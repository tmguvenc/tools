#ifndef COMMON_H
#define COMMON_H

#include <exception>
#include <opencv2/opencv.hpp>

class FileNotFoundException : public std::exception{
    virtual const char* what() const throw()
    {
        return "Cascade file not found!";
    }
};


static inline cv::Rect expandRect(const cv::Rect& rect, float factor) {
    assert(factor > 0);
    cv::Rect expandedRect;
    expandedRect.width = static_cast<int>(rect.width*factor);
    expandedRect.height = static_cast<int>(rect.height*factor);
    expandedRect.x = CV_IMAX(rect.x - (int)((expandedRect.width - rect.width) / 2), 0);
    expandedRect.y = CV_IMAX(rect.y - (int)((expandedRect.height - rect.height) / 2), 0);
    return expandedRect;
}

static inline std::string extractVideoFileName(const std::string& fileName){
    auto slash_index = fileName.find_last_of('/');
    auto dot_index = fileName.find_last_of('.');

    if(slash_index != -1 && dot_index != -1){
        return fileName.substr(slash_index + 1, dot_index - slash_index - 1);
    }

    return std::string();
}

#endif
