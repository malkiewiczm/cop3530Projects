#ifndef _KVPAIR_H_
#define _KVPAIR_H_

namespace cop3530 {
	template<typename key_t, typename value_t>
		class KVPair {
	public:
		key_t &key;
		value_t &value;
	KVPair(key_t &key, value_t &value)
		: key(key), value(value) {}

	KVPair(const KVPair &other)
		: key(other.key), value(other.value) {}

		KVPair &operator= (const KVPair &other) {
			this->key = other.key;
			this->value = other.value;
			return *this;
		}
	};
}

#endif
