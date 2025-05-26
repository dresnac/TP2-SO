#!/bin/bash

if [ -z "$1" ]; then
  echo "Por favor, proporciona un tipo de SO como argumento ('MAC' o 'Ubuntu' o 'WSL')."
  exit 1
fi
SO=$1

if [ "$SO" == "MAC" ]; then
  sudo qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev coreaudio,id=speaker -machine pcspk-audiodev=speaker
elif [ "$SO" == "Ubuntu" ]; then
  sudo qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
elif [ "$SO" == "WSL" ]; then
  sudo qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker,server=/mnt/wslg/PulseServer -machine pcspk-audiodev=speaker
else
  echo "Tipo de SO no reconocido. Usa 'linux' o 'windows'."
  exit 1
fi
