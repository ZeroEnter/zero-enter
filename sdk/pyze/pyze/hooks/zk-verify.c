/**
 * This hook just accepts any transaction coming through it
 */
#include "hookapi.h"

int64_t hook(uint32_t reserved ) {

    TRACESTR("Accept.c: Called.");

    // check for the presence of a memo
    uint8_t memos[2048];
    int64_t memos_len = otxn_field(SBUF(memos), sfMemos);

    uint32_t verification_format_len = 0, verification_data_len = 0;
    uint8_t* verification_format_ptr = 0, *verification_data_ptr = 0;

    // // Get proof
    // int64_t proof_lookup = sto_subarray(memos, memos_len, 0);
    // if (proof_lookup < 0)
    // {
    //     rollback(SBUF("No proof"), 56);
    // }
    // else
    // {
    //     proof_ptr = SUB_OFFSET(proof_lookup) + memos;
    //     proof_len = SUB_LENGTH(proof_lookup);
    // }


    // Get verification key
    int64_t memo_lookup = sto_subarray(memos, memos_len, 1);
    if (memo_lookup < 0)
    {
        rollback(SBUF("No verification"), 58);
    }
    else
    {
        uint8_t*  memo_ptr = SUB_OFFSET(memo_lookup) + memos;
        uint32_t  memo_len = SUB_LENGTH(memo_lookup);

         trace(SBUF("Memo: "), memo_ptr, memo_len, 1);

        // memos are nested inside an actual memo object, so we need to subfield
        // equivalently in JSON this would look like memo_array[i]["Memo"]
        memo_lookup = sto_subfield(memo_ptr, memo_len, sfMemo);
        memo_ptr = SUB_OFFSET(memo_lookup) + memo_ptr;
        memo_len = SUB_LENGTH(memo_lookup);

        // now we lookup the subfields of the memo itself
        // again, equivalently this would look like memo_array[i]["Memo"]["MemoData"], ... etc.
        int64_t data_lookup = sto_subfield(memo_ptr, memo_len, sfMemoData);
        int64_t format_lookup = sto_subfield(memo_ptr, memo_len, sfMemoFormat);

        TRACEVAR(data_lookup);
        TRACEVAR(format_lookup);

        // if any of these lookups fail the request is malformed
        if (data_lookup < 0 || format_lookup < 0)
            rollback(SBUF("Blacklist: Memo transaction did not contain correct memo format."), 40);

        // care must be taken to add the correct pointer to an offset returned by sub_array or sub_field
        // since we are working relative to the specific memo we must add memo_ptr, NOT memos or something else
        uint8_t* verification_data_ptr = SUB_OFFSET(data_lookup) + memo_ptr;
        uint32_t verification_data_len = SUB_LENGTH(data_lookup);

        uint8_t* verification_format_ptr = SUB_OFFSET(format_lookup) + memo_ptr;
        uint32_t verification_format_len = SUB_LENGTH(format_lookup);


    }




     int64_t result = ezkl_verify(SBUF("{SETTINGS.JSON}"), SBUF("{KEY.SGS}"),  SBUF("{PRIMARY_KEY}"), verification_format_ptr, verification_format_len, verification_data_ptr, verification_data_len);
     if (result < 1)
         rollback("Zero Proof Is Not Valid!", 16, 1);


    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}