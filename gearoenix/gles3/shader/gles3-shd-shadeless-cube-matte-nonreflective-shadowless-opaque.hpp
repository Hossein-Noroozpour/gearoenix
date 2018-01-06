#ifndef GEAROENIX_gles3_SHADER_SHADELESS_CUBE_MATTE_NONREFLECTIVE_SHADOWLESS_OPAQUE_HPP
#define GEAROENIX_gles3_SHADER_SHADELESS_CUBE_MATTE_NONREFLECTIVE_SHADOWLESS_OPAQUE_HPP
#include "../../core/cr-build-configuration.hpp"
#ifdef USE_OPENGL_ES3
#include "../../render/material/rnd-mat-shadeless-cube-matte-nonreflective-shadowless-opaque.hpp"
#include "../gles3.hpp"
#include "gles3-shader.hpp"
namespace gearoenix {
namespace gles3 {
    namespace buffer {
        class Uniform;
    }
    namespace pipeline {
        class Pipeline;
    }
    namespace shader {
        class ShadelessCubeMatteNonreflectiveShadowlessOpaque : public Shader {
        public:
            class Resources : public render::material::ShadelessCubeMatteNonreflectiveShadowlessOpaque::Resources {
            public:
                Resources(Engine* e, pipeline::Pipeline* pip, buffer::Uniform* u);
                void bind();
            };

        protected:
            GLuint mvp;
            GLuint m;
            GLuint ctxt;
            GLuint vtx_shd;
            GLuint frg_shd;
            GLint vtx_att_ind;

        public:
            ShadelessCubeMatteNonreflectiveShadowlessOpaque(Engine* eng, core::EndCaller<core::EndCallerIgnore> end);
            ~ShadelessCubeMatteNonreflectiveShadowlessOpaque();
            void use();
            const std::vector<render::shader::stage::Id>& get_stages_ids() const;
            void set_mvp(const GLfloat* data);
            void set_m(const GLfloat* data);
        };
    }
}
}
#endif
#endif