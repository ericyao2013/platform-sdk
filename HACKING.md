# Hacking

This document summarizes guidelines and helpful options when hacking on the `airmapd` codebase.

## Selectively Redirecting APIs to Custom Host and Port

Occasionally, during local development, being able to selectively redirect requests to specific AirMap APIs is helpful.
For that, the following environment variables can be set:

 * `AIRMAP_PROTOCOL_AIRCRAFTS`, `AIRMAP_HOST_AIRCRAFTS`, `AIRMAP_PORT_AIRCRAFTS` and `AIRMAP_ROUTE_AIRCRAFTS` for API `aircrafts`
 * `AIRMAP_PROTOCOL_AIRSPACES`, `AIRMAP_HOST_AIRSPACES`, `AIRMAP_PORT_AIRSPACES` and `AIRMAP_ROUTE_AIRSPACES` for API `airspaces`
 * `AIRMAP_PROTOCOL_AUTHENTICATOR`, `AIRMAP_HOST_AUTHENTICATOR`, `AIRMAP_PORT_AUTHENTICATOR` and `AIRMAP_ROUTE_AUTHENTICATOR` for authentication purposes
 * `AIRMAP_PROTOCOL_FLIGHTS`, `AIRMAP_HOST_FLIGHTS`, `AIRMAP_PORT_FLIGHTS` and `AIRMAP_ROUTE_FLIGHTS` for API `flights`
 * `AIRMAP_PROTOCOL_PILOTS`, `AIRMAP_HOST_PILOTS`, `AIRMAP_PORT_PILOTS` and `AIRMAP_ROUTE_PILOTS` for API `pilots`
 * `AIRMAP_PROTOCOL_STATUS`, `AIRMAP_HOST_STATUS`, `AIRMAP_PORT_STATUS` and `AIRMAP_ROUTE_STATUS` for API `status`
 * `AIRMAP_PROTOCOL_SSO`, `AIRMAP_HOST_SSO` and `AIRMAP_PORT_SSO` for the SSO endpoint

An example invocation of `airmap` redirecting API flights to `localhost:3000`, routing via `/bla/blubb`, accessing without SSL would look like:
```
AIRMAP_PROTOCOL_FLIGHTS=http AIRMAP_HOST_FLIGHTS=localhost AIRMAP_PORT_FLIGHTS=3000 AIRMAP_ROUTE_FLIGHTS=/bla/blubb airmap test --test-suite=laanc.phoenix.zoo
```
