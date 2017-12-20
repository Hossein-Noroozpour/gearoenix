#ifndef GEAROENIX_DX11_TEXTURE_2D_HPP
#define GEAROENIX_DX11_TEXTURE_2D_HPP
#include "../../core/cr-build-configuration.hpp"
#ifdef USE_DIRECTX11
#include "../../render/texture/rnd-txt-texture-2d.hpp"
#include <d3d11.h>
#include <memory>
namespace gearoenix {
namespace core {
    class EndCaller;
}
namespace system {
    class File;
}
namespace dx11 {
    class Engine;
    namespace texture {
        class Texture2D : public render::texture::Texture2D {
        private:
			ID3D11ShaderResourceView* srv = nullptr;

        public:
            Texture2D(system::File* file, Engine* engine, std::shared_ptr<core::EndCaller> end);
            ~Texture2D();
			const ID3D11ShaderResourceView* get_shader_resource_view();
        };
    }
}
}
#endif
#endif