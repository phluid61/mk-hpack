# Deployment

## Build Process

### Prerequisites

- GCC (or a compatible C compiler)
- GNU Make
- Ruby (for Huffman table generation)

For Debian packaging additionally:

- `debhelper` (compat level 13)
- `dpkg-buildpackage`

### Build commands

| Command | Artefacts |
|---------|-----------|
| `make` | `lib/libmkhpack.so.2.0.0`, `lib/libmkhpack.a`, `lib/mkhpack.h`, `lib/mkhpack.pc` |
| `make dist` | `mkhpack-2.0.0-src.tar.gz` (source tarball) |
| `make bdist` | `mkhpack-2.0.0.tar.gz` (binary tarball) |
| `dpkg-buildpackage -us -uc -b` | `libmkhpack1_*.deb`, `libmkhpack-dev_*.deb` |

### Installation

```sh
make install                        # installs to /usr/local
make install PREFIX=/usr            # installs to /usr
make install DESTDIR=/tmp/staging   # staged install for packaging
```

See [CONFIGURATION.md](CONFIGURATION.md) for a full description of install paths.

## CI/CD Pipelines

### Release workflow (`release.yml`)

**Trigger:** Push to `main` branch, excluding documentation-only changes
(`*.md`, `LICENSE`, `AGENTS.md`).

**Runner:** `ubuntu-latest`

**Steps:**

1. Checkout the repository
2. Install build dependencies (`build-essential`, `debhelper`, `ruby`)
3. Extract the `VERSION` from `Makefile`
4. Check whether a git tag for this version already exists — if so, skip the release
5. Build and test (`make check`)
6. Build source and binary tarballs (`make dist bdist`)
7. Build Debian packages (`dpkg-buildpackage -us -uc -b`)
8. Create an annotated git tag (`vX.Y.Z`)
9. Create a GitHub Release with auto-generated notes, attaching:

   - Source tarball
   - Binary tarball
   - `.deb` packages

**Concurrency:** The `release` group ensures only one release runs at a time;
in-progress releases are not cancelled.

### GitHub Pages workflow (`update-pages.yml`)

**Trigger:** Push to `main` affecting `README.md`, `LICENSE`, or
`code_of_conduct.md`; also available via manual `workflow_dispatch`.

**Runner:** `ubuntu-latest`

**Steps:**

1. Checkout the `gh-pages` branch
2. Fetch and overlay `README.md`, `LICENSE`, and `code_of_conduct.md` from `main`
3. If files changed, set up Ruby 3.3 and build the site (`bundle exec rake compile`)
4. Commit and push the updated `gh-pages` branch

## CI/CD Variables

### Release workflow

| Variable / Secret | Type | Description | Used In |
|-------------------|------|-------------|---------|
| `secrets.GITHUB_TOKEN` | secret | Automatic GitHub token for creating releases and tags | `release.yml` (Create tag, Create GitHub Release steps) |

No custom secrets or repository variables are required.

## Packaging

### Debian

The `debian/` directory provides packaging metadata for building `.deb` packages:

- **`libmkhpack1`** — runtime package containing `libmkhpack.so.*`
- **`libmkhpack-dev`** — development package containing headers, static library,
  development symlink, and pkg-config file

Key files:

| File | Purpose |
|------|---------|
| `debian/control` | Package metadata and dependencies |
| `debian/rules` | Build rules (delegates to `make lib`, `make install`, `make check`) |
| `debian/changelog` | Debian changelog |
| `debian/copyright` | Machine-readable copyright in DEP-5 format |
| `debian/libmkhpack1.install` | File list for the runtime package |
| `debian/libmkhpack-dev.install` | File list for the development package |
| `debian/source/format` | `3.0 (native)` |

### RPM

`mkhpack.spec` provides an RPM spec file with `mkhpack` and `mkhpack-devel`
sub-packages. Build requires: `gcc`, `make`, `ruby`.

### Tarballs

- **Source tarball** (`make dist`): includes all source files needed to build
  from scratch
- **Binary tarball** (`make bdist`): includes pre-built shared and static
  libraries, headers, and the pkg-config file

## Pre-Deployment Checklist

1. Update `VERSION` in `Makefile` if releasing a new version
2. Update `debian/changelog` with the new version entry
3. Run `make check` locally to verify all tests pass
4. Merge the `development` branch into `main`
5. Push to `main` — the release workflow handles the rest

## Rollback

To rollback a release:

1. Delete the GitHub Release via the web UI or `gh release delete vX.Y.Z`
2. Delete the git tag: `git push origin :refs/tags/vX.Y.Z`
3. Revert the merge commit on `main` if necessary
4. Push to `main` — if the `VERSION` still exists as a tag, the workflow will skip
