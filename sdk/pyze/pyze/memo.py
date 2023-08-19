import binascii

from xrpl.models import Memo


def string_to_hex(s):
    if isinstance(s, str):
        return binascii.hexlify(s.encode()).decode()

    return binascii.hexlify(s).decode()


def generate_verification_memos():
    return [
        Memo.from_dict({
            "memo_data": string_to_hex("data"),
            "memo_type": string_to_hex("type/proof")
        }),
        Memo.from_dict({
            "memo_data": string_to_hex("verification_key"),
            "memo_type": string_to_hex("type/verification_key")
        }),
        Memo.from_dict({
            "memo_data": string_to_hex("public_data"),
            "memo_type": string_to_hex("type/public_data")
        }),
    ]