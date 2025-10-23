#!/usr/bin/env python3
"""
Encrypt interviewer guide files before committing to git.
Usage: python encrypt_interviewer_files.py
"""

import os
import sys
from pathlib import Path

# XOR-based encryption (simple but effective for obfuscation)
def encrypt_decrypt(data: bytes, key: str) -> bytes:
    """XOR encryption/decryption (same operation for both)"""
    key_bytes = key.encode('utf-8')
    key_len = len(key_bytes)
    return bytes([data[i] ^ key_bytes[i % key_len] for i in range(len(data))])

def process_file(filepath: Path, password: str, output_suffix: str):
    """Encrypt a file and save with .encrypted extension"""
    try:
        with open(filepath, 'rb') as f:
            plaintext = f.read()
        
        encrypted = encrypt_decrypt(plaintext, password)
        
        output_file = filepath.with_suffix(filepath.suffix + output_suffix)
        with open(output_file, 'wb') as f:
            f.write(encrypted)
        
        print(f"✓ Encrypted: {filepath.name} -> {output_file.name}")
        return True
    except Exception as e:
        print(f"✗ Error encrypting {filepath.name}: {e}")
        return False

def main():
    # Files to encrypt
    files_to_encrypt = [
        'FILE_GUIDE.md',
        'INTERVIEWER_GUIDE.md',
        'PROJECT_SUMMARY.md',
        'QUICK_REFERENCE.md',
        'SOLUTION.md',
        'START_HERE.md'
    ]
    
    # Get password from user
    print("=" * 60)
    print("Interviewer Files Encryption")
    print("=" * 60)
    password = input("Enter encryption password: ").strip()
    
    if not password:
        print("Error: Password cannot be empty")
        sys.exit(1)
    
    print(f"\nEncrypting {len(files_to_encrypt)} files...\n")
    
    script_dir = Path(__file__).parent
    success_count = 0
    
    for filename in files_to_encrypt:
        filepath = script_dir / filename
        if filepath.exists():
            if process_file(filepath, password, '.encrypted'):
                success_count += 1
        else:
            print(f"⚠ Warning: {filename} not found, skipping")
    
    print(f"\n{'=' * 60}")
    print(f"Encrypted {success_count}/{len(files_to_encrypt)} files")
    print(f"{'=' * 60}")
    
    if success_count > 0:
        print("\nNext steps:")
        print("1. Review the .encrypted files")
        print("2. Delete the original files (or add to .gitignore)")
        print("3. Commit the .encrypted files to git")
        print("4. Use decrypt_interviewer_files.py to restore them")

if __name__ == '__main__':
    main()
