# RSA Key Generator (C + GMP + OpenSSL)
Welcome to my first github project !
A custom RSA key pair generator written in C, using GMP/MPIR and OpenSSL.
Now this might look like reinventing the wheel but the sole purpose of this project was for me to have an understanding and feel for combining encryption algorithms with C programming. This is just for learning purposes only and should not be used in production systems. By using this, you acknowledge that it may contain vulnerabilites and are doing it at your own risk.

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
### Windows
- Make sure you're Windows 10/11 64-bit x86_64 arch
- Download and install MinGW-w64 and add the 'bin' folder to your system PATH
- Usually MinGW also installs GNU Make along with it, if not, download [GNU Make For Windows](https://gnuwin32.sourceforge.net/downlinks/make.php)
- Download [Git for Windows](https://github.com/git-for-windows/git/releases/download/v2.48.1.windows.1/Git-2.48.1-64-bit.exe) and make sure to check installing Git Bash
- Clone and set up vcpkg
  ```bash
  git clone https://github.com/microsoft/vcpkg.git
  cd vcpkg
  .\bootstrap-vcpkg.bat
  ```
- Install the following dependencies using vcpkg
  ```bash
  vcpkg install mpir:x64-windows
  vcpkg install openssl:x64-windows
  vcpkg install gmp:x64-windows
  ```
- Integrate vcpkg globally
  ```bash
  vcpkg integrate install
  ```
- Clone the RSA repository
  ```bash
  git clone https://github.com/sidhantsharma24/RSA.git
  ```
#### Build the project
- Use Git Bash for this step
  ```bash
  make windows VCPKG_DIR=/c/Users/YourUsername/vcpkg/installed/x64-windows
  ```
#### Run the program
```bash
cd build
./rsa_program.exe
```

### Linux (debian) 
- GCC (Linux) or MinGW (Windows)
- GMP or MPIR library installed
- OpenSSL development libraries installed
#### You might need to build MPIR from source
- [MPIR Library](https://github.com/wbhart/mpir)

#### Additional packages
```bash
sudo apt-get install libgmp-dev libssl-dev
```

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







