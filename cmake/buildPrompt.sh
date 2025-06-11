#!/bin/bash
if command -v gcc > /dev/null 2>&1 && command -v cmake >/dev/null 2>&1 && [ -x "./ninja" ]; then
  exec bash
else
  echo  "Check build tool Installation..."
  exec bash
fi

