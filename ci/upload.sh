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

chmod -R o-rwx Packages
chmod -R g-wx Packages
chmod -R g+X Packages

for i in Packages/"${COLLECTION}"/{fedora,centos}/*/{source,os/*,debug/*}/*; do

  # Gitlab upload file_name field only allows letters, numbers, dot, dash, and
  # underscore
  i=$(printf '%s' "${i}" | sed 's/[^a-zA-Z0-9\.\-_]/_/g')

  curl --silent \
       --header "JOB-TOKEN: ${CI_JOB_TOKEN}" \
       --upload-file "${i}" \
                     "${CI_API_V4_URL}/projects/${CI_PROJECT_ID}/packages/generic/graphviz-releases/${GV_VERSION}/${i##*/}" | tee response.json
  #jq --exit-status '.ok' response.json
done

#md5sum graphviz-"${GV_VERSION}".tar.gz >graphviz-"${GV_VERSION}".tar.gz.md5
md5sum graphviz-"$(cat VERSION)".tar.gz >graphviz-"${GV_VERSION}".tar.gz.md5
#for i in graphviz-"${GV_VERSION}".tar.gz graphviz-"${GV_VERSION}".tar.gz.md5; do
for i in graphviz-"$(cat VERSION)".tar.gz graphviz-"${GV_VERSION}".tar.gz.md5; do

  # Gitlab upload file_name field only allows letters, numbers, dot, dash, and
  # underscore
  i=$(printf '%s' "${i}" | sed 's/[^a-zA-Z0-9\.\-_]/_/g')

  curl --silent \
       --header "JOB-TOKEN: ${CI_JOB_TOKEN}" \
       --upload-file "${i}" \
                     "${CI_API_V4_URL}/projects/${CI_PROJECT_ID}/packages/generic/graphviz-releases/${GV_VERSION}/portable_source/${i}" | tee response.json
  #jq --exit-status '.ok' response.json
done
