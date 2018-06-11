/* eslint-env mocha */

const assert = require('assert')
const crypto = require('crypto')

const generateRSAKeypair = require('./')

const MESSAGE = Buffer.from('This is a test!')

describe('generate-rsa-keypair', function () {
  it('should generate a keypair', function () {
    const pair = generateRSAKeypair()

    const encrypted = crypto.publicEncrypt(pair.public, MESSAGE)
    const decrypted = crypto.privateDecrypt(pair.private, encrypted)

    assert.ok(decrypted.equals(MESSAGE))
  })
})
