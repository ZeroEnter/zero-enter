/* tslint:disable */
/* eslint-disable */
export function vecU64ToFelt(a: number, b: number, c: number): void;
export function vecU64ToInt(a: number, b: number, c: number): void;
export function vecU64ToFloat(a: number, b: number, c: number): number;
export function floatToVecU64(a: number, b: number, c: number): void;
export function poseidonHash(a: number, b: number, c: number): void;
export function elgamalGenRandom(a: number, b: number, c: number): void;
export function elgamalEncrypt(a: number, b: number, c: number, d: number, e: number, f: number, g: number): void;
export function elgamalDecrypt(a: number, b: number, c: number, d: number, e: number): void;
export function verify(a: number, b: number, c: number, d: number, e: number, f: number, g: number, h: number): number;
export function prove(a: number, b: number, c: number, d: number, e: number, f: number, g: number, h: number, i: number, j: number, k: number): void;
export function init_panic_hook(): void;
export function rustsecp256k1_v0_8_1_context_create(a: number): number;
export function rustsecp256k1_v0_8_1_context_destroy(a: number): void;
export function rustsecp256k1_v0_8_1_default_illegal_callback_fn(a: number, b: number): void;
export function rustsecp256k1_v0_8_1_default_error_callback_fn(a: number, b: number): void;
export const memory: WebAssembly.Memory;
export function __wbindgen_add_to_stack_pointer(a: number): number;
export function __wbindgen_malloc(a: number, b: number): number;
export function __wbindgen_free(a: number, b: number, c: number): void;
export function __wbindgen_realloc(a: number, b: number, c: number, d: number): number;
export function __wbindgen_exn_store(a: number): void;
export function __wbindgen_thread_destroy(a: number, b: number): void;
export function __wbindgen_start(): void;
