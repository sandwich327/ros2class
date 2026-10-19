#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
using namespace std::chrono_literals;
using rclcpp::WallRate;
#include "rcl_interfaces/msg/set_parameters_result.hpp"

class variedInterval : public rclcpp::Node
{
private:
    /* data */
    double tmrInterval = 100.0;
    rclcpp::TimerBase::SharedPtr tmr;

    // 回调句柄
    rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr callback_handle;

    // 独立定义的回调处理函数
    rcl_interfaces::msg::SetParametersResult onParameterChange(const std::vector<rclcpp::Parameter> &parameters)
    {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        result.reason = "";

        for (const auto &param : parameters)
        {
            if (param.get_name() == "tmrInterval")
            {

                // 类型校验：double 类型
                if (param.get_type() != rclcpp::ParameterType::PARAMETER_DOUBLE)
                {
                    result.successful = false;
                    result.reason = "参数 'tmrInterval' 必须是 double 类型";
                    RCLCPP_WARN(this->get_logger(), "%s", result.reason.c_str());
                    return result;
                }

                // 获取新值
                double new_value = param.as_double();

                tmrInterval = new_value;

                RCLCPP_INFO(this->get_logger(), "参数更新成功: %.4f", new_value);

                if (new_value < 0.0)
                {
                    result.successful = false;
                    result.reason = "参数值超出范围 < [0.0]";
                    return result;
                }
                else
                {
                    // 更新定时器周期
                    tmr->cancel();
                    tmr = this->create_wall_timer(std::chrono::milliseconds(static_cast<int>(tmrInterval)),
                                                  std::bind(&variedInterval::tmr_callback, this));
                }
            }
        }
        return result;
    }

    void tmr_callback()
    {
        auto message = std_msgs::msg::String();
        message.data = "Hello World";
        RCLCPP_INFO(this->get_logger(), message.data.c_str());
    }

public:
    variedInterval() : rclcpp::Node("variedInterval")
    {
        RCLCPP_INFO(this->get_logger(), "initialized");
        this->declare_parameter("tmrInterval", 100.0);
        // this->get_parameter("tmrInterval", tmrInterval);
        tmrInterval = this->get_parameter("tmrInterval").as_double();

        callback_handle = this->add_on_set_parameters_callback(
            std::bind(&variedInterval::onParameterChange, this, std::placeholders::_1));

        tmr = this->create_wall_timer(std::chrono::milliseconds(static_cast<int>(tmrInterval)),
                                      std::bind(&variedInterval::tmr_callback, this));
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<variedInterval>();
    rclcpp::spin(node);
    rclcpp::shutdown();
}
