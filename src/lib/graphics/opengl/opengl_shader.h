#pragma once
#include "graphics/shader.h"
namespace bve {
    namespace graphics {
        namespace opengl {
            class opengl_shader : public shader {
            public:
                opengl_shader(const std::vector<std::filesystem::path>& sources);
                virtual ~opengl_shader() override;
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
            private:
                void create();
                void destroy();
                GLuint create_shader(const std::string& source, GLenum type, std::optional<std::filesystem::path> path);
                GLint get_location(const std::string& name);
                GLuint m_program;
                std::vector<std::filesystem::path> m_sources;
            };
        }
    }
}
