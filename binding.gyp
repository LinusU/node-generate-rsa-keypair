{
  "targets": [{
    "target_name": "generate-rsa-keypair",
    "sources": [ "binding.cc" ],
    "include_dirs" : [
      "<!(node -e \"require('nan')\")",
      "<(node_root_dir)/deps/openssl/openssl/include"
    ]
  }]
}
