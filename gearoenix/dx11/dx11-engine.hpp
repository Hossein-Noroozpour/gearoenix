#ifndef GEAROENIX_DIRECTX11_ENGINE_HPP
#define GEAROENIX_DIRECTX11_ENGINE_HPP
#include "../core/cr-build-configuration.hpp"
#ifdef USE_DIRECTX11
#include "../render/rnd-engine.hpp"
#include <memory>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#ifdef PROFILING_MODE
#include <chrono>
#endif
namespace gearoenix {
	namespace dx11 {
		namespace texture {
			class Texture2D;
		}
		class Engine : public render::Engine {
		private:
			unsigned int graphic_memory_size;
			float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
			IDXGISwapChain* p_swapchain = nullptr;
			ID3D11Device* p_device = nullptr;
			ID3D11DeviceContext* p_immediate_context = nullptr;
			ID3D11RenderTargetView* p_render_target_view = nullptr;
			ID3D11Texture2D* p_depth_stencil_buffer = nullptr;
			ID3D11DepthStencilState* p_depth_stencil_state = nullptr;
			ID3D11DepthStencilView* p_depth_stencil_view = nullptr;
			ID3D11RasterizerState* p_raster_state = nullptr;
#ifdef PROFILING_MODE
			unsigned int prof_frames_count = 0;
			std::chrono::high_resolution_clock::time_point prof_last_time_draw;
			double prof_frames_time = 0.0;
#endif
		public:
			Engine(system::Application* sys_app);
			~Engine();
			static bool is_supported();
			void window_changed();
			void update();
			void terminate();
			render::texture::Texture2D* create_texture_2d(system::File* file, std::shared_ptr<core::EndCaller> c);
			render::texture::Cube* create_texture_cube(system::File* file, std::shared_ptr<core::EndCaller> c);
			render::buffer::Mesh* create_mesh(unsigned int vec, system::File* file, std::shared_ptr<core::EndCaller> c);
			render::buffer::Uniform* create_uniform(unsigned int s, std::shared_ptr<core::EndCaller> c);
			render::shader::Shader* create_shader(core::Id sid, system::File* file, std::shared_ptr<core::EndCaller> c);
			render::shader::Resources* create_shader_resources(core::Id sid, render::pipeline::Pipeline* p, render::buffer::Uniform* ub, std::shared_ptr<core::EndCaller> c);
			render::pipeline::Pipeline* create_pipeline(core::Id sid, std::shared_ptr<core::EndCaller> c);
		};
	}
}
#endif
#endif
