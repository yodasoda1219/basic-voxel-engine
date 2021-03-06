#include "bve_pch.h"
#include "texture_atlas.h"
#include "block.h"

namespace bve {
    texture_atlas::uniform_data texture_atlas::get_uniform_data() {
        uniform_data data;
        data.image = 0; // lets just hardcode this, shall we?
        data.texture_size = this->m_texture_size;
        data.grid_size = this->m_atlas_size;
        auto& block_register = registry::get().get_register<block>();
        for (const std::pair<namespaced_name, std::pair<glm::ivec2, glm::ivec2>>& pair : this->m_texture_dimensions) {
            std::optional<size_t> index_optional = block_register.get_index(pair.first);
            if (!index_optional) {
                throw std::runtime_error("[texture atlas] the specified block does not exist");
            }
            size_t index = *index_optional;
            data.texture_dimensions[index].grid_position = pair.second.first;
            data.texture_dimensions[index].texture_dimensions = pair.second.second;
        }
        return data;
    }

    glm::ivec2 texture_atlas::get_texture_size() {
        return this->m_texture_size;
    }

    glm::ivec2 texture_atlas::get_atlas_size() {
        return this->m_atlas_size;
    }

    ref<graphics::texture> texture_atlas::get_texture() {
        return this->m_texture;
    }

    std::vector<namespaced_name> texture_atlas::get_included_block_names() {
        std::vector<namespaced_name> names;
        for (const auto& pair : this->m_texture_dimensions) {
            names.push_back(pair.first);
        }
        return names;
    }

    // Compute power of two greater than or equal to `n`
    // From approach 4 documented at https://www.techiedelight.com/round-next-highest-power-2/
    static int32_t find_next_power_of_2(int32_t n)
    {
        // decrement `n` (to handle the case when `n` itself is a power of 2)
        n--;

        // set all bits after the last set bit
        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;

        // increment `n` and return
        return ++n;
    }

    texture_atlas::texture_atlas(const std::vector<std::pair<namespaced_name, texture_data>>& textures, ref<graphics::object_factory> object_factory) {
        constexpr int32_t channels = 4;
        // the texture atlas is supposed to be aligned in rows!
        constexpr int32_t max_row_size = 16;

        std::vector<std::pair<namespaced_name, texture_data>> textures_copy(textures.size());
        std::copy(textures.begin(), textures.end(), textures_copy.begin());

        int32_t texture_count = (int32_t)textures_copy.size();
        if (texture_count > max_row_size) {
            this->m_atlas_size.x = max_row_size;
            this->m_atlas_size.y = find_next_power_of_2((texture_count - (texture_count % max_row_size)) / max_row_size + 1);
        } else {
            this->m_atlas_size.x = find_next_power_of_2(texture_count);
            this->m_atlas_size.y = 1;
        }

        glm::ivec2 largest_texture_size = glm::ivec2(0);
        for (auto& pair : textures_copy) {
            // If the texture has less than the desired number of channels, then create a copy with filler data.
            if (pair.second.channels != channels) {
                if (pair.second.channels > channels) {
                    throw std::runtime_error("[texture atlas] an included texture cannot have more than " + std::to_string(channels) + " channels");
                }
                // this is very hacky but i had to make it work
                std::vector<uint8_t> new_data(pair.second.data.size());
                std::copy(pair.second.data.begin(), pair.second.data.end(), new_data.begin());
                for (size_t i = pair.second.channels; i < new_data.size() + 1; i += channels) {
                    auto it = new_data.begin();
                    std::advance(it, i);
                    std::vector<uint8_t> filler_data((size_t)channels - pair.second.channels);
                    memset(filler_data.data(), 255, filler_data.size() * sizeof(uint8_t));
                    new_data.insert(it, filler_data.begin(), filler_data.end());
                }
                pair.second.data = new_data;
                pair.second.channels = channels;
            }

            // Accumulate the largest texture in the atlas.
            glm::ivec2 texture_size = glm::ivec2(pair.second.width, pair.second.height);
            if (texture_size.x > largest_texture_size.x) {
                largest_texture_size.x = texture_size.x;
            }
            if (texture_size.y > largest_texture_size.y) {
                largest_texture_size.y = texture_size.y;
            }
        }
        // Round the largest texture up to a power of two
        largest_texture_size.x = find_next_power_of_2(largest_texture_size.x);
        largest_texture_size.y = find_next_power_of_2(largest_texture_size.y);

        // The final texture size is based on the blocks row x columns and the largest texture size.
        this->m_texture_size = largest_texture_size * this->m_atlas_size;

        // Copy individual textures into the final atlas texture.
        size_t buffer_size = (size_t)this->m_texture_size.x * this->m_texture_size.y * channels;
        std::vector<uint8_t> atlas_texture_data(buffer_size);
        for (size_t i = 0; i < buffer_size; i += (size_t)channels) {
            for (size_t j = 0; j < 3; j++) {
                atlas_texture_data[i + j] = 0;
            }
            if (channels > 3) {
                for (size_t j = 3; j < (size_t)channels; j++) {
                    atlas_texture_data[i + j] = 255;
                }
            }
        }
        for (size_t i = 0; i < textures_copy.size(); i++) {
            // Determine the atlas grid position for this texture
            glm::ivec2 atlas_position;
            if (this->m_atlas_size.y > 1) {
                int32_t index = (int32_t)i;
                atlas_position.x = index % this->m_atlas_size.x;
                atlas_position.y = (index - atlas_position.x) / this->m_atlas_size.x;
            } else {
                atlas_position = glm::ivec2((int32_t)i, 0);
            }

            // Copy the texture into the atlas texture
            const auto& pair = textures_copy[i];
            const auto& data = pair.second;
            glm::ivec2 texture_size = glm::ivec2(data.width, data.height);
            this->m_texture_dimensions[pair.first] = { atlas_position, texture_size };
            for (int32_t y = 0; y < texture_size.y; y++) {
                glm::ivec2 atlas_buffer_position = (atlas_position * largest_texture_size) + glm::ivec2(0, y);
                size_t atlas_buffer_index = (((size_t)atlas_buffer_position.y * this->m_texture_size.x) + atlas_buffer_position.x) * channels;
                size_t texture_buffer_index = ((size_t)y * texture_size.x) * channels;
                for (int32_t x = 0; x < texture_size.x; x++) {
                    for (int32_t component = 0; component < channels; component++) {
                        size_t index_offset = ((size_t)x * channels) + component;
                        atlas_texture_data[atlas_buffer_index + index_offset] = data.data[texture_buffer_index + index_offset];
                    }
                }
            }
        }

        // Create the texture in the graphics device
        this->m_texture = object_factory->create_texture(atlas_texture_data, this->m_texture_size.x, this->m_texture_size.y, channels);
    }
}