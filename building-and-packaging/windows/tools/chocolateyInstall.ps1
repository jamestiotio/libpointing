$url32 = 'https://github.com/INRIA/libpointing/releases/download/v1.0.8/libpointing-v1.0.8-Win32-VS2015.zip'
$checksum32 = '78E480B64105680C34306E507C6691E085CCA08978BCCF874CBA7737ACBF1BF6'
$url64 = 'https://github.com/INRIA/libpointing/releases/download/v1.0.8/libpointing-v1.0.8-Win64-VS2015.zip'
$checksum64 = 'C4D79A20991E24D66C71C5B4639FBFB5EE52862B38262CA35A9751D097EEFEBE'

$location = $Env:programFiles + '\libpointing\'

Install-ChocolateyZipPackage -PackageName 'libpointing' `
 -Url $url32 `
 -UnzipLocation $location `
 -Url64 $url64 `
 -Checksum $checksum32 `
 -ChecksumType 'sha256' `
 -Checksum64 $checksum64 `
 -ChecksumType64 'sha256' `
