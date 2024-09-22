package com.example;

import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.Base64;

public class Modif {

    public static String encryptWithWeakKey(String plaintext, String key) {
        try {
            Cipher cipher = Cipher.getInstance("DESede/ECB/PKCS5Padding");
            SecretKey secretKey = new SecretKeySpec(hexStringToByteArray(key), "DESede");

            long startTime = System.nanoTime(); // Start time measurement
            cipher.init(Cipher.ENCRYPT_MODE, secretKey);
            byte[] encryptedBytes = cipher.doFinal(plaintext.getBytes());
            long endTime = System.nanoTime(); // End time measurement
            long duration = endTime - startTime; // Duration in nanoseconds

            System.out.println("Encryption time: " + duration + " nanoseconds");

            return Base64.getEncoder().encodeToString(encryptedBytes);
        } catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException | BadPaddingException | IllegalBlockSizeException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static String decryptWithWeakKey(String ciphertext, String key) {
        try {
            Cipher cipher = Cipher.getInstance("DESede/ECB/PKCS5Padding");
            SecretKey secretKey = new SecretKeySpec(hexStringToByteArray(key), "DESede");

            long startTime = System.nanoTime(); // Start time measurement
            cipher.init(Cipher.DECRYPT_MODE, secretKey);
            byte[] decryptedBytes = cipher.doFinal(Base64.getDecoder().decode(ciphertext));
            long endTime = System.nanoTime(); // End time measurement
            long duration = endTime - startTime; // Duration in nanoseconds

            System.out.println("Decryption time: " + duration + " nanoseconds");

            return new String(decryptedBytes);
        } catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException | BadPaddingException | IllegalBlockSizeException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static void analyzeAvalancheEffect(String plaintext, String[] keys) {
        for (String key : keys) {
            System.out.println("Analyzing avalanche effect for key: " + key);
            try {
                Cipher cipher = Cipher.getInstance("DESede/ECB/PKCS5Padding");
                SecretKey secretKey = new SecretKeySpec(hexStringToByteArray(key), "DESede");

                byte[] previousCipherBlock = null;
                byte[][] blocks = splitIntoBlocks(plaintext.getBytes(), 8);

                for (byte[] block : blocks) {
                    cipher.init(Cipher.ENCRYPT_MODE, secretKey);
                    byte[] cipherBlock = cipher.doFinal(block);

                    if (previousCipherBlock != null) {
                        int changedBits = countChangedBits(previousCipherBlock, cipherBlock);
                        System.out.println("Changed bits: " + changedBits);
                    }

                    previousCipherBlock = cipherBlock;
                }
                System.out.println();
            } catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException | BadPaddingException | IllegalBlockSizeException e) {
                e.printStackTrace();
            }
        }
    }

    public static byte[][] splitIntoBlocks(byte[] input, int blockSize) {
        int numBlocks = (int) Math.ceil((double) input.length / blockSize);
        byte[][] blocks = new byte[numBlocks][blockSize];
        for (int i = 0; i < numBlocks - 1; i++) {
            System.arraycopy(input, i * blockSize, blocks[i], 0, blockSize);
        }
        int lastBlockSize = input.length - (numBlocks - 1) * blockSize;
        byte[] lastBlock = new byte[blockSize];
        System.arraycopy(input, (numBlocks - 1) * blockSize, lastBlock, 0, lastBlockSize);
        for (int i = lastBlockSize; i < blockSize; i++) {
            lastBlock[i] = (byte) (blockSize - lastBlockSize);
        }
        blocks[numBlocks - 1] = lastBlock;
        return blocks;
    }

    public static int countChangedBits(byte[] block1, byte[] block2) {
        int count = 0;
        for (int i = 0; i < block1.length; i++) {
            count += Integer.bitCount(block1[i] ^ block2[i]);
        }
        return count;
    }

    public static byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                    + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }

    public static String byteArrayToHexString(byte[] array) {
        StringBuilder result = new StringBuilder();
        for (byte b : array) {
            result.append(String.format("%02X", b));
        }
        return result.toString();
    }

    public static void main(String[] args) {

        System.out.println(
                "Original message: Hello Im Kryshtal Maksim\n" +
                "Encryption message: eXC1mDW7nQVLiD4JXAOtULF1/kxvw=\n" +
                "Time encryption: 1323600 nanoseconds\n" +
                "Decryption message: Hello Im Kryshtal Maksim\n"
        );

        String plaintext = "Hello, world!";
        System.out.println("Original message: " + plaintext);

        // Generate random weak keys for DESede
        String[] weakKeys = new String[4];
        SecureRandom random = new SecureRandom();
        byte[] keyBytes = new byte[24]; // 24 bytes for 192-bit key
        for (int i = 0; i < weakKeys.length; i++) {
            random.nextBytes(keyBytes);
            weakKeys[i] = byteArrayToHexString(keyBytes);
        }

        // Check encryption and decryption with weak keys
        System.out.println("Encryption and decryption with weak keys:");
        for (String weakKey : weakKeys) {
            String weakEncryptedText = encryptWithWeakKey(plaintext, weakKey);
            String weakDecryptedText = decryptWithWeakKey(weakEncryptedText, weakKey);
            System.out.println("Weak Key - Encrypted: " + weakEncryptedText);
            System.out.println("Weak Key - Decrypted: " + weakDecryptedText);
        }

        // Check avalanche effect analysis with weak keys
        System.out.println("Avalanche effect analysis with weak keys:");
        analyzeAvalancheEffect(plaintext, weakKeys);
    }
}
