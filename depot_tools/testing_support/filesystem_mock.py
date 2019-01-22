# Copyright (c) 2011 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import errno
import fnmatch
import os
import re
import StringIO


def _RaiseNotFound(path):
  raise IOError(errno.ENOENT, path, os.strerror(errno.ENOENT))


class MockFileSystem(object):
  """Stripped-down version of WebKit's webkitpy.common.system.filesystem_mock

  Implements a filesystem-like interface on top of a dict of filenames ->
  file contents. A file content value of None indicates that the file should
  not exist (IOError will be raised if it is opened;
  reading from a missing key raises a KeyError, not an IOError."""

  def __init__(self, files=None):
    self.files = files or {}
    self.written_files = {}
    self._sep = '/'

  @property
  def sep(self):
    return self._sep

  def _split(self, path):
    return path.rsplit(self.sep, 1)

  def abspath(self, path):
    if path.endswith(self.sep):
      return path[:-1]
    return path

  def dirname(self, path):
    if self.sep not in path:
      return ''
    return self._split(path)[0] or self.sep

  def exists(self, path):
    return self.isfile(path) or self.isdir(path)

  def isabs(self, path):
    return path.startswith(self.sep)

  def isfile(self, path):
    return path in self.files and self.files[path] is not None

  def isdir(self, path):
    if path in self.files:
      return False
    if not path.endswith(self.sep):
      path += self.sep

    # We need to use a copy of the keys here in order to avoid switching
    # to a different thread and potentially modifying the dict in
    # mid-iteration.
    files = self.files.keys()[:]
    return any(f.startswith(path) for f in files)

  def join(self, *comps):
    # TODO: Might want tests for this and/or a better comment about how
    # it works.
    return re.sub(re.escape(os.path.sep), self.sep, os.path.join(*comps))

  def glob(self, path):
    return fnmatch.filter(self.files.keys(), path)

  def open_for_reading(self, path):
    return StringIO.StringIO(self.read_binary_file(path))

  def read_binary_file(self, path):
    # Intentionally raises KeyError if we don't recognize the path.
    if self.files[path] is None:
      _RaiseNotFound(path)
    return self.files[path]

  @staticmethod
  def relpath(path, base):
    # This implementation is wrong in many ways; assert to check them for now.
    assert path.startswith(base)
    assert base.endswith('/')
    return path[len(base):]
