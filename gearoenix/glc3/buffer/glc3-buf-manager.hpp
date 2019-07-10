#ifndef GEAROENIX_GLC3_BUFFER_MANAGER_HPP
#define GEAROENIX_GLC3_BUFFER_MANAGER_HPP
#include "../../core/cr-build-configuration.hpp"
#ifdef GX_USE_OPENGL_CLASS_3
#include "../../render/buffer/rnd-buf-manager.hpp"
namespace gearoenix::glc3 {
namespace engine {
    class Engine;
}
namespace buffer {
    class Manager : public render::buffer::Manager {
    public:
        explicit Manager(engine::Engine* e) noexcept;
        ~Manager() noexcept final = default;
        render::buffer::Uniform* create_uniform(unsigned int size) noexcept final;
        render::buffer::Static* create_static(std::vector<math::BasicVertex> vertices, const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept final;
        render::buffer::Static* create_static(std::vector<std::uint32_t> indices, const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept final;
    };
}
}
#endif
#endif