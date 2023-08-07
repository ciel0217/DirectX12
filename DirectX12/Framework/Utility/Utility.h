#define GETSET(type, name) \
    public: \
        type Get##name() const { return name; } \
        void Set##name(type value) { name = value; }\
	private: \
		type name; 