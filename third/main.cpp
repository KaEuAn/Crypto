#include "../kuznechik/kuznechik.cpp"

std::string D[] = {
    {char(0x80), char(0x81), char(0x82), char(0x83), char(0x84), char(0x85), char(0x86), 
    char(0x87), char(0x88), char(0x89), char(0x8A), char(0x8B), char(0x8C), char(0x8D), char(0x8E),
    char(0x8F)}
    ,{char(0x90), char(0x91), char(0x92), char(0x93), char(0x94), char(0x95), char(0x96),
    char(0x97), char(0x98), char(0x99), char(0x9A), char(0x9B), char(0x9C), char(0x9D), char(0x9E), char(0x9F)} 
  };


struct CTR_ACPKM {
    int N;
    int s;
    int IV;
    std::string start_key;


    CTR_ACPKM(std::string start_key) : N(16), s(16), IV(0), start_key(start_key) {}

    std::string ACPKM(std::string key) {
        
        Solution sol(key);
        std::string ans('0', 0);
        for(int i = 0; i < 2; ++i) {
            std::string ss(D[i]);
            ans += sol.encode(ss);
        }
        return ans;
    }

    std::string Xor(std::string a, std::string b) {
        int N = a.size();
        std::string ans('0', N);
        for(int i = 0; i < N; ++i) {
            ans = a[i] ^ b[i];
        }
        return ans;
    }

    std::string encode(std::string& message) {
        int N = message.size();
        std::string key = ACPKM(key);
        std::string ans('0', 0);
        int q = N / s;
        int cur_s = 0;
        for(int i = 0; i < q; ++i, ++IV) {
            
            std::string CTR = std::to_string(IV) + std::string('0', N/2);
            while(CTR.size() < N) {
                CTR = "0" + CTR;
            }
            int end = std::min((i + 1) * s, N);
            std::string cur_mes(message.begin() + i * s, message.begin() + end);
            std::cout << 11111 << cur_mes;
            Solution sol(key);
            std::string enc = sol.encode(CTR);
            ans += Xor(cur_mes, enc);

            ++cur_s;

            if (cur_s * s >= N) {
                cur_s = 0;
                key = ACPKM(key);

            }
        }
        return ans;

    }

};

int main() {
    std::string start_key = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
    CTR_ACPKM sol1(start_key);
    std::string xxx = "dsafsdfl;ajdfoasjfdlsafjdlksafsdlfjskldfjksdlajgpiaghewfdsja;fohafpds;fhjdlsafhdopwafhdsa";
    std::cout << '\n' << "ans" << '\n' << sol1.encode(xxx);
    return 0;
}