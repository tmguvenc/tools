#ifndef FACE_FINDER_H
#define FACE_FINDER_H

#include <string>
#include <tbb/concurrent_queue.h>
#include <image.hpp>
#include <vector>

namespace cv {
    class CascadeClassifier;
    class Mat;
}

enum FileType{
    Video = 0,
    Image = 1,
    Other = 2
};

class FaceFinder{

    using Queue = tbb::concurrent_bounded_queue<spImage>;
public:
    explicit FaceFinder(Queue* queue);
    ~FaceFinder();

    void run(const std::vector<std::string>& files);
    void run(const std::string& camera_url);

protected:
    std::vector<std::shared_ptr<cv::Mat> > find_face(const cv::Mat& image);
    void process_video_file(const std::string& file);
    void process_image_file(const std::string& file);
    FileType get_fileType(const std::string &file);

private:
    Queue* m_queue;
    cv::CascadeClassifier* m_classifier;
    uint32_t m_image_index;
};

#endif
