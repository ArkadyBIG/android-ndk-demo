# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# opencv
#set(OpenCV_STATIC on)
#set(OpenCV_LOCATION ${OPENCV_ANDROID}/sdk/native/jni)
#find_package(OpenCV REQUIRED)



# Declares and names the project.

project("testndk")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# OpenCV
add_library(lib_opencv SHARED IMPORTED)
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libopencv_java4.so)

# Tensorflow
add_library(lib_tflite SHARED IMPORTED)
set_target_properties(lib_tflite PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libtensorflowlite_jni.so)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jniIncludes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_library( # Sets the name of the library.
        testndk

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
#        FaceDetector.cpp
        native-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        testndk

        # OpenCV lib
        lib_opencv

        lib_tflite

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})


# Introduce preprocessor variables to keep paths of asset files
set(FACE_DETECTION_CONFIGURATION "${CMAKE_CURRENT_SOURCE_DIR}/assets/deploy.prototxt")
set(FACE_DETECTION_WEIGHTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/res10_300x300_ssd_iter_140000_fp16.caffemodel")

set(TENSORFLOW_MODEL_TFLITE "${CMAKE_CURRENT_SOURCE_DIR}/assets/mobilenet_v1_1_0_224_quant.tflite")

target_compile_definitions(${PROJECT_NAME} PRIVATE FACE_DETECTION_CONFIGURATION="${FACE_DETECTION_CONFIGURATION}")
target_compile_definitions(${PROJECT_NAME} PRIVATE FACE_DETECTION_WEIGHTS="${FACE_DETECTION_WEIGHTS}")
target_compile_definitions(${PROJECT_NAME} PRIVATE TENSORFLOW_MODEL_TFLITE="${TENSORFLOW_MODEL_TFLITE}")
