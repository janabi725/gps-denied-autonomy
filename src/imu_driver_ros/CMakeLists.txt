cmake_minimum_required(VERSION 3.8)
project(imu_driver_ros)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package (rclcpp REQUIRED)
find_package (std_msgs REQUIRED)
find_package (sensor_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)


add_executable(
imu_publisher 
src/imu_publisher.cpp 
src/icm20948.cpp)

add_executable(
listener 
src/imu_subscriber.cpp)


ament_target_dependencies(
imu_publisher 
rclcpp 
std_msgs
sensor_msgs )


ament_target_dependencies(
listener 
rclcpp 
std_msgs)


target_link_libraries(imu_publisher i2c)

install(TARGETS
  imu_publisher
  DESTINATION lib/${PROJECT_NAME}
)

target_include_directories(imu_publisher PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

ament_package()
