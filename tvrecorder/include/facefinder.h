#ifndef FACE_FINDER_H
#define FACE_FINDER_H

#include <string>
#include <tbb/concurrent_queue.h>
#include <image.hpp>
#include <vector>

namespace cv {
    class Mat;
}

class FaceFinder{

    using Queue = tbb::concurrent_bounded_queue<spImage>;
public:
    explicit FaceFinder(Queue* queue);
    ~FaceFinder();

    void run(const std::string& camera_url);

private:
    Queue* m_queue;
    uint32_t m_image_index;
};

#endif
