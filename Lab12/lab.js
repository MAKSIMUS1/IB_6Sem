const forge = require('node-forge');
const EC = require('elliptic').ec;
const { performance } = require('perf_hooks');

function generateRSAKeys() {
    const keypair = forge.pki.rsa.generateKeyPair({ bits: 2048, e: 0x10001 });
    const publicKeyPem = forge.pki.publicKeyToPem(keypair.publicKey);
    const privateKeyPem = forge.pki.privateKeyToPem(keypair.privateKey);

    return { publicKeyPem, privateKeyPem };
}

function signMessageRSA(message, privateKeyPem) {
    const privateKey = forge.pki.privateKeyFromPem(privateKeyPem);
    const md = forge.md.sha256.create();
    md.update(message, 'utf8');
    const signature = privateKey.sign(md);
    return signature;
}

function verifySignatureRSA(message, signature, publicKeyPem) {
    const publicKey = forge.pki.publicKeyFromPem(publicKeyPem);
    const md = forge.md.sha256.create();
    md.update(message, 'utf8');
    const verified = publicKey.verify(md.digest().bytes(), signature);
    return verified;
}


const ec = new EC('secp256k1'); // Можно выбрать другую кривую, если необходимо

function generateElGamalKeys() {
    const key = ec.genKeyPair();
    return {
        publicKey: key.getPublic('hex'),
        privateKey: key.getPrivate('hex')
    };
}

function generateSchnorrKeys() {
    const key = ec.genKeyPair();
    return {
        publicKey: key.getPublic('hex'),
        privateKey: key.getPrivate('hex')
    };
}

function signMessageElGamal(message, privateKeyHex) {
    const key = ec.keyFromPrivate(privateKeyHex, 'hex');
    const msgHash = forge.md.sha256.create().update(message).digest().toHex();
    const signature = key.sign(msgHash);
    return {
        r: signature.r.toString(16),
        s: signature.s.toString(16)
    };
}

function verifySignatureElGamal(message, signature, publicKeyHex) {
    const key = ec.keyFromPublic(publicKeyHex, 'hex');
    const msgHash = forge.md.sha256.create().update(message).digest().toHex();
    return key.verify(msgHash, signature);
}

function signMessageSchnorr(message, privateKeyHex) {
    const key = ec.keyFromPrivate(privateKeyHex, 'hex');
    const msgHash = forge.md.sha256.create().update(message).digest().toHex();
    const signature = key.sign(msgHash);
    return {
        r: signature.r.toString(16),
        s: signature.s.toString(16)
    };
}

function verifySignatureSchnorr(message, signature, publicKeyHex) {
    const key = ec.keyFromPublic(publicKeyHex, 'hex');
    const msgHash = forge.md.sha256.create().update(message).digest().toHex();
    return key.verify(msgHash, signature);
}

function measureExecutionTime(fn, ...args) {
    const start = performance.now();
    const result = fn(...args);
    const end = performance.now();
    const duration = end - start;
    return { result, duration };
}


const { publicKeyPem, privateKeyPem } = generateRSAKeys();
const message = 'Hello, World!';

// RSA
let { result: rsaSignature, duration: rsaSignTime } = measureExecutionTime(signMessageRSA, message, privateKeyPem);
console.log(`RSA sign time: ${rsaSignTime} ms`);

let { result: rsaVerification, duration: rsaVerifyTime } = measureExecutionTime(verifySignatureRSA, message, rsaSignature, publicKeyPem);
console.log(`RSA verify time: ${rsaVerifyTime} ms`);

// ElGamal
let { publicKey: elgamalPublicKey, privateKey: elgamalPrivateKey } = generateElGamalKeys();
let { result: elgamalSignature, duration: elgamalSignTime } = measureExecutionTime(signMessageElGamal, message, elgamalPrivateKey);
console.log(`ElGamal sign time: ${elgamalSignTime} ms`);

let { result: elgamalVerification, duration: elgamalVerifyTime } = measureExecutionTime(verifySignatureElGamal, message, elgamalSignature, elgamalPublicKey);
console.log(`ElGamal verify time: ${elgamalVerifyTime} ms`);

// Schnorr
let { publicKey: schnorrPublicKey, privateKey: schnorrPrivateKey } = generateSchnorrKeys();
let { result: schnorrSignature, duration: schnorrSignTime } = measureExecutionTime(signMessageSchnorr, message, schnorrPrivateKey);
console.log(`Schnorr sign time: ${schnorrSignTime} ms`);

let { result: schnorrVerification, duration: schnorrVerifyTime } = measureExecutionTime(verifySignatureSchnorr, message, schnorrSignature, schnorrPublicKey);
console.log(`Schnorr verify time: ${schnorrVerifyTime} ms`);
