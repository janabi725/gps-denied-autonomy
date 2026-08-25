#!/bin/bash

docker run -it --rm \
  --privileged \
  --network host \
  --ipc host \
  -v /dev:/dev \
  --device=/dev:/dev \
  -v /run/udev:/run/udev:ro \
  -v $(pwd):/workspace \
  gps-denied:ros_system