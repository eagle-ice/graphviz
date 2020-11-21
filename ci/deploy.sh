#!/usr/bin/env bash

# steps to be run for the “deploy” task in CI (see ../.gitlab-ci.yml)

if [ -z ${CI+x} ]; then
  printf 'CI environment variable unset; refusing to run\n' >&2
  exit 1
fi

if [ ! -e VERSION ]; then
  printf 'VERSION file is missing\n' >&2
  exit 1
fi

if [ ! -e COLLECTION ]; then
  printf 'COLLECTION file is missing\n' >&2
  exit 1
fi

if [ -z ${DEPLOY_PRIVATE_KEY+x} ]; then
  printf 'DEPLOY_PRIVATE_KEY unset\n' >&2
  exit 1
fi

if [ -z ${DEPLOY_USER+x} ]; then
  printf 'DEPLOY_USER unset\n' >&2
  exit 1
fi

if [ -z ${DEPLOY_HOST+x} ]; then
  printf 'DEPLOY_HOST unset\n' >&2
  exit 1
fi

# exit on error
set -e

# echo commands as they are run
set -x

GV_VERSION=$(cat VERSION)
COLLECTION=$(cat COLLECTION)

# configure SSH
eval $(ssh-agent -s)
cat "$DEPLOY_PRIVATE_KEY" | tr -d '\r' | ssh-add - > /dev/null
mkdir -p ~/.ssh
chmod 700 ~/.ssh
ssh-keyscan "$DEPLOY_HOST" >> ~/.ssh/known_hosts
chmod 644 ~/.ssh/known_hosts

# adjust Graphviz package files’ permissions
chmod -R o-rwx Packages
chmod -R g-wx Packages
chmod -R g+X Packages

# copy Graphviz package to the deployment machine
ssh "$DEPLOY_USER"@"$DEPLOY_HOST" 'rm -rf Packages/'"$COLLECTION"'/{fedora,centos,ubuntu,windows}'
scp -r Packages/* "$DEPLOY_USER"@"$DEPLOY_HOST"':'Packages/
ssh "$DEPLOY_USER"@"$DEPLOY_HOST" 'for i in Packages/'"$COLLECTION"'/{fedora,centos}/*/{source,os/*,debug/*}; do createrepo $i; done'
scp graphviz-fedora.repo graphviz-centos.repo "$DEPLOY_USER"@"$DEPLOY_HOST"':'Packages/
ssh "$DEPLOY_USER"@"$DEPLOY_HOST" mkdir -p Packages/"$COLLECTION"/portable_source

# copy a checksum for the tarball to the deployment machine
md5sum graphviz-"$GV_VERSION".tar.gz >graphviz-"$GV_VERSION".tar.gz.md5
scp graphviz-"$GV_VERSION".tar.gz graphviz-"$GV_VERSION".tar.gz.md5 "$DEPLOY_USER"@"$DEPLOY_HOST"':'Packages/"$COLLECTION"/portable_source/
