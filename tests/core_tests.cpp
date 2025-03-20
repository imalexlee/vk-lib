#include <gtest/gtest.h>
#include <vk_lib/core.h>

class CoreTestsFixture : public testing::Test {
public:
    CoreTestsFixture(): basic_instance(nullptr), api_version(VK_API_VERSION_1_0) {
        // set latest Vulkan api version for the machine running these tests
        const auto vkEnumerateInstanceVersion =
            reinterpret_cast<PFN_vkEnumerateInstanceVersion>(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));
        if (vkEnumerateInstanceVersion) {
            vkEnumerateInstanceVersion(&api_version);
        }

        InstanceBuilder instance_builder;
        instance_builder_set_names(&instance_builder, "app name", "engine name");
        instance_builder_set_versions(&instance_builder, api_version, VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0));
        instance_builder_instance_create(&instance_builder, &basic_instance);
    }

    ~CoreTestsFixture() override {
        instance_destroy(basic_instance);
    }

protected:
    uint32_t api_version;
    VkInstance basic_instance;
};

TEST_F(CoreTestsFixture, createBasicInstance) {
    ASSERT_NE(basic_instance, nullptr);
}

TEST_F(CoreTestsFixture, createDebugInstance) {
    bool validation_layer_available   = false;
    const char* validation_layer_name = "VK_LAYER_KHRONOS_validation";
    std::vector<VkLayerProperties> available_layers;
    instance_enumerate_layer_properties(&available_layers);

    for (const auto& layer : available_layers) {
        if (strcmp(layer.layerName, validation_layer_name) == 0) {
            validation_layer_available = true;
            break;
        }
    }

    if (!validation_layer_available) {
        GTEST_SKIP() << "Validation layer not available, skipping test";
    }

    // Now run the test knowing the layer is available
    InstanceBuilder builder{};
    instance_builder_set_names(&builder, "Test App", "Test Engine");
    instance_builder_set_versions(&builder, api_version, VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0));

    const char* layers[] = {validation_layer_name};
    instance_builder_set_layers(&builder, layers);

    VkInstance debug_instance = nullptr;
    VkResult result           = instance_builder_instance_create(&builder, &debug_instance);

    EXPECT_EQ(result, VK_SUCCESS);
    EXPECT_NE(debug_instance, nullptr);
}

TEST_F(CoreTestsFixture, createWrongDebugInstance) {
    bool validation_layer_available = false;
    // misspelled
    const char* validation_layer_name = "VK_LYER_KHRONS_vali";

    InstanceBuilder builder{};
    instance_builder_set_names(&builder, "Test App", "Test Engine");
    instance_builder_set_versions(&builder, api_version, VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0));

    const char* layers[] = {validation_layer_name};
    instance_builder_set_layers(&builder, layers);

    VkInstance debug_instance = nullptr;
    VkResult result           = instance_builder_instance_create(&builder, &debug_instance);

    EXPECT_EQ(result, VK_ERROR_LAYER_NOT_PRESENT);
    EXPECT_EQ(debug_instance, nullptr);
}