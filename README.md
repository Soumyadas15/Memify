# Memify

  

Memify is a lightweight, high-performance in-memory data store implemented in C++. Inspired by Redis, Memify provides efficient key-value storage with additional features like HMAC signature verification for secure communications. This project was built from the ground up, including features such as chunk-based processing for large data handling and an LRU (Least Recently Used) caching mechanism.

  

## Features

  

-  **High-Performance Key-Value Storage**: Optimized for speed, made using C++.

-  **Security**: Implements HMAC signature verification on both the server and client to ensure security.

-  **Chunk-Based Processing**: Handles large data transfers by processing data in manageable chunks.

-  **LRU Cache**: Built-in support for Least Recently Used caching to manage memory and enhance performance.

  

## Getting Started

  

### Prerequisites

  

To build and run Memify, you will need:

  

- A C++ compiler that supports C++11 or later (e.g., GCC, Clang)

- OpenSSL library for secure key generation

- CMake for building the project

  

### Installation

  

1.  **Clone the Repository**

  

```bash

git clone https://github.com/Soumyadas15/memify.git

cd memify

```

  

2.  **Generate a Secure 32-Byte Key Using OpenSSL**

  

Memify requires a secure key for HMAC signature verification. Generate a 32-byte key using OpenSSL:

  

```bash

openssl rand -base64 32

```

  

Copy the generated key, which will be used in the `config.ini` file.

  

3.  **Create a `config.ini` File**

  

Create a `config.ini` file in the root directory of the project and add the following configuration:

  

```ini

[settings]

secret_key = "your-generated-32-byte-key"

port = "8080"

```

  

Replace `"your-generated-32-byte-key"` with the key generated in the previous step.

  

4.  **Build the Project**

  

```bash

mkdir build

cd build

cmake ..

make

```

  

5.  **Run the Server**

  

```bash

./Memify

```

  

### Configuration

  

The `config.ini` file is used to configure the server settings and security options. Ensure the paths to your TLS certificate and key files are correct, and update the `secret_key` with the secure key generated earlier.

  

### Usage

  

To interact with Memify, you can use any framework of your choice. In your native environment, it will run on 127.0.0.1

## COMMANDS:

As of now, Memify can process key-value data and Geospatial data. Also currently, you need to pass this data through a serializer.

### Key-Value
Memify can act like a key-value in memory database. You can set and get values by providing their keys. To set keys:

```bash
SET <key> <value> <duration>
```

Example:

```bash
SET name Soumya 200
```

This will create a key called 'name' and set it's value to Soumya. The data will be stored for 200 seconds.

Similarly to find a key,
```bash
GET <key>
```
Similarly to delete a key,
```bash
DELETE <key>
```


### Geo-spatial
Memify can act like a in memory database to compute geo-spatial data.

```bash
GEOSET <key> <value> <latitude> <longitude>
```

Example:

```bash
GEOSET location1 Kolkata 22.5744 88.3629
```

This will create a key called 'location1' and set it's name to "Kolkata". The coordinates would be stored as well. The data is stored on a 2-dimensional map. That means, you can add more values to the 'location1' key.

Upcoming commands:
```bash
GEOROUTE 
GEODISTANCE 
GEORADIUS 
GEOELEVATION
```

### Time-Series
Memify can act like a in memory database to compute time-series data.

Upcoming commands:
```bash
GEOROUTE 
GEODISTANCE 
GEORADIUS 
GEOELEVATION
```

## ADDITIONAL INFO:

If you're interested in contributing to the project, you can just fork the repo. If you're new to open source, check out [how to contribute to a project](https://docs.github.com/en/get-started/exploring-projects-on-github/contributing-to-a-project)