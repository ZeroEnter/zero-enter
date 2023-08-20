/**
 * This hook just accepts any transaction coming through it
 */
#include "hookapi.h"

int64_t hook(uint32_t reserved ) {

    TRACESTR("Accept.c: Called.");

    // check for the presence of a memo
    uint8_t memos[2048];
    int64_t memos_len = otxn_field(SBUF(memos), sfMemos);

    uint32_t proof_len = 0, verification_len = 0;
    uint8_t* proof_ptr = 0, *verification_ptr = 0;

    // Get proof
    int64_t proof_lookup = sto_subarray(memos, memos_len, 0);
    if (proof_lookup < 0)
    {
        rollback(SBUF("No proof"), 56);
    }
    else
    {
        proof_ptr = SUB_OFFSET(proof_lookup) + memos;
        proof_len = SUB_LENGTH(proof_lookup);
    }


    // Get verification key
    int64_t verification_lookup = sto_subarray(memos, memos_len, 1);
    if (verification_lookup < 0)
    {
        rollback(SBUF("No verification"), 58);
    }
    else
    {
        verification_ptr = SUB_OFFSET(verification_lookup) + memos;
        verification_len = SUB_LENGTH(verification_lookup);
    }


    int64_t result = ezkl_verify(SBUF("{SETTINGS.JSON}"), proof_ptr, proof_len, verification_ptr, verification_len);
    if (result < 1)
        rollback("Zero Proof Is Not Valid!", 16, 1);


    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}