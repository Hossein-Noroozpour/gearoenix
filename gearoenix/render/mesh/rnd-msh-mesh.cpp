#include "rnd-msh-mesh.hpp"
#include "../../core/cr-end-caller.hpp"
#include "../../system/sys-file.hpp"
#include "../buffer/rnd-buf-mesh.hpp"
#include "../material/rnd-mat-material.hpp"
#include "../rnd-engine.hpp"

gearoenix::render::mesh::Mesh::Mesh(system::File* f, Engine* e, std::shared_ptr<core::EndCaller> c)
{

    LOGE("location: " << f->tell());
    mat = material::Material::read(f, e, c);
    buf = new buffer::Mesh(mat->get_vertex_elements_count(), f, e, c);
}

gearoenix::render::mesh::Mesh::~Mesh()
{
    delete mat;
    delete buf;
}
