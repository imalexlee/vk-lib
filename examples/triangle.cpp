#include "vk_lib/core.h"

#include <vulkan/vk_enum_string_helper.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vk_lib/commands.h>
#include <vk_lib/pipelines.h>
#include <vk_lib/presentation.h>
#include <vk_lib/rendering.h>
#include <vk_lib/resources.h>
#include <vk_lib/shaders.h>
#include <vk_lib/synchronization.h>

#define VK_CHECK(x)                                                                                                                                  \
    do {                                                                                                                                             \
        VkResult err = x;                                                                                                                            \
        if (err) {                                                                                                                                   \
            std::cerr << "Detected Vulkan error: " << string_VkResult(err) << std::endl;                                                             \
            abort();                                                                                                                                 \
        }                                                                                                                                            \
    } while (0)

struct SwapchainContext {
    VkSwapchainKHR           swapchain{};
    VkSurfaceFormatKHR       surface_format{};
    VkExtent2D               extent{};
    std::vector<VkImage>     images{};
    std::vector<VkImageView> image_views{};
};

struct GraphicsPipeline {
    VkPipeline       pipeline{};
    VkPipelineLayout pipeline_layout{};
    VkShaderModule   vert_shader{};
    VkShaderModule   frag_shader{};
};

struct Frame {
    VkSemaphore                  image_available_semaphore{};
    VkSemaphore                  render_finished_semaphore{};
    VkFence                      in_flight_fence{};
    VkRenderingInfoKHR           rendering_info{};
    VkCommandBuffer              command_buffer{};
    VkRenderingAttachmentInfoKHR attachment_info{};
    VkCommandBufferSubmitInfoKHR command_buffer_submit_info{};
    VkSemaphoreSubmitInfoKHR     wait_semaphore_submit_info{};
    VkSemaphoreSubmitInfoKHR     signal_semaphore_submit_info{};
    VkSubmitInfo2                submit_info_2{};
    VkImageMemoryBarrier2        end_render_image_memory_barrier{};
    VkDependencyInfoKHR          dependency_info{};
};

struct VkContext {
    VkInstance         instance{};
    VkPhysicalDevice   physical_device{};
    VkDevice           device{};
    VkCommandPool      frame_command_pool{};
    VkQueue            graphics_queue{};
    VkQueue            present_queue{};
    GLFWwindow*        window{};
    uint32_t           graphics_present_queue_family{};
    VkSurfaceKHR       surface{};
    SwapchainContext   swapchain_ctx{};
    GraphicsPipeline   graphics_pipeline{};
    std::vector<Frame> frames{};
    uint64_t           curr_frame{};
};

[[noreturn]] void abort_message(std::string_view message) {
    std::cerr << message << std::endl;
    std::abort();
}

VkInstance create_instance() {
    if (!glfwVulkanSupported()) {
        abort_message("GLFW cannot find the vulkan loader and an ICD");
    }
    uint32_t                 glfw_extension_count = 0;
    const char**             glfw_extensions      = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    std::vector<const char*> extensions{};
    extensions.reserve(glfw_extension_count + 2);
    extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    for (uint32_t i = 0; i < glfw_extension_count; i++) {
        extensions.push_back(glfw_extensions[i]);
    }
    std::vector<const char*> layers;
#ifndef NDEBUG
    layers.push_back("VK_LAYER_KHRONOS_validation");
#endif
    VkApplicationInfo    app_info    = application_info("hello triangle", "engine name", VK_API_VERSION_1_3);
    VkInstanceCreateInfo instance_ci = instance_create_info(&app_info, layers, extensions);
    VkInstance           instance;
    VK_CHECK(vkCreateInstance(&instance_ci, nullptr, &instance));
    return instance;
}

VkPhysicalDevice select_physical_device(VkInstance instance) {
    // Find a device that supports vulkan 1.3. Prefer discrete GPU's
    std::vector<VkPhysicalDevice> physical_devices;
    VK_CHECK(enumerate_physical_devices(instance, &physical_devices));
    VkPhysicalDevice chosen_device = nullptr;
    for (const VkPhysicalDevice& physical_device : physical_devices) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physical_device, &properties);
        if (properties.apiVersion < VK_API_VERSION_1_3) {
            continue;
        }
        chosen_device = physical_device;
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return chosen_device;
        }
    }
    if (chosen_device == nullptr) {
        abort_message("Could not find a suitable physical device");
    }
    return chosen_device;
}

uint32_t select_queue_family(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
    const std::vector<VkQueueFamilyProperties> queue_family_properties = physical_device_queue_family_properties(physical_device);
    // Find a queue family with both graphics and presentation capabilities
    for (uint32_t i = 0; i < queue_family_properties.size(); i++) {
        const VkQueueFamilyProperties* family_properties = &queue_family_properties[i];
        if (family_properties->queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            bool present_supported = false;
            physical_device_get_surface_support(physical_device, surface, i, &present_supported);
            if (present_supported) {
                return i;
            }
        }
    }
    abort_message("Could not find a suitable queue family");
}

VkDevice create_logical_device(VkPhysicalDevice physical_device, uint32_t queue_family) {
    std::array              queue_priorities   = {1.f};
    VkDeviceQueueCreateInfo queue_ci           = device_queue_create_info(queue_family, 1, queue_priorities);
    std::array              queue_create_infos = {queue_ci};

    std::array device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
                                    VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME};
    // query for required features first
    VkPhysicalDeviceVulkan13Features vk_1_3_features{};
    vk_1_3_features.sType                                = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    VkPhysicalDeviceFeatures2 physical_device_features_2 = VkPhysicalDeviceFeatures2{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR};
    physical_device_features_2.pNext                     = &vk_1_3_features;

    if (vk_1_3_features.dynamicRendering == VK_FALSE || vk_1_3_features.synchronization2 == VK_FALSE) {
        abort_message("Required features are not supported by this device");
    }
    // enable only dynamic rendering and sync 2 now, instead of all the supported features
    vk_1_3_features                  = VkPhysicalDeviceVulkan13Features{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
    vk_1_3_features.dynamicRendering = VK_TRUE;
    vk_1_3_features.synchronization2 = VK_TRUE;

    physical_device_features_2       = VkPhysicalDeviceFeatures2{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR};
    physical_device_features_2.pNext = &vk_1_3_features;

    VkDeviceCreateInfo device_ci = device_create_info(queue_create_infos, device_extensions, nullptr, &physical_device_features_2);
    VkDevice           device;
    create_device_with_entrypoints(physical_device, &device_ci, &device);

    return device;
}

SwapchainContext create_swapchain_context(VkPhysicalDevice physical_device, VkDevice device, VkSurfaceKHR surface, GLFWwindow* window) {
    std::vector<VkSurfaceFormatKHR> surface_formats;
    VK_CHECK(physical_device_get_surface_formats(physical_device, surface, &surface_formats));
    VkSurfaceFormatKHR format = surface_formats[0];
    for (const VkSurfaceFormatKHR& available_format : surface_formats) {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            format = available_format;
        }
    }
    VkSurfaceCapabilitiesKHR capabilities;
    VK_CHECK(physical_device_get_surface_capabilities(physical_device, surface, &capabilities));
    VkExtent2D swapchain_extent{};
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        swapchain_extent = capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        swapchain_extent.width  = std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        swapchain_extent.height = std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }
    // we want triple buffering
    uint32_t image_count = std::max(capabilities.minImageCount, 3u);
    if (capabilities.maxImageCount != 0) {
        image_count = std::min(image_count, capabilities.maxImageCount);
    }

    SwapchainBuilder swapchain_builder{};
    swapchain_builder_set_image_properties(&swapchain_builder, format.format, format.colorSpace, swapchain_extent.width, swapchain_extent.height,
                                           image_count, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    swapchain_builder_set_presentation_properties(&swapchain_builder, VK_PRESENT_MODE_FIFO_KHR);
    swapchain_builder_set_surface_properties(&swapchain_builder, surface, capabilities.currentTransform);
    swapchain_builder_set_sharing_mode(&swapchain_builder, VK_SHARING_MODE_EXCLUSIVE);

    VkSwapchainKHR swapchain;
    swapchain_builder_swapchain_create(&swapchain_builder, device, &swapchain);

    SwapchainContext swapchain_context{};
    swapchain_context.extent         = swapchain_extent;
    swapchain_context.surface_format = format;
    swapchain_context.swapchain      = swapchain;

    VK_CHECK(swapchain_get_images(device, swapchain, &swapchain_context.images));

    swapchain_context.image_views.reserve(swapchain_context.images.size());
    for (VkImage image : swapchain_context.images) {
        VkImageView image_view;
        image_view_create(device, image, VK_IMAGE_VIEW_TYPE_2D, format.format, VK_IMAGE_ASPECT_COLOR_BIT, &image_view);
        swapchain_context.image_views.push_back(image_view);
    }

    return swapchain_context;
}

VkShaderModule load_shader(VkDevice device, const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        abort_message("Failed to find shader");
    }
    const size_t      file_size = file.tellg();
    std::vector<char> shader_data(file_size);
    file.seekg(0);
    file.read(shader_data.data(), static_cast<uint32_t>(file_size));
    VkShaderModule shader_module;
    VK_CHECK(shader_module_create(device, reinterpret_cast<const uint32_t*>(shader_data.data()), file_size, &shader_module));
    return shader_module;
}

GraphicsPipeline create_graphics_pipeline(VkDevice device, VkFormat color_attachment_format, uint32_t width, uint32_t height) {
    VkShaderModule vert_shader = load_shader(device, "../../examples/shaders/triangle.vert.spv");
    VkShaderModule frag_shader = load_shader(device, "../../examples/shaders/triangle.frag.spv");

    const VkViewport viewport = viewport_create(static_cast<float>(width), static_cast<float>(height));
    const VkRect2D   scissor  = rect_2d_create(width, height);

    VkPipelineLayout pipeline_layout;
    VK_CHECK(pipeline_layout_create(device, {}, {}, &pipeline_layout));

    std::array                             color_attachment_formats = {color_attachment_format};
    const VkPipelineRenderingCreateInfoKHR rendering_create_info    = pipeline_rendering_create_info(color_attachment_formats);

    GraphicsPipelineBuilder builder{};
    graphics_pipeline_builder_add_shader_stage(&builder, VK_SHADER_STAGE_VERTEX_BIT, vert_shader);
    graphics_pipeline_builder_add_shader_stage(&builder, VK_SHADER_STAGE_FRAGMENT_BIT, frag_shader);
    graphics_pipeline_builder_set_input_assembly_state(&builder, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    graphics_pipeline_builder_set_viewport(&builder, &viewport, &scissor);
    graphics_pipeline_builder_set_rasterization_state(&builder, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
    graphics_pipeline_builder_set_multisample_state(&builder, VK_SAMPLE_COUNT_1_BIT);
    graphics_pipeline_builder_set_layout(&builder, pipeline_layout);
    graphics_pipeline_builder_add_color_blend_attachment(&builder, false);
    graphics_pipeline_builder_set_color_blend_state(&builder, false);
    graphics_pipeline_builder_set_pNext(&builder, &rendering_create_info);

    VkPipeline pipeline;
    VK_CHECK(graphics_pipeline_builder_pipeline_create(&builder, device, &pipeline));

    GraphicsPipeline graphics_pipeline{};
    graphics_pipeline.pipeline        = pipeline;
    graphics_pipeline.pipeline_layout = pipeline_layout;
    graphics_pipeline.vert_shader     = vert_shader;
    graphics_pipeline.frag_shader     = frag_shader;

    return graphics_pipeline;
}

std::vector<Frame> init_frames(VkDevice device, VkCommandPool command_pool, std::span<VkImageView> frame_image_views, std::span<VkImage> frame_images,
                               uint32_t queue_family_index) {
    std::vector<Frame> frames;
    frames.resize(frame_image_views.size());

    for (uint32_t i = 0; i < frame_image_views.size(); i++) {
        Frame* frame = &frames[i];

        VkImageView frame_image_view = frame_image_views[i];
        frame->attachment_info       = rendering_attachment_info_create(frame_image_view, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                                        VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE);

        VkCommandBufferAllocateInfo command_buffer_ai = command_buffer_allocate_info(command_pool);
        vkAllocateCommandBuffers(device, &command_buffer_ai, &frame->command_buffer);
        VK_CHECK(semaphore_create(device, &frame->image_available_semaphore));
        VK_CHECK(semaphore_create(device, &frame->render_finished_semaphore));
        VK_CHECK(fence_create(device, VK_FENCE_CREATE_SIGNALED_BIT, &frame->in_flight_fence));

        frame->command_buffer_submit_info = command_buffer_submit_info(frame->command_buffer);
        frame->wait_semaphore_submit_info =
            semaphore_submit_info_create(frame->image_available_semaphore, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR);
        frame->signal_semaphore_submit_info =
            semaphore_submit_info_create(frame->render_finished_semaphore, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR);

        frame->submit_info_2 =
            submit_info_2(&frame->command_buffer_submit_info, &frame->wait_semaphore_submit_info, &frame->signal_semaphore_submit_info);

        VkImageSubresourceRange subresource_range = image_subresource_range_create(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);

        VkImage frame_image                    = frame_images[i];
        frame->end_render_image_memory_barrier = image_memory_barrier_2_create(
            frame_image, subresource_range, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, queue_family_index, queue_family_index,
            VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
            VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_2_NONE);
        frame->dependency_info = dependency_info_create(&frame->end_render_image_memory_barrier, nullptr, nullptr);
    }

    return frames;
}

void destroy_resources(VkContext* vk_context) {
    vkDeviceWaitIdle(vk_context->device);
    for (Frame& frame : vk_context->frames) {
        semaphore_destroy(vk_context->device, frame.image_available_semaphore);
        semaphore_destroy(vk_context->device, frame.render_finished_semaphore);
        fence_destroy(vk_context->device, frame.in_flight_fence);
    }
    vkDestroyCommandPool(vk_context->device, vk_context->frame_command_pool, nullptr);
    pipeline_destroy(vk_context->device, vk_context->graphics_pipeline.pipeline);
    pipeline_layout_destroy(vk_context->device, vk_context->graphics_pipeline.pipeline_layout);
    shader_module_destroy(vk_context->device, vk_context->graphics_pipeline.vert_shader);
    shader_module_destroy(vk_context->device, vk_context->graphics_pipeline.frag_shader);
    swapchain_destroy(vk_context->device, vk_context->swapchain_ctx.swapchain);
    surface_destroy(vk_context->instance, vk_context->surface);
    for (VkImageView image_view : vk_context->swapchain_ctx.image_views) {
        image_view_destroy(vk_context->device, image_view);
    }
    vkDestroyDevice(vk_context->device, nullptr);

    glfwDestroyWindow(vk_context->window);
    glfwTerminate();
    vkDestroyInstance(vk_context->instance, nullptr);
};

int main() {
    VkContext vk_context{};
    if (!glfwInit()) {
        abort_message("GLFW cannot be initialized");
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    vk_context.window          = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);
    vk_context.instance        = create_instance();
    vk_context.physical_device = select_physical_device(vk_context.instance);
    VK_CHECK(glfwCreateWindowSurface(vk_context.instance, vk_context.window, nullptr, &vk_context.surface));
    vk_context.graphics_present_queue_family = select_queue_family(vk_context.physical_device, vk_context.surface);
    vk_context.device                        = create_logical_device(vk_context.physical_device, vk_context.graphics_present_queue_family);
    vkGetDeviceQueue(vk_context.device, vk_context.graphics_present_queue_family, 0, &vk_context.graphics_queue);
    vkGetDeviceQueue(vk_context.device, vk_context.graphics_present_queue_family, 0, &vk_context.present_queue);
    vk_context.swapchain_ctx     = create_swapchain_context(vk_context.physical_device, vk_context.device, vk_context.surface, vk_context.window);
    vk_context.graphics_pipeline = create_graphics_pipeline(vk_context.device, vk_context.swapchain_ctx.surface_format.format,
                                                            vk_context.swapchain_ctx.extent.width, vk_context.swapchain_ctx.extent.height);
    VkCommandPoolCreateInfo command_pool_ci =
        command_pool_create_info(vk_context.graphics_present_queue_family, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    vkCreateCommandPool(vk_context.device, &command_pool_ci, nullptr, &vk_context.frame_command_pool);

    vk_context.frames = init_frames(vk_context.device, vk_context.frame_command_pool, vk_context.swapchain_ctx.image_views,
                                    vk_context.swapchain_ctx.images, vk_context.graphics_present_queue_family);

    const VkRect2D render_area = rect_2d_create(vk_context.swapchain_ctx.extent.width, vk_context.swapchain_ctx.extent.height);
    while (!glfwWindowShouldClose(vk_context.window)) {
        glfwPollEvents();
        int width, height;
        glfwGetFramebufferSize(vk_context.window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(vk_context.window, &width, &height);
            glfwWaitEvents();
        }
        const uint32_t           frame_index            = vk_context.curr_frame % 3;
        const Frame*             current_frame          = &vk_context.frames[frame_index];
        std::array               color_attachment_infos = {current_frame->attachment_info};
        const VkRenderingInfoKHR rendering_info         = rendering_info_create(render_area, color_attachment_infos);

        VkCommandBuffer command_buffer = current_frame->command_buffer;

        VK_CHECK(fence_wait(vk_context.device, current_frame->in_flight_fence));
        VK_CHECK(fence_reset(vk_context.device, current_frame->in_flight_fence));

        uint32_t swapchain_image_index;
        swapchain_acquire_next_image(vk_context.device, vk_context.swapchain_ctx.swapchain, &swapchain_image_index,
                                     current_frame->image_available_semaphore);

        VK_CHECK(vkResetCommandBuffer(command_buffer, 0));

        VkCommandBufferBeginInfo begin_info = command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        VK_CHECK(vkBeginCommandBuffer(command_buffer, &begin_info));

        rendering_begin(command_buffer, &rendering_info);

        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk_context.graphics_pipeline.pipeline);

        vkCmdDraw(command_buffer, 3, 1, 0, 0);

        rendering_end(command_buffer);

        pipeline_barrier_2_insert(command_buffer, &current_frame->dependency_info);

        VK_CHECK(vkEndCommandBuffer(command_buffer));

        VK_CHECK(vkQueueSubmit2(vk_context.graphics_queue, 1, &current_frame->submit_info_2, current_frame->in_flight_fence));

        VkPresentInfoKHR present_info =
            present_info_create(&vk_context.swapchain_ctx.swapchain, &swapchain_image_index, &current_frame->render_finished_semaphore);

        queue_present(vk_context.present_queue, &present_info);

        vk_context.curr_frame++;
    }
    destroy_resources(&vk_context);
}