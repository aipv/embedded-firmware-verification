#!/bin/bash

set -e

MQTT_HOST=${MQTT_HOST:-192.168.0.242}

MQTT_PORT=${MQTT_PORT:-1883}

OTA_PORT=${OTA_PORT:-8080}

OTA_IMAGE=${1:-build/signed_firmware.bin}

TOPIC="/esp32/ota"

IMAGE_NAME=$(basename "$OTA_IMAGE")

IP=$(hostname -I | awk '{print $1}')

OTA_URL="http://$IP:$OTA_PORT/$IMAGE_NAME"

echo "[INFO] Sending OTA trigger"
echo "[INFO] MQTT Broker : $MQTT_HOST:$MQTT_PORT"
echo "[INFO] OTA URL     : $OTA_URL"

mosquitto_pub \
    -h "$MQTT_HOST" \
    -p "$MQTT_PORT" \
    -t "$TOPIC" \
    -m "$OTA_URL"

echo "[INFO] OTA trigger sent."
