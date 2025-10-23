#!/usr/bin/env python3
"""
Decrypt interviewer guide files after pulling from git.
Usage: python decrypt_interviewer_files.py
"""

import os
import sys
from pathlib import Path

# XOR-based encryption (same operation for encryption and decryption)
def encrypt_decrypt(data: bytes, key: str) -> bytes:
    """XOR encryption/decryption (same operation for both)"""
    key_bytes = key.encode('utf-8')
    key_len = len(key_bytes)
    return bytes([data[i] ^ key_bytes[i % key_len] for i in range(len(data))])

def process_file(filepath: Path, password: str):
    """Decrypt a file and restore original"""
    try:
        with open(filepath, 'rb') as f:
            encrypted = f.read()
        
        decrypted = encrypt_decrypt(encrypted, password)
        
        # Remove .encrypted extension
        output_file = filepath.with_suffix('')
        with open(output_file, 'wb') as f:
            f.write(decrypted)
        
        print(f"✓ Decrypted: {filepath.name} -> {output_file.name}")
        return True
    except Exception as e:
        print(f"✗ Error decrypting {filepath.name}: {e}")
        return False

def main():
    print("=" * 60)
    print("Interviewer Files Decryption")
    print("=" * 60)
    
    # Get password from user
    password = input("Enter decryption password: ").strip()
    
    if not password:
        print("Error: Password cannot be empty")
        sys.exit(1)
    
    # Find all .encrypted files
    script_dir = Path(__file__).parent
    encrypted_files = list(script_dir.glob('*.encrypted'))
    
    if not encrypted_files:
        print("\nNo .encrypted files found in the current directory")
        sys.exit(0)
    
    print(f"\nFound {len(encrypted_files)} encrypted file(s)\n")
    
    success_count = 0
    for filepath in encrypted_files:
        if process_file(filepath, password):
            success_count += 1
    
    print(f"\n{'=' * 60}")
    print(f"Decrypted {success_count}/{len(encrypted_files)} files")
    print(f"{'=' * 60}")
    
    if success_count > 0:
        print("\n✓ Files decrypted successfully!")
        print("  You can now access the interviewer guides.")
    elif len(encrypted_files) > 0:
        print("\n✗ Decryption failed - wrong password?")
        sys.exit(1)

if __name__ == '__main__':
    main()
