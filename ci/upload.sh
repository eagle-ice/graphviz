#!/usr/bin/env bash

# steps for uploading build assets for a release (see ../.gitlab-ci.yml)
#
# This is based on Gitlab’s generic package example,
# https://gitlab.com/gitlab-org/release-cli/-/tree/master/docs/examples/release-assets-as-generic-package

if [ -z ${CI+x} ]; then
  printf 'CI environment variable unset; refusing to run\n' >&2
  exit 1
fi

# fail on error
set -e

# fail if any part of a pipeline fails
set -o pipefail

# echo commands
set -x

# echo some useful things into the log for debugging
uname -rms
if [ -e /etc/os-release ]; then
  cat /etc/os-release
fi

#install dependencies
env DEBIAN_FRONTEND=noninteractive apt-get update -y
for dep in curl jq; do
  if ! which ${dep} &>/dev/null; then
    env DEBIAN_FRONTEND=noninteractive apt-get install --no-install-recommends -y ${dep}
  fi
done

export GV_VERSION=$(cat VERSION)
export COLLECTION=$(cat COLLECTION)

# Gitlab’s generic package publishing API only supports a specific version
# format, so we can only upload releases
if ! egrep -q '^\d+\.\d+\.\d+$' VERSION; then
  printf '%s is not a release; skipping upload\n' "${GV_VERSION}" >&2
  # FIXME: currently it is not possible to programmatically mark this job as
  # skipped, https://gitlab.com/gitlab-org/gitlab/-/issues/16733
  # exit 0
fi

# FIXME
export GV_VERSION=1.0.0

# remove executable bit that causes Gitlab's upload functionality to reject
# files
find Packages -type f -exec chmod a-x "{}" \;

for src in Packages/"${COLLECTION}"/{fedora,centos}/*/{source,os/*,debug/*}/*; do

  # Gitlab upload file_name field only allows letters, numbers, dot, dash, and
  # underscore
  dst=$(printf '%s' "${src##*/}" | sed 's/[^a-zA-Z0-9\.\-_]/_/g')

  curl --silent --include --verbose \
       --header "JOB-TOKEN: ${CI_JOB_TOKEN}" \
       --upload-file "${src}" \
                     "${CI_API_V4_URL}/projects/${CI_PROJECT_ID}/packages/generic/graphviz-releases/${GV_VERSION}/${dst}" | tee response.json
  # FIXME
  #jq --exit-status '.ok' response.json
done

# FIXME
#md5sum graphviz-"${GV_VERSION}".tar.gz >graphviz-"${GV_VERSION}".tar.gz.md5
md5sum graphviz-"$(cat VERSION)".tar.gz >graphviz-"${GV_VERSION}".tar.gz.md5
#for src in graphviz-"${GV_VERSION}".tar.gz graphviz-"${GV_VERSION}".tar.gz.md5; do
for src in graphviz-"$(cat VERSION)".tar.gz graphviz-"${GV_VERSION}".tar.gz.md5; do

  # Gitlab upload file_name field only allows letters, numbers, dot, dash, and
  # underscore
  dst=$(printf '%s' "${src}" | sed 's/[^a-zA-Z0-9\.\-_]/_/g')

  curl --silent --include --verbose \
       --header "JOB-TOKEN: ${CI_JOB_TOKEN}" \
       --upload-file "${src}" \
                     "${CI_API_V4_URL}/projects/${CI_PROJECT_ID}/packages/generic/graphviz-releases/${GV_VERSION}/portable_source/${dst}" | tee response.json
  # FIXME
  #jq --exit-status '.ok' response.json
done
