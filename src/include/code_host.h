#pragma once
namespace bve {
    namespace managed {
        class wrapper : public ref_counted {
        public:
            wrapper(MonoDomain* domain);
            virtual ~wrapper() = default;
            MonoDomain* get_domain();
            virtual void* get() = 0;
            virtual MonoImage* get_image() = 0;
        private:
            MonoDomain* m_domain;
        };
        class object : public wrapper {
        public:
            static void handle_exception(ref<object> exception);
            object(MonoObject* _object, MonoDomain* domain);
            object(const std::string& string, MonoDomain* domain);
            virtual ~object() override;
            ref<object> get(MonoClassField* field);
            template<typename... Args> ref<object> get(MonoProperty* property, Args*... args) {
                MonoObject* _object = mono_gchandle_get_target(this->m_handle);
                MonoObject* exc = nullptr;
                std::vector<void*> args_vector = { std::forward<Args*>(args)... };
                void** args_ptr = args_vector.empty() ? nullptr : args_vector.data();
                MonoObject* value = mono_property_get_value(property, _object, args_ptr, &exc);
                if (exc) {
                    ref<object> exception = ref<object>::create(exc, this->get_domain());
                    handle_exception(exception);
                }
                ref<object> returned_object;
                if (value) {
                    returned_object = ref<object>::create(value, this->get_domain());
                }
                return returned_object;
            }
            void set(MonoClassField* field, void* value);
            template<typename... Args> void set(MonoProperty* property, void* value, Args*... args) {
                MonoObject* _object = mono_gchandle_get_target(this->m_handle);
                MonoObject* exc = nullptr;
                std::vector<void*> args_vector = { value };
                std::vector<void*> passed_args = { std::forward<Args*>(args)... };
                for (void* passed_arg : passed_args) {
                    args_vector.push_back(passed_arg);
                }
                mono_property_set_value(property, _object, args_vector.data(), &exc);
                if (exc) {
                    ref<object> exception = ref<object>::create(exc, this->get_domain());
                    handle_exception(exception);
                }
            }
            std::string get_string();
            const void* unbox() {
                MonoObject* _object = mono_gchandle_get_target(this->m_handle);
                return mono_object_unbox(_object);
            }
            template<typename T> const T& unbox() {
                return *(T*)this->unbox();
            }
            ref<object> invoke(MonoMethod* method, const std::vector<void*>& args) {
                if (!method) {
                    throw std::runtime_error("[managed object] attempted to call nullptr!");
                }
                MonoObject* _object = mono_gchandle_get_target(this->m_handle);
                MonoObject* exc = nullptr;
                void** args_ptr = args.size() > 0 ? (void**)args.data() : nullptr;
                MonoObject* returned = mono_runtime_invoke(method, _object, args_ptr, &exc);
                if (exc) {
                    ref<object> exception = ref<object>::create(exc, this->get_domain());
                    handle_exception(exception);
                }
                ref<object> returned_object;
                if (returned) {
                    returned_object = ref<object>::create(returned, this->get_domain());
                }
                return returned_object;
            }
            template<typename... Args> ref<object> invoke(MonoMethod* method, Args*... args) { return this->invoke(method, { std::forward<Args*>(args)... }); }
            virtual void* get() override;
            virtual MonoImage* get_image() override;
        private:
            uint32_t m_handle;
        };
        class delegate : public wrapper {
        public:
            delegate(ref<object> _object);
            delegate(void* _delegate, MonoDomain* domain);
            virtual ~delegate() override;
            template<typename... Args> ref<object> invoke(Args*... args) {
                MonoObject* _delegate = mono_gchandle_get_target(this->m_handle);
                MonoObject* exc = nullptr;
                std::vector<void*> args_vector = { std::forward<Args*>(args)... };
                void** args_ptr = args_vector.size() > 0 ? args_vector.data() : nullptr;
                MonoObject* returned = mono_runtime_delegate_invoke(_delegate, args_ptr, &exc);
                if (exc) {
                    ref<object> exception = ref<object>::create(exc, this->get_domain());
                    object::handle_exception(exception);
                }
                ref<object> returned_object;
                if (returned) {
                    returned_object = ref<object>::create(returned, this->get_domain());
                }
                return returned_object;
            }
            virtual void* get() override;
            virtual MonoImage* get_image() override;
        private:
            uint32_t m_handle;
        };
        class class_ : public wrapper {
        public:
            static ref<class_> get_class(ref<object> _object);
            class_(MonoClass* _class, MonoDomain* domain);
            std::string get_name();
            void get_name(std::string& namespace_name, std::string& class_name);
            MonoMethod* get_method(const std::string& descriptor);
            MonoClassField* get_field(const std::string& name);
            MonoProperty* get_property(const std::string& name);
            bool derives_from(ref<class_> cls);
            bool is_value_type();
            size_t data_size();
            template<typename... Args> static ref<object> invoke(MonoMethod* method, Args*... args) {
                MonoDomain* domain = mono_domain_get();
                if (!method) {
                    throw std::runtime_error("[managed class] attempted to call nullptr!");
                }
                std::vector<void*> args_vector = { std::forward<Args*>(args)... };
                MonoObject* exc = nullptr;
                void** args_ptr = args_vector.size() > 0 ? args_vector.data() : nullptr;
                MonoObject* returned = mono_runtime_invoke(method, nullptr, args_ptr, &exc);
                if (exc) {
                    ref<object> exception = ref<object>::create(exc, domain);
                    object::handle_exception(exception);
                }
                ref<object> returned_object;
                if (returned) {
                    returned_object = ref<object>::create(returned, domain);
                }
                return returned_object;
            }
            ref<object> instantiate(const std::vector<void*>& args) {
                MonoDomain* domain = this->get_domain();
                MonoObject* instance = mono_object_new(domain, this->m_class);
                ref<object> referenced_object = ref<object>::create(instance, domain);
                MonoMethod* constructor = this->get_method("*:.ctor");
                referenced_object->invoke(constructor, args);
                return referenced_object;
            }
            template<typename... Args> ref<object> instantiate(Args*... args) { return this->instantiate({ std::forward<Args*>(args)... }); }
            virtual void* get() override;
            virtual MonoImage* get_image() override;
        private:
            MonoClass* m_class;
            MonoImage* m_image;
        };
        class assembly;
        class type : public wrapper {
        public:
            static ref<type> get_type(ref<class_> _class);
            type(MonoType* _type, MonoDomain* domain);
            type(MonoReflectionType* _type, MonoDomain* domain);
            ref<class_> get_class();
            MonoReflectionType* get_object();
            virtual void* get() override;
            virtual MonoImage* get_image() override;
        private:
            MonoType* m_type;
        };
        class assembly : public wrapper {
        public:
            static ref<assembly> get_assembly(ref<wrapper> wrapper_object);
            assembly(MonoAssembly* _assembly, MonoDomain* domain);
            ref<class_> get_class(const std::string& name);
            ref<class_> get_class(const std::string& namespace_name, const std::string& class_name);
            MonoMethod* get_method(const std::string& descriptor);
            MonoReflectionAssembly* get_object();
            virtual void* get() override;
            virtual MonoImage* get_image() override;
        private:
            MonoAssembly* m_assembly;
            MonoImage* m_image;
        };
    }
    class code_host : public ref_counted {
    public:
        static std::unordered_map<std::string, void*> get_script_wrappers();
        static void remove_current();
        static ref<code_host> current();
        code_host();
        ~code_host();
        code_host(const code_host&) = delete;
        code_host& operator=(const code_host&) = delete;
        void make_current();
        MonoDomain* get_domain();
        void load_assembly(const fs::path& path, bool ref_only = false);
        std::vector<ref<managed::assembly>> get_loaded_assemblies();
        template<typename T> void register_function(const std::string& name, const T& func) {
            mono_add_internal_call(name.c_str(), (void*)func);
        }
        ref<managed::class_> find_class(const std::string& name);
        ref<managed::class_> find_class(const std::string& namespace_name, const std::string& class_name);
    private:
        MonoDomain* m_domain;
        std::vector<ref<managed::assembly>> m_assemblies;
    };
}