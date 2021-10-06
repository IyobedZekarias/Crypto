
<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
<!-- [![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url] -->



<!-- PROJECT LOGO -->
<!-- <br />
<p align="center">
  <a href="https://github.com/github_username/repo_name">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Banned Software</h3>

  <p align="center">
    project_description
    <br />
    <a href="https://github.com/github_username/repo_name"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/github_username/repo_name">View Demo</a>
    ·
    <a href="https://github.com/github_username/repo_name/issues">Report Bug</a>
    ·
    <a href="https://github.com/github_username/repo_name/issues">Request Feature</a>
  </p>
</p> -->



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
        <a href="#usage">Usage</a>
        <ul>
            <li><a href="#Random">Random</a></li>
            <li><a href="#XOR">XOR</a></li>
            <li><a href="#AES">AES</a></li>
        </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <!-- <li><a href="#roadmap">Roadmap</a></li> -->
    <!-- <li><a href="#contributing">Contributing</a></li> -->
    <li><a href="#license">License</a></li>
    <!-- <li><a href="#contact">Contact</a></li> -->
    <!-- <li><a href="#acknowledgements">Acknowledgements</a></li> -->
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

<!-- [![Product Name Screen Shot][product-screenshot]](https://example.com) -->

<!-- Here's a blank template to get started:
**To avoid retyping too much info. Do a search and replace with your text editor for the following:**
`github_username`, `repo_name`, `twitter_handle`, `email`, `project_title`, `project_description` -->

This is a library for cryptographic tools that are being made during CS350V-03 Banned Software. All of the code was/is being written by Iyobed Zekarias for educational purposes. 


<!-- USAGE EXAMPLES -->
## Usage

#### Random

#### XOR
XOR is a weak cipher to secure a message using a key. XOR will, as the name signifies, xor a key and a binary file and store the results into an output file. For the best cipher the key and the message in the input file should both be the same length. For examle, if the binary file contains 0A 1F 4B the key should look something like 1B5CAF. For this example, the XOR algorithm will do these steps, 0A ⊕ 1B, 1F ⊕ 5C, then 4B ⊕ AF, which will give the result 11 43 e4.

There are four options for the XOR function 
1. Read from a file, XOR with the key, then write to a file
2. Read from a buffer, XOR with the key, then write to a file
3. Read from a buffer, XOR with the key, then write to a buffer
4. Read from a file, XOR with the key, then write to a buffer

##### Run
* Make the executable
 ```sh
 make xor
 ```
* To see a demo
 ```sh
 make p=xor demo
 ```

* To run
 ```sh
 ./xor <key> <input> <output>
 ```

#### AES
The Advanced Encryption Standard (AES) is a block cipher that was established by NIST in 2001. AES has been approved by the NSA for top secret information. This implementation of AES is using the 128 bit version which means it uses a 128 bit key to do the encrypting. This code uses the intel AES intrinsics that are built into most chips that intel makes. AES will only work with plaintext (data to encrypt) that has a length in bytes that is a multiple of 16. Therefore, the padding on this algorithm is that it finds the number of bytes that it needs to add for the message to have a lenght that is a multiple of 16, n, and appends the n repeating to the end of the message until the length is the required length. 

##### Steps in AES
###### Key Expansion
Round keys are derived from the key that was input into the system. The AES schedule is what is used to derive the round keys and for AES-128 10 round keys are made. 

###### Initial Round Key Addition
The state is combined with the first round key using a bitwise xor. 

###### 9 Rounds Process
1. SubBytes: Bytes are subsituted and replaced according to a lookup table
2. ShiftRows: Bytes are transposed and moved in the horizontal direction
3. MixColumns: Four bytes in each column are combined and mixed
4. AddRoundKey

###### Final Round 
1. SubBytes
2. ShiftRows
3. AddRoundKey
----Mix Columns is unnecessary in the final round

##### Run
###### AESECB
* Make the executable
 ```sh
 make aesecb
 ```
* To see a demo
 ```sh
 make file
 ./file 16 key
 ./file <preferred length of bytes> plaintext
 make p=aes demo
 ```

* To run
 ```sh
 ./aesecb <key file> <input file> <output file> <"encode" | "decode">
 ```

 ###### AESCBC
* Make the executable
 ```sh
 make aescbc
 ```
* To see a demo
 ```sh
 make file
 ./file 16 key
 ./file <preferred length of bytes> plaintext
 make p=aes demo
 ```

* To run
 ```sh
 ./aescbc <key file> <input file> <output file> <"encode" | "decode">
 ```

<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

You will need to have some c++ compiler. 
You can install the gnu gcc compiler on windows or clang on mac

#### Windows
Download WSL. Instructions here https://docs.microsoft.com/en-us/windows/wsl/install-win10
Open WSL and run the following commands
* gcc
 ```sh
 sudo apt-get update
 sudo apt-get install build-essential manpages-dev
 ```

* verify
 ```sh
 gcc --version
 ```

#### Mac
Open Terminal and run the following commands

* clang
 ```sh
 xcode-select --install
 ```

Follow installation directions

* verify
 ```sh
 clang --version
 ```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/Lipscomb-CCT/crypto-IyobedZekarias.git
   ```
2. Build
   ```sh
   make
   ```




<!-- ROADMAP -->
<!-- ## Roadmap

See the [open issues](https://github.com/github_username/repo_name/issues) for a list of proposed features (and known issues).
 -->


<!-- CONTRIBUTING -->
<!-- ## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request -->



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
<!-- ## Contact

Your Name - [@twitter_handle](https://twitter.com/twitter_handle) - email

Project Link: [https://github.com/github_username/repo_name](https://github.com/github_username/repo_name)
 -->


