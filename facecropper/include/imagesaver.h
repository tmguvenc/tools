#ifndef IMAGE_SAVER_H
#define IMAGE_SAVER_H

#include <tbb/concurrent_queue.h>
#include <image.hpp>

class ImageSaver{
    using Queue = tbb::concurrent_bounded_queue<spImage>;

public:
    ImageSaver(Queue* queue);
    ~ImageSaver();

    void run();

private:
    Queue* m_queue;
};

#endif
