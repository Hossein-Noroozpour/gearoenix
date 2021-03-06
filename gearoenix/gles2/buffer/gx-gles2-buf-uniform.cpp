#include "gx-gles2-buf-uniform.hpp"
#ifdef GX_USE_OPENGL_ES2
#include "../engine/gx-gles2-eng-engine.hpp"
#include <cstring>

const void* gearoenix::gles2::buffer::Uniform::get_data() const noexcept
{
    return data.data();
}

void* gearoenix::gles2::buffer::Uniform::get_data() noexcept
{
    return data.data();
}

void gearoenix::gles2::buffer::Uniform::update(const void* const src) noexcept
{
    std::memcpy(data.data(), src, size);
}

gearoenix::gles2::buffer::Uniform::Uniform(const std::size_t s, const std::size_t frame_number, engine::Engine* const e) noexcept
    : render::buffer::Uniform(s, frame_number, e)
    , data(s)
{
}

gearoenix::gles2::buffer::Uniform::~Uniform() noexcept
{
    data.clear();
}

#endif
