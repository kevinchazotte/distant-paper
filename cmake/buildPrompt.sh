#!/bin/bash
if command -v gcc > /dev/null 2>&1 && command -v cmake >/dev/null 2>&1 && command -v ninja >/dev/null 2>&1; then
  echo "Build environment ready..."
  exec bash
else
  echo "Check installation of build tools..."
  exec bash
fi

