#include <memory>
#include <iostream>
#include <boost/filesystem.hpp>
#include <tbb/tbb_thread.h>
#include <facefinder.h>
#include <imagesaver.h>
#include <iomanip>

namespace fs = boost::filesystem;

inline std::vector<std::string> getAllFiles(const std::string& path){
    auto dir = fs::system_complete(path);

    std::vector<std::string> list;

    if(!fs::is_directory(dir)){
        return list;
    }else{

        fs::recursive_directory_iterator end_iter;

        for (fs::recursive_directory_iterator dir_itr(dir); dir_itr != end_iter; ++dir_itr){
            try{
                if(fs::is_regular_file(dir_itr->status())){
                    auto ext = fs::extension(dir_itr->path());
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                    if(ext == ".jpg" || ext == ".jpeg" || ext == ".png")
                        list.push_back(fs::system_complete(dir_itr->path()).generic_string());
                }
            }catch(const std::exception& ex){
                std::cerr << ex.what() << std::endl;
            }
        }
    }

    return list;
}

static inline bool equals(char* str1, char* str2){
    return strcmp(str1, str2) == 0;
}

enum Option{
    File,
    Directory,
    Webcam
};

int main(int argc, char* argv[])
{
    std::string path;

    if(argc != 3){

        std::cout << std::setw(12) << "usage:"
                  << std::setw(12) << "FaceCropper -d [path]\n"
                  << std::setw(12) << "or:"
                  << std::setw(12) << "FaceCropper -f [file_path]\n"
                  << std::setw(12) << "or:"
                  << std::setw(12) << "FaceCropper -w [url]" << std::endl;

        return -1;
    }

    Option opt;

    if(equals(argv[1], "-d")){
        path = fs::system_complete(argv[2]).generic_string();
        opt = Directory;
    }else if(equals(argv[1], "-f")){
        path = fs::system_complete(argv[2]).generic_string();
        opt = File;
    }else if(equals(argv[1], "-w")){
        path = argv[2];
        opt = Webcam;
    }else{
        std::cerr << "Invalid option" << std::endl;
        return -1;
    }

    tbb::concurrent_bounded_queue<std::shared_ptr<CroppedImage>> image_queue;

    image_queue.set_capacity(100);

    FaceFinder face_finder(&image_queue);
    ImageSaver saver(&image_queue);

    std::unique_ptr<tbb::tbb_thread> face_thread;

    switch (opt) {
    case File:
        face_thread = std::make_unique<tbb::tbb_thread>([&face_finder, &path](){
            std::string filename = fs::system_complete(path).generic_string();
            face_finder.run({filename});
        });
        break;
    case Directory:
        face_thread = std::make_unique<tbb::tbb_thread>([&face_finder, &path](){
            const auto file_list = getAllFiles(path);
            std::cout << "found " << file_list.size() << " files" << std::endl;
            face_finder.run(file_list);
        });
        break;
    case Webcam:
        face_thread = std::make_unique<tbb::tbb_thread>([&face_finder, &path](){
            face_finder.run(path);
        });
        break;
    default:
        std::cout << "invalid option" << std::endl;
        break;
    }

    saver.run();

    if(face_thread->joinable()){
        face_thread->join();
    }

    return 0;
}
