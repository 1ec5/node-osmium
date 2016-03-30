#ifndef OSM_ENTITY_WRAP_HPP
#define OSM_ENTITY_WRAP_HPP

#include "include_nan.hpp"

// osmium
namespace osmium {
    class OSMEntity;
}

#include "utils.hpp"

namespace node_osmium {

    class OSMEntityWrap : public node::ObjectWrap {

        const osmium::OSMEntity* m_entity;

    protected:

        typedef v8::Handle<v8::Value> accessor_type(v8::Local<v8::String> property, const v8::AccessorInfo& info);

        static void set_accessor(v8::Persistent<v8::FunctionTemplate> t, const char* name, accessor_type getter, v8::PropertyAttribute attributes) {
            t->InstanceTemplate()->SetAccessor(v8::String::NewSymbol(name), getter, nullptr, v8::Handle<v8::Value>(), v8::DEFAULT, attributes);
        }

        template<class T>
        static v8::Handle<v8::Value> tags_impl(const v8::Arguments& args) {
            v8::HandleScope scope;

            const T& object = static_cast<const T&>(unwrap<OSMEntityWrap>(args.This()));
            switch (args.Length()) {
                case 0: {
                    v8::Local<v8::Object> tags = v8::Object::New();
                    for (const auto& tag : object.tags()) {
                        tags->Set(v8::String::New(tag.key()), v8::String::New(tag.value()));
                    }
                    return scope.Close(tags);
                }
                case 1: {
                    if (!args[0]->IsString()) {
                        break;
                    }
                    v8::String::Utf8Value key { args[0] };
                    const char* value = object.tags().get_value_by_key(*key);
                    return scope.Close(value ? v8::String::New(value) : v8::Undefined());
                }
            }

            return ThrowException(v8::Exception::TypeError(v8::String::New("call tags() without parameters or with a string (the key)")));
        }

    public:

        static v8::Persistent<v8::FunctionTemplate> constructor;
        static void Initialize(v8::Handle<v8::Object> target);
        static v8::Handle<v8::Value> New(const v8::Arguments& args);

        OSMEntityWrap() :
            m_entity(nullptr) {
        }

        OSMEntityWrap(const osmium::OSMEntity& entity) :
            m_entity(&entity) {
        }

        void set(const osmium::OSMEntity& entity) {
            m_entity = &entity;
        }

        const osmium::OSMEntity& get() {
            return *m_entity;
        }

    protected:

        virtual ~OSMEntityWrap() = default;

    }; // class OSMEntityWrap

} // namespace node_osmium

#endif // OSM_ENTITY_WRAP_HPP
