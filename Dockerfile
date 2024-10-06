# Use an official Ubuntu base image
FROM ubuntu:latest

# Set a non-interactive frontend to avoid user interaction during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container to /app/Memify
WORKDIR /app/Memify

# Copy the entire project directory into the container
COPY . .

# Move the config.ini file one directory up from the current directory
RUN mv config.ini ../

# Run CMake to configure the project
RUN cmake .

# Build the project using make
RUN make

# Set the entrypoint to the Memify executable
ENTRYPOINT ["./Memify"]

# Set default arguments for the ENTRYPOINT
CMD ["default_secret_key", "default_port"]
