# RSA Key Generator (C + GMP + OpenSSL)
Welcome to my first github project !
A custom RSA key pair generator written in C, using GMP/MPIR for big integer math and OpenSSL for PEM export. Supports configurable key sizes, passphrase-protected private keys, and CRT optimizations for efficient decryption.

##  Features

- Generate RSA public/private key pairs
- Configurable key sizes: 512, 1024, 2048, 3072, 4096 bits
- Outputs keys in standard **PEM** format (PKCS#1)
- Passphrase-protected private keys using AES-256-CBC
- Optimized with **Chinese Remainder Theorem (CRT) parameters**
- Compatible with **OpenSSL** tools
- Lightweight and fast (uses GMP/MPIR for big integer math)

##  Demo

```bash
$ ./build/rsa_program --outdir output
Enter key size (bits): 2048
[+] Generating 2048-bit RSA keypair...
[+] Exporting keys...
[+] RSA key validated successfully.
[+] Public key exported to output/public.pem
[+] Private key exported to output/private.pem
[+] RSA keypair generation complete. Keys are stored in 'output'.
```

---

##  Installation & Build
### Prerequisites 
- GCC (Linux) or MinGW (Windows)
- GMP or MPIR library installed
- OpenSSL development libraries installed
#### For Linux debian
```bash
sudo apt-get install libgmp-dev libssl-dev
```
#### For Windows
- Install MPIR and OpenSSL
- Add their paths to your compiler

## Build project
```bash
make
```
## Clean build files
```bash
make clean
```
## Usage
```bash
./build/rsa_program --outdir output
```

## Output
Keys will be exported in PEM format to the specified directory (or the default output/ directory).

| File  | Description |
| ------------- | ------------- |
| public.pem | RSA Public Key in PKCS#1 PEM format|
| private.pem | RSA Private Key in PKCS#1 PEM format, encrypted with AES-256-CBC and passphrase: passphrase123  |

### View keys
- Private Key
  ```bash
  openssl rsa -in output/private.pem -text -noout
  ```
- Public key
  ```bash
  openssl rsa -pubin -in output/public.pem -text -noout
  ```
## Author
Sidhant Sharma ([Github](https://github.com/sidhantsharma24))







