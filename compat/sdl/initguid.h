/* initguid.h pour tcc : DEFINE_GUID définit la variable au lieu de la déclarer */
#undef DEFINE_GUID
#define DEFINE_GUID(n,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
	const GUID n = { l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } }
