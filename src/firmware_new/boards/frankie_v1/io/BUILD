package(default_visibility = ["//visibility:public"])

load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cc_library(
    name = "gpio_pin",
    srcs = ["gpio_pin.c"],
    hdrs = ["gpio_pin.h"],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [
        "//firmware_new/boards/frankie_v1:hal",
    ],
)

cc_library(
    name = "pwm_pin",
    srcs = ["pwm_pin.c"],
    hdrs = ["pwm_pin.h"],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [
        "//firmware_new/boards/frankie_v1:hal",
    ],
)

cc_library(
    name = "allegro_a3931_motor_driver",
    srcs = ["allegro_a3931_motor_driver.c"],
    hdrs = ["allegro_a3931_motor_driver.h"],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [
        ":gpio_pin",
        ":pwm_pin",
    ],
)

cc_library(
    name = "infineon_TLE009_E1000_angle_sensor",
    srcs = ["infineon_TLE5009_E1000_angle_sensor.c"],
    hdrs = ["infineon_TLE5009_E1000_angle_sensor.h"],
)

cc_test(
    name = "infineon_TLE009_E1000_angle_sensor_test",
    srcs = ["infineon_TLE5009_E1000_angle_sensor_test.cpp"],
    deps = [
        "//firmware_new/boards/frankie_v1/io:infineon_TLE009_E1000_angle_sensor",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name = "drivetrain",
    srcs = ["drivetrain.c"],
    hdrs = ["drivetrain.h"],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [":drivetrain_unit"],
)

cc_library(
    name = "drivetrain_unit",
    srcs = ["drivetrain_unit.c"],
    hdrs = ["drivetrain_unit.h"],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [":allegro_a3931_motor_driver"],
)

cc_library(
    name = "proto_multicast_communication",
    srcs = [
        "proto_multicast_communication_profile.c",
        "proto_multicast_communication_tasks.c",
    ],
    hdrs = [
        "proto_multicast_communication_profile.h",
        "proto_multicast_communication_tasks.h",
    ],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [
        ":ublox_odinw262_communicator",
        "//firmware_new/boards/frankie_v1:hal",
        "//firmware_new/boards/frankie_v1:lwip",
        "//shared/proto:tbots_nanopb_proto",
        "@nanopb",
    ],
)

cc_library(
    name = "network_logger",
    srcs = ["network_logger.c"],
    hdrs = ["network_logger.h"],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [
        "//firmware_new/boards/frankie_v1:hal",
        "//firmware_new/boards/frankie_v1:lwip",
        "//firmware_new/boards/frankie_v1:rtos",
        "//firmware_new/boards/frankie_v1/io:proto_multicast_communication",
        "//shared/proto:tbots_nanopb_proto",
        "@nanopb",
    ],
    alwayslink = True,
)

cc_library(
    name = "uart_logger",
    srcs = ["uart_logger.c"],
    hdrs = ["uart_logger.h"],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [
        "//firmware_new/boards/frankie_v1:hal",
        "//firmware_new/boards/frankie_v1:rtos",
        "//firmware_new/boards/frankie_v1:usart",
        "//shared/proto:tbots_nanopb_proto",
        "@nanopb",
    ],
    alwayslink = True,
)

cc_library(
    name = "ublox_odinw262_communicator",
    srcs = ["ublox_odinw262_communicator.c"],
    hdrs = ["ublox_odinw262_communicator.h"],
    restricted_to = ["//cc_toolchain:stm32h7"],
    deps = [
        ":gpio_pin",
        "//firmware/app/logger",
        "//firmware_new/boards/frankie_v1:hal",
        "//firmware_new/boards/frankie_v1:rtos",
        "//firmware_new/boards/frankie_v1:usart",
        "//firmware_new/boards/frankie_v1/io:ublox_odinw262_communicator_buffer_utils",
        "//shared:constants",
        "//shared/proto:tbots_nanopb_proto",
        "@nanopb",
    ],
    alwayslink = True,
)

cc_library(
    name = "ublox_odinw262_communicator_buffer_utils",
    srcs = ["ublox_odinw262_communicator_buffer_utils.c"],
    hdrs = ["ublox_odinw262_communicator_buffer_utils.h"],
    alwayslink = True,
)

# external cmake repo for property based testing
cmake_external(
    name = "rapidcheck",
    lib_source = "@rapidcheck//:all",
    static_libraries = ["librapidcheck.a"],
)

cc_test(
    name = "ublox_odinw262_communicator_buffer_utils_test",
    srcs = ["ublox_odinw262_communicator_buffer_utils_test.cpp"],
    deps = [
        ":rapidcheck",
        "//firmware_new/boards/frankie_v1/io:ublox_odinw262_communicator_buffer_utils",
        "@gtest//:gtest_main",
    ],
)
