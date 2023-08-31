#include "hookapi.h"

/**
 * @invoke ttPAYMENT ttACCOUNT_SET
 * @field admin
 */

int64_t hook(uint32_t reserved)
{
    // check for the presence of a memo
    uint8_t memos[2048];
    int64_t memos_len = otxn_field(SBUF(memos), sfMemos);

    uint32_t payload_len = 0, signature_len = 0, publickey_len = 0;
    uint8_t* payload_ptr = 0, *signature_ptr = 0, *publickey_ptr = 0;
    if (memos_len <= 0)
       rollback(SBUF("ZK: Not provided."), 0);
        
    /**
     * 'Signed Memos' for hooks are supplied in triples in the following 'default' format:
     * NB: The +1 identifies the payload, you may provide multiple payloads
     * Memo: { MemoData: <app data>,   MemoFormat: "signed/payload+1",   MemoType: [application defined] }
     * Memo: { MemoData: <signature>,  MemoFormat: "signed/signature+1", MemoType: [application defined] }
     * Memo: { MemoData: <public_key>, MemoFormat: "signed/publickey+1", MemoType: [application defined] }
     **/

    // loop through the three memos (if 3 are even present) to parse out the relevant fields
    for (int i = 0; GUARD(3), i < 3; ++i)
    {
        // the memos are presented in an array object, which we must index into
        int64_t memo_lookup = sto_subarray(memos, memos_len, i);

        TRACEVAR(memo_lookup);
        if (memo_lookup < 0)
            rollback(SBUF("Blacklist: Memo transaction did not contain correct format."), 30);

        // if the subfield/array lookup is successful we must extract the two pieces of returned data
        // which are, respectively, the offset at which the field occurs and the field's length
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
        uint8_t* data_ptr = SUB_OFFSET(data_lookup) + memo_ptr;
        uint32_t data_len = SUB_LENGTH(data_lookup);

        uint8_t* format_ptr = SUB_OFFSET(format_lookup) + memo_ptr;
        uint32_t format_len = SUB_LENGTH(format_lookup);

        // we can use a helper macro to compare the format fields and determine which MemoData is assigned
        // to each pointer. Note that the last parameter here tells the macro how many times we will hit this
        // line so it in turn can correctly configure its GUARD(), otherwise we will get a guard violation
        int is_payload = 0, is_signature = 0, is_publickey = 0;
        BUFFER_EQUAL_STR_GUARD(is_payload, format_ptr, format_len,   "signed/payload+1", 3);
        BUFFER_EQUAL_STR_GUARD(is_signature, format_ptr, format_len, "signed/signature+1", 3);
        BUFFER_EQUAL_STR_GUARD(is_publickey, format_ptr, format_len, "signed/publickey+1", 3);

        // assign the pointers according to the detected MemoFormat
        if (is_payload)
        {
            payload_ptr = data_ptr;
            payload_len = data_len;
        } else if (is_signature)
        {
            signature_ptr = data_ptr;
            signature_len = data_len;
        } else if (is_publickey)
        {
            publickey_ptr = data_ptr;
            publickey_len = data_len;
        }
    }

     // check the signature is valid
     if (!util_verify(payload_ptr,    payload_len,
                      signature_ptr,  signature_len,
                      publickey_ptr,  publickey_len))
         rollback(SBUF("ZK Verif: Invalid signature in memo."), 60);

  

    accept (0,0,0); 

    _g(1,1);   // every hook needs to import guard function and use it at least once
    // unreachable
    return 0;
}
