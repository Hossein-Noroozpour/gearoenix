#include "rnd-mdl-static.hpp"
#include "../../core/cr-end-caller.hpp"
#include "../../system/sys-file.hpp"

gearoenix::render::model::Static::Static(system::File* f, Engine* e, std::shared_ptr<core::EndCaller> c)
    : mesh(new Mesh(f, e, c))
{
    core::Count cc;
    f->read(cc);
    children.resize(cc);
    for (core::Count i = 0; i < cc; ++i)
        children[i] = Model::read_child(f, e, c);
}
