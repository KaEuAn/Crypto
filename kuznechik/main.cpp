#include "kuznechik.cpp"


void test_encode_decode() {
    std::string start_key = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
    std::string plain_text = "1122334455667700ffeeddccbbaa9988";
    std::string encoded_text = "7f679d90bebc24305a468d42b9d4edcd";
    Solution sol(start_key);
    sol.encode(plain_text);
    sol.decode(encoded_text);
}

void test_time() {
    std::string start_key = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
    std::string plain_text = "1122334455667700ffeeddccbbaa9988";
    std::string encoded_text = "7f679d90bebc24305a468d42b9d4edcd";
    Solution sol(start_key);
    sol.encode_for_test(plain_text);
    sol.decode_for_test(encoded_text);
}

int main() {
    std::cin.tie(0);
    std::cout.sync_with_stdio(false);
    test_encode_decode();
    //test_time();
    return 0;
}