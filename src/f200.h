/*
    INTEL CORPORATION PROPRIETARY INFORMATION This software is supplied under the
    terms of a license agreement or nondisclosure agreement with Intel Corporation
    and may not be copied or disclosed except in accordance with the terms of that
    agreement.
    Copyright(c) 2015 Intel Corporation. All Rights Reserved.
*/

#pragma once
#ifndef LIBREALSENSE_F200_H
#define LIBREALSENSE_F200_H

#include "device.h"
#include "f200-private.h" // todo - refactor so we don't need this here

#include <atomic>
#include <thread>
#include <condition_variable>

namespace rsimpl
{
    namespace f200 { class IVCAMHardwareIO; }

    class f200_camera final : public rs_device
    {
        std::timed_mutex usbMutex;

        f200::CameraCalibrationParameters base_calibration;
        f200::IVCAMTemperatureData base_temperature_data;
        f200::IVCAMThermalLoopParams thermal_loop_params;

        float last_temperature_delta;

        std::thread temperatureThread;
        std::atomic<bool> runTemperatureThread;
        std::mutex temperatureMutex;
        std::condition_variable temperatureCv;

        void temperature_control_loop();
    public:      
        f200_camera(std::shared_ptr<uvc::device> device, const static_device_info & info, const f200::CameraCalibrationParameters & calib, const f200::IVCAMTemperatureData & temp, const f200::IVCAMThermalLoopParams & params);
        ~f200_camera();

        void on_before_start(const std::vector<subdevice_mode_selection> & selected_modes) override;
        int convert_timestamp(int64_t timestamp) const override { return static_cast<int>(timestamp / 100000); }
    };

    std::shared_ptr<rs_device> make_f200_device(std::shared_ptr<uvc::device> device);
    std::shared_ptr<rs_device> make_sr300_device(std::shared_ptr<uvc::device> device);
}

#endif
