#include "gles2-buf-uniform.hpp"
#ifdef GX_USE_OPENGL_ES2
#include "../../system/sys-log.hpp"
#include "../engine/gles2-eng-engine.hpp"
#include <cstring>

gearoenix::gles2::buffer::Uniform::Uniform(const unsigned int s, const std::shared_ptr<engine::Engine>& e)
    : render::buffer::Uniform(s, e)
    , data(new unsigned char[s])
{
}

gearoenix::gles2::buffer::Uniform::~Uniform()
{
    delete[] data;
    data = nullptr;
}

void gearoenix::gles2::buffer::Uniform::update(const void* src)
{
    std::memcpy(data, src, size);
}

void* gearoenix::gles2::buffer::Uniform::get_data()
{
    return data;
}

#endif