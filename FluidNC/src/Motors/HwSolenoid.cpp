#include "HwSolenoid.h"

#include "../Machine/MachineConfig.h"
#include "../System.h"  // mpos_to_steps() etc
#include "../Pin.h"
#include "../Limits.h"  // limitsMaxPosition

namespace MotorDrivers {

    void HwSolenoid::init() {
        if (_output_pin.undefined()) {
            log_warn("    HwSolenoid disabled: No output pin");
            _has_errors = true;
            return;  // We cannot continue without the output pin
        }

        config_message();
    }

    void HwSolenoid::update() {
        set_location();
    }

    void HwSolenoid::config_message() {
        log_info("    " << name() << " Pin: " << _output_pin.name() << " Direction Invert: " << _dir_invert);
    }

    void HwSolenoid::set_location() {
        if (_has_errors) {
            return;
        }

        float mpos = steps_to_mpos(get_axis_motor_steps(_axis_index), _axis_index);  // get the axis machine position in mm

        bool is_solenoid_on = _dir_invert ? (mpos < 0.0) : (mpos > 0.0);

        log_warn("is_solenoid_on: " << is_solenoid_on);

        _output_pin.synchronousWrite(is_solenoid_on);
    }

    void HwSolenoid::set_disable(bool disable) {}  // NOP

    namespace {
        MotorFactory::InstanceBuilder<HwSolenoid> registration("hwsolenoid");
    }
}
