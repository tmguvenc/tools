#include <facefinder.h>
#include <common.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

FaceFinder::FaceFinder(FaceFinder::Queue *queue) :
    m_queue(queue),
    m_classifier(new cv::CascadeClassifier),
    m_image_index(0)
{

}

FaceFinder::~FaceFinder()
{
    if(m_classifier){
        delete m_classifier;
        m_classifier = nullptr;
    }
}

void FaceFinder::run(const std::vector<std::string> &files)
{
    if(!m_classifier->load("/home/turanmurat/opencv/data/haarcascades/haarcascade_frontalface_alt.xml")) {
        throw FileNotFoundException();
    }

    for(const auto& file : files){
        std::cout << "processing " << file << " ...  " << std::flush;

        switch (get_fileType(file)) {
        case FileType::Video:
            process_video_file(file);
            break;
        case FileType::Image:
            process_image_file(file);
        default:
            break;
        }

        std::cout << "done" << std::endl;
    }

    m_queue->push(nullptr);
    std::cout << "processing done!\n";
}

void FaceFinder::run(const std::string &camera_url)
{
    if(!m_classifier->load("/home/turanmurat/opencv/data/haarcascades/haarcascade_frontalface_alt.xml")) {
        throw FileNotFoundException();
    }

    std::cout << "processing " << camera_url << " ... " << std::flush;

    process_video_file(camera_url);

    std::cout << "done" << std::endl;
    m_queue->push(nullptr);
    std::cout << "processing done!\n";
}

std::vector<std::shared_ptr<cv::Mat>> FaceFinder::find_face(const cv::Mat &image)
{
    std::vector<std::shared_ptr<cv::Mat>> face_images;

    std::vector<cv::Rect> faces;

    std::vector<int> reject_levels;
    std::vector<double> levelWeights;

    cv::Mat gray;

    cv::cvtColor(image, gray, CV_BGR2GRAY);

    m_classifier->detectMultiScale(image, faces, reject_levels, levelWeights, 1.2, 5, cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30), cv::Size(), true);

    for(const auto& face : faces){

        auto expanded_face = expandRect(face, 1.8f);

        auto croppedImage = std::make_shared<cv::Mat>();

        cv::getRectSubPix(image, {expanded_face.width, expanded_face.height}, {expanded_face.x + expanded_face.width/2, expanded_face.y + expanded_face.height/2}, *croppedImage);

        face_images.push_back(croppedImage);
    }

    return std::move(face_images);
}

void FaceFinder::process_video_file(const std::string &file)
{
    cv::VideoCapture capture(file);
    if(!capture.isOpened()){
        std::cerr << "cannot open " << file << std::endl;
        return;
    }

    auto video_file_name = extractVideoFileName(file);

    cv::Mat frame;

    uint32_t frame_index = 0;

    while(true){
        capture >> frame;
        if(frame.empty()) break;

        if(frame_index++ % 25 == 0){
            for(auto& face : find_face(frame)){
                m_queue->push(std::make_shared<CroppedImage>(std::forward<std::shared_ptr<cv::Mat>>(face), file, video_file_name, ++m_image_index));
            }
        }
        cv::waitKey(1);
    }

    capture.release();
}

void FaceFinder::process_image_file(const std::string &file)
{
    auto frame = cv::imread(file);

    if(!frame.empty()){
        for(auto& face : find_face(frame)){
            m_queue->push(std::make_shared<CroppedImage>(std::forward<std::shared_ptr<cv::Mat>>(face), file, "from_photos", ++m_image_index));
        }
    }else{
        std::cerr << "cannot read " << file << std::endl;
    }
}

FileType FaceFinder::get_fileType(const std::string& fileName)
{
    const auto& ext = fs::extension(fileName);

    return (ext == ".avi" || ext == ".mp4" || ext == ".mkv") ? FileType::Video :
                                                               (ext == ".jpg" || ext == ".bmp" || ext == ".png" || ext == ".jpeg") ? FileType::Image : FileType::Other;
}
