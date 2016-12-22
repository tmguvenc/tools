#include <memory>
#include <iostream>
#include <boost/filesystem.hpp>
#include <tbb/tbb_thread.h>
#include <facefinder.h>
#include <imagesaver.h>

namespace fs = boost::filesystem;

static inline bool equals(char* str1, char* str2){
    return strcmp(str1, str2) == 0;
}

int main(int argc, char* argv[])
{
    std::string path;

    if(argc != 3){
        std::cout << "\or:          TVRecorder -w [url]" << std::endl;
        return -1;
    }

    if(equals(argv[1], "-w")){
        path = argv[2];
    }else{
        std::cerr << "Invalid option" << std::endl;
        return -1;
    }

    tbb::concurrent_bounded_queue<std::shared_ptr<CroppedImage>> image_queue;

    image_queue.set_capacity(100);

    FaceFinder face_finder(&image_queue);
    ImageSaver saver(&image_queue);

    auto face_thread = std::make_unique<tbb::tbb_thread>([&face_finder, &path](){
        face_finder.run(path);
    });

    saver.run();

    if(face_thread->joinable()){
        face_thread->join();
    }

    return 0;
}
