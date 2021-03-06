/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SIGNED_HDR_H
#define SIGNED_HDR_H

#include <inttypes.h>

enum shdr_img_type {
	SHDR_TA = 0,
	SHDR_TA_NOSIGN,
	SHDR_TA_SIGNED,
};

#define SHDR_MAGIC	0x4f545348

enum img_key_type {
	IMG_KEY_NONE = 0,
	IMG_KEY_AES256_ENCRYPT = 2,
};

#define TA_SIGN_RSA_PUB_KEY_SIZE	(256)
#define TA_SIGN_SHA_DIGEST_SIZE		(32)
#define TA_ENC_AES_KEY_SIZE			(32)
#define TA_ENC_AES_IV_SIZE			(16)
#define TA_ENC_AES_BLOCK_SIZE		(16)

/**
 * struct shdr - signed header
 * @magic:	magic number must match SHDR_MAGIC
 * @img_type:	image type, values defined by enum shdr_img_type
 * @img_size:	image size in bytes
 * @algo:	algorithm, defined by public key algorithms TEE_ALG_*
 *		from TEE Internal API specification
 * @hash_size:	size of the signed hash
 * @sig_size:	size of the signature
 * @hash:	hash of an image
 * @sig:	signature of @hash
 */
struct shdr {
	uint32_t magic;
	uint32_t version;
	uint32_t flags;
	uint32_t algo;
	uint32_t arb_cvn;
	uint32_t img_type;
	uint32_t img_size;
	uint32_t key_type;
	uint32_t marketid;
	uint32_t rsv[7];

	uint8_t nonce[TA_ENC_AES_IV_SIZE];
	uint8_t aes_key[TA_ENC_AES_KEY_SIZE];
	uint8_t aes_iv[TA_ENC_AES_IV_SIZE];

	/*
	 * Commented out element used to visualize the layout dynamic part
	 * of the struct.
	 *
	 * hash is accessed through the macro SHDR_GET_HASH and
	 * signature is accessed through the macro SHDR_GET_SIG
	 *
	 * uint8_t hash[hash_size];
	 * uint8_t sig[sig_size];
	 */
};

#define TA_HEADER_INFO_SIZE	(sizeof(struct shdr))
#define TA_HEADER_TOTAL_SIZE    (TA_HEADER_INFO_SIZE + TA_SIGN_SHA_DIGEST_SIZE +\
		3 * TA_SIGN_RSA_PUB_KEY_SIZE)

#define SHDR_GET_SIZE(x)	((((struct shdr *)(x))->img_type == \
		SHDR_TA_SIGNED)?(TA_HEADER_INFO_SIZE + TA_SIGN_SHA_DIGEST_SIZE +\
			3*TA_SIGN_RSA_PUB_KEY_SIZE) : (TA_HEADER_INFO_SIZE + TA_SIGN_SHA_DIGEST_SIZE))
#define SHDR_GET_HASH(x)	((uint8_t *)(x) + TA_HEADER_INFO_SIZE)
#define SHDR_GET_SIG(x)		((uint8_t *)(x) + TA_HEADER_INFO_SIZE +\
		TA_SIGN_SHA_DIGEST_SIZE)
#define SHDR_GET_PUB_KEY(x)	((uint8_t *)(x) + TA_HEADER_INFO_SIZE +\
		TA_SIGN_SHA_DIGEST_SIZE + TA_SIGN_RSA_PUB_KEY_SIZE)
#define SHDR_GET_SIG_KEY(x)	((uint8_t *)(x) + TA_HEADER_INFO_SIZE +\
		TA_SIGN_SHA_DIGEST_SIZE + TA_SIGN_RSA_PUB_KEY_SIZE + TA_SIGN_RSA_PUB_KEY_SIZE)
#endif /*SIGNED_HDR_H*/

