#ifndef GEAROENIX_RENDER_REFLECTION_RUNTIME_HPP
#define GEAROENIX_RENDER_REFLECTION_RUNTIME_HPP
#include "../../core/sync/cr-sync-end-caller.hpp"
#include "rnd-rfl-reflection.hpp"

namespace gearoenix::render::camera {
class Camera;
}

namespace gearoenix::render::texture {
class Target;
}

namespace gearoenix::render::reflection {
class Runtime final : public Reflection {
    GX_GET_CREF_PRV(std::shared_ptr<texture::TextureCube const>, environment)
    GX_GET_ARRC_PRV(std::shared_ptr<camera::Camera>, cameras, 6)
    GX_GET_ARRC_PRV(std::shared_ptr<texture::Target>, targets, 6)
public:
    Runtime(core::Id id, engine::Engine* e, const core::sync::EndCaller<core::sync::EndCallerIgnore>& end) noexcept;
    ~Runtime() noexcept final;
};
}
#endif
