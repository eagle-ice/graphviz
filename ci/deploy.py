#!/usr/bin/env python3

'''
steps for deploying a new release (see ../.gitlab-ci.yml)

This is based on Gitlab’s generic package example,
https://gitlab.com/gitlab-org/release-cli/-/tree/master/docs/examples/release-assets-as-generic-package
'''

import argparse
import hashlib
import json
import logging
import os
import re
import shutil
import stat
import subprocess
import sys
from typing import Optional
import urllib.request

log = None

def upload(version: str, path: str, name: Optional[str] = None) -> str:
  '''
  upload a file to the Graphviz generic package with the given version
  '''

  # use the path as the name if no other was given
  if name is None:
    name = path

  # Gitlab upload file_name field only allows letters, numbers, dot, dash, and
  # underscore
  safe = re.sub(r'[^a-zA-Z0-9.\-]', '_', name)
  log.info(f'escaped name {name} to {safe}')

  target = f'{os.environ["CI_API_V4_URL"]}/projects/' \
    f'{os.environ["CI_PROJECT_ID"]}/packages/generic/graphviz-releases/' \
    f'{version}/{safe}'

  headers = { 'JOB-TOKEN':os.environ["CI_JOB_TOKEN"] }

  log.info(f'uploading {path} to {target}')
  with open(path, 'rb') as f:
    req = urllib.request.Request(target, f, headers)
    with urllib.request.urlopen(req) as resp:
      outcome = resp.read().decode('utf-8')
  log.info(f'response: {outcome}')

  if json.loads(outcome)['message'] != '201 Created':
    raise Exception(f'upload failed: {outcome}')

  return target

def main(args: [str]) -> int:

  # setup logging to print to stderr
  global log
  ch = logging.StreamHandler()
  log = logging.getLogger('deploy.py')
  log.addHandler(ch)

  # parse command line arguments
  parser = argparse.ArgumentParser(description='Graphviz deployment script')
  parser.add_argument('--version', help='Override version number used to '
    'create a release. Without this, the contents of the VERSION file will be '
    'used.')
  parser.add_argument('--verbose', action='store_true', help='Print more '
    'diagnostic information.')
  options = parser.parse_args(args[1:])

  if options.verbose:
    log.setLevel(logging.INFO)

  if os.environ.get('CI') is None:
    log.error('CI environment variable unset; refusing to run')
    return -1

  # echo some useful things for debugging
  log.info(f'os.uname(): {os.uname()}')
  if os.path.exists('/etc/os-release'):
    with open('/etc/os-release') as f:
      log.info('/etc/os-release:')
      for i, line in enumerate(f):
        log.info(f' {i + 1}:{line[:-1]}')

  # bail out early if we do not have release-cli to avoid uploading assets that
  # become orphaned when we fail to create the release
  if not shutil.which('release-cli'):
    log.error('release-cli not found')
    return -1

  # retrieve version name left by prior CI tasks
  log.info('reading VERSION')
  with open('VERSION') as f:
    gv_version = f.read().strip()

  tarball = f'graphviz-{gv_version}.tar.gz'
  if not os.path.exists(tarball):
    log.error(f'source {tarball} not found')
    return -1

  # generate a checksum for the source tarball
  log.info(f'MD5 summing {tarball}')
  checksum = f'{tarball}.md5'
  with open(checksum, 'wt') as f:
    f.write(f'{hashlib.md5(tarball).hexdigest()}  {tarball}\n')

  # list of assets we have uploaded
  assets: [str] = []

  assets += upload(options.version, tarball)
  assets += upload(options.version, checksum)

  for stem, _, leaves in os.walk('Packages'):
    for leaf in leaves:
      path = os.path.join(stem, leaf)

      # get existing permissions
      mode = os.stat(path).st_mode

      # fixup permissions, o-rwx g-wx
      os.chmod(path, mode & ~stat.S_IRWXO & ~stat.S_IWGRP & ~stat.S_IXGRP)

      assets += upload(options.version, path, path[len('Packages/'):])

  # construct a command to create the release itself
  cmd = ['release-cli', 'create', '--name', options.version, '--tag-name',
    options.version, '--description', 'See the [CHANGELOG](https://gitlab.com/'
    'graphviz/graphviz/-/blob/master/CHANGELOG.md).']
  for a in assets:
    name = os.path.basename(a)
    url = a
    cmd += ['--assetslink', json.dumps({'name':name, 'url':url})]

  # create the release
  subprocess.check_call(cmd)

  return 0

if __name__ == '__main__':
  sys.exit(main(sys.argv))
