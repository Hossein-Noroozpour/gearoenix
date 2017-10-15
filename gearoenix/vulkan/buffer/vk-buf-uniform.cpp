#include "vk-buf-uniform.hpp"
#include "../../system/sys-file.hpp"
#include "../../system/sys-log.hpp"
#include "../command/vk-cmd-buffer.hpp"
#include "../vk-engine.hpp"
#include "vk-buf-manager.hpp"
#include "vk-buf-sub-buffer.hpp"

gearoenix::render::buffer::Uniform::Uniform(unsigned int us, Engine* e)
    : us(us)
    , uc(e->get_frames_count())
    , vbuf(new SubBuffer*[uc])
    , cbuf(new SubBuffer*[uc])
{
    Manager* vbm = e->get_v_buffer_manager();
    Manager* cbm = e->get_cpu_buffer_manager();
    for (unsigned int i = 0; i < uc; ++i) {
        vbuf[i] = vbm->create_subbuffer(us);
        cbuf[i] = cbm->create_subbuffer(us);
    }
}

gearoenix::render::buffer::Uniform::~Uniform()
{
    for (unsigned int i = 0; i < uc; ++i) {
        delete vbuf[i];
        delete cbuf[i];
    }
    delete[] cbuf;
    delete[] vbuf;
}

const gearoenix::render::buffer::SubBuffer* const* gearoenix::render::buffer::Uniform::get_vbuf() const
{
    return vbuf;
}

const gearoenix::render::buffer::SubBuffer* const* gearoenix::render::buffer::Uniform::get_cbuf() const
{
    return cbuf;
}

unsigned int gearoenix::render::buffer::Uniform::get_count() const
{
    return uc;
}

unsigned int gearoenix::render::buffer::Uniform::get_size() const
{
    return us;
}
