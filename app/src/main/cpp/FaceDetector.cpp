//
// Created by Oleg Babichev on 02.12.21.
//

#include "FaceDetector.h"

//auto FACE_DETECTION_CONFIGURATION = "assets/deploy.prototxt";
//auto FACE_DETECTION_WEIGHTS = "assets/res10_300x300_ssd_iter_140000_fp16.caffemodel";

FaceDetector::FaceDetector() : confidence_threshold_(0.5), input_image_height_(300),
                               input_image_width_(300),
                               scale_factor_(1.0), mean_values_({104., 177.0, 123.0}) {
    network_ = cv::dnn::readNetFromCaffe("assets/deploy.prototxt", "assets/res10_300x300_ssd_iter_140000_fp16.caffemodel");
    if (network_.empty()) {
        std::ostringstream ss;
//        ss << "Failed to load network with the following settings:\n"
//           << "Configuration: " + std::string(FACE_DETECTION_CONFIGURATION) + "\n"
//           << "Binary: " + std::string(FACE_DETECTION_WEIGHTS) + "\n";
        throw std::invalid_argument(ss.str());
    }
}

std::vector <cv::Rect> FaceDetector::detect_face_rectangles(const cv::Mat &frame) {
    cv::Mat input_blob = cv::dnn::blobFromImage(frame, scale_factor_,
                                                cv::Size(input_image_width_, input_image_height_),
                                                mean_values_, false, false);
    network_.setInput(input_blob, "data");
    cv::Mat detection = network_.forward("detection_out");
    cv::Mat detection_matrix(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    std::vector <cv::Rect> faces;

    for (int i = 0; i < detection_matrix.rows; i++) {
        float confidence = detection_matrix.at<float>(i, 2);

        if (confidence < confidence_threshold_) {
            continue;
        }
        int x_left_bottom = static_cast<int>(detection_matrix.at<float>(i, 3) * frame.cols);
        int y_left_bottom = static_cast<int>(detection_matrix.at<float>(i, 4) * frame.rows);
        int x_right_top = static_cast<int>(detection_matrix.at<float>(i, 5) * frame.cols);
        int y_right_top = static_cast<int>(detection_matrix.at<float>(i, 6) * frame.rows);

        faces.emplace_back(x_left_bottom, y_left_bottom, (x_right_top - x_left_bottom),
                           (y_right_top - y_left_bottom));
    }

    return faces;
}
