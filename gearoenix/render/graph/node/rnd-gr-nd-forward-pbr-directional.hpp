#ifndef GEAROEMIX_RENDER_GRAPH_NODE_FORWARD_PBR_HPP
#define GEAROEMIX_RENDER_GRAPH_NODE_FORWARD_PBR_HPP
#include "rnd-gr-nd-node.hpp"
#include "../../../math/math-matrix.hpp"
#include "../../../math/math-vector.hpp"
#include "../../../core/sync/cr-sync-end-caller.hpp"
#include <vector>
#include <memory>

namespace gearoenix {
	namespace render {
		namespace buffer {
			class Uniform;
			class Manager;
		}
		namespace camera {
			class Camera;
		}
		namespace command {
			class Buffer;
		}
		namespace light {
			class Directional;
		}
		namespace model {
			class Model;
		}
		namespace pipeline {
			class Resource;
			class ResourceSet;
			class Pipeline;
		}
		namespace scene {
			class Scene;
		}
		namespace sync {
			class Semaphore;
		}
		namespace texture {
			class Cube;
			class Texture;
			class Texture2D;
		}
		namespace graph {
			namespace node {
				struct ForwardPbrDirectionalKernel {
					command::Buffer *secondary_cmd = nullptr;
					unsigned int latest_render_data_pool = 0;
					std::vector<std::tuple<buffer::Uniform *, pipeline::ResourceSet *> > render_data_pool;

					ForwardPbrDirectionalKernel(Engine* e, const unsigned int kernel_index);
					~ForwardPbrDirectionalKernel();
				};

				struct ForwardPbrDirectionalFrame {
					command::Buffer *primary_cmd = nullptr;
					sync::Semaphore *semaphore = nullptr;
					pipeline::Resource *pipeline_resource = nullptr;
					bool input_texture_changed = false;
					std::vector<ForwardPbrDirectionalKernel> kernels;

					ForwardPbrDirectionalFrame(Engine* e);
					~ForwardPbrDirectionalFrame();
				};

				struct ForwardPbrDirectionalUniform {
					math::Mat4x4 mvp = math::Mat4x4();
					math::Mat4x4 directional_light_view_projection_biases;
					math::Vec4 direction_light_color;
					math::Vec4 direction_light_direction_count;
				};

				/// This renders only one directional light with one shadow map.
				/// It needs following textures:
				///     0 - diffuse environment
				///     1 - specular environment
				///     2 - ambient occlusion
				///     3 - shadow map
				///     4 - brdflut
				/// In here I do not care for race issues (for performance reason).
				/// The user of this class must use its functionalities in their correct contextes.
				class ForwardPbrDirectional : public Node {
				private:
					std::vector<ForwardPbrDirectionalFrame> frames;

				public:
					ForwardPbrDirectional(Engine* e, core::sync::EndCaller<core::sync::EndCallerIgnore> call);
					~ForwardPbrDirectional();

					void set_diffuse_environment(const std::shared_ptr<texture::Cube>& t);
					void set_specular_environment(const std::shared_ptr<texture::Cube>& t);
					void set_ambient_occlusion(const std::shared_ptr<texture::Texture2D>& t);
					void set_shadow_mapper(const std::shared_ptr<texture::Texture2D>& t);
					void set_brdflut(const std::shared_ptr<texture::Texture2D>& t);

					void set_input_texture(const std::shared_ptr<texture::Texture>& t, const unsigned int index) override;
					/// This will be called at the start of each frame
					void update();
					void record(const scene::Scene& s, const camera::Camera& c, const light::Directional& l, const model::Model& m, const unsigned int kernel_index);
					/// This will be called at the end of each frame for pushing jobs to GPU
					void submit();
				};
			}
		}
	}
}
#endif