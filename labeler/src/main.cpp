#include <memory>
#include <iostream>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

namespace fs = boost::filesystem;

static inline bool equals(char* str1, char* str2){
    return strcmp(str1, str2) == 0;
}

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

std::vector<std::string> names = {
    "oyku",
    "murat",
    "sezin",
    "seyhan",
    "fatma",
    "remzi",
    "jale",
    "seray",
    "cihan"
};

void fill_panel(const std::vector<std::string>& names, cv::Mat left){
    auto index = 1;

    cv::Point point(10, 10);

    for(const auto& name : names){
        point.y = index * 20;
        cv::putText(left, std::to_string(index++) +  ". " + name, point, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0,0,255), 1);
    }
}

int main(int argc, char* argv[])
{
    std::string path;

    if(argc != 3){
        std::cout << "Usage: labeler -d [directory]" << std::endl;
        return -1;
    }

    if(equals(argv[1], "-d")){
        path = argv[2];
    }else{
        std::cerr << "Invalid option" << std::endl;
        return -1;
    }

    cv::Mat panel = cv::Mat::zeros(500, 700, CV_8UC3);

    cv::Rect left_roi(0, 0, 200, 500);
    cv::Rect right_roi(200, 0, 500, 500);

    fill_panel(names, panel(left_roi));

    auto right_panel = panel(right_roi);


    for(const auto& file : getAllFiles(path)){
        auto img = cv::imread(file);

        cv::namedWindow(file);
        cv::moveWindow(file, 300, 300);

        if(img.empty()){
            std::cerr << "cannot read file: " << file << std::endl;
            continue;
        }

        cv::resize(img, right_panel, right_panel.size());

        cv::imshow(file, panel);
        auto ch = cv::waitKey(0);

        if(ch == 'q')
            break;
        else if(ch == 'r'){
            fs::remove(file);
            std::cout << "deleted file: " << file << std::endl;
        }else if(ch > 48 && ch < 58){
            try{
                auto dest_name = "/home/turanmurat/Desktop/person/" + names[ch - 49] + "/" + std::to_string(fs::file_size(file));

                if(!fs::exists(dest_name + ".jpg")){
                    fs::copy(file, dest_name + ".jpg");
                    std::cout << "file moved to " << dest_name + ".jpg" << std::endl;
                }

                fs::remove(file);
            }catch(const std::exception& ex){
                std::cerr << ex.what() << std::endl;
            }
        }

        cv::destroyWindow(file);
    }

    std::cout << "done" << std::endl;

    return 0;
}
