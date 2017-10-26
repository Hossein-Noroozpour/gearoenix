#ifndef GEAROENIX_GLES2_PIPELINE_PIPELINE_HPP
#define GEAROENIX_GLES2_PIPELINE_PIPELINE_HPP
#include "../../core/cr-build-configuration.hpp"
#ifdef USE_OPENGL_ES2
#include "../../render/pipeline/rnd-pip-pipeline.hpp"
#include <memory>
namespace gearoenix {
namespace core {
    class EndCaller;
}
namespace gles2 {
    class Engine;
    namespace shader {
        class Shader;
    }
    namespace pipeline {
        class Pipeline : public render::pipeline::Pipeline {
        private:
        public:
            Pipeline(core::Id sid, Engine* eng, std::shared_ptr<core::EndCaller> call);
            ~Pipeline();
            void bind();
        };
    }
}
}
#endif
#endif