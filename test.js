/* eslint-env mocha */

var assert = require('assert')
var crypto = require('crypto')

var generateRSAKeypair = require('./')

const MESSAGE = new Buffer('This is a test!')

describe('generate-rsa-keypair', function () {
  it('should generate a keypair', function () {
    var pair = generateRSAKeypair()

    var encrypted = crypto.publicEncrypt(pair.public, MESSAGE)
    var decrypted = crypto.privateDecrypt(pair.private, encrypted)

    assert.ok(decrypted.equals(MESSAGE))
  })
})
