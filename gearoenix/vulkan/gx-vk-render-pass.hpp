#ifndef GEAROENIX_VULKAN_RENDER_PASS_HPP
#define GEAROENIX_VULKAN_RENDER_PASS_HPP
#include "../core/gx-cr-build-configuration.hpp"
#ifdef USE_VULKAN
#include "gx-vk-linker.hpp"
#include <memory>

namespace gearoenix {
namespace render {
    namespace device {
        class Logical;
    }
    class Swapchain;
    class RenderPass {
    private:
        Swapchain* swapchain;
        VkRenderPass vulkan_data;

    public:
        RenderPass(Swapchain* swapchain);
        ~RenderPass();
        const VkRenderPass& get_vulkan_data() const;
    };
}
}
#endif
#endif