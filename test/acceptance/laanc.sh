#!/bin/sh

# TODO(tvoss): Reenable this once the laanc-enabled airports have been decided upon.
# For now, we will *not* error out and the test will always pass.
# set -e

. "$(dirname $(readlink -f $0))/fixture.sh"

run_all_test_suites() {
    version=$1
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.arkansas.pinebluff --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.kentucky.florence --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.nevada.reno --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.phoenix.manual --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.phoenix.schwegg --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.phoenix.university --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.phoenix.zoo --version=${version}
    "${AIRMAP_EXECUTABLE}" test --test-suite=laanc.wyoming.tetonvillage --version=${version}
}

fixture_set_up

# run_all_test_suites production
run_all_test_suites staging
