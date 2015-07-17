#include <node.h>
#include <nan.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>

using namespace v8;

int bits = 2048;
unsigned long exponent = 65537;

NAN_METHOD(generate_rsa_keypair) {
  NanScope();

  RSA *rsa;
  BIO *bio;
  char *raw;
  long rawLength;

  Local<Object> buffer;
  Local<Object> result = NanNew<Object>();

  /* Generate key */

  rsa = RSA_generate_key(bits, exponent, NULL, NULL);

  if (rsa == NULL) {
    NanThrowError("Failed to generate key");
  }

  /* Allocate BIO */

  bio = BIO_new(BIO_s_mem());

  if (bio == NULL) {
    NanThrowError("Failed to create bio");
  }

  /* Dump private key */

  if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
    BIO_vfree(bio);
    NanThrowError("Failed to write private key");
  }

  rawLength = BIO_get_mem_data(bio, &raw);
  result->Set(NanNew<String>("private"), NanNew<String>(raw, rawLength));

  /* Reallocate BIO */

  BIO_vfree(bio);
  bio = BIO_new(BIO_s_mem());

  if (bio == NULL) {
    NanThrowError("Failed to create bio");
  }

  /* Dump public key */

  if (!PEM_write_bio_RSA_PUBKEY(bio, rsa)) {
    BIO_vfree(bio);
    NanThrowError("Failed to write public key");
  }

  rawLength = BIO_get_mem_data(bio, &raw);
  result->Set(NanNew<String>("public"), NanNew<String>(raw, rawLength));

  /* Return values */

  NanReturnValue(result);
}

void Initialize(v8::Handle<v8::Object> exports) {
  exports->Set(NanNew("generateRSAKeypair"), NanNew<v8::FunctionTemplate>(generate_rsa_keypair)->GetFunction());
}

NODE_MODULE(generate_rsa_keypair, Initialize)
