#ifndef GEAROEMIX_GLES2_PIPELINE_FORWARD_PBR_DIRECTIONAL_SHADOW_RESOURCE_SET_HPP
#define GEAROEMIX_GLES2_PIPELINE_FORWARD_PBR_DIRECTIONAL_SHADOW_RESOURCE_SET_HPP
#include "gles2-pip-resource-set.hpp"
#include "../../core/sync/cr-sync-end-caller.hpp"
namespace gearoenix {
	namespace gles2 {
		namespace shader {
			class ForwardPbrDirectionalShadow;
		}
    namespace pipeline {
        class ForwardPbrDirectionalShadowResourceSet : public ResourceSet {
		public:
			ForwardPbrDirectionalShadowResourceSet(const std::shared_ptr<shader::ForwardPbrDirectionalShadow> &shd);
			//void set_scene(const std::shared_ptr<render::scene::Scene>& s) override final;
			//void set_camera(const std::shared_ptr<render::camera::Camera>& c) override final;
			//void set_light(const std::shared_ptr<render::light::Light>& l) override final;
			//void set_model(const std::shared_ptr<render::model::Model>& m) override final;
			//void set_mesh(const std::shared_ptr<render::mesh::Mesh>& m) override final;
			//void set_material(const std::shared_ptr<render::material::Material>& m) override final;
			//void set_node(const render::graph::node::Node *const node) override final;
        };
    }
}
}
#endif