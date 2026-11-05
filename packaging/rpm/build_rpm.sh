#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"

VERSION="0.1.1"
NAME="lumaedge"
TARBALL_NAME="${NAME}-${VERSION}.tar.gz"
# Use /tmp to avoid path issues with spaces in mount points (e.g. "New Volume")
BUILD_DIR="/tmp/lumaedge-rpmbuild"
OUTPUT_DIR="${ROOT_DIR}/build/rpm"

echo "=== Building ${NAME} v${VERSION} RPM Package ==="

# Clean and prepare rpmbuild tree
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
mkdir -p "${OUTPUT_DIR}"

# Create source tarball
echo "-> Creating source archive ${TARBALL_NAME}..."
git -C "${ROOT_DIR}" archive --format=tar.gz --prefix="${NAME}-${VERSION}/" -o "${BUILD_DIR}/SOURCES/${TARBALL_NAME}" HEAD

# Copy spec file
cp "${SCRIPT_DIR}/lumaedge.spec" "${BUILD_DIR}/SPECS/"

# Build RPM
echo "-> Running rpmbuild..."
rpmbuild --define "_topdir ${BUILD_DIR}" -ba "${BUILD_DIR}/SPECS/lumaedge.spec"

echo "-> Copying generated RPMs to ${OUTPUT_DIR}..."
cp "${BUILD_DIR}"/RPMS/*/*.rpm "${OUTPUT_DIR}/" 2>/dev/null || true
cp "${BUILD_DIR}"/SRPMS/*.rpm "${OUTPUT_DIR}/" 2>/dev/null || true

echo "=== RPM Build Successful! ==="
ls -lh "${OUTPUT_DIR}"/*.rpm
