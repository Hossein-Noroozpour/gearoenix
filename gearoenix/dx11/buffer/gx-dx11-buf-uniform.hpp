#ifndef GEAROENIX_DX11_BUFFER_UNIFORM_HPP
#define GEAROENIX_DX11_BUFFER_UNIFORM_HPP
#include "../../core/gx-cr-build-configuration.hpp"
#ifdef GX_USE_DIRECTX11
#include "../../core/sync/gx-cr-sync-end-caller.hpp"
#include "../../render/buffer/gx-rnd-buf-uniform.hpp"
#include <d3d11.h>
namespace gearoenix {
namespace dx11 {
    class Engine;
    namespace buffer {
        class Uniform : public render::buffer::Uniform {
        private:
            ID3D11Buffer* ub = nullptr;

        public:
            Uniform(unsigned int s, Engine*, core::sync::EndCaller<core::sync::EndCallerIgnore> c);
            ~Uniform();
            void update(const void* src, unsigned int data_size);
            void* get_data();
            void set_for_vertex_shader();
            void set_for_fragment_shader();
        };
    } // namespace buffer
} // namespace dx11
} // namespace gearoenix
#endif
#endif // GEAROENIX_DX11_BUFFER_UNIFORM_HPP
