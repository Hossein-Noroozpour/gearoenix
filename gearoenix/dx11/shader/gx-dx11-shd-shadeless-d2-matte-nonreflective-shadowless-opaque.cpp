#include "gx-dx11-shd-shadeless-d2-matte-nonreflective-shadowless-opaque.hpp"
#ifdef USE_DIRECTX11
#include "../../core/gx-cr-static.hpp"
#include "../../system/gx-sys-log.hpp"
#include "../buffer/dx11-buf-uniform.hpp"
#include "../dx11-engine.hpp"
#include "../pipeline/dx11-pip-pipeline.hpp"
#include "../texture/dx11-txt-2d.hpp"
#include "../texture/dx11-txt-sampler.hpp"

gearoenix::dx11::shader::ShadelessD2MatteNonreflectiveShadowlessOpaque::Resources::Resources(Engine* e, pipeline::Pipeline* pip, buffer::Uniform* u)
    : render::material::ShadelessD2MatteNonreflectiveShadowlessOpaque::Resources(e, pip, u)
{
}

void gearoenix::dx11::shader::ShadelessD2MatteNonreflectiveShadowlessOpaque::Resources::bind()
{
    buffer::Uniform* uniform = static_cast<buffer::Uniform*>(u);
    ShadelessD2MatteNonreflectiveShadowlessOpaque* shd = static_cast<ShadelessD2MatteNonreflectiveShadowlessOpaque*>(pip->get_shader());
    uniform->set_for_vertex_shader();
    static_cast<const texture::Texture2D*>(txt2d)->bind(0);
    shd->use();
}

gearoenix::dx11::shader::ShadelessD2MatteNonreflectiveShadowlessOpaque::ShadelessD2MatteNonreflectiveShadowlessOpaque(core::Id my_id, Engine* eng, core::sync::EndCaller<core::sync::EndCallerIgnore> end)
    : Shader(my_id, eng, end)
{
    eng->add_load_function([this, eng, end]() -> void {
        // todo Shader compilation must move to blender part.
        const char p_vertex_src_data[] = "cbuffer UniformBuffer {\n"
                                         "    matrix mvp;\n"
                                         "};\n"
                                         "struct VertexInputType {\n"
                                         "    float3 position : POSITION;\n"
                                         "    float2 texcoord : TEXCOORD;\n"
                                         "};\n"
                                         "struct PixelInputType\n"
                                         "{\n"
                                         "    float4 position : SV_POSITION;\n"
                                         "    float2 texcoord : TEXCOORD;\n"
                                         "};\n"
                                         "PixelInputType main(VertexInputType input) {\n"
                                         "    PixelInputType output;\n"
                                         "    output.position = mul(float4(input.position, 1.0), mvp);\n"
                                         "    output.texcoord = input.texcoord;\n"
                                         "    return output;\n"
                                         "}\n";
        std::vector<D3D11_INPUT_ELEMENT_DESC> desc(2);
        GX_SET_ZERO(desc[0]);
        desc[0].SemanticName = "POSITION";
        desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        GX_SET_ZERO(desc[1]);
        desc[1].SemanticName = "TEXCOORD";
        desc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
        desc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        compile_shader(p_vertex_src_data, render::shader::stage::VERTEX, desc);
        // todo Shader compilation must move to blender part.
        const char p_fragment_src_data[] = "Texture2D txt;\n"
                                           "SamplerState smp;\n"
                                           "struct PixelInputType {\n"
                                           "    float4 position : SV_POSITION;\n"
                                           "    float2 texcoord : TEXCOORD;\n"
                                           "};\n"
                                           "float4 main(PixelInputType input) : SV_TARGET {\n"
                                           "	return txt.Sample(smp, input.texcoord);\n"
                                           "}\n";
        compile_shader(p_fragment_src_data, render::shader::stage::FRAGMENT);
        (void)end;
    });
}

gearoenix::dx11::shader::ShadelessD2MatteNonreflectiveShadowlessOpaque::~ShadelessD2MatteNonreflectiveShadowlessOpaque()
{
}

void gearoenix::dx11::shader::ShadelessD2MatteNonreflectiveShadowlessOpaque::use()
{
    run();
    Engine* engine = static_cast<Engine*>(eng);
    engine->get_sampler()->bind(0);
}

const std::vector<gearoenix::render::shader::stage::Id>& gearoenix::dx11::shader::ShadelessD2MatteNonreflectiveShadowlessOpaque::get_stages_ids() const
{
    return graphic_2_stage;
}
#endif
