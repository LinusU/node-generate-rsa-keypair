{
  "targets": [{
    "target_name": "generate_rsa_keypair",
    "sources": [ "binding.c" ],
    "include_dirs" : [ "<(node_root_dir)/deps/openssl/openssl/include" ]
  }]
}
