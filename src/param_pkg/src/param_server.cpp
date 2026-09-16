#include "rclcpp/rclcpp.hpp"

class MinimalParamServer : public rclcpp::Node
{
public:
  MinimalParamServer() : Node("minimal_param_server", rclcpp::NodeOptions().allow_undeclared_parameters(true))
  {
    //param will be revised from launch file
    this->declare_parameter("launchparam", "default");
    this->declare_parameter("param1", "default");
    this->declare_parameter("param2", 0);
  }
  // 声明参数；
  void declare_param()
  {
    // 声明参数并设置默认值
    this->declare_parameter("car_type", "Tiger");
    this->declare_parameter("height", 1.50);
    this->declare_parameter("wheels", 4);
    // 需要设置 rclcpp::NodeOptions().allow_undeclared_parameters(true),否则非法
    this->set_parameter(rclcpp::Parameter("undcl_test", 100));
  }
  // 查询参数
  void get_param()
  {
    RCLCPP_INFO(this->get_logger(), "------------------查----------------");
    // 获取指定
    rclcpp::Parameter car_type = this->get_parameter("car_type");
    RCLCPP_INFO(this->get_logger(), "car_type:%s", car_type.as_string().c_str());
    RCLCPP_INFO(this->get_logger(), "height:%.2f", this->get_parameter("height").as_double());
    RCLCPP_INFO(this->get_logger(), "wheels:%ld", this->get_parameter("wheels").as_int());
    RCLCPP_INFO(this->get_logger(), "undcl_test:%ld", this->get_parameter("undcl_test").as_int());
    // 判断包含
    RCLCPP_INFO(this->get_logger(), "包含car_type? %d", this->has_parameter("car_type"));
    RCLCPP_INFO(this->get_logger(), "包含car_typesxxxx? %d", this->has_parameter("car_typexxxx"));
    // 获取所有
    auto params = this->get_parameters({"car_type", "height", "wheels"});
    for (auto &param : params)
    {
      RCLCPP_INFO(this->get_logger(), "name = %s, value = %s", param.get_name().c_str(), param.value_to_string().c_str());
    }

    //param from launch file
    RCLCPP_INFO(this->get_logger(), "launchparam = %s", this->get_parameter("launchparam").as_string().c_str());
    RCLCPP_INFO(this->get_logger(), "param1 = %s", this->get_parameter("param1").as_string().c_str());
    RCLCPP_INFO(this->get_logger(), "param2 = %ld", this->get_parameter("param2").as_int());
  }
  // 修改参数
  void update_param()
  {
    RCLCPP_INFO(this->get_logger(), "------------------改----------------");
    this->set_parameter(rclcpp::Parameter("height", 1.75));
    RCLCPP_INFO(this->get_logger(), "height:%.2f", this->get_parameter("height").as_double());
  }
  // 删除参数
  void del_param()
  {
    RCLCPP_INFO(this->get_logger(), "------------------删----------------");
    // this->undeclare_parameter("car_type");
    // RCLCPP_INFO(this->get_logger(),"删除操作后， car_type 还存在马吗? %d",this->has_parameter("car_type"));
    RCLCPP_INFO(this->get_logger(), "删除操作前， undcl_test 存在吗? %d", this->has_parameter("undcl_test"));
    this->undeclare_parameter("undcl_test");
    RCLCPP_INFO(this->get_logger(), "删除操作前， undcl_test 存在吗? %d", this->has_parameter("undcl_test"));
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto paramServer = std::make_shared<MinimalParamServer>();
  paramServer->declare_param();
  paramServer->get_param();
  paramServer->update_param();
  paramServer->del_param();
  rclcpp::spin(paramServer);
  rclcpp::shutdown();
  return 0;
}
