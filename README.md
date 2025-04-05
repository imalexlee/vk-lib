## Vulkan Library

A lightweight C++ abstraction layer for Vulkan that simplifies initialization while providing sensible defaults.

## Features

- Simplified creation of Vulkan parameter and state structures
- Automatic handling of two-call pattern for enumeration functions
- Direct mapping to Vulkan structures

## Benefits

- Reduces boilerplate code
- Prevents common initialization mistakes
- You don't have to "learn" this library as naming conventions map 1:1 to Vulkan
- Maintains full control and visibility of Vulkan objects
- Balances common sense modern C++ features like `std::span` with C-like simplicity

## Usage Examples

### Instance Creation

```cpp
// Create application info
auto app_info = vk_lib::application_info(
"My App", // application name
"My Engine", // engine name
VK_API_VERSION_1_3, // Vulkan API version
VK_MAKE_API_VERSION(0, 1, 0, 0), // app version
VK_MAKE_API_VERSION(0, 1, 0, 0) // engine version
);

// Setup instance extensions and layers
std::array extensions = { "VK_KHR_surface" };
std::array layers = { "VK_LAYER_KHRONOS_validation" };

// Create instance
auto instance_info = vk_lib::instance_create_info(&app_info, layers, extensions);

VkInstance instance;
if (vk_lib::create_instance_with_entrypoints(&instance_info, &instance) != VK_SUCCESS) {
// Handle error
}
```

### Device Creation

```cpp
std::array queue_priorities = { 1.0f };
auto queue_info = vk_lib::device_queue_create_info(
0, // family index
1, // queue count
queue_priorities);

std::array device_extensions = {
VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Set up device features
VkPhysicalDeviceFeatures device_features{};
device_features.samplerAnisotropy = VK_TRUE;

// Create device info
std::array queue_create_infos = { queue_info };
auto device_info = vk_lib::device_create_info(queue_create_infos, device_extensions, &device_features);

// Create logical device
VkDevice device;
if (vk_lib::create_device_with_entrypoints(physical_device, &device_info, &device) != VK_SUCCESS) {
// Handle error
}
```

## Requirements

- C++20 compatible compiler
- Vulkan SDK
- [volk](https://github.com/zeux/volk) (Which is pulled in automatically through CMake's `FetchContent`)

## Notes

This library is designed to be a thin wrapper around Vulkan, focusing on making the API more ergonomic while maintaining
full performance and control. It does not hide Vulkan concepts or create high-level abstractions, allowing developers to
maintain direct access to Vulkan objects and functions.

The helper functions follow a consistent naming convention that matches the Vulkan structure names they create, making
it easy to understand their purpose and mapping to Vulkan concepts.