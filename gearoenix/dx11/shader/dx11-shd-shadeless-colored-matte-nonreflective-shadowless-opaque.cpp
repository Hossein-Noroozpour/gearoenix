#include "dx11-shd-shadeless-colored-matte-nonreflective-shadowless-opaque.hpp"
#ifdef USE_DIRECTX11
#include "../../system/sys-log.hpp"
#include "../buffer/dx11-buf-uniform.hpp"
#include "../dx11-engine.hpp"
#include "../pipeline/dx11-pip-pipeline.hpp"
#include "../../core/cr-static.hpp"

gearoenix::dx11::shader::ShadelessColoredMatteNonreflectiveShadowlessOpaque::Resources::Resources(Engine* e, pipeline::Pipeline* pip, buffer::Uniform* u)
    : render::material::ShadelessColoredMatteNonreflectiveShadowlessOpaque::Resources(e, pip, u)
{
}

void gearoenix::dx11::shader::ShadelessColoredMatteNonreflectiveShadowlessOpaque::Resources::bind()
{
    buffer::Uniform* uniform = reinterpret_cast<buffer::Uniform*>(u);
    ShadelessColoredMatteNonreflectiveShadowlessOpaque* shd = reinterpret_cast<ShadelessColoredMatteNonreflectiveShadowlessOpaque*>(pip->get_shader());
	uniform->set_for_vertex_shader();
	uniform->set_for_fragment_shader();
    shd->use();
}

gearoenix::dx11::shader::ShadelessColoredMatteNonreflectiveShadowlessOpaque::ShadelessColoredMatteNonreflectiveShadowlessOpaque(Engine* eng, std::shared_ptr<core::EndCaller> end)
    : Shader(eng, end)
{
	eng->add_load_function([this, eng, end]() -> void {
		// todo Shader compilation must move to blender part.
		const char p_vertex_src_data[] =
			"cbuffer UniformBuffer {\n"
			"    matrix mvp;\n"
			"    float4 color;\n"
			"};\n"
			"struct VertexInputType {\n"
			"    float3 position : POSITION;\n"
			"};\n"
			"struct PixelInputType\n"
			"{\n"
			"    float4 position : SV_POSITION;\n"
			"};\n"
			"PixelInputType main(VertexInputType input) {\n"
			"    PixelInputType output;\n"
			"    output.position = mul(float4(input.position, 1.0), mvp);\n"
			"    return output;\n"
			"}\n";
		D3D11_INPUT_ELEMENT_DESC desc;
		setz(desc);
		desc.SemanticName = "POSITION";
		desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		compile_shader(p_vertex_src_data, render::shader::stage::VERTEX, { desc });

		// todo Shader compilation must move to blender part.
		const char p_fragment_src_data[] =
			"cbuffer UniformBuffer {\n"
			"    matrix mvp;\n"
			"    float4 color;\n"
			"};\n"
			"struct PixelInputType {\n"
			"    float4 position : SV_POSITION;\n"
			"};\n"
			"float4 main(PixelInputType input) : SV_TARGET {\n"
			"	return color;\n"
			"}\n";
		compile_shader(p_fragment_src_data, render::shader::stage::FRAGMENT);
		(void) end;
	});
}

gearoenix::dx11::shader::ShadelessColoredMatteNonreflectiveShadowlessOpaque::~ShadelessColoredMatteNonreflectiveShadowlessOpaque()
{
}

void gearoenix::dx11::shader::ShadelessColoredMatteNonreflectiveShadowlessOpaque::use()
{
	run();
}

const std::vector<gearoenix::render::shader::stage::Id>& gearoenix::dx11::shader::ShadelessColoredMatteNonreflectiveShadowlessOpaque::get_stages_ids() const
{
    return graphic_2_stage;
}
#endif