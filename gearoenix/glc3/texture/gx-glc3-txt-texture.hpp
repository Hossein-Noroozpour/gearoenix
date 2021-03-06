#ifndef GEAROENIX_GLC3_TEXTURE_HPP
#define GEAROENIX_GLC3_TEXTURE_HPP
#include "../../core/gx-cr-build-configuration.hpp"
#ifdef GX_USE_OPENGL_CLASS_3
#include "../../gl/gx-gl-types.hpp"

namespace gearoenix::render::texture {
class Texture;
}

namespace gearoenix::glc3::texture {
class Texture {
public:
    Texture() = delete;
    static void bind(const render::texture::Texture* t, gl::enumerated texture_unit) noexcept;
    static void generate_mipmap(const render::texture::Texture* t) noexcept;
};
}

#endif
#endif
