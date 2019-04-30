#ifndef GEAROEMIX_VULKAN_TEXTURE_CUBE_HPP
#define GEAROEMIX_VULKAN_TEXTURE_CUBE_HPP
#include "../../core/cr-build-configuration.hpp"
#ifdef USE_VULKAN
#include "../../render/texture/rnd-txt-texture.hpp"
namespace gearoenix {
namespace system {
    class File;
}
namespace render {
    class Engine;
    namespace texture {
        class TextureCube : public Texture {
        public:
            TextureCube(system::stream::Stream* file, Engine* engine);
        };
    }
}
}
#endif
#endif