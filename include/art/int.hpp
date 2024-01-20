#pragma once

#define bit64 INTPTR_MAX == INT64_MAX
#define bit32 INTPTR_MAX == INT32_MAX

/**
 * @brief Calculates alignment chunks required to fit x bytes.
 */
#define alfit(x, alignment) ((x + alignment - 1) / alignment)

static_assert(alfit(0x100000, 4096) == 256);

/**
 * @brief Floors down x to alignment chunks.
 */
#define alfloor(x, alignment) (x / alignment)

/**
 * @brief Calculates the offset in an alignment chunk.
 */
#define aloffset(x, alignment) (x - (x / alignment) * alignment)

namespace art {
    typedef signed char    s8;
    typedef unsigned char  u8;
    typedef signed short   s16;
    typedef unsigned short u16;
    typedef signed int     s32;
    typedef unsigned int   u32;
    typedef signed long    s64;
    typedef unsigned long  u64;

#if bit64
    typedef s64 ssz;
    typedef u64 usz;
#else
    typedef s32 ssz;
    typedef u32 usz;
#endif

    typedef usz addr_t; // A type that describes a virtual address.
    typedef usz phys_t; // A type that describes a physical address.

#ifdef __BYTE_ORDER__
#define bigendian __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define littleendian __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#endif

    /**
     * @brief A lookup table for the bitc() function.
     */
    inline constexpr u8 bitc_lookup[16] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    };

    /**
     * @brief A class that allows for quick manipulation of bits in a value.
     */
    template <typename T>
    class bitman {
        public:
        constexpr bitman(T& val) : _ref(val) {}

        /**
         * @brief Gets the value of the bit at the specified position.
         * @param pos Bit position.
         * @return The value of the bit.
         */
        constexpr bool get(usz pos) {
            return (_ref >> pos) & 1;
        }

        /**
         * @brief Sets the value of the bit at the specified position.
         * @param pos   Bit position.
         * @param value Value to set the bit to.
         */
        constexpr void set(usz pos, bool value = true) {
            _ref = value ? (_ref | (1 << pos)) : (_ref & ~(1 << pos));
        }

        private:
        T& _ref;
    };

    /**
     * @brief Returns the larger value.
     */
    template <typename T>
    constexpr T max(T a, T b) {
        return (a > b) ? a : b;
    }

    /**
     * @brief Returns the largest value out of the given arguments.
     */
    template <typename T, typename... Ts>
    constexpr T max(T a, T b, Ts... c) {
        return max(a, max(b, c...));
    }

    /**
     * @brief Returns the smallest value.
     */
    template <typename T>
    constexpr T min(T a, T b) {
        return (a < b) ? a : b;
    }

    /**
     * @brief Returns the smallest value out of the given arguments.
     */
    template <typename T, typename... Ts>
    constexpr T min(T a, T b, Ts... c) {
        return min(a, min(b, c...));
    }

    /**
     * @brief Counts set bits in an array.
     * @return The amount of set bits.
     */
    constexpr int bitc(u8* str, int len) {
        int count = 0;

        for (int i = 0; i < len; i++) {
            count += bitc_lookup[(str[i] & 0x0F) >> 0];
            count += bitc_lookup[(str[i] & 0xF0) >> 4];
        }

        return count;
    }

    /**
     * @brief Counts set bits in a u32.
     * @return The amount of set bits.
     */
    constexpr u32 bitc(u32 v) {
        int count = 0;

        count += bitc_lookup[(v & 0x0000000F) >> 0];
        count += bitc_lookup[(v & 0x000000F0) >> 4];
        count += bitc_lookup[(v & 0x00000F00) >> 8];
        count += bitc_lookup[(v & 0x0000F000) >> 12];
        count += bitc_lookup[(v & 0x000F0000) >> 16];
        count += bitc_lookup[(v & 0x00F00000) >> 20];
        count += bitc_lookup[(v & 0x0F000000) >> 24];
        count += bitc_lookup[(v & 0xF0000000) >> 28];

        return count;
    }

    /**
     * @brief Counts set bits in a number.
     * @return The amount of set bits.
     */
    template <typename T>
    constexpr int bitc(T& v) {
        return bitc((u8*) &v, sizeof(T));
    }

    /**
     * @brief Returns a number with the specified count of least significant bits set.
     * @param count Count of bits to set.
     * @return The bitmask.
     */
    template <typename T>
    constexpr T bitmask(int count) {
        if (count == sizeof(T) * 8) // fine, GCC.
            return (T) -1;

        return (T) ((1u << count) - 1u);
    }

    /**
     * @brief Returns true if x is in between min and max inclusively.
     */
    template <typename T, typename m, typename M>
    constexpr T inrange(T x, m min, M max) {
        return x >= min && x <= max;
    }

    inline u16 flip(u16 i) {
        return ((i & 0xFF) >> 8) | (i << 8);
    }

    inline u32 flip(u32 i) {
        return ((i & 0xFF000000) >> 24) | ((i & 0x00FF0000) >> 8) |
               ((i & 0x0000FF00) << 8) | ((i & 0x000000FF) << 24);
    }

    inline u64 flip(u64 i) {
        return ((i & 0xFF00000000000000) >> 56) | ((i & 0x00FF000000000000) >> 48) |
               ((i & 0x0000FF0000000000) >> 40) | ((i & 0x000000FF00000000) >> 32) |
               ((i & 0x00000000FF000000) << 32) | ((i & 0x0000000000FF0000) << 40) |
               ((i & 0x000000000000FF00) << 48) | ((i & 0x00000000000000FF) << 56);
    }

    template <typename T>
    inline T from_le(T x) {
        return bigendian ? flip(x) : x;
    }

    template <typename T>
    inline T to_le(T x) {
        return bigendian ? flip(x) : x;
    }

    template <typename T>
    inline T from_be(T x) {
        return littleendian ? flip(x) : x;
    }

    template <typename T>
    inline T to_be(T x) {
        return littleendian ? flip(x) : x;
    }

    static_assert(sizeof(u8) == 1);
    static_assert(sizeof(u16) == 2);
    static_assert(sizeof(u32) == 4);
    static_assert(sizeof(u64) == 8);

    /**
     * @brief Little endian type.
     * Automatically converts between endianness if need-be.
     */
    template <typename T>
    struct le {
        T value;

        T get() {
            return from_le(value);
        }

        void set(T value) {
            this->value = to_le(value);
        }

        operator T() {
            return this->get();
        }

        le& operator=(const T value) {
            this->set(value);
            return *this;
        }

        le& operator|=(const T value) {
            this->set(this->get() | value);
            return *this;
        }

        le& operator&=(const T value) {
            this->set(this->get() & value);
            return *this;
        }

        le& operator^=(const T value) {
            this->set(this->get() ^ value);
            return *this;
        }
    };

    /**
     * @brief Big endian type.
     * Automatically converts between endianness if need-be.
     */
    template <typename T>
    struct be {
        T value;

        T get() {
            return from_be(value);
        }

        void set(T value) {
            this->value = to_be(value);
        }

        operator T() {
            return get();
        }

        be& operator=(const T value) {
            this->set(value);
            return *this;
        }

        be& operator|=(const T value) {
            this->set(get() | value);
            return *this;
        }

        be& operator&=(const T value) {
            set(get() & value);
            return *this;
        }

        be& operator^=(const T value) {
            set(get() ^ value);
            return *this;
        }
    };
}