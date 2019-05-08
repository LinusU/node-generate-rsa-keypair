#include <assert.h>

#define NAPI_VERSION 1
#include <node_api.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>

#define BITS 2048
#define EXPONENT 65537

napi_value generate_rsa_keypair(napi_env env, napi_callback_info info) {
  RSA *rsa;
  BIO *bio;
  char *raw;
  long raw_length;

  napi_value result;
  assert(napi_create_object(env, &result) == napi_ok);

  /* Generate key */

  rsa = RSA_generate_key(BITS, EXPONENT, NULL, NULL);

  if (rsa == NULL) {
    assert(napi_throw_error(env, "RSA_generate_key", "Failed to generate key") == napi_ok);
    return NULL;
  }

  /* Allocate BIO */

  bio = BIO_new(BIO_s_mem());

  if (bio == NULL) {
    RSA_free(rsa);
    assert(napi_throw_error(env, "BIO_new", "Failed to create bio") == napi_ok);
    return NULL;
  }

  /* Dump private key */

  if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
    BIO_vfree(bio);
    RSA_free(rsa);
    assert(napi_throw_error(env, "PEM_write_bio_RSAPrivateKey", "Failed to write private key") == napi_ok);
    return NULL;
  }

  raw_length = BIO_get_mem_data(bio, &raw);

  napi_value private_string;
  assert(napi_create_string_utf8(env, raw, raw_length, &private_string) == napi_ok);
  assert(napi_set_named_property(env, result, "private", private_string) == napi_ok);

  /* Reallocate BIO */

  BIO_vfree(bio);
  bio = BIO_new(BIO_s_mem());

  if (bio == NULL) {
    RSA_free(rsa);
    assert(napi_throw_error(env, "BIO_new", "Failed to create bio") == napi_ok);
    return NULL;
  }

  /* Dump public key */

  if (!PEM_write_bio_RSA_PUBKEY(bio, rsa)) {
    BIO_vfree(bio);
    RSA_free(rsa);
    assert(napi_throw_error(env, "PEM_write_bio_RSA_PUBKEY", "Failed to write public key") == napi_ok);
    return NULL;
  }

  raw_length = BIO_get_mem_data(bio, &raw);

  napi_value public_string;
  assert(napi_create_string_utf8(env, raw, raw_length, &public_string) == napi_ok);
  assert(napi_set_named_property(env, result, "public", public_string) == napi_ok);

  /* Return values */

  BIO_vfree(bio);
  RSA_free(rsa);

  return result;
}

static napi_value Init(napi_env env, napi_value exports) {
  napi_value result;
  assert(napi_create_object(env, &result) == napi_ok);

  napi_value generate_rsa_keypair_fn;
  assert(napi_create_function(env, "generateRSAKeypair", NAPI_AUTO_LENGTH, generate_rsa_keypair, NULL, &generate_rsa_keypair_fn) == napi_ok);
  assert(napi_set_named_property(env, result, "generateRSAKeypair", generate_rsa_keypair_fn) == napi_ok);

  return result;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
