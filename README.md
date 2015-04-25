Ever worked with a Blu-ray and had to split or edit a video but wanted to still make use of the chapters? Enter ChapterTime: a CLI-based application that will allow you to delay chapter times, convert to new frame rates and/or purge chapters.

*chaptertime sourcefile [-options]*

**Options:**<br />
*+/-100ms* 			apply a positive or negative chapter delay  
*-23.976/...* 		define source fps to be \"23.976\" (default), \"24.000\", ...  
*-changeTo24.000* 	change source fps to \"23.976\" (default), \"24.000\", ...  
*-removeLast*		removes the last chapter in the source  

ChapterTime is written using C++11 and will not work on prior versions.

Copyright (C) 2015 Shane Panke.

This software utilizes the pugixml library (http://pugixml.org). pugixml is Copyright (C) 2006-2015 Arseny Kapoulkine.