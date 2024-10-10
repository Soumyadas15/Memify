# Use an official Ubuntu base image
FROM ubuntu:latest

# Set a non-interactive frontend to avoid user interaction during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    uuid-runtime \  # Install uuid-runtime for generating security keys
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

# Expose the application's default port
EXPOSE 6379

# Add the entrypoint script
COPY entrypoint.sh /app/entrypoint.sh

# Make the script executable
RUN chmod +x /app/entrypoint.sh

# Set the entrypoint to the script
ENTRYPOINT ["/app/entrypoint.sh"]


az ad sp create-for-rbac --name "memifyGithubActions" --role contributor \
  --scopes /subscriptions/9abb6a50-fb7c-4649-ab21-abb63bc3a574/resourceGroups/othema_group \
  --sdk-auth