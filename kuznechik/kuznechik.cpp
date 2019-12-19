#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <ctime>

typedef unsigned char u32;
typedef unsigned int u64;

static const u32 nonlinear_transform_perm[256] = {
        252, 238, 221, 17, 207, 110, 49, 22, 251, 196,
        250, 218, 35, 197, 4, 77, 233, 119, 240, 219,
        147, 46, 153, 186, 23, 54, 241, 187, 20, 205,
        95, 193, 249, 24, 101, 90, 226, 92, 239, 33,
        129, 28, 60, 66, 139, 1, 142, 79, 5, 132, 2,
        174, 227, 106, 143, 160, 6, 11, 237, 152, 127,
        212, 211, 31, 235, 52, 44, 81, 234, 200, 72,
        171, 242, 42, 104, 162, 253, 58, 206, 204, 181,
        112, 14, 86, 8, 12, 118, 18, 191, 114, 19, 71,
        156, 183, 93, 135, 21, 161, 150, 41, 16, 123,
        154, 199, 243, 145, 120, 111, 157, 158, 178, 177,
        50, 117, 25, 61, 255, 53, 138, 126, 109, 84,
        198, 128, 195, 189, 13, 87, 223, 245, 36, 169,
        62, 168, 67, 201, 215, 121, 214, 246, 124, 34,
        185, 3, 224, 15, 236, 222, 122, 148, 176, 188,
        220, 232, 40, 80, 78, 51, 10, 74, 167, 151, 96,
        115, 30, 0, 98, 68, 26, 184, 56, 130, 100, 159,
        38, 65, 173, 69, 70, 146, 39, 94, 85, 47, 140,
        163, 165, 125, 105, 213, 149, 59, 7, 88, 179,
        64, 134, 172, 29, 247, 48, 55, 107, 228, 136,
        217, 231, 137, 225, 27, 131, 73, 76, 63, 248,
        254, 141, 83, 170, 144, 202, 216, 133, 97, 32,
        113, 103, 164, 45, 43, 9, 91, 203, 155, 37,
        208, 190, 229, 108, 82, 89, 166, 116, 210, 230,
        244, 180, 192, 209, 102, 175, 194, 57, 75, 99,
        182
};

static const u32 linear_transform_perm[16] = {
        148, 32, 133, 16, 194, 192, 1, 251, 1, 192,
        194, 16, 133, 32, 148, 1
};


/* Матрица преобразования L. */
u32 LTransformationMatrix[16][16] = {
    0xcf, 0x6e, 0xa2, 0x76, 0x72, 0x6c, 0x48, 0x7a, 0xb8, 0x5d, 0x27, 0xbd, 0x10, 0xdd, 0x84, 0x94,
    0x98, 0x20, 0xc8, 0x33, 0xf2, 0x76, 0xd5, 0xe6, 0x49, 0xd4, 0x9f, 0x95, 0xe9, 0x99, 0x2d, 0x20,
    0x74, 0xc6, 0x87, 0x10, 0x6b, 0xec, 0x62, 0x4e, 0x87, 0xb8, 0xbe, 0x5e, 0xd0, 0x75, 0x74, 0x85,
    0xbf, 0xda, 0x70, 0x0c, 0xca, 0x0c, 0x17, 0x1a, 0x14, 0x2f, 0x68, 0x30, 0xd9, 0xca, 0x96, 0x10,
    0x93, 0x90, 0x68, 0x1c, 0x20, 0xc5, 0x06, 0xbb, 0xcb, 0x8d, 0x1a, 0xe9, 0xf3, 0x97, 0x5d, 0xc2,
    0x8e, 0x48, 0x43, 0x11, 0xeb, 0xbc, 0x2d, 0x2e, 0x8d, 0x12, 0x7c, 0x60, 0x94, 0x44, 0x77, 0xc0,
    0xf2, 0x89, 0x1c, 0xd6, 0x02, 0xaf, 0xc4, 0xf1, 0xab, 0xee, 0xad, 0xbf, 0x3d, 0x5a, 0x6f, 0x01,
    0xf3, 0x9c, 0x2b, 0x6a, 0xa4, 0x6e, 0xe7, 0xbe, 0x49, 0xf6, 0xc9, 0x10, 0xaf, 0xe0, 0xde, 0xfb,
    0x0a, 0xc1, 0xa1, 0xa6, 0x8d, 0xa3, 0xd5, 0xd4, 0x09, 0x08, 0x84, 0xef, 0x7b, 0x30, 0x54, 0x01,
    0xbf, 0x64, 0x63, 0xd7, 0xd4, 0xe1, 0xeb, 0xaf, 0x6c, 0x54, 0x2f, 0x39, 0xff, 0xa6, 0xb4, 0xc0,
    0xf6, 0xb8, 0x30, 0xf6, 0xc4, 0x90, 0x99, 0x37, 0x2a, 0x0f, 0xeb, 0xec, 0x64, 0x31, 0x8d, 0xc2,
    0xa9, 0x2d, 0x6b, 0x49, 0x01, 0x58, 0x78, 0xb1, 0x01, 0xf3, 0xfe, 0x91, 0x91, 0xd3, 0xd1, 0x10,
    0xea, 0x86, 0x9f, 0x07, 0x65, 0x0e, 0x52, 0xd4, 0x60, 0x98, 0xc6, 0x7f, 0x52, 0xdf, 0x44, 0x85,
    0x8e, 0x44, 0x30, 0x14, 0xdd, 0x02, 0xf5, 0x2a, 0x8e, 0xc8, 0x48, 0x48, 0xf8, 0x48, 0x3c, 0x20,
    0x4d, 0xd0, 0xe3, 0xe8, 0x4c, 0xc3, 0x16, 0x6e, 0x4b, 0x7f, 0xa2, 0x89, 0x0d, 0x64, 0xa5, 0x94,
    0x6e, 0xa2, 0x76, 0x72, 0x6c, 0x48, 0x7a, 0xb8, 0x5d, 0x27, 0xbd, 0x10, 0xdd, 0x84, 0x94, 0x01,
};

/* Матрица, обратная к матрице преобразования L. */
u32 inversedLTransformationMatrix[16][16] = {
    0x01, 0x94, 0x84, 0xdd, 0x10, 0xbd, 0x27, 0x5d, 0xb8, 0x7a, 0x48, 0x6c, 0x72, 0x76, 0xa2, 0x6e,
    0x94, 0xa5, 0x64, 0x0d, 0x89, 0xa2, 0x7f, 0x4b, 0x6e, 0x16, 0xc3, 0x4c, 0xe8, 0xe3, 0xd0, 0x4d,
    0x20, 0x3c, 0x48, 0xf8, 0x48, 0x48, 0xc8, 0x8e, 0x2a, 0xf5, 0x02, 0xdd, 0x14, 0x30, 0x44, 0x8e,
    0x85, 0x44, 0xdf, 0x52, 0x7f, 0xc6, 0x98, 0x60, 0xd4, 0x52, 0x0e, 0x65, 0x07, 0x9f, 0x86, 0xea,
    0x10, 0xd1, 0xd3, 0x91, 0x91, 0xfe, 0xf3, 0x01, 0xb1, 0x78, 0x58, 0x01, 0x49, 0x6b, 0x2d, 0xa9,
    0xc2, 0x8d, 0x31, 0x64, 0xec, 0xeb, 0x0f, 0x2a, 0x37, 0x99, 0x90, 0xc4, 0xf6, 0x30, 0xb8, 0xf6,
    0xc0, 0xb4, 0xa6, 0xff, 0x39, 0x2f, 0x54, 0x6c, 0xaf, 0xeb, 0xe1, 0xd4, 0xd7, 0x63, 0x64, 0xbf,
    0x01, 0x54, 0x30, 0x7b, 0xef, 0x84, 0x08, 0x09, 0xd4, 0xd5, 0xa3, 0x8d, 0xa6, 0xa1, 0xc1, 0x0a,
    0xfb, 0xde, 0xe0, 0xaf, 0x10, 0xc9, 0xf6, 0x49, 0xbe, 0xe7, 0x6e, 0xa4, 0x6a, 0x2b, 0x9c, 0xf3,
    0x01, 0x6f, 0x5a, 0x3d, 0xbf, 0xad, 0xee, 0xab, 0xf1, 0xc4, 0xaf, 0x02, 0xd6, 0x1c, 0x89, 0xf2,
    0xc0, 0x77, 0x44, 0x94, 0x60, 0x7c, 0x12, 0x8d, 0x2e, 0x2d, 0xbc, 0xeb, 0x11, 0x43, 0x48, 0x8e,
    0xc2, 0x5d, 0x97, 0xf3, 0xe9, 0x1a, 0x8d, 0xcb, 0xbb, 0x06, 0xc5, 0x20, 0x1c, 0x68, 0x90, 0x93,
    0x10, 0x96, 0xca, 0xd9, 0x30, 0x68, 0x2f, 0x14, 0x1a, 0x17, 0x0c, 0xca, 0x0c, 0x70, 0xda, 0xbf,
    0x85, 0x74, 0x75, 0xd0, 0x5e, 0xbe, 0xb8, 0x87, 0x4e, 0x62, 0xec, 0x6b, 0x10, 0x87, 0xc6, 0x74,
    0x20, 0x2d, 0x99, 0xe9, 0x95, 0x9f, 0xd4, 0x49, 0xe6, 0xd5, 0x76, 0xf2, 0x33, 0xc8, 0x20, 0x98,
    0x94, 0x84, 0xdd, 0x10, 0xbd, 0x27, 0x5d, 0xb8, 0x7a, 0x48, 0x6c, 0x72, 0x76, 0xa2, 0x6e, 0xcf,
};




u64 galua_modul = 451; // 256 + 128 + 64 + 2 + 1
u32 nonlinear_transform_perm_inv[256];

u32 smart_multi[256 * 256];

//calculate multiply in Galua field
u64 multiplication(u64 a, u64 b) {
    u64 c = 0;
    //i - current bit
    for(u64 i = 1; i < 256; i <<=1, b <<=1) {
        if (a & i) c ^= b; //multiplication
    }
    for(u64 i = 256 * 128, mod = galua_modul << 7; i >= 256; i >>= 1, mod >>= 1) {
        if (c & i) c ^= mod; //modul
    }
    return c;
}

struct Solution {

    struct Bytes {
        u32* bytes;
        static const u32 len_block = 16;

        Bytes(size_t len) {
            bytes = new u32[len];
        }

        Bytes() : Bytes(len_block) {}

        ~Bytes() {
            delete[] bytes;
        }
        void fill_with_zeros() {
            for(u64 i = 0; i < len_block; ++i) {
                bytes[i] = 0;
            }
        }


        /*u32 get_linear_transform() const {
            u64 ans = 0; //because can oversize
            for(u64 i = 0; i < len_block; ++i) {
                ans ^= smart_multi[linear_transform_perm[i] * 256 + bytes[i]];
            }
            return ans;
        }

        //linear block
        void linear_transform() {
            u32 ans = get_linear_transform();
            for(u64 i = len_block - 1; i > 0; --i) {
                bytes[i] = bytes[i - 1];
            }
            bytes[0] = ans;
        }*/

        void make_linear_transform() {
            /*for(u64 i = 0; i < len_block; ++i) {
                linear_transform();
            }*/
            Bytes new_bytes;
            for(u64 i = 0; i < 16; ++i) {
                new_bytes.bytes[i] = smart_multi[bytes[0] * 256 + LTransformationMatrix[0][i]];
                for(u64 j = 1; j < 16; ++j) {
                    new_bytes.bytes[i] ^= smart_multi[bytes[j] * 256 + LTransformationMatrix[j][i]];
                }
            }
            ((uint64_t*) bytes)[0] = ((uint64_t*) new_bytes.bytes)[0];
            ((uint64_t*) bytes)[1] = ((uint64_t*) new_bytes.bytes)[1];
        }

        //inverse linear block
        /*void linear_transform_inv() {
            u32 ans = bytes[0];
            for(u64 i = 0; i < len_block; ++i) {
                bytes[i] = bytes[i + 1];
            }
            bytes[15] = ans;
            bytes[15] = get_linear_transform();
        }*/

        void make_linear_transform_inv() {
            /*for(u64 i = 0; i < len_block; ++i) {
                linear_transform_inv();
            }*/
            Bytes new_bytes;
            for(u64 i = 0; i < 16; ++i) {
                new_bytes.bytes[i] = smart_multi[bytes[0] * 256 + inversedLTransformationMatrix[0][i]];
                for(u64 j = 1; j < 16; ++j) {
                    new_bytes.bytes[i] ^= smart_multi[bytes[j] * 256 + inversedLTransformationMatrix[j][i]];
                }
            }
            ((uint64_t*) bytes)[0] = ((uint64_t*) new_bytes.bytes)[0];
            ((uint64_t*) bytes)[1] = ((uint64_t*) new_bytes.bytes)[1];
        }

        //nonlinear block
        void nonlinear_transform() {
            for(u64 i = 0; i < len_block; ++i) {
                bytes[i] = nonlinear_transform_perm[bytes[i]];
            }
        }

        //nonlinear inverse block
        void nonlinear_transform_inv() {
            for(u64 i = 0; i < len_block; ++i) {
                bytes[i] = nonlinear_transform_perm_inv[bytes[i]];
            }
        }

        void my_xor(const Bytes& byt) {
            /*for(u64 i = 0; i < len_block; ++i) {
                bytes[i] ^= byt.bytes[i];
            }*/
            ((uint64_t*) bytes)[0] ^= ((uint64_t*) byt.bytes)[0];
            ((uint64_t*) bytes)[1] ^= ((uint64_t*) byt.bytes)[1];
        }

        //encode decode
        void encode(const std::vector<Bytes>& keys) {
            my_xor(keys[0]);
            for(u64 i = 1; i < 10; ++i) {
                nonlinear_transform();
                make_linear_transform();
                my_xor(keys[i]);
            }
        }

        void decode(const std::vector<Bytes>& keys) {
            my_xor(keys[9]);
            for(u64 i = 8; i <= 8; --i) {
                make_linear_transform_inv();
                nonlinear_transform_inv();
                my_xor(keys[i]);
            }
        }

        inline char to_char(u32 num) const {
            if (num < 10) return num + '0';
            return num + 'a' - 10;
        }

        void print() const {
            for(u64 i = 0; i < len_block; ++i) {
                std::cout << to_char(bytes[i] >> 4);
                std::cout << to_char(bytes[i] & 15);
            }
            std::cout << '\n';
        }
        
        std::string toStr() const {
            std::string s;
            for(u64 i = 0; i < len_block; ++i) {
                s += to_char(bytes[i] >> 4);
                s += to_char(bytes[i] & 15);
            }
            return s;
        }
    };

    std::vector<Bytes> keys;
    Bytes* initial_constants;

    inline u32 get_dig(char x) const {
        if ('0' <= x && x <= '9') return x - '0';
        return x - 'a' + 10;
    }

    void hex_to_bytes(const std::string& s, Bytes& first, Bytes& second) const {
        //u64 len = 32;

        for(u64 i = 0; i < 16; ++i) {
            first.bytes[i] = get_dig(s[2 * i]) << 4;
            first.bytes[i] += get_dig(s[2 * i + 1]);
        }
        for(u64 i = 16; i < 32; ++i) {
            second.bytes[i - 16] = get_dig(s[2 * i]) << 4;
            second.bytes[i - 16] += get_dig(s[2 * i + 1]);
        }
    }

    void hex_to_bytes(const std::string& s, Bytes& first) const {
        //u64 len = 16;

        for(u64 i = 0; i < 16; ++i) {
            first.bytes[i] = get_dig(s[2 * i]) << 4;
            first.bytes[i] += get_dig(s[2 * i + 1]);
        }
    }

    void init_smart_multy() const {
        for(u64 i = 0; i < 256; ++i) {
            for (u64 j = 0; j < 256; ++j) {
                smart_multi[i * 256 + j] = multiplication(i, j);
            }
            
        }
    }

    void init_reverse() {
        for (size_t i = 0; i < 256; ++i) {
            nonlinear_transform_perm_inv[nonlinear_transform_perm[i]] = i;
        }
    }

    void init_bytes() {
        for(u64 i = 0; i < 32; ++i) {
            Bytes& bytes = initial_constants[i];
            bytes.fill_with_zeros();
            bytes.bytes[15] = i + 1;
            bytes.make_linear_transform();
        }
    }

    //key generation
    void keys_iteration(Bytes& key1, Bytes& key2, u32 const_num) const {
        u32 buffer[16];
        memcpy(buffer, key1.bytes, 16);
        key1.my_xor(initial_constants[const_num]);
        key1.nonlinear_transform();
        key1.make_linear_transform();
        key1.my_xor(key2);
        memcpy(key2.bytes, buffer, 16);
    }

    void generate_next_keys(const Bytes& key1, const Bytes& key2, Bytes& key3, Bytes& key4, u64 pair_num) const {
        memcpy(key3.bytes, key1.bytes, 16);
        memcpy(key4.bytes, key2.bytes, 16);
        for(u64 i = 0; i < 8; ++i) {
            keys_iteration(key3, key4, pair_num * 8 + i);
        }
    }


    void init_keys(const std::string& initial_key) {
        hex_to_bytes(initial_key, keys[0], keys[1]);
        for(u64 i = 0; i < 4; ++i) {
            generate_next_keys(keys[2 * i], keys[2 * i + 1], keys[2 * i + 2], keys[2 * i + 3], i);
        }
    }


    std::string encode(const std::string& s) const {
        Bytes b;
        hex_to_bytes(s, b);
        b.encode(keys);
        b.print();
        return b.toStr();
    }

    std::string decode(const std::string& s) const {
        Bytes b;
        hex_to_bytes(s, b);
        b.decode(keys);
        b.print();
        return b.toStr();
    }

    void encode_for_test(const std::string& s) const {
        Bytes b;
        hex_to_bytes(s, b);
        clock_t start = clock();
        for(u64 i = 0; i < 1024 * 64 * 100; ++i) {
            b.encode(keys);
        }
        std::cout << ((double)clock() - start)/CLOCKS_PER_SEC;
    }

    void decode_for_test(const std::string& s) const {
        Bytes b;
        hex_to_bytes(s, b);
        clock_t start = clock();
        for(u64 i = 0; i < 1024 * 64 * 100; ++i) {
            b.decode(keys);
        }
        std::cout << ((double)clock() - start)/CLOCKS_PER_SEC << '\n';
    }



    Solution(const std::string& initial_key): keys(10) {
        initial_constants = new Bytes[32];
        init_smart_multy();
        init_reverse();
        init_bytes();
        init_keys(initial_key);
    }

    ~Solution() {
        delete[] initial_constants;
    }

};
