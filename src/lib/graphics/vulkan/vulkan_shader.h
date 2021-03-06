#pragma once
#include "vulkan_object_factory.h"
#include "shader_parser.h"
namespace bve {
    namespace graphics {
        namespace vulkan {
            struct descriptor_set {
                VkDescriptorSetLayout layout;
                std::vector<VkDescriptorSet> sets;
            };
            class vulkan_context;
            class vulkan_shader : public shader {
            public:
                static std::vector<ref<vulkan_shader>> get_active_shaders();
                vulkan_shader(ref<vulkan_object_factory> factory, const std::vector<fs::path>& sources);
                virtual ~vulkan_shader() override;
                virtual void reload() override;
                virtual void bind() override;
                virtual void unbind() override;
                virtual void set_int(const std::string& name, int32_t value) override;
                virtual void set_float(const std::string& name, float value) override;
                virtual void set_ivec2(const std::string& name, const glm::ivec2& value) override;
                virtual void set_ivec3(const std::string& name, const glm::ivec3& value) override;
                virtual void set_ivec4(const std::string& name, const glm::ivec4& value) override;
                virtual void set_vec2(const std::string& name, const glm::vec2& value) override;
                virtual void set_vec3(const std::string& name, const glm::vec3& value) override;
                virtual void set_vec4(const std::string& name, const glm::vec4& value) override;
                virtual void set_mat4(const std::string& name, const glm::mat4& value) override;
                virtual int32_t get_int(const std::string& name) override;
                virtual float get_float(const std::string& name) override;
                virtual glm::ivec2 get_ivec2(const std::string& name) override;
                virtual glm::ivec3 get_ivec3(const std::string& name) override;
                virtual glm::ivec4 get_ivec4(const std::string& name) override;
                virtual glm::vec2 get_vec2(const std::string& name) override;
                virtual glm::vec3 get_vec3(const std::string& name) override;
                virtual glm::vec4 get_vec4(const std::string& name) override;
                virtual glm::mat4 get_mat4(const std::string& name) override;
                void update_descriptor_sets(size_t image_index);
                const std::vector<VkPipelineShaderStageCreateInfo>& get_create_info() { return this->m_pipeline_create_info; }
                const std::vector<descriptor_set>& get_descriptor_sets() { return this->m_descriptor_sets; }
            private:
                void compile();
                VkShaderModule compile_shader(shader_type type, const shader_parser& parser);
                void cleanup();
                void create_descriptor_sets();
                void destroy_descriptor_sets();
                ref<vulkan_object_factory> m_factory;
                std::vector<fs::path> m_sources;
                VkDevice m_device;
                std::vector<VkPipelineShaderStageCreateInfo> m_pipeline_create_info;
                std::vector<descriptor_set> m_descriptor_sets;
                VkDescriptorPool m_descriptor_pool;
                friend class vulkan_context;
            };
        }
    }
}