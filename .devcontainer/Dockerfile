# Offizielles ROS 2 Humble Image
FROM osrf/ros:humble-desktop

# System updaten und Standard-Tools installieren
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    python3-pip \
    nano \
    git \
    && rm -rf /var/lib/apt/lists/*

# ROS 2 bei jedem Terminal-Start automatisch sourcen
RUN echo "source /opt/ros/humble/setup.bash" >> /root/.bashrc