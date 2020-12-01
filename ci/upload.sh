#!/usr/bin/env bash

# steps for uploading build asserts for a release (see ../.gitlab-ci.yml)
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

export GV_VERSION=$(cat VERSION)
export COLLECTION=$(cat COLLECTION)

chmod -R o-rwx Packages
chmod -R g-wx Packages
chmod -R g+X Packages

for i in Packages/"${COLLECTION}"/{fedora,centos}/*/{source,os/*,debug/*}; do
  curl --header "JOB-TOKEN: ${CI_JOB_TOKEN}" \
       --upload-file "${i}" \
                     "${CI_API_V4_URL}/projects/${CI_PROJECT_ID}/packages/generic/graphviz-releases/${GV_VERSION}/${i##*/}"
done

md5sum graphviz-"${GV_VERSION}".tar.gz >graphviz-"${GV_VERSION}".tar.gz.md5
for i in graphviz-"${GV_VERSION}".tar.gz graphviz-"${GV_VERSION}".tar.gz.md5; do
  curl --header "JOB-TOKEN: ${CI_JOB_TOKEN}" \
       --upload-file "${i}" \
                     "${CI_API_V4_URL}/projects/${CI_PROJECT_ID}/packages/generic/graphviz-releases/${GV_VERSION}/portable_source/${i}"
done
