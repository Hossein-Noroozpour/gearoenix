//#ifndef GEAROENIX_GLES2_SHADER_DIRECTIONAL_COLORED_SPECULATED_NONREFLECTIVE_SHADOWLESS_OPAQUE_HPP
//#define GEAROENIX_GLES2_SHADER_DIRECTIONAL_COLORED_SPECULATED_NONREFLECTIVE_SHADOWLESS_OPAQUE_HPP
//#include "../../core/gx-cr-build-configuration.hpp"
//#ifdef USE_DIRECTX11
//#include "../../render/material/gx-rnd-mat-directional-colored-speculated-nonreflective-shadowless-opaque.hpp"
//#include "../gx-gles2.hpp"
//#include "gx-gles2-shader.hpp"
//namespace gearoenix {
//namespace gles2 {
//    namespace buffer {
//        class Uniform;
//    }
//    namespace pipeline {
//        class Pipeline;
//    }
//    namespace shader {
//        class DirectionalColoredSpeculatedNonreflectiveShadowlessOpaque : public Shader {
//        public:
//            class Resources : public render::material::DirectionalColoredSpeculatedNonreflectiveShadowlessOpaque::Resources {
//            public:
//                Resources(Engine* e, pipeline::Pipeline* pip, buffer::Uniform* u);
//                void bind();
//            };
//
//        protected:
//            GLuint vp;
//            GLuint m;
//            GLuint sun;
//            GLuint sun_color;
//            GLuint eye;
//            GLuint spec_color;
//            GLuint spec_factors;
//            GLuint ambl_color;
//            GLuint vtx_shd;
//            GLuint frg_shd;
//            GLint vtx_att_ind;
//            GLint nrm_att_ind;
//
//        public:
//            DirectionalColoredSpeculatedNonreflectiveShadowlessOpaque(Engine* eng, std::shared_ptr<core::sync::EndCaller> end);
//            ~DirectionalColoredSpeculatedNonreflectiveShadowlessOpaque();
//            void use();
//            const std::vector<render::shader::stage::Id>& get_stages_ids() const;
//            void set_vp(const GLfloat* data);
//            void set_m(const GLfloat* data);
//            void set_sun(const GLfloat* data);
//            void set_sun_color(const GLfloat* data);
//            void set_eye(const GLfloat* data);
//            void set_spec_color(const GLfloat* data);
//            void set_spec_factors(const GLfloat* data);
//            void set_ambl_color(const GLfloat* data);
//        };
//    }
//}
//}
//#endif
//#endif
