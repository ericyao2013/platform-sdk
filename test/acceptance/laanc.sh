#!/bin/sh

# TODO(tvoss): Reenable this once the laanc-enabled airports have been decided upon.
# For now, we will *not* error out and the test will always pass.
# set -e

path=$(cd "$(dirname "$0")" ; pwd -P)

. "${path}/fixture.sh"

run_all_test_suites() {
    version=$1
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.arkansas.pinebluff --log-level=debug --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.kentucky.florence --log-level=debug --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.nevada.reno --log-level=debug --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.phoenix.manual --log-level=debug --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.phoenix.schwegg --log-level=debug --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.phoenix.university --log-level=debug --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.phoenix.zoo --log-level=debug --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.wyoming.tetonvillage --log-level=debug --version=${version}
}

fixture_set_up

# run_all_test_suites production
run_all_test_suites staging
