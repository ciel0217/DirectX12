#define GETSET(type, name) \
    public: \
        type Get##name() const { return name; } \
        void Set##name(type value) { name = value; }\
	private: \
		type name; 

#ifdef NAME_GPU_RESOURCE
inline void SetName(ID3D12Object* pObject, LPCWSTR name) {
	pObject->SetName(name);
}

#else
inline void SetName(ID3D12Object* pObject, LPCWSTR name) {}

#endif

#define NAME_D3D12_OBJECT(x) SetName(x, L#x)
