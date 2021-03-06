#ifndef GEAROENIX_GLES2_COMMAND_BUFFER_HPP
#define GEAROENIX_GLES2_COMMAND_BUFFER_HPP
#include "../../core/gx-cr-build-configuration.hpp"
#ifdef GX_USE_OPENGL_ES2
#include "../../gl/gx-gl-types.hpp"
#include "../../render/command/gx-rnd-cmd-buffer.hpp"

namespace gearoenix::gles2::texture {
class Target;
}

namespace gearoenix::gles2::command {
class Buffer final : public render::command::Buffer {
public:
    explicit Buffer(render::command::Type command_buffer_type) noexcept;
    ~Buffer() noexcept final = default;
    [[nodiscard]] std::pair<gl::uint, const texture::Target*> play(
        const texture::Target* bound_target,
        gl::uint bound_shader_program) const noexcept;
};
}
#endif
#endif