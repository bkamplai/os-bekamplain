//
//  main.swift
//  Symmetric Key
//
//  Created by Brandon Kamplain on 11/26/23.
//

import Foundation
import CryptoKit

func encrypt(_ plaintext: String, _ key: SymmetricKey) -> String? {
    guard let dataToEncrypt = plaintext.data(using: .utf8) else { return nil }
    
    do {
        let sealedBox = try AES.GCM.seal(dataToEncrypt, using: key)
        
        if let encryptedData = sealedBox.combined {
            return encryptedData.base64EncodedString()
        } else {
            print("Failed to get encrypted data")
            return nil
        }
    } catch {
        print("Encryption error: \(error)")
        return nil
    }
}

func decrypt(_ ciphertext: String, _ key: SymmetricKey) -> String? {
    guard let ciphertextData = Data(base64Encoded: ciphertext) else {
        print("Failed to convert Base64 string to Data")
        return nil
    }
    
    do {
        let sealedBox = try AES.GCM.SealedBox(combined: ciphertextData)
        let decryptedData = try AES.GCM.open(sealedBox, using: key)
        return String(data: decryptedData, encoding: .utf8)
    } catch {
        print("Decryption error: \(error)")
        return nil
    }
}

func stringToSymmetricKey(_ string: String) -> SymmetricKey {
    //  Convert string to data
    guard let data = string.data(using: .utf8) else {
        fatalError("String conversion to data failed")
    }
    
    //  Hash the data to get a fixed sized output
    let hash = SHA256.hash(data: data)
    
    //  Create a SymmetricKey from the hash
    return SymmetricKey(data: hash)
}

let stringKey = "OperatingSystemsDesign"
let key = stringToSymmetricKey(stringKey)
let plaintext = """
Create and use a symmetric key to encrypt and decrypt a message.  Save a conversation with ChatGPT on the tools you used and why it is secure.

Extra credit for using the OS specific api for these.  Save conversation, and code, and screenshot in github.

[you can use the openssl command line tools for this, but not for extra credit]
"""

if let encryptedMessage = encrypt(plaintext, key),
   let decryptedMessage = decrypt(encryptedMessage, key) {
    print("Original:\n\(plaintext)\n")
    print("Encrypted: \(encryptedMessage)\n")
    print("Decrypted:\n\(decryptedMessage)")
} else {
    print("Encryption or decryption failed.")
}
