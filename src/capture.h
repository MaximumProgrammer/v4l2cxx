#pragma once

#include "v4l2cxx.h"
class capture {

public:

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    capture(std::string device, std::function<void(uint8_t *p_data, size_t len)> callback)
            : callback_(callback) {

        fd_ = util_v4l2::open_device(device, &err_);
        ASSERT_ERR_CODE(err_);

    }

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    capture(std::string device, uint32_t width, uint32_t height, pixel_format format,
            std::function<void(uint8_t *p_data, size_t len)> callback)
            : callback_(callback) {


        fd_ = util_v4l2::open_device(device, &err_);
        ASSERT_ERR_CODE(err_);

        util_v4l2::set_format(fd_, width, height, format, &err_);
        ASSERT_ERR_CODE(err_);

        util_v4l2::init_mmap(fd_, buffers, &err_);
        ASSERT_ERR_CODE(err_);

        util_v4l2::set_capture_steamon(fd_, &err_);
        ASSERT_ERR_CODE(err_);

        auto formats = util_v4l2::query_formats(fd_,nullptr);




        v4l2_format format1;
        format1.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        if (-1 == util_v4l2::xioctl(fd_, VIDIOC_G_FMT, &format1)) {



        }
        int x = 5;

    }

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    void run() {
        util_v4l2::queue_frames(fd_, NUM_OF_MAP_BUFFER, &err_);
        ASSERT_ERR_CODE(err_);

        // blocks indefintly.
        util_v4l2::mainloop(fd_, buffers, callback_, &err_);
        ASSERT_ERR_CODE(err_);
    }

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    void set_format(int width, int height, pixel_format format) {
        util_v4l2::set_format(fd_, width, height, format, &err_);
        ASSERT_ERR_CODE(err_);

        util_v4l2::init_mmap(fd_, buffers, &err_);
        ASSERT_ERR_CODE(err_);

        util_v4l2::set_capture_steamon(fd_, &err_);
        ASSERT_ERR_CODE(err_);
    }

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

    int read() {
        int numOfBytes = util_v4l2::read_one_frame(fd_, buffers, callback_, &err_);
        ASSERT_ERR_CODE(err_);
        return numOfBytes;
    }

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

private:

    int fd_{};
    constexpr static int NUM_OF_MAP_BUFFER = 4;
    struct util_v4l2::buffer buffers[NUM_OF_MAP_BUFFER];
    error_code err_ = error_code::ERR_NO_ERROR;
    std::function<void(uint8_t *p_data, size_t len)> callback_;
};