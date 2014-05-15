#include <bitcoin/bitcoin.hpp>
#include <wallet/wallet.hpp>
#include "util.hpp"

using namespace bc;
using namespace libwallet;

//
// FIXME: copy-pasted from libwallet/hd_keys.cpp
//

// long_hash is used for hmac_sha512 in libbitcoin
constexpr size_t half_long_hash_size = long_hash_size / 2;
typedef byte_array<half_long_hash_size> half_long_hash;

/**
 * Corresponds to a split HMAC-SHA256 result, as used in BIP 32.
 */
struct split_long_hash
{
    half_long_hash L;
    half_long_hash R;
};

static split_long_hash split(const long_hash& hmac)
{
    split_long_hash I;
    std::copy(hmac.begin(), hmac.begin() + half_long_hash_size, I.L.begin());
    std::copy(hmac.begin() + half_long_hash_size, hmac.end(), I.R.begin());
    return I;
}
//
// FIXME: copy-paste ends
//

int main(int argc, char** argv)
{
    std::string encoded_child_private_key = argv[1];
    std::string encoded_parent_public_key = argv[2];

    std::cout << encoded_child_private_key << std::endl;
    std::cout << encoded_parent_public_key << std::endl;

    hd_private_key child_private_key;
    hd_public_key  parent_public_key;

    child_private_key.set_serialized(encoded_child_private_key);
    parent_public_key.set_serialized(encoded_parent_public_key);

    hd_private_key parent_private_key;

    // FIXME
    data_chunk data;
    data.reserve(33 + 4);
    extend_data(data, parent_public_key.public_key());
    extend_data(data, to_big_endian(child_private_key.lineage().child_number));

    // FIXME: math not verified
    auto I = split(hmac_sha512_hash(data, to_data_chunk(parent_public_key.chain_code())));

    // FIXME: no ec_sub() yet
    // FIXME: math not verified
    //parent_private_key = ec_sub(child_private_key.private_key(), I.L);

    std::cout << parent_private_key.serialize() << std::endl;

    return 0;
}
