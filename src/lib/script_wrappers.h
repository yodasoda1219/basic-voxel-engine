#pragma once
#include "lighting/light.h"
#include "input_manager.h"
#include "model.h"
namespace bve {
    namespace script_wrappers {
        struct NamespacedName {
            MonoString* namespace_name;
            MonoString* local_name;
        };
        using Type = MonoReflectionType*;
        using IntPtr = void*;
        using string = MonoString*;
        using Vector3 = glm::vec3;
        using Vector3I = glm::ivec3;
        using LightType = lighting::light_type;
        using uint = uint32_t;
        using object = MonoObject*;
        using Vector2 = glm::vec2;
        using Vector2I = glm::ivec2;

        void BasicVoxelEngine_Application_Quit();
        double BasicVoxelEngine_Application_GetDeltaTime();
        IntPtr BasicVoxelEngine_Application_GetWorld();
        IntPtr BasicVoxelEngine_Application_GetInputManager();
        IntPtr BasicVoxelEngine_Application_GetFactory();
        IntPtr BasicVoxelEngine_Application_GetWindow();

        void BasicVoxelEngine_Logger_PrintDebug(string message);
        void BasicVoxelEngine_Logger_PrintInfo(string message);
        void BasicVoxelEngine_Logger_PrintWarning(string message);
        void BasicVoxelEngine_Logger_PrintError(string message);

        void BasicVoxelEngine_Graphics_Factory_DestroyRef(IntPtr address);
        IntPtr BasicVoxelEngine_Graphics_Factory_CreateTexture(IntPtr address, MonoObject* imageData);
        IntPtr BasicVoxelEngine_Graphics_Factory_CreateContext(IntPtr address);
        IntPtr BasicVoxelEngine_Graphics_Factory_CreateShader(IntPtr address, MonoObject* sourceList);

        IntPtr BasicVoxelEngine_Model_LoadModel(string path, IntPtr factory);
        void BasicVoxelEngine_Model_DestroyRef(IntPtr address);
        int32_t BasicVoxelEngine_Model_GetMeshCount(IntPtr address);
        MonoObject* BasicVoxelEngine_Model_GetMesh(IntPtr address, int32_t index, int32_t* vertexOffset, int32_t* indexOffset, int32_t* textureCount);
        IntPtr BasicVoxelEngine_Model_GetTexture(IntPtr address, int32_t meshIndex, int32_t textureIndex);
        int32_t BasicVoxelEngine_Model_GetVertexCount(IntPtr address);
        model::vertex BasicVoxelEngine_Model_GetVertex(IntPtr address, int32_t index);
        int32_t BasicVoxelEngine_Model_GetIndexCount(IntPtr address);
        uint BasicVoxelEngine_Model_GetIndex(IntPtr address, int32_t index);

        void BasicVoxelEngine_MeshTextureOffsetData_Destroy(IntPtr address);
        int32_t BasicVoxelEngine_MeshTextureOffsetData_GetCount(IntPtr address);
        string BasicVoxelEngine_MeshTextureOffsetData_GetKey(IntPtr address, int32_t index);
        int32_t BasicVoxelEngine_MeshTextureOffsetData_GetValue(IntPtr address, string key);

        string BasicVoxelEngine_AssetManager_GetAssetPath(string assetName);

        void BasicVoxelEngine_Lighting_Light_Destroy(IntPtr address);
        void BasicVoxelEngine_Lighting_Light_SetColor(IntPtr address, Vector3 color);
        void BasicVoxelEngine_Lighting_Light_SetAmbientStrength(IntPtr address, float strength);
        void BasicVoxelEngine_Lighting_Light_SetSpecularStrength(IntPtr address, float strength);
        LightType BasicVoxelEngine_Lighting_Light_GetType(IntPtr address);

        IntPtr BasicVoxelEngine_Lighting_Spotlight_Create();
        void BasicVoxelEngine_Lighting_Spotlight_SetDirection(IntPtr address, Vector3 direction);
        void BasicVoxelEngine_Lighting_Spotlight_SetCutoff(IntPtr address, float cutoff);

        IntPtr BasicVoxelEngine_Lighting_PointLight_Create();
        void BasicVoxelEngine_Lighting_PointLight_SetConstant(IntPtr address, float constant);
        void BasicVoxelEngine_Lighting_PointLight_SetLinear(IntPtr address, float linear);
        void BasicVoxelEngine_Lighting_PointLight_SetQuadratic(IntPtr address, float quadratic);

        void BasicVoxelEngine_World_Destroy(IntPtr address);
        int32_t BasicVoxelEngine_World_GetBlock(IntPtr address, Vector3I position);
        void BasicVoxelEngine_World_SetBlock(IntPtr address, Vector3I position, int32_t index);
        void BasicVoxelEngine_World_AddOnBlockChangedCallback(IntPtr address, MonoObject* callback);

        object BasicVoxelEngine_Entity_AddComponent(uint id, IntPtr world, Type type);
        object BasicVoxelEngine_Entity_GetComponent(uint id, IntPtr world, Type type);
        bool BasicVoxelEngine_Entity_HasComponent(uint id, IntPtr world, Type type);
        void BasicVoxelEngine_Entity_RemoveComponent(uint id, IntPtr world, Type type);
        void BasicVoxelEngine_Entity_RegisterComponents();

        Vector3 BasicVoxelEngine_Components_TransformComponent_GetTranslation(IntPtr address);
        void BasicVoxelEngine_Components_TransformComponent_SetTranslation(IntPtr address, Vector3 value);
        Vector3 BasicVoxelEngine_Components_TransformComponent_GetRotation(IntPtr address);
        void BasicVoxelEngine_Components_TransformComponent_SetRotation(IntPtr address, Vector3 value);
        Vector3 BasicVoxelEngine_Components_TransformComponent_GetScale(IntPtr address);
        void BasicVoxelEngine_Components_TransformComponent_SetScale(IntPtr address, Vector3 value);

        Vector3 BasicVoxelEngine_Components_CameraComponent_GetDirection(IntPtr address);
        void BasicVoxelEngine_Components_CameraComponent_SetDirection(IntPtr address, Vector3 value);
        Vector3 BasicVoxelEngine_Components_CameraComponent_GetUp(IntPtr address);
        void BasicVoxelEngine_Components_CameraComponent_SetUp(IntPtr address, Vector3 value);
        bool BasicVoxelEngine_Components_CameraComponent_GetPrimary(IntPtr address);
        void BasicVoxelEngine_Components_CameraComponent_SetPrimary(IntPtr address, bool value);
        float BasicVoxelEngine_Components_CameraComponent_GetNearPlane(IntPtr address);
        void BasicVoxelEngine_Components_CameraComponent_SetNearPlane(IntPtr address, float value);
        float BasicVoxelEngine_Components_CameraComponent_GetFarPlane(IntPtr address);
        void BasicVoxelEngine_Components_CameraComponent_SetFarPlane(IntPtr address, float value);

        object BasicVoxelEngine_Components_ScriptComponent_Bind(Type type, MonoObject* args, IntPtr address);

        void BasicVoxelEngine_InputManager_DestroyRef(IntPtr address);
        bool BasicVoxelEngine_InputManager_GetMouseEnabled(IntPtr address);
        void BasicVoxelEngine_InputManager_SetMouseEnabled(IntPtr address, bool value);
        void BasicVoxelEngine_InputManager_GetMouse(IntPtr address, Vector2* offset);
        input_manager::key_state BasicVoxelEngine_InputManager_GetKey(IntPtr address, int32_t key);
        input_manager::key_state BasicVoxelEngine_InputManager_GetMouseButton(IntPtr address, int32_t button);

        IntPtr BasicVoxelEngine_Graphics_ImageData_Load(string path);
        void BasicVoxelEngine_Graphics_ImageData_Destroy(IntPtr address);
        int32_t BasicVoxelEngine_Graphics_ImageData_GetWidth(IntPtr address);
        int32_t BasicVoxelEngine_Graphics_ImageData_GetHeight(IntPtr address);
        int32_t BasicVoxelEngine_Graphics_ImageData_GetChannels(IntPtr address);
        uint8_t BasicVoxelEngine_Graphics_ImageData_GetByte(IntPtr address, int32_t index);

        void BasicVoxelEngine_Graphics_Texture_DestroyRef(IntPtr address);
        Vector2I BasicVoxelEngine_Graphics_Texture_GetSize(IntPtr address);
        int32_t BasicVoxelEngine_Graphics_Texture_GetChannels(IntPtr address);

        void BasicVoxelEngine_Window_DestroyRef(IntPtr address);
        IntPtr BasicVoxelEngine_Window_GetContext(IntPtr address);
        Vector2I BasicVoxelEngine_Window_GetFramebufferSize(IntPtr address);

        void BasicVoxelEngine_Graphics_Shader_DestroyRef(IntPtr address);
        void BasicVoxelEngine_Graphics_Shader_Reload(IntPtr address);
        void BasicVoxelEngine_Graphics_Shader_Bind(IntPtr address);
        void BasicVoxelEngine_Graphics_Shader_Unbind(IntPtr address);
        void BasicVoxelEngine_Graphics_Shader_Set(IntPtr address, string name, object value, Type type);
        void BasicVoxelEngine_Graphics_Shader_Get(IntPtr address, string name, object value, Type type);
        void BasicVoxelEngine_Graphics_Shader_InitializeUniforms();

        void BasicVoxelEngine_Graphics_Context_DestroyRef(IntPtr address);
        void BasicVoxelEngine_Graphics_Context_MakeCurrent(IntPtr address);
        void BasicVoxelEngine_Graphics_Context_DrawIndexed(IntPtr address, int32_t indexCount);

        IntPtr BasicVoxelEngine_Mesh_AllocVertexBuffer(int32_t totalSize);
        void BasicVoxelEngine_Mesh_CopyVertex(IntPtr buffer, int32_t index, int32_t stride, void* address);
        void BasicVoxelEngine_Mesh_FreeVertexBuffer(IntPtr address);

        IntPtr BasicVoxelEngine_CommandList_Create(IntPtr rendererAddress);
        void BasicVoxelEngine_CommandList_Destroy(IntPtr address, IntPtr rendererAddress);
        void BasicVoxelEngine_CommandList_AddMesh(MonoObject* mesh, IntPtr address, IntPtr rendererAddress);
        void BasicVoxelEngine_CommandList_Close(MonoObject* vertexAttributes, IntPtr address, IntPtr rendererAddress);

        void BasicVoxelEngine_Renderer_DestroyRef(IntPtr address);
        void BasicVoxelEngine_Renderer_Render(IntPtr commandListAddress, IntPtr contextAddress, IntPtr address);
        void BasicVoxelEngine_Renderer_SetShader(IntPtr shaderAddress, IntPtr address);
        void BasicVoxelEngine_Renderer_SetTexture(int32_t index, IntPtr textureAddress, IntPtr address);
    }
}