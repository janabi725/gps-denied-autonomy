# GPS-Denied Visual Autonomy

**A Raspberry Pi 5-based Visual-Inertial Odometry (VIO) platform for robust navigation without GPS.**  
Project focused on deep understanding of sensor fusion, computer vision and embedded AI.

---

## Overview

This project implements a complete **GPS-denied visual autonomy system** inspired by Theseus Micro VPS.  
It combines a high-frame-rate camera with a 9-axis IMU on a Raspberry Pi 5 to estimate position and orientation in real time — completely independent of satellite signals.

The goal is to build a practical, low-cost platform for autonomous ground vehicles (and later drones) while applying **First Principles Thinking** and the "Mens et Manus" philosophy.  
Current focus: robust Visual-Inertial Odometry, sensor fusion and gradual integration of deep-learning-based feature matching.

---

## Key Features

- Real-time Visual-Inertial Odometry (VIO) using OpenVINS / roboeye framework
- Tight camera-IMU fusion with online calibration
- 6-DoF pose estimation (position + orientation) without GPS
- Modular ROS 2 architecture for easy extension
- Support for both handheld testing and 2WD rover platform
- Planned deep-learning upgrade (SuperPoint / LightGlue feature matching)
- Full First-Principles documentation and experiment logging

---

## Hardware

- **Compute**: Raspberry Pi 5 (4 GB)
- **Camera**: ArduCam OV9281 1Mpx Global Shutter
- **IMU**: ICM-20948 3-axis 
---

## Software Stack

- **OS**: Raspberry Pi OS 64-bit (Bookworm)
- **Framework**: ROS 2 Humble/Iron + OpenVINS / roboeye
- **Vision**: OpenCV + ORB / planned SuperPoint
- **Fusion**: Extended Kalman Filter (EKF) + Graph Optimization
- **Languages**: Python 3 + C++ (for performance-critical parts)
- **ML**: PyTorch (for future deep feature extractors)

---

## Quick Start

```bash
# 1. Clone the repository
git clone https://github.com/janabi725/gps-denied-visual-autonomy.git
cd gps-denied-visual-autonomy

# 2. Install dependencies (on Raspberry Pi)
./scripts/setup.sh

# 3. Calibrate camera and IMU (one-time)
ros2 launch vio_calibration calibrate.launch.py

# 4. Run VIO (live)
ros2 launch vio_core vio.launch.py
