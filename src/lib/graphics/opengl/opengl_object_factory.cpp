#include "bve_pch.h"
#include "opengl_object_factory.h"
#include "opengl_pipeline.h"
#include "opengl_buffer.h"
#include "opengl_context.h"
#include "opengl_shader.h"
#include "opengl_texture.h"
#include "opengl_uniform_buffer.h"
namespace bve {
    namespace graphics {
        namespace opengl {
            ref<pipeline> opengl_object_factory::create_pipeline() {
                return ref<opengl_pipeline>::create();
            }
            ref<buffer> opengl_object_factory::create_vertex_buffer(const void* data, size_t size) {
                return ref<opengl_buffer>::create(data, size, GL_ARRAY_BUFFER);
            }
            ref<buffer> opengl_object_factory::create_index_buffer(const std::vector<uint32_t>& data) {
                return ref<opengl_buffer>::create(data.data(), data.size() * sizeof(uint32_t), GL_ELEMENT_ARRAY_BUFFER);
            }
            ref<context> opengl_object_factory::create_context() {
                return ref<opengl_context>::create();
            }
            ref<shader> opengl_object_factory::create_shader(const std::vector<fs::path>& sources) {
                return ref<opengl_shader>::create(sources);
            }
            ref<texture> opengl_object_factory::create_texture(const std::vector<uint8_t>& data, int32_t width, int32_t height, int32_t channels) {
                return ref<opengl_texture>::create(data, width, height, channels);
            }
            ref<uniform_buffer> opengl_object_factory::create_uniform_buffer(size_t size, uint32_t binding) {
                return ref<opengl_uniform_buffer>::create(size, binding);
            }
        }
    }
}