#!/bin/sh

set -e

config_pattern_staging="{
  \"credentials\": {
    \"api-key\": \"%s\",
    \"oauth\": {
      \"client-id\": \"%s\",
      \"device-id\": \"%s\",
      \"password\": \"%s\",
      \"username\": \"%s\"
    }
  },
  \"host\": \"api.airmap.com\",
  \"sso\": {
    \"host\": \"sso.airmap.io\",
    \"port\": 443
  },
  \"telemetry\": {
    \"host\": \"api-udp-telemetry.stage.airmap.com\",
    \"port\": 16060
  },
  \"traffic\": {
    \"host\": \"mqtt-stage.airmap.io\",
    \"port\": 8883
  },
  \"version\": \"staging\"
}
"

config_pattern_production="{
  \"credentials\": {
    \"api-key\": \"%s\",
    \"oauth\": {
      \"client-id\": \"%s\",
      \"device-id\": \"%s\",
      \"password\": \"%s\",
      \"username\": \"%s\"
    }
  },
  \"host\": \"api.airmap.com\",
  \"sso\": {
    \"host\": \"sso.airmap.io\",
    \"port\": 443
  },
  \"telemetry\": {
    \"host\": \"api.k8s.stage.airmap.com\",
    \"port\": 32020
  },
  \"traffic\": {
    \"host\": \"mqtt-prod.airmap.io\",
    \"port\": 8883
  },
  \"version\": \"production\"
}
"

fixture_set_up() {
    printenv

    if test "${CIRCLECI}" = "true"; then
        echo "setting up ci test environment"
        mkdir -p ~/.config/airmap/production || true
        mkdir -p ~/.config/airmap/staging || true

        printf "${config_pattern_production}" \
            "${AIRMAP_CIRCLECI_API_KEY}" \
            "${AIRMAP_CIRCLECI_CLIENT_ID}" \
            "${AIRMAP_CIRCLECI_DEVICE_ID}" \
            "${AIRMAP_CIRCLECI_PASSWORD}" \
            "${AIRMAP_CIRCLECI_USERNAME}" > ~/.config/airmap/production/config.json || true

        printf "${config_pattern_staging}" \
            "${AIRMAP_CIRCLECI_API_KEY}" \
            "${AIRMAP_CIRCLECI_CLIENT_ID}" \
            "${AIRMAP_CIRCLECI_DEVICE_ID}" \
            "${AIRMAP_CIRCLECI_PASSWORD}" \
            "${AIRMAP_CIRCLECI_USERNAME}" > ~/.config/airmap/staging/config.json || true
    fi

    "${AIRMAP_EXECUTABLE}" login --log-level=debug --version=production
    "${AIRMAP_EXECUTABLE}" login --log-level=debug --version=staging
}

