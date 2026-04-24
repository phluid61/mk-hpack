# Documentation Index

This directory contains the full documentation set for **mkhpack**, a C library
implementing HPACK header compression for HTTP/2 ([RFC 7541](https://www.rfc-editor.org/rfc/rfc7541.html)).

## Documentation Inventory

| Document | Description |
|----------|-------------|
| [DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md) | Prerequisites, project structure, build instructions, testing, and development workflow |
| [ARCHITECTURE.md](ARCHITECTURE.md) | System design, component relationships, and data flow |
| [DATABASE.md](DATABASE.md) | Data persistence (not applicable to this project) |
| [API.md](API.md) | Public C API — all exported functions with parameters and error codes |
| [BUSINESS_RULES.md](BUSINESS_RULES.md) | Encoding rules, validation constraints, and domain logic from RFC 7541 |
| [CONFIGURATION.md](CONFIGURATION.md) | Build-time configuration, compiler flags, and install paths |
| [DEPLOYMENT.md](DEPLOYMENT.md) | Build process, CI/CD pipelines, packaging, and installation |
| [DEPENDENCIES.md](DEPENDENCIES.md) | Build and runtime dependencies |
| [TROUBLESHOOTING.md](TROUBLESHOOTING.md) | Common build issues, test failures, and debugging guidance |

## Quick Navigation by Role

- **New contributor:** Start with [DEVELOPER_GUIDE.md](DEVELOPER_GUIDE.md), then [ARCHITECTURE.md](ARCHITECTURE.md)
- **Library consumer:** Start with [API.md](API.md), then the usage examples in the [README](../README.md)
- **Packager / release engineer:** Start with [DEPLOYMENT.md](DEPLOYMENT.md), then [CONFIGURATION.md](CONFIGURATION.md)
- **Debugging a problem:** Start with [TROUBLESHOOTING.md](TROUBLESHOOTING.md)

## External Resources

| Resource | URL |
|----------|-----|
| RFC 7541 — HPACK | [rfc-editor.org/rfc/rfc7541](https://www.rfc-editor.org/rfc/rfc7541.html) |
| Repository | [github.com/phluid61/mk-hpack](https://github.com/phluid61/mk-hpack) |
| Issue Tracker | [GitHub Issues](https://github.com/phluid61/mk-hpack/issues) |
| GitHub Pages | [phluid61.github.io/mk-hpack](https://phluid61.github.io/mk-hpack/) |
