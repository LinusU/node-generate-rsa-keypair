#include <node.h>
#include <nan.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>

using namespace v8;

int bits = 2048;
unsigned long exponent = 65537;

NAN_METHOD(generate_rsa_keypair) {
  RSA *rsa;
  BIO *bio;
  char *raw;
  long rawLength;

  Local<Object> buffer;
  Local<Object> result = Nan::New<Object>();

  /* Generate key */

  rsa = RSA_generate_key(bits, exponent, NULL, NULL);

  if (rsa == NULL) {
    return Nan::ThrowError("Failed to generate key");
  }

  /* Allocate BIO */

  bio = BIO_new(BIO_s_mem());

  if (bio == NULL) {
    return Nan::ThrowError("Failed to create bio");
  }

  /* Dump private key */

  if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
    BIO_vfree(bio);
    return Nan::ThrowError("Failed to write private key");
  }

  rawLength = BIO_get_mem_data(bio, &raw);
  result->Set(Nan::New<String>("private").ToLocalChecked(), Nan::New<String>(raw, rawLength).ToLocalChecked());

  /* Reallocate BIO */

  BIO_vfree(bio);
  bio = BIO_new(BIO_s_mem());

  if (bio == NULL) {
    return Nan::ThrowError("Failed to create bio");
  }

  /* Dump public key */

  if (!PEM_write_bio_RSA_PUBKEY(bio, rsa)) {
    BIO_vfree(bio);
    return Nan::ThrowError("Failed to write public key");
  }

  rawLength = BIO_get_mem_data(bio, &raw);
  result->Set(Nan::New<String>("public").ToLocalChecked(), Nan::New<String>(raw, rawLength).ToLocalChecked());

  /* Return values */

  info.GetReturnValue().Set(result);
}

void Initialize(v8::Handle<v8::Object> exports) {
  exports->Set(Nan::New("generateRSAKeypair").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(generate_rsa_keypair)->GetFunction());
}

NODE_MODULE(generate_rsa_keypair, Initialize)
