#!/bin/sh

set -e

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

    "${AIRMAP_EXECUTABLE}" login --version=production
    "${AIRMAP_EXECUTABLE}" login --version=staging
}

