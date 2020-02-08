#ifndef GEAROENIX_GLES2_TEXTURE_TARGET_2D_HPP
#define GEAROENIX_GLES2_TEXTURE_TARGET_2D_HPP
#include "../../core/cr-build-configuration.hpp"
#ifdef GX_USE_OPENGL_ES2
#include "../../core/sync/cr-sync-end-caller.hpp"
#include "../../gl/gl-types.hpp"
#include "../../render/texture/rnd-txt-info.hpp"
#include "../../render/texture/rnd-txt-target-2d.hpp"
#include <vector>

namespace gearoenix::gles2::engine {
class Engine;
}

namespace gearoenix::gles2::texture {
class Target2D : public render::texture::Target2D {
private:
    std::vector<gl::uint> texture_objects;
    gl::sint framebuffer = -1;
    gl::sint depth_buffer = -1;

    void state_init() const noexcept;

    Target2D(core::Id my_id, engine::Engine* e) noexcept;

public:
    explicit Target2D(engine::Engine* e) noexcept;
    [[nodiscard]] static std::shared_ptr<Target2D> construct(
        core::Id my_id,
        engine::Engine* e,
        const std::vector<render::texture::Info>& infos,
        unsigned int width,
        unsigned int height,
        const core::sync::EndCaller<core::sync::EndCallerIgnore>& call) noexcept;
    ~Target2D() noexcept final;
    void bind() const noexcept;
    void bind_textures(const std::vector<gl::enumerated>& texture_units) const noexcept;
};
}

#endif
#endif