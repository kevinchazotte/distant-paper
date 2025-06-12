#!/bin/bash
if command -v gcc > /dev/null 2>&1 && command -v cmake >/dev/null 2>&1 && command -v ninja >/dev/null 2>&1; then
  exec bash && echo "Build environment ready..."
else
  echo  "Check build tool Installation..."
  exec bash
fi

