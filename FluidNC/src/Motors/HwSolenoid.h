#pragma once

#include "RcServo.h"

namespace MotorDrivers {
    class HwSolenoid : public RcServo {
    protected:
        Pin           _output_pin;              // Register output pin as a Pin class so we can call .write on it.
        bool          _dir_invert     = false;  // Direction invert setting
        const uint8_t _update_rate_ms = 20;     // How frequently the solenoid gets updated
        void          config_message() override;
        void          update() override;

    public:
        HwSolenoid() = default;

        void set_location();
        void init() override;
        void set_disable(bool disable) override;

        // Configuration handlers:
        void group(Configuration::HandlerBase& handler) override {
            handler.item("output_pin", _output_pin);
            handler.item("direction_invert", _dir_invert);
        }

        const char* name() const override { return "hwsolenoid"; }
    };
}
