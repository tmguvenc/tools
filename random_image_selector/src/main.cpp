#include <memory>
#include <iostream>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <boost/random.hpp>

namespace fs = boost::filesystem;

static inline bool equals(char* str1, char* str2){
    return strcmp(str1, str2) == 0;
}

inline std::map<std::string, std::vector<std::string>> getAllFiles(const std::string& path){
    auto dir = fs::system_complete(path);

    //std::vector<ImageFile> list;
    std::map<std::string, std::vector<std::string>> map;

    if(!fs::is_directory(dir)){
        return map;
    }else{

        fs::recursive_directory_iterator end_iter;

        for (fs::recursive_directory_iterator dir_itr(dir); dir_itr != end_iter; ++dir_itr){
            try{
                if(fs::is_regular_file(dir_itr->status())){
                    auto ext = fs::extension(dir_itr->path());
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                    if(ext == ".jpg" || ext == ".jpeg" || ext == ".png")
                    {
                        const auto full_path = fs::system_complete(dir_itr->path()).generic_string();
                        auto aa = full_path.substr(0, full_path.find_last_of('/'));

                        auto parent_name = aa.substr(aa.find_last_of('/') + 1);

                        map[parent_name].push_back(full_path);
                    }
                }
            }catch(const std::exception& ex){
                std::cerr << ex.what() << std::endl;
            }
        }
    }

    return map;
}

int main(int argc, char* argv[])
{
    std::string path;

    if(argc != 3){
        std::cout << "Usage: random_image_selector -d [directory]" << std::endl;
        return -1;
    }

    if(equals(argv[1], "-d")){
        path = argv[2];
    }else{
        std::cerr << "Invalid option" << std::endl;
        return -1;
    }

    const std::string test_dir(path + "/test/");

    fs::create_directory(test_dir);

    boost::random::mt19937 rng;

    auto map = getAllFiles(path);

    for(const auto& parent : map){

        auto size = parent.second.size();

        auto test_file_size = size * 0.15;
        boost::random::uniform_int_distribution<> randomizer(0, size - 1);

        for(int i = 0; i < test_file_size; ++i){
            auto index = randomizer(rng);

            auto try_count = 0;

            while(!fs::exists(parent.second[index]) && ++try_count < 5)
                index = randomizer(rng);

            auto full_path = parent.second[index];

            auto file_name = full_path.substr(full_path.find_last_of('/') + 1);
            auto dest_dir = test_dir + parent.first + "/";

            if(!fs::exists(dest_dir))
                fs::create_directory(dest_dir);

            fs::copy(full_path, dest_dir + file_name);
            fs::remove(full_path);
        }
    }

    return 0;
}
