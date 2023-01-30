$i = [ref]1
gci *.mp3 | rename-item -NewName {'{0:000}{1}' -f $i.value++, $_.Name.Substring(3)} -WhatIf
