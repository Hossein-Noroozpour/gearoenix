#ifndef GEAROENIX_VULKAN_BUFFER_MANAGER_HPP
#define GEAROENIX_VULKAN_BUFFER_MANAGER_HPP
#include "../../core/gx-cr-build-configuration.hpp"
#ifdef GX_USE_VULKAN
#include "../../core/gx-cr-static.hpp"
#include "../../render/buffer/gx-rnd-buf-manager.hpp"

namespace gearoenix::vulkan::engine {
class Engine;
}

namespace gearoenix::vulkan::image {
class Image;
}

namespace gearoenix::vulkan::memory {
class Manager;
}

namespace gearoenix::vulkan::buffer {
class Buffer;
class Static;
class Manager final : public render::buffer::Manager {
    GX_GET_REFC_PRV(std::shared_ptr<memory::Manager>, memory_manager)
private:
    std::weak_ptr<Manager> self;
    std::vector<std::shared_ptr<Buffer>> per_frame_cpu_root_buffers;
    std::shared_ptr<Buffer> upload_root_buffer;
    std::shared_ptr<Buffer> gpu_root_buffer;

    std::vector<std::tuple<std::shared_ptr<Buffer>, std::shared_ptr<Static>>> copy_buffers;
    std::vector<std::tuple<std::shared_ptr<Buffer>, std::shared_ptr<image::Image>>> copy_images;

    Manager(std::shared_ptr<memory::Manager> memory_manager, engine::Engine* eng) noexcept;

public:
    [[nodiscard]] static std::shared_ptr<Manager> construct(std::shared_ptr<memory::Manager> memory_manager, engine::Engine* eng) noexcept;
    ~Manager() noexcept final;
    [[nodiscard]] std::shared_ptr<render::buffer::Uniform> create_uniform(std::size_t size) noexcept final;
    [[nodiscard]] std::shared_ptr<render::buffer::Static> create_static(const std::vector<math::BasicVertex>& vertices, const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept final;
    [[nodiscard]] std::shared_ptr<render::buffer::Static> create_static(const std::vector<std::uint32_t>& indices, const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept final;
};
}
#endif
#endif
