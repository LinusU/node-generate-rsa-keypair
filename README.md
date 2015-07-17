# generate-rsa-keypair

Generate an RSA keypair.

## Installation

```bash
npm install --save generate-rsa-keypair
```

## Usage

```javascript
var generateRSAKeypair = require('generate-rsa-keypair')

var pair = generateRSAKeypair()

console.log(pair.private) // String with the private key in PEM format
console.log(pair.public)  // String with the public key in PEM format
```

## API

### `generateRSAKeypair()`

Return a keypair, an object with two strings: `private` and `public`.

## License

MIT
