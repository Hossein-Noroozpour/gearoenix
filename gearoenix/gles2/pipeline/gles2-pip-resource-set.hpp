#ifndef GEAROEMIX_GLES2_PIPELINE_RESOURCE_SET_HPP
#define GEAROEMIX_GLES2_PIPELINE_RESOURCE_SET_HPP
#include "../../render/pipeline/rnd-pip-resource-set.hpp"
#include "../../core/sync/cr-sync-end-caller.hpp"
#include "../../gl/gl-types.hpp"
namespace gearoenix {
	namespace gles2 {
		namespace shader {
			class Shader;
		}
    namespace pipeline {
        class ResourceSet : public render::pipeline::ResourceSet {
		protected:
			const std::shared_ptr<shader::Shader> shd;
		public:
			ResourceSet(const std::shared_ptr<shader::Shader> &shd);
			virtual void bind(gl::uint &bound_shader_program) const;
        };
    }
}
}
#endif
