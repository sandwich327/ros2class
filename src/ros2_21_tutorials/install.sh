#!/bin/bash
#author:guyuehoem-qiaolongLi
#email:liqiaolong@guyuehome.com

# 自动选择并安装ROS2脚本
set -e

# 获取Ubuntu的版本信息
UBUNTU_VERSION=$(lsb_release -sr)
UBUNTU_CODENAME=$(lsb_release -cs)
ARCH=$(dpkg --print-architecture)

# 根据Ubuntu版本选择ROS2版本
case "$UBUNTU_VERSION" in
    "20.04")
        ROS2_VERSION="foxy"
        ;;
    "22.04")
        ROS2_VERSION="humble"
        ;;
    "24.04")
        ROS2_VERSION="jazzy" 
        ;;
    *)
        echo "不支持的Ubuntu版本: $UBUNTU_VERSION"
        exit 1
        ;;
esac

echo "检测到Ubuntu版本: $UBUNTU_VERSION ($UBUNTU_CODENAME)"
echo "选择安装ROS2版本: $ROS2_VERSION"

# 更新系统
echo "更新系统包列表..."
sudo apt update && sudo apt upgrade -y

# 安装必要的依赖
echo "安装必要的依赖包..."
sudo apt install -y curl gnupg lsb-release software-properties-common

# 创建密钥目录
sudo mkdir -p /usr/share/keyrings

# 下载并添加ROS2 GPG密钥
# sudo rm -f /usr/share/keyrings/ros-archive-keyring.gpg
if [ ! -f /usr/share/keyrings/ros-archive-keyring.gpg ]; then
    curl -fsSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key | sudo gpg --dearmor -o /usr/share/keyrings/ros-archive-keyring.gpg
fi

# 验证密钥文件是否存在
if [ ! -f "/usr/share/keyrings/ros-archive-keyring.gpg" ]; then
    echo "错误：无法创建ROS2 GPG密钥文件"
    exit 1
fi

# 添加ROS2的APT源（使用新的格式）
echo "添加ROS2的APT源..."
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] https://mirrors.tuna.tsinghua.edu.cn/ros2/ubuntu $UBUNTU_CODENAME main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# 更新包列表
echo "更新包列表..."
sudo apt update

# 安装ROS2
echo "安装ROS2 ${ROS2_VERSION}..."
sudo apt install -y ros-${ROS2_VERSION}-desktop

echo "安装开发工具和依赖 ${ROS2_VERSION}..."
sudo apt install -y \
    ros-${ROS2_VERSION}-rosidl-default-generators \
    ros-${ROS2_VERSION}-rosidl-default-runtime \
    python3-colcon-common-extensions \
    python3-colcon-ros \
    python3-colcon-cmake \
    python3-rosdep \
    build-essential \
    python3-flake8  \
    python3-pytest-cov \
    python3-pip \
    python3-setuptools \
    libzbar-dev \
    ntpdate

# 时间同步
sudo ntpdate ntp.ubuntu.com

# 安装额外的Python工具
echo "安装额外的Python工具..."
# 检查pip版本并相应调整安装命令
PIP_VERSION=$(pip3 --version | awk '{print $2}' | cut -d. -f1)

if [ "$PIP_VERSION" -ge 23 ]; then
    # 新版本pip使用 --break-system-packages
    pip3 install -U -i https://pypi.tuna.tsinghua.edu.cn/simple \
        argcomplete \
        pytest-repeat \
        pytest-rerunfailures \
        --break-system-packages
else
    # 旧版本pip不需要 --break-system-packages
    pip3 install -U -i https://pypi.tuna.tsinghua.edu.cn/simple \
        argcomplete \
        pytest-repeat \
        pytest-rerunfailures
fi


echo "配置并初始化rosdep..."
if [ ! -f "/etc/ros/rosdep/sources.list.d/20-default.list" ]; then
    # 创建rosdep文件夹
    sudo mkdir -p /etc/ros/rosdep/sources.list.d/

    # 设置rosdistro源
    export ROSDISTRO_INDEX_URL=https://mirrors.tuna.tsinghua.edu.cn/rosdistro/index-v4.yaml

    # 下载rosdep源文件
    echo "下载rosdep源文件..."
    sudo wget https://mirrors.tuna.tsinghua.edu.cn/github-raw/ros/rosdistro/master/rosdep/sources.list.d/20-default.list -O /etc/ros/rosdep/sources.list.d/20-default.list

    # 初始化rosdep
    # sudo rosdep init || true

    # 更新rosdep
    echo "更新rosdep..."
    rosdep update
fi

# Add ROSDISTRO_INDEX_URL to environment variables
if ! grep -q "export ROSDISTRO_INDEX_URL=" ~/.bashrc; then
    echo "export ROSDISTRO_INDEX_URL=https://mirrors.tuna.tsinghua.edu.cn/rosdistro/index-v4.yaml" >> ~/.bashrc
fi

# 使用方法（先进入工作空间）
# rosdep install -i --from-path src --rosdistro $ROS_DISTRO -y

# 卸载rosdep
# # 删除 rosdep 配置文件和目录
# sudo rm -rf /etc/ros/rosdep
# sudo rm -rf ~/.ros/rosdep

# # 使用 apt 卸载 python3-rosdep 包
# sudo apt remove --purge -y python3-rosdep
# sudo apt autoremove -y

# 设置环境变量
echo "设置环境变量..."
if ! grep -q "source /opt/ros/${ROS2_VERSION}/setup.bash" ~/.bashrc; then
    echo "source /opt/ros/${ROS2_VERSION}/setup.bash" >> ~/.bashrc
fi

# 立即应用环境变量
source /opt/ros/${ROS2_VERSION}/setup.bash

echo "ROS2 ${ROS2_VERSION} 安装完成！"

# 验证安装
echo "验证ROS2安装..."
if command -v ros2 &> /dev/null; then
    echo "ros2已安装成功"
else
    echo "警告：ros2 命令未找到，可能需要重新启动终端或手动source环境变量"
    echo "请运行: source ~/.bashrc"
fi

echo "依赖包安装方式请使用如下指令:'cd ~/dev_ws & rosdep install -i --from-path src --rosdistro $ROS_DISTRO -y'"
