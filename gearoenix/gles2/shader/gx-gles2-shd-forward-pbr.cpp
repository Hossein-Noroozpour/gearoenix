#include "gx-gles2-shd-forward-pbr.hpp"
#ifdef GX_USE_OPENGL_ES2
#include "../../core/gx-cr-function-loader.hpp"
#include "../engine/gx-gles2-eng-engine.hpp"

#define GX_GLES2_SHADER_SRC_EFFECT_UNIFORMS                                                                                              \
    "uniform vec4  effect_point_lights_color_min_radius[" GX_MAX_POINT_LIGHTS_STR "];\n"                                                 \
    "uniform vec4  effect_point_lights_position_max_radius[" GX_MAX_POINT_LIGHTS_STR "];\n"                                              \
    "uniform mat4  effect_shadow_caster_directional_lights_cascades_view_projection_bias[" GX_MAX_DIRECTIONAL_LIGHTS_CASCADES_STR "];\n" \
    "uniform vec4  effect_shadow_caster_directional_lights_color_cascades_count[" GX_MAX_DIRECTIONAL_LIGHTS_SHADOW_CASTER_STR "];\n"     \
    "uniform vec4  effect_shadow_caster_directional_lights_direction[" GX_MAX_DIRECTIONAL_LIGHTS_SHADOW_CASTER_STR "];\n"                \
    "uniform float effect_point_lights_count;\n"                                                                                         \
    "uniform float effect_shadow_caster_directional_lights_count;\n"

#define GX_GLES2_SHADER_FORWARD_PBR_VARYING \
    "varying vec3 out_pos;\n"               \
    "varying vec3 out_nrm;\n"               \
    "varying vec3 out_tng;\n"               \
    "varying vec3 out_btg;\n"               \
    "varying vec2 out_uv;\n"                \
    "varying vec3 out_directional_lights_cascades_projected[" GX_MAX_DIRECTIONAL_LIGHTS_CASCADES_STR "];\n"

gearoenix::gles2::shader::ForwardPbr::ForwardPbr(engine::Engine* const e, const core::sync::EndCaller<core::sync::EndCallerIgnore>& c) noexcept
    : Shader(e, c)
{
    e->get_function_loader()->load([this] {
        const std::string vertex_shader_code = GX_GLES2_SHADER_SRC_DEFAULT_VERTEX_STARTING
            "uniform mat4  camera_vp;\n" GX_GLES2_SHADER_SRC_EFFECT_UNIFORMS
            "uniform mat4  model_m;\n" GX_GLES2_SHADER_FORWARD_PBR_VARYING
            "void main()\n"
            "{\n"
            "    vec4 pos = model_m * vec4(position, 1.0);\n"
            "    out_pos = pos.xyz;\n"
            "    out_nrm = normalize((model_m * vec4(normal, 0.0)).xyz);\n"
            "    out_tng = normalize((model_m * vec4(tangent.xyz, 0.0)).xyz);\n"
            "    out_btg = cross(out_nrm, out_tng) * tangent.w;\n"
            "    out_uv = uv;\n"
            "    int effect_shadow_caster_directional_lights_count_int = int(effect_shadow_caster_directional_lights_count);\n"
            "    int i = 0;\n"
            "    for(int dir_i = 0; dir_i < " GX_MAX_DIRECTIONAL_LIGHTS_SHADOW_CASTER_STR "; ++dir_i) {\n"
            "        if(dir_i < effect_shadow_caster_directional_lights_count_int) {\n" // This is because of poor devices
            "            int effect_shadow_caster_directional_lights_cascades_count_int = int(effect_shadow_caster_directional_lights_color_cascades_count[dir_i].w);\n"
            "            int diff_ccc_cc = " GX_MAX_SHADOW_CASCADES_STR " - effect_shadow_caster_directional_lights_cascades_count_int;\n"
            "            for(int j = 0; j < " GX_MAX_SHADOW_CASCADES_STR "; ++j) {\n"
            "                if(j < effect_shadow_caster_directional_lights_cascades_count_int) {\n"
#if GX_MAX_DIRECTIONAL_LIGHTS_CASCADES == 1
#define GX_SHADOW_INDEX "0"
#else
#define GX_SHADOW_INDEX "i"
#endif
            "                    vec4 light_pos = effect_shadow_caster_directional_lights_cascades_view_projection_bias[" GX_SHADOW_INDEX "] * pos;\n"
            "                    light_pos.xyz /= light_pos.w;\n"
            "                    light_pos.z *= 0.5;\n"
            "                    light_pos.z += 0.5;\n"
#if GX_MAX_DIRECTIONAL_LIGHTS_CASCADES == 1
#define GX_DIRECTIONAL_LIGHT_CASCADE_INDEX "0"
#else
#define GX_DIRECTIONAL_LIGHT_CASCADE_INDEX "i"
#endif
            "                    out_directional_lights_cascades_projected[" GX_DIRECTIONAL_LIGHT_CASCADE_INDEX "] = light_pos.xyz;\n"
            "                }\n"
            "                ++i;\n"
            "            }\n"
            "            i += diff_ccc_cc;\n"
            "        }\n"
            "    }\n"
            "    gl_Position = camera_vp * pos;\n"
            "}";

        const std::string fragment_shader_code = GX_GLES2_SHADER_SRC_DEFAULT_FRAGMENT_STARTING
            "uniform float       material_alpha;\n"
            "uniform float       material_alpha_cutoff;\n"
            "uniform vec3        material_emission_factor;\n"
            "uniform float       material_metallic_factor;\n"
            "uniform float       material_normal_scale;\n"
            "uniform float       material_occlusion_strength;\n"
            "uniform float       material_roughness_factor;\n"
            "uniform sampler2D   material_base_color;\n"
            "uniform sampler2D   material_metallic_roughness;\n"
            "uniform sampler2D   material_normal;\n"
            "uniform sampler2D   material_emissive;\n"
            // scenes uniform(s)
            "uniform vec3        scene_ambient_light;\n"
            "uniform vec4        scene_directional_lights_color[" GX_MAX_DIRECTIONAL_LIGHTS_STR "];\n"
            "uniform vec4        scene_directional_lights_direction[" GX_MAX_DIRECTIONAL_LIGHTS_STR "];\n"
            "uniform float       scene_directional_lights_count;\n"
            "uniform float       scene_ssao_samples_count;\n"
            "uniform float       scene_ssao_radius;\n"
            "uniform float       scene_ssao_z_tolerance;\n"
            "uniform samplerCube effect_diffuse_environment;\n"
            "uniform samplerCube effect_specular_environment;\n"
            "uniform sampler2D   effect_ambient_occlusion;\n"
            "uniform sampler2D   effect_shadow_caster_directional_lights_cascades_shadow_map[" GX_MAX_DIRECTIONAL_LIGHTS_CASCADES_STR "];\n"
            "uniform sampler2D   effect_brdflut;\n" GX_GLES2_SHADER_SRC_EFFECT_UNIFORMS
            // camera uniform(s)
            "uniform vec3        camera_position;\n"
            "uniform float       camera_hdr_tune_mapping;\n"
            "uniform float       camera_gamma_correction;\n" GX_GLES2_SHADER_FORWARD_PBR_VARYING
            // Normal Distribution Function Trowbridge-Reitz GGX
            "float distribution_ggx(const vec3 normal, const vec3 halfway, const float roughness_p2, const float roughness_p2_minus_1) {\n"
            "    float denom = max(dot(normal, halfway), 0.0);\n"
            "    denom *= denom;\n"
            "    denom *= roughness_p2_minus_1;\n"
            "    denom += 1.0;\n"
            "    denom *= denom;\n"
            "    denom *= GX_PI;\n"
            "    return roughness_p2 / denom;\n"
            "}\n"
            "float geometry_schlick_ggx(const float nd, const float geometry_schlick_k, const float geometry_schlick_k_inv) {\n"
            "    return 1.0 / (nd * geometry_schlick_k_inv + geometry_schlick_k);\n"
            "}\n"
            "float geometry_smith(const float normal_dot_light, const float normal_dot_view, const float geometry_schlick_k, const float geometry_schlick_k_inv) {\n"
            "    float ggx2 = geometry_schlick_ggx(normal_dot_view, geometry_schlick_k, geometry_schlick_k_inv);\n"
            "    float ggx1 = geometry_schlick_ggx(normal_dot_light, geometry_schlick_k, geometry_schlick_k_inv);\n"
            "    return ggx1 * ggx2;\n"
            "}\n"
            // f0_inv = 1.0 - f0
            "vec3 fresnel_schlick(const float nv, const vec3 f0, const vec3 f0_inv) {\n"
            "    float inv = 1.0 - nv;\n"
            "    float inv2 = inv * inv;\n"
            "    float inv4 = inv2 * inv2;\n"
            "    float inv5 = inv4 * inv;\n"
            "    return f0 + (f0_inv * inv5);\n"
            "}\n"
            // cos_theta is (n dot v)
            "vec3 fresnel_schlick_roughness(const float nv, const vec3 f0, const float roughness)\n"
            "{\n"
            "    float inv = 1.0 - nv;\n"
            "    float inv2 = inv * inv;\n"
            "    float inv4 = inv2 * inv2;\n"
            "    float inv5 = inv4 * inv;\n"
            "    return f0 + ((max(vec3(1.0 - roughness), f0) - f0) * inv5);\n"
            "}\n"
            "vec3 compute_light(\n"
            "    const vec3 light_direction, const vec3 light_color, const vec3 view, const vec3 normal,\n"
            "    const float normal_dot_view, const float roughness_p2, const float roughness_p2_minus_1,\n"
            "    const float geometry_schlick_k, const float geometry_schlick_k_inv, const vec3 f0,\n"
            "    const vec3 f0_inv, const float one_minus_metallic, const vec3 albedo) {\n"
            "    float normal_dot_light = dot(normal, light_direction);\n"
            "    if(normal_dot_light <= 0.0) return vec3(0.0);\n"
            "    vec3 half_vec = normalize(view + light_direction);\n"
            //   Cook-Torrance BRDF
            "    float ndf = distribution_ggx(normal, half_vec, roughness_p2, roughness_p2_minus_1);\n"
            "    float geo = geometry_smith(normal_dot_light, normal_dot_view, geometry_schlick_k, geometry_schlick_k_inv);\n"
            "    vec3 fresnel = fresnel_schlick(dot(half_vec, view), f0, f0_inv);\n"
            "    vec3 specular = ndf * geo * fresnel * 0.25;\n"
            //   kS is equal to Fresnel
            "    vec3 ks = fresnel;\n"
            //   for energy conservation, the irradiance and radiance light can't
            //   be above 1.0 (unless the surface emits light); to preserve this
            //   relationship the irradiance component (kD) should equal 1.0 - kS.
            //   multiply kD by the inverse metal-ness such that only non-metals
            //   have irradiance lighting, or a linear blend if partly metal (pure metals
            //   have no irradiance light).
            "    vec3 kd = (vec3(1.0) - ks) * one_minus_metallic;\n"
            //   scale light by normal_dot_light
            //   add to outgoing radiance Lo
            //   note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
            "    return (kd * albedo * ( 1.0 / GX_PI) + specular) * light_color * normal_dot_light;\n"
            "}\n"
            "void main() {\n"
            //   material properties
            "    vec4 tmp_v4 = texture2D(material_base_color, out_uv);\n"
            "    vec3 emission = texture2D(material_emissive, out_uv).xyz * material_emission_factor;\n"
            "    tmp_v4.w *= material_alpha;\n"
            "    vec4 albedo = tmp_v4;\n"
            "    if(albedo.w < material_alpha_cutoff) discard;\n"
            "	 tmp_v4.xy = texture2D(material_metallic_roughness, out_uv).xy;\n"
            "    tmp_v4.xy *= vec2(material_metallic_factor, material_roughness_factor);\n"
            "    float metallic = tmp_v4.x;\n"
            "    float roughness = tmp_v4.y;\n"
            "    float roughness_p2 = roughness * roughness;\n"
            "    float roughness_p2_minus_1 = roughness_p2 - 1.0;\n"
            "    float geometry_schlick_k = roughness + 1.0;\n"
            "    geometry_schlick_k *= geometry_schlick_k;\n"
            "    geometry_schlick_k *= 0.125;\n"
            "    float geometry_schlick_k_inv = 1.0 - geometry_schlick_k;\n"
            "    float one_minus_metallic = 1.0 - metallic;\n"
            //   TODO: in future maybe I will add ao in here
            //   input lighting data
            "    vec3 normal = mat3(out_tng, out_btg, out_nrm) * ((texture2D(material_normal, out_uv).xyz - 0.5) * 2.0 * material_normal_scale);\n"
            "    vec3 view = normalize(camera_position - out_pos);\n"
            "    vec3 reflection = reflect(-view, normal);\n"
            "    float normal_dot_view = max(dot(normal, view), 0.0);\n"
            //   calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
            //   of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
            //   TODO: in future I must bring builder fresnel factor 0 (the hard coded 0.4) from material uniform data
            "    vec3 f0 = mix(vec3(0.04), albedo.xyz, metallic);\n"
            "    vec3 f0_inv = vec3(1.0) - f0;\n"
            //   reflectance equation
            "    vec3 lo = vec3(0.0);\n"
            "    int effect_point_lights_count_int = int(effect_point_lights_count);\n"
            //   computing point lights
            "    for(int i = 0; i < " GX_MAX_POINT_LIGHTS_STR "; ++i)\n"
            "    {\n"
            "        if(i < effect_point_lights_count_int) {\n"
            //           calculate per-light radiance
            "            vec3 light_vec = effect_point_lights_position_max_radius[i].xyz - out_pos;\n"
            //           TODO: in future consider max and min radius (in physics)
            "            float distance = length(light_vec);\n"
            "            float distance_inv = 1.0 / distance;\n"
            "            vec3 light_direction = light_vec * distance_inv;\n"
            "            float attenuation = distance_inv * distance_inv;\n"
            "            vec3 radiance = effect_point_lights_color_min_radius[i].xyz * attenuation;\n"
            "            lo += compute_light(light_direction, radiance, view, normal, normal_dot_view, roughness_p2,\n"
            "                roughness_p2_minus_1, geometry_schlick_k, geometry_schlick_k_inv, f0, f0_inv,\n"
            "                one_minus_metallic, albedo.xyz);\n"
            "        }\n"
            "    }\n"
            "    int scene_directional_lights_count_int = int(scene_directional_lights_count);\n"
            //   computing directional lights
            "    for(int i = 0; i < " GX_MAX_DIRECTIONAL_LIGHTS_STR "; ++i) {\n"
            "        if(i < scene_directional_lights_count_int) {\n"
            "            lo += compute_light(\n"
            "                -scene_directional_lights_direction[i].xyz,\n"
            "                scene_directional_lights_color[i].xyz,\n"
            "                view, normal, normal_dot_view, roughness_p2, roughness_p2_minus_1, geometry_schlick_k,\n"
            "                geometry_schlick_k_inv, f0, f0_inv, one_minus_metallic, albedo.xyz);\n"
            "        }\n"
            "    }\n"
            "    int effect_shadow_caster_directional_lights_count_int = int(effect_shadow_caster_directional_lights_count);\n"
            "    int l_cas_i = 0;\n"
            "    for(int dir_i = 0; dir_i < " GX_MAX_DIRECTIONAL_LIGHTS_SHADOW_CASTER_STR "; ++dir_i) {\n"
            "        if(dir_i < effect_shadow_caster_directional_lights_count_int) {\n"
            "            bool is_in_directional_light = true;\n"
            "            float normal_dot_light = max(dot(out_nrm, -effect_shadow_caster_directional_lights_direction[dir_i].xyz), 0.0);\n"
            "            float shadow_bias = 0.001;\n"
            "            if(normal_dot_light > 0.0) {\n"
            "                shadow_bias = clamp(sqrt((0.000025 / (normal_dot_light * normal_dot_light)) - 0.000025), 0.001, 0.02);\n"
            "            } else {\n"
            "                is_in_directional_light = false;"
            "            }\n"
            "            if(is_in_directional_light) {\n"
            "                int cascades_count_int = int(effect_shadow_caster_directional_lights_color_cascades_count[dir_i].w);\n"
            "                for(int i = 0; i < " GX_MAX_SHADOW_CASCADES_STR "; ++i) {\n"
            "                    if(i < cascades_count_int) {\n"
            "                        vec3 light_uv = out_directional_lights_cascades_projected[i];\n"
            "                        if (light_uv.x > 0.0 && light_uv.x < 1.0 && light_uv.y > 0.0 && light_uv.y < 1.0) {\n"
            "                            vec2 depth_vec = texture2D(effect_shadow_caster_directional_lights_cascades_shadow_map[i], light_uv.xy).xy;\n"
            "                            float depth = depth_vec.y * 0.00390625 + depth_vec.x;\n"
            "                            if(depth + shadow_bias <= light_uv.z) {\n"
            "                                is_in_directional_light = false;\n"
            "                            }\n"
            "                            break;\n"
            "                        }\n"
            "                    }\n"
            "                }\n"
            "            }\n"
            "            if(is_in_directional_light) {\n"
            "                lo += compute_light(-effect_shadow_caster_directional_lights_direction[dir_i].xyz,\n"
            "                    effect_shadow_caster_directional_lights_color_cascades_count[dir_i].xyz,\n"
            "                    view, normal, normal_dot_view, roughness_p2, roughness_p2_minus_1, geometry_schlick_k,\n"
            "                    geometry_schlick_k_inv, f0, f0_inv, one_minus_metallic, albedo.xyz);\n"
            "            }\n"
            "        }\n"
            "        l_cas_i = dir_i * " GX_MAX_SHADOW_CASCADES_STR ";\n"
            "    }\n"
            "    vec3 ambient = scene_ambient_light * albedo.xyz;\n"
            "    tmp_v4.xyz = ambient + lo + emission;\n"
            "    if(camera_gamma_correction > 0.001) {\n"
            //       HDR tone mapping
            "        tmp_v4.xyz = tmp_v4.xyz / (tmp_v4.xyz + vec3(camera_hdr_tune_mapping));\n"
            //       gamma correct
            "        tmp_v4.xyz = pow(tmp_v4.xyz, vec3(1.0 / camera_gamma_correction));\n"
            "    }\n"
            "    gl_FragColor = vec4(tmp_v4.xyz, albedo.w);\n"
            "}";

        set_vertex_shader(vertex_shader_code);
        set_fragment_shader(fragment_shader_code);
        link();
        GX_GLES2_SHADER_SET_TEXTURE_INDEX_STARTING
        GX_GLES2_THIS_GET_UNIFORM(material_alpha)
        GX_GLES2_THIS_GET_UNIFORM(material_alpha_cutoff)
        GX_GLES2_THIS_GET_UNIFORM_TEXTURE(material_base_color)
        GX_GLES2_THIS_GET_UNIFORM(material_emission_factor)
        GX_GLES2_THIS_GET_UNIFORM_TEXTURE(material_emissive)
        GX_GLES2_THIS_GET_UNIFORM(material_metallic_factor)
        GX_GLES2_THIS_GET_UNIFORM_TEXTURE(material_metallic_roughness)
        GX_GLES2_THIS_GET_UNIFORM_TEXTURE(material_normal)
        GX_GLES2_THIS_GET_UNIFORM(material_normal_scale)
        //GX_GLES2_THIS_GET_UNIFORM(material_occlusion_strength)
        GX_GLES2_THIS_GET_UNIFORM(material_roughness_factor)
        GX_GLES2_THIS_GET_UNIFORM(camera_position)
        GX_GLES2_THIS_GET_UNIFORM(camera_vp)
        GX_GLES2_THIS_GET_UNIFORM(camera_hdr_tune_mapping)
        GX_GLES2_THIS_GET_UNIFORM(camera_gamma_correction)
        // TODO
        //GX_GLES2_THIS_GET_UNIFORM(effect_ambient_occlusion)
        GX_GLES2_THIS_GET_UNIFORM(effect_point_lights_color_min_radius)
        GX_GLES2_THIS_GET_UNIFORM(effect_point_lights_position_max_radius)
        GX_GLES2_THIS_GET_UNIFORM_TEXTURE_ARRAY(effect_shadow_caster_directional_lights_cascades_shadow_map)
        GX_GLES2_THIS_GET_UNIFORM(effect_shadow_caster_directional_lights_cascades_view_projection_bias)
        GX_GLES2_THIS_GET_UNIFORM(effect_shadow_caster_directional_lights_color_cascades_count)
        GX_GLES2_THIS_GET_UNIFORM(effect_shadow_caster_directional_lights_direction)
        GX_GLES2_THIS_GET_UNIFORM(effect_shadow_caster_directional_lights_count)
        GX_GLES2_THIS_GET_UNIFORM(model_m)
        GX_GLES2_THIS_GET_UNIFORM(scene_ambient_light)
        GX_GLES2_THIS_GET_UNIFORM(scene_directional_lights_color)
        GX_GLES2_THIS_GET_UNIFORM(scene_directional_lights_direction)
        GX_GLES2_THIS_GET_UNIFORM(scene_directional_lights_count)
        // GX_GLES2_THIS_GET_UNIFORM(scene_ssao_radius)
        // GX_GLES2_THIS_GET_UNIFORM(scene_ssao_samples_count)
        // GX_GLES2_THIS_GET_UNIFORM(scene_ssao_z_tolerance)
    });
}

gearoenix::gles2::shader::ForwardPbr::~ForwardPbr() noexcept = default;

void gearoenix::gles2::shader::ForwardPbr::bind() const noexcept
{
    Shader::bind();
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(material_base_color)
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(material_emissive)
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(material_metallic_roughness)
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(material_normal)
    // GX_GLES2_SHADER_SET_TEXTURE_INDEX_UNIFORM(effect_ambient_occlusion)
    GX_GLES2_SHADER_SET_TEXTURE_INDEX_ARRAY_UNIFORM(effect_shadow_caster_directional_lights_cascades_shadow_map)
}
#endif
