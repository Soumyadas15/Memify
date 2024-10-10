#!/bin/bash

# Define default values if environment variables are not set
DEFAULT_LINK="localhost"
DEFAULT_PORT="6379"
DEFAULT_SECURITY_KEY=$(uuidgen)

# Assign environment variables or defaults to configuration variables
LINK=${LINK:-$DEFAULT_LINK}
PORT=${PORT:-$DEFAULT_PORT}
SECURITY_KEY=${SECURITY_KEY:-$DEFAULT_SECURITY_KEY}

# Create or overwrite config.ini file
CONFIG_FILE="/app/config.ini"

echo "Creating or updating config.ini with the following values:"
echo "link = $LINK"
echo "port = $PORT"
echo "security_key = $SECURITY_KEY"

cat <<EOL > $CONFIG_FILE
link=$LINK
port=$PORT
security_key=$SECURITY_KEY
EOL

# Start the main application
echo "Starting Memify..."
exec ./Memify
