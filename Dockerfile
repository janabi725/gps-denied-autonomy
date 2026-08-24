FROM ros:humble-ros-base

# Zusätzliche Pakete für dein Projekt
RUN apt-get update && apt-get install -y \
    python3-pip \
    python3-colcon-common-extensions \
    ros-humble-cv-bridge \
    ros-humble-image-transport \
    ros-humble-vision-opencv \
    ros-humble-sensor-msgs \
    libopencv-dev \
    python3-opencv \
    ros-humble-camera-ros \
    ros-humble-v4l2-camera \
    libi2c-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

CMD ["bash"]